#include "FontRipper.h"

#include <minmax.h>

const int g_iMinChar = 33;
const int g_iMaxChar = 127; // exclusive
const int g_iTextRectSize = 512;

CFontRipper::CFontRipper()
{
	m_hDc = 0;
}

void CFontRipper::_ExportDcToBitmap(CBitmap & _bmOut)
{
	int iWidth = GetDeviceCaps(m_hDc, HORZRES);
	int iHeight = GetDeviceCaps(m_hDc, VERTRES);

	_bmOut = CBitmap(iWidth, iHeight, 0);

	for(int iRow = 0; iRow < iHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < iWidth; iColumn++)
		{
			COLORREF pixDc = GetPixel(m_hDc, iColumn, iRow);

			_bmOut.SetPixel(iRow, iColumn, pixDc);
		}
	}
}

wstring CFontRipper::_ConvertStringToWide(const string & _sText)
{
	int nLength = _sText.length();
	wchar_t * pwcBuffer = new wchar_t[nLength + 1];

	ZeroMemory(pwcBuffer, sizeof(wchar_t) * (nLength + 1));
	char * pcBuffer = (char *)(wchar_t *)pwcBuffer;

	for(int i = 0; i < nLength; i++)
	{
		int iTargetIndex = i * 2;
		pcBuffer[iTargetIndex] = _sText[i];
	}

	wstring wsOut(pwcBuffer);
	delete [] pwcBuffer;

	return wsOut;
}

void CFontRipper::_SelectFontAndSize(const string & _sFontName, int _iSize)
{
	SetTextColor(m_hDc, 0xffffL);
	SetBkColor(m_hDc, 0);

	wstring wsFontName = _ConvertStringToWide(_sFontName);
	const wchar_t * pwcFontName = wsFontName.c_str();

	HFONT font = CreateFont(_iSize, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, pwcFontName);
	SelectObject(m_hDc, font);
}

void CFontRipper::_CreateStringBitmapTrimmed(const string & _sText, CBitmap & _bmOut, POINT & _pos)
{
   wstring wsText = _ConvertStringToWide(_sText);

	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = rText.top;
	rText.right = rText.bottom = g_iTextRectSize;

	// TODO: still necessary?
	SetTextColor(m_hDc, 0xffffffL);
	SetBkColor(m_hDc, 0);

	FillRect(m_hDc, &rText, (HBRUSH)GetStockObject(BLACK_BRUSH));
   DrawText(m_hDc, wsText.c_str(), -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

	CBitmap bmFull;
	_ExportDcToBitmap(bmFull);

	GetNonBlackData(bmFull, _pos, _bmOut);
}

int CFontRipper::_CalcD(char _c1, char _c2)
{
	CBitmap bmText;
	POINT pos;
	stringstream ss;

	ss << _c1;
	_CreateStringBitmapTrimmed(ss.str(), bmText, pos);
	int iWWidth = bmText.GetWidth();
	ss.str("");

	ss << _c2;
	_CreateStringBitmapTrimmed(ss.str(), bmText, pos);
	int iAWidth = bmText.GetWidth();
	ss.str("");

	int iExpectedWidth = iWWidth + iAWidth;

	ss << _c1 << _c2;
	_CreateStringBitmapTrimmed(ss.str(), bmText, pos);
	int iRealWidth = bmText.GetWidth();

	return iRealWidth - iExpectedWidth;
}

void CFontRipper::_CalcSpaceSize()
{
	CBitmap bmText;
	POINT pos;

	cout << "Calculating space width..." << endl;

	// Assume A has no left and right buffer.
	_CreateStringBitmapTrimmed("A A", bmText, pos);
	int iWidthWithSpace = bmText.GetWidth();

	_CreateStringBitmapTrimmed("AA", bmText, pos);
	int iWidthWithoutSpace = bmText.GetWidth();

	int iSpaceSize = iWidthWithSpace - iWidthWithoutSpace;
	m_iSpaceSize = iSpaceSize;
}

void CFontRipper::_FillCharDescs_BitmapUpperLeft()
{
	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = rText.top = 0;
	rText.right = rText.bottom = g_iTextRectSize;

	RECT rBlack;
	ZeroMemory(&rBlack, sizeof(rBlack));

	rBlack.left = rBlack.top = 0;
	rBlack.right = 1920;
	rBlack.bottom = 1200;

	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		cout << "For bitmap and upper left: " << iChar << "..." << endl;
		int iTargetIndex = iChar - g_iMinChar;
		SCharDesc & charDesc = m_vCharDescs[iTargetIndex];

		char cChar = (char)iChar;
      char buffer[2];
      buffer[0] = cChar;
      buffer[1] = '\0';

      string sText = buffer;
      _CreateStringBitmapTrimmed(sText, charDesc.m_bitmap, charDesc.m_pUpperLeft);
	}
}

void CFontRipper::_FillCharDescs_LeftRight()
{
	// Assume 'A' has no left and right, and
	// left and right is independent of neighboring chars.
	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		cout << "For left and right: " << iChar << "..." << endl;

		SCharDesc& charDesc = m_vCharDescs[iChar - g_iMinChar];
		char cChar = (char)iChar;

		charDesc.m_iBufferLeft = _CalcD('A', cChar);
		charDesc.m_iBufferRight = _CalcD(cChar, 'A');
	}
}

// iMaxChar == exclusive
int CFontRipper::_CalcTotalWidth(int _iStartChar, int _iMaxChar)
{
	int iTotalWidth = 0;

	for(int iChar = _iStartChar; iChar < _iMaxChar; iChar++)
	{
		iTotalWidth += m_vCharDescs[iChar].m_bitmap.GetWidth();
	}

	return iTotalWidth;
}

int CFontRipper::_CalcMaxRowWidth(int _nCharPerRow)
{
	int nRows = (GetCharCount() + _nCharPerRow - 1) / _nCharPerRow;
	int iMaxRowWidth = 0;

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		int iStartChar = iRow * _nCharPerRow;
		int iMaxChar = iStartChar + _nCharPerRow;

		iMaxChar = min(iMaxChar, GetCharCount());

		int iRowWidth = _CalcTotalWidth(iStartChar, iMaxChar);

		iMaxRowWidth = max(iMaxRowWidth, iRowWidth);
	}

	return iMaxRowWidth;
}

// iMaxChar == exclusive
int CFontRipper::_CalcMaxHeight(int _iStartChar, int _iMaxChar)
{
	int iMaxHeight = 0;

	for(int iChar = _iStartChar; iChar < _iMaxChar; iChar++)
	{
		int iHeight = m_vCharDescs[iChar].m_bitmap.GetHeight();

		iMaxHeight = max(iMaxHeight, iHeight);
	}

	return iMaxHeight;
}

int CFontRipper::_CalcTotalRowsHeight(int _nCharPerRow)
{
	int nRows = (GetCharCount() + _nCharPerRow - 1) / _nCharPerRow;
	int iTotalRowsHeight = 0;

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		int iStartChar = iRow * _nCharPerRow;
		int iMaxChar = iStartChar + _nCharPerRow;

		iMaxChar = min(iMaxChar, GetCharCount());

		int iRowHeight = _CalcMaxHeight(iStartChar, iMaxChar);

		iTotalRowsHeight += iRowHeight;
	}

	return iTotalRowsHeight;
}

void CFontRipper::_CreateBigBitmap()
{
	const int nCharPerRow = 10;
	int nRows = (GetCharCount() + nCharPerRow - 1) / nCharPerRow;

	int iRowStartPixel = 0;
	int iNextRowStartPixel = 0;
	int iCurrentColumnPixel = 0;
	int iMaxHeight = 0;

	int iBitmapWidth = _CalcMaxRowWidth(nCharPerRow);
	int iBitmapHeight = _CalcTotalRowsHeight(nCharPerRow);

	m_bmBigBitmap = CBitmap(iBitmapWidth, iBitmapHeight, 0);

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		iRowStartPixel = iNextRowStartPixel;
		iCurrentColumnPixel = 0;

		for(int iColumn = 0; iColumn < nCharPerRow; iColumn++)
		{
			int iChar = iRow * nCharPerRow + iColumn;

			if (iChar >= GetCharCount())
			{
				break;
			}

			SCharDesc & charDesc = m_vCharDescs[iChar];
			CBitmap & bmChar = charDesc.m_bitmap;
			m_bmBigBitmap.InsertBitmap(iRowStartPixel, iCurrentColumnPixel, bmChar);

			charDesc.m_pInBitmap.x = iCurrentColumnPixel;
			charDesc.m_pInBitmap.y = iRowStartPixel;

			iCurrentColumnPixel += bmChar.GetWidth();

			int iThisCharNextRowPixel = iRowStartPixel + bmChar.GetHeight();

			iNextRowStartPixel = max(iNextRowStartPixel, iThisCharNextRowPixel);
		}
	}
}

void CFontRipper::_WriteInfoToIniFile()
{
	cout << "Writing info to ini file..." << endl;

	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		int iSourceIndex = iChar - g_iMinChar;
		SCharDesc & charDesc = m_vCharDescs[iSourceIndex];

		stringstream ssFormattedIndex;

		// Set formatting.
		ssFormattedIndex.fill('0');
		ssFormattedIndex.width(3);

		ssFormattedIndex << right << iChar;

		string sFormattedIndex = ssFormattedIndex.str();
			
		m_ofOut << endl;
		m_ofOut << "[char" << sFormattedIndex << "]" << endl;
		m_ofOut << "width = " << charDesc.m_bitmap.GetWidth() << endl;
		m_ofOut << "height = " << charDesc.m_bitmap.GetHeight() << endl;
		m_ofOut << "upperLeftX = " << charDesc.m_pUpperLeft.x << endl;
		m_ofOut << "upperLeftY = " << charDesc.m_pUpperLeft.y << endl;
		m_ofOut << "inBitmapX = " << charDesc.m_pInBitmap.x << endl;
		m_ofOut << "inBitmapY = " << charDesc.m_pInBitmap.y << endl;
		m_ofOut << "bufferLeft = " << charDesc.m_iBufferLeft << endl;
		m_ofOut << "bufferRight = " << charDesc.m_iBufferRight << endl;
	}
}

string CFontRipper::_SanitizeFileNameBase(const string & _sFileBase)
{
	string sOutput = _sFileBase;
	for(int i = 0; i < (int)sOutput.length(); i++)
	{
		char c = sOutput[i];

		if(c == ' ')
		{
			c = '_';
			sOutput[i] = c;
		}
	}

	return sOutput;
}

void CFontRipper::_WriteToBmpIni(const string & _sOutName)
{
	string sSanitizedFileBase = _SanitizeFileNameBase(_sOutName);
	string sPathAndBase = "out_chars/" + sSanitizedFileBase + ".";
	string sIniFileName = sPathAndBase + "ini";
	string sBmpFileName = sPathAndBase + "bmp";

	m_ofOut.open(sIniFileName.c_str());

	m_ofOut << "[info]" << endl;
	m_ofOut << "name = " << m_sFontName << endl;
	m_ofOut << "minChar = " << m_iMinChar << endl;
	m_ofOut << "maxChar = " << m_iMaxChar << endl;
	m_ofOut << "spaceSize = " << m_iSpaceSize << endl;
	m_ofOut << "fontHeight = " << m_iFontHeight << endl;
	m_ofOut << "windowsFontHeight = " << m_iWindowsFontHeight << endl;
	m_ofOut << "fontName = " << m_sFontName << endl;
	m_ofOut << "lineDistance = " << m_iLineDistance << endl;
	m_ofOut << "initialRowOffset = " << m_iInitialRowOffset << endl;

	_WriteInfoToIniFile();
	m_bmBigBitmap.Save(sBmpFileName.c_str());
}

void CFontRipper::_MoveFontUp()
{
	int iMinY = INT_MAX;

	for(int i = 0; i < GetCharCount(); i++)
	{
		const SCharDesc & charDesc = m_vCharDescs[i];

		int iStartY = charDesc.m_pUpperLeft.y;

		iMinY = min(iMinY, iStartY);
	}

	for(int i = 0; i < GetCharCount(); i++)
	{
		SCharDesc & charDesc = m_vCharDescs[i];

		charDesc.m_pUpperLeft.y -= iMinY;
	}

	m_iInitialRowOffset = iMinY;
}

void CFontRipper::_CalcFontHeight()
{
	int iMaxHeight = 0;

	for(int i = 0; i < GetCharCount(); i++)
	{
		const SCharDesc & charDesc = m_vCharDescs[i];

		int iHeight = charDesc.m_bitmap.GetHeight() + charDesc.m_pUpperLeft.y;
		iMaxHeight = max(iMaxHeight, iHeight);
	}

	m_iFontHeight = iMaxHeight;
}

void CFontRipper::_GetHeighestLowestChar(int & _iHeighest, int & _iLowest)
{
	int iMaxHeight = 0;
	
	_iHeighest = -1;

	for(int i = 0; i < GetCharCount(); i++)
	{
		SCharDesc & charDesc = m_vCharDescs[i];
		int iCharHeight = charDesc.m_pUpperLeft.y + charDesc.m_bitmap.GetHeight();

		if(iCharHeight > iMaxHeight)
		{
			iMaxHeight = iCharHeight;
			_iLowest = i;
		}

		if((charDesc.m_pUpperLeft.y == 0) && (_iHeighest == -1))
		{
			_iHeighest = i;
		}
	}
}

void CFontRipper::_CalcFontLineDistance()
{
	RECT rText;
	rText.top = rText.left = 0;
	rText.right = 1920;
	rText.bottom = 1200;

	CBitmap bmAll;

	int iHeightest = 0, iLowest = 0;
	_GetHeighestLowestChar(iHeightest, iLowest);

	char cHeighest = (char)(g_iMinChar + iHeightest);
	char cLowest = (char)(g_iMinChar + iLowest);

	cout << "Calculating line distance..." << endl;
	cout << "h: " << cHeighest << ", l: " << cLowest << endl;

	stringstream ss;
	ss << cHeighest << cLowest;

	string sText = ss.str();

   CBitmap bmSub;
	POINT pPos;
   _CreateStringBitmapTrimmed(sText, bmSub, pPos);

	int iSingleHeight = bmSub.GetHeight();

	// Built two-line bmp.
	ss << "\n" << cHeighest << cLowest << endl;

	sText = ss.str();
	
   _CreateStringBitmapTrimmed(sText, bmSub, pPos);

	int iDoubleHeight = bmSub.GetHeight();
	int iLineDistance = iDoubleHeight - 2 * iSingleHeight;
	m_iLineDistance = iLineDistance;

	cout << "Line distance: " << iLineDistance << endl;
}

bool CFontRipper::RipFont(const std::string & _sFontName, int _iFontSize)
{
	m_iMinChar = g_iMinChar;
	m_iMaxChar = g_iMaxChar;

	int nChars = m_iMaxChar - m_iMinChar;
	m_vCharDescs.resize(nChars);

	HDC hDcDesktop = GetDC(NULL);

	int iWidth = GetDeviceCaps(hDcDesktop, HORZRES);
	int iHeight = GetDeviceCaps(hDcDesktop, VERTRES);

	m_hDc = CreateCompatibleDC(hDcDesktop);
	HBITMAP hBmp = CreateCompatibleBitmap(m_hDc, iWidth, iHeight);
	SelectObject(m_hDc, hBmp);

	_SelectFontAndSize(_sFontName, _iFontSize);
	SelectObject(m_hDc, GetStockObject(BLACK_BRUSH));

	m_sFontName = _sFontName;
	m_iWindowsFontHeight = _iFontSize;

	_FillCharDescs_BitmapUpperLeft();
	_MoveFontUp();
	_FillCharDescs_LeftRight();
	_CalcSpaceSize();
	_CalcFontHeight();
	_CalcFontLineDistance();

	_CreateBigBitmap();

	stringstream ssBaseName;
	ssBaseName << _sFontName << "_" << _iFontSize;

	string sBaseName = ssBaseName.str();
	_WriteToBmpIni(sBaseName);

	return true;
}
