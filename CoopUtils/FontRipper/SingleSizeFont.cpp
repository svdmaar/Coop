#include "SingleSizeFont.h"

#include <sstream>

using namespace std;

CSingleSizeFont::CSingleSizeFont()
{
	m_iMinChar = 0;
	m_iMaxChar = 0;
	m_iWindowsFontHeight = 0;
	m_iLineDistance = 0;
	m_iAverageWidth = 0;
	m_iAverageHeight = 0;
	m_iSpaceSize = 0;
	m_iFontHeight = 0;
	m_iInitialRowOffset = 0;
}

string CSingleSizeFont::_FormatCharIndex(int _iChar)
{
	stringstream ssFormattedIndex;

	// Set formatting.
	ssFormattedIndex.fill('0');
	ssFormattedIndex.width(3);

	ssFormattedIndex << right << _iChar;

	string sFormattedIndex = ssFormattedIndex.str();
	return sFormattedIndex;
}

bool CSingleSizeFont::_ReadFontFromBmpIni(const string & _sFilenameBase)
{
	string sIniFile = _sFilenameBase + ".ini";
	string sBmpFile = _sFilenameBase + ".bmp";

	if(!m_bmBigBitmap.Load(sBmpFile))
	{
		return false;
	}

	if(!m_iniFile.Load(sIniFile))
	{
		return false;
	}

	m_iMinChar = m_iniFile.GetValueInt("info", "minChar");
	m_iMaxChar = m_iniFile.GetValueInt("info", "maxChar");

	int nChars = m_iMaxChar - m_iMinChar;
	m_vCharDescs.resize(nChars);

	for(int iChar = m_iMinChar; iChar < m_iMaxChar; iChar++)
	{
		int iTargetIndex = iChar - m_iMinChar;
		string sBlock = "char" + _FormatCharIndex(iChar);

		RECT rSubBitmap;
		ZeroMemory(&rSubBitmap, sizeof(rSubBitmap));

		rSubBitmap.top = m_iniFile.GetValueInt(sBlock, "inBitmapY");
		rSubBitmap.left = m_iniFile.GetValueInt(sBlock, "inBitmapX");
		
		int iCharWidth = m_iniFile.GetValueInt(sBlock, "width");
		int iCharHeight = m_iniFile.GetValueInt(sBlock, "height");
		
		rSubBitmap.bottom = rSubBitmap.top + iCharHeight;
		rSubBitmap.right = rSubBitmap.left + iCharWidth;

		SCharDesc & charDesc = m_vCharDescs[iTargetIndex];
		charDesc.m_bitmap = m_bmBigBitmap.GetSubBitmap(rSubBitmap);

		charDesc.m_pInBitmap.x = rSubBitmap.left;
		charDesc.m_pInBitmap.y = rSubBitmap.top;

		charDesc.m_iBufferLeft = m_iniFile.GetValueInt(sBlock, "bufferLeft");
		charDesc.m_iBufferRight = m_iniFile.GetValueInt(sBlock, "bufferRight");

		charDesc.m_pUpperLeft.x = m_iniFile.GetValueInt(sBlock, "upperLeftX");
		charDesc.m_pUpperLeft.y = m_iniFile.GetValueInt(sBlock, "upperLeftY");
	}

	m_iSpaceSize = m_iniFile.GetValueInt("info", "spaceSize");
	m_iFontHeight = m_iniFile.GetValueInt("info", "fontHeight");
	m_sFontName = m_iniFile.GetValueString("info", "fontName");
	m_iWindowsFontHeight = m_iniFile.GetValueInt("info", "fontWindowsHeight");
	m_iLineDistance = m_iniFile.GetValueInt("info", "lineDistance");
	m_iInitialRowOffset = m_iniFile.GetValueInt("info", "initialRowOffset");

	return true;
}

bool CSingleSizeFont::DrawStringToBitmap(const string & _sText, CBitmap & _bmOut) const
{
	_bmOut = CBitmap(0, 0, 0);
	if(_sText.length() == 0)
	{
		return true;
	}

	vector<SCharPosition> vCharPos;
	if(!_CalcCharPositions(_sText, vCharPos))
	{
		return false;
	}

	int iWidth = 0;
	int iHeight = 0;
	for(int i = 0; i < (int)vCharPos.size(); i++)
	{
		const SCharPosition & charPos = vCharPos[i];
		int iCharIndex = charPos.m_iIndex;
		const SCharDesc & charDesc = m_vCharDescs[iCharIndex];

		int iCharHeight = charPos.m_pPos.y + charDesc.m_bitmap.GetHeight();
		int iCharWidth = charPos.m_pPos.x + charDesc.m_bitmap.GetWidth();

		iHeight = max(iHeight, iCharHeight);
		iWidth = max(iWidth, iCharWidth);
	}

	_bmOut = CBitmap(iWidth, iHeight, 0);

	for(int i = 0; i < (int)vCharPos.size(); i++)
	{
		const SCharPosition & charPos = vCharPos[i];
		int iCharIndex = charPos.m_iIndex;
		const SCharDesc & charDesc = m_vCharDescs[iCharIndex];
		POINT pPos = charPos.m_pPos;

		_bmOut.InsertBitmap(pPos.y, pPos.x, charDesc.m_bitmap);
	}

	return true;
}

bool CSingleSizeFont::_CalcCharPositions(const std::string & _sText, std::vector<SCharPosition> & _vCharPos) const
{
	int nLength = _sText.length();
	int iCurrentRow = m_iInitialRowOffset;
	int iCurrentColumn = 0;

	for(int iIndex = 0; iIndex < nLength; iIndex++)
	{
		char c = _sText[iIndex];

		if(c == ' ')
		{
			iCurrentColumn += m_iSpaceSize;
			continue;
		}

		if(c == '\n')
		{
			iCurrentRow += m_iFontHeight + m_iLineDistance;
			iCurrentColumn = 0;
			continue;
		}

		int iIntChar = (int)c;
		bool bValidChar = (iIntChar >= m_iMinChar) && (iIntChar < m_iMaxChar);

		if(!bValidChar)
		{
			c = '?';
			iIntChar = (int)c;
		}

		int iCharIndex = iIntChar - m_iMinChar;
		const SCharDesc & charDesc = m_vCharDescs[iCharIndex];

		if(iCurrentColumn > 0)
		{
			iCurrentColumn += charDesc.m_iBufferLeft;
		}

		SCharPosition charPos;
		ZeroMemory(&charPos, sizeof(charPos));

		charPos.m_iIndex = iCharIndex;
		charPos.m_pPos.x = iCurrentColumn;
		charPos.m_pPos.y = charDesc.m_pUpperLeft.y + iCurrentRow;
		_vCharPos.push_back(charPos);

		iCurrentColumn += charDesc.m_bitmap.GetWidth();
		iCurrentColumn += charDesc.m_iBufferRight;
	}

	return true;
}

bool CSingleSizeFont::Load(const string & _sFilenameBase)
{
	return _ReadFontFromBmpIni(_sFilenameBase);
}
