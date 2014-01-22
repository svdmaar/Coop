#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"
#include "../IniFileReader/IniFile.h"
#include "FontRipper.h"
#include "CharDesc.h"
#include "CharPosition.h"
#include "SingleSizeFont.h"

#include <iostream>
#include <string>
#include <vector>
#include <minmax.h>
#include <locale>
#include <sstream>
#include <fstream>

using namespace std;

const int g_iMinChar = 33;
const int g_iMaxChar = 127; // exclusive
const int g_nCharCount = g_iMaxChar - g_iMinChar;

HDC g_hDc = (HDC)0;
const int g_iTextRectSize = 512;
ofstream g_ofOut;

SCharDesc g_chars[g_nCharCount];
CBitmap g_bmBigBitmap;

string g_sFontName;
int g_iWindowsFontHeight = 0;
int g_iLineDistance = 0;

int g_iAverageWidth = 0;
int g_iAverageHeight = 0;
int g_iSpaceSize = 0;
int g_iFontHeight = 0;
int g_iInitialRowOffset = 0;

void ExportDcToBitmap(HDC hDc, CBitmap& bmOut)
{
	int iWidth = GetDeviceCaps(hDc, HORZRES);
	int iHeight = GetDeviceCaps(hDc, VERTRES);

	bmOut = CBitmap(iWidth, iHeight, 0);

	for(int iRow = 0; iRow < iHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < iWidth; iColumn++)
		{
			COLORREF pixDc = GetPixel(hDc, iColumn, iRow);

			bmOut.SetPixel(iRow, iColumn, pixDc);
		}
	}
}

string FormatCharIndex(int _iChar)
{
	stringstream ssFormattedIndex;

	// Set formatting.
	ssFormattedIndex.fill('0');
	ssFormattedIndex.width(3);

	ssFormattedIndex << right << _iChar;

	string sFormattedIndex = ssFormattedIndex.str();
	return sFormattedIndex;
}

int CALLBACK FontNameEnumCallback(CONST LOGFONTW * lpelf, CONST TEXTMETRICW *, DWORD, LPARAM)
{
	wcout << lpelf->lfFaceName << endl;

	return TRUE;
}

void ListFontNames(HDC hDc)
{
	EnumFontFamilies(hDc, NULL, FontNameEnumCallback, NULL);
}

void SelectArialBlack(HDC hDc)
{
	SetTextColor(hDc, 0xffffL);
	SetBkColor(hDc, 0);
	HFONT font = CreateFont(200, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial Black");
	SelectObject(hDc, font);
}

wstring ConvertStringToWide(const string & _sText)
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
	return wsOut;
}

void SelectFontAndSize(const string & _sFontName, int _iSize)
{
	SetTextColor(g_hDc, 0xffffL);
	SetBkColor(g_hDc, 0);

	wstring wsFontName = ConvertStringToWide(_sFontName);
	const wchar_t * pwcFontName = wsFontName.c_str();

	HFONT font = CreateFont(_iSize, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, pwcFontName);
	SelectObject(g_hDc, font);
}

void CreateStringBitmap(const string& _sText, CBitmap& _bmOut, POINT& _pos)
{
	int nLength = _sText.length();
	int nBufferSize = nLength + 1;

	wchar_t * pwcBuffer = new wchar_t[nBufferSize];
	ZeroMemory(pwcBuffer, sizeof(wchar_t) * nBufferSize);

	char * pCharOutBuffer = (char *)(void *)pwcBuffer;

	for(int iIndex = 0; iIndex < nLength; iIndex++)
	{
		int iTargetIndex = iIndex * 2;
		char cChar = _sText[iIndex];

		pCharOutBuffer[iTargetIndex] = cChar;
	}

	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = rText.top;
	rText.right = rText.bottom = g_iTextRectSize;

	SetTextColor(g_hDc, 0xffffffL);
	SetBkColor(g_hDc, 0);
	FillRect(g_hDc, &rText, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawText(g_hDc, pwcBuffer, -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

	CBitmap bmFull;
	ExportDcToBitmap(g_hDc, bmFull);

	GetNonBlackData(bmFull, _pos, _bmOut);
}

void ExportChars()
{
	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = rText.top;
	rText.right = rText.bottom = g_iTextRectSize;

	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		cout << iChar << "..." << endl;

		char cChar = (char)iChar;
		wchar_t wcBuffer[4];

		ZeroMemory(wcBuffer, sizeof(wcBuffer));

		char * pcFirstChar = (char *)(void *)wcBuffer;
		*pcFirstChar = cChar;

		FillRect(g_hDc, &rText, (HBRUSH)GetStockObject(WHITE_BRUSH));
		DrawText(g_hDc, wcBuffer, -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

		CBitmap bmFull;
		ExportDcToBitmap(g_hDc, bmFull);

		CBitmap bmChar;
		POINT pos;
		GetNonBlackData(bmFull, pos, bmChar);

		stringstream ssFormattedIndex;

		// Set formatting.
		ssFormattedIndex.fill('0');
		ssFormattedIndex.width(3);

		ssFormattedIndex << right << iChar;

		string sFormattedIndex = ssFormattedIndex.str();
			
		stringstream ssFileName;
		ssFileName << "out_chars/" << sFormattedIndex << ".bmp";
			
		string sFileName = ssFileName.str();

		bmChar.Save(sFileName);

		g_ofOut << endl;
		g_ofOut << "[char" << sFormattedIndex << "]" << endl;
		g_ofOut << "minX = " << pos.x << endl;
		g_ofOut << "minY = " << pos.y << endl;
	}
}

void GenerateKerningPairs(char _c1, char _c2)
{
	CBitmap bmText;
	POINT pos;
	stringstream ss;

	ss << _c1;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iWWidth = bmText.GetWidth();
	ss.str("");

	ss << _c2;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iAWidth = bmText.GetWidth();
	ss.str("");

	int iExpectedWidth = iWWidth + iAWidth;

	ss << _c1 << _c2;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iRealWidth = bmText.GetWidth();

	cout << _c1 << ", " << _c2 << ": " << iExpectedWidth << ", " << iRealWidth << endl;
}

int CalcD(char _c1, char _c2)
{
	CBitmap bmText;
	POINT pos;
	stringstream ss;

	ss << _c1;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iWWidth = bmText.GetWidth();
	ss.str("");

	ss << _c2;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iAWidth = bmText.GetWidth();
	ss.str("");

	int iExpectedWidth = iWWidth + iAWidth;

	ss << _c1 << _c2;
	CreateStringBitmap(ss.str(), bmText, pos);
	int iRealWidth = bmText.GetWidth();

	return iRealWidth - iExpectedWidth;
}

void CalcSpaceSize()
{
	CBitmap bmText;
	POINT pos;

	cout << "Calculating space width..." << endl;

	// Assume A has no left and right buffer.
	CreateStringBitmap("A A", bmText, pos);
	int iWidthWithSpace = bmText.GetWidth();

	CreateStringBitmap("AA", bmText, pos);
	int iWidthWithoutSpace = bmText.GetWidth();

	int iSpaceSize = iWidthWithSpace - iWidthWithoutSpace;
	g_iSpaceSize = iSpaceSize;
}

void ListKerningPairs()
{
	for(int iChar1 = g_iMinChar; iChar1 < g_iMaxChar; iChar1++)
	{
		for(int iChar2 = g_iMinChar; iChar2 < g_iMaxChar; iChar2++)
		{
			char c1 = (char)iChar1;
			char c2 = (char)iChar2;

			GenerateKerningPairs(c1, c2);
		}
	}
}

void FigureOutDotPair()
{
	GenerateKerningPairs('A', 'e');
}

void FillCharDescs_BitmapUpperLeft()
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

		char cChar = (char)iChar;
		wchar_t wcBuffer[4];

		ZeroMemory(wcBuffer, sizeof(wcBuffer));

		char * pcFirstChar = (char *)(void *)wcBuffer;
		*pcFirstChar = cChar;

		FillRect(g_hDc, &rBlack, (HBRUSH)GetStockObject(BLACK_BRUSH));
		DrawText(g_hDc, wcBuffer, -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

		CBitmap bmFull;
		ExportDcToBitmap(g_hDc, bmFull);

		int iTargetIndex = iChar - g_iMinChar;
		SCharDesc& charDesc = g_chars[iTargetIndex];

		GetNonBlackData(bmFull, charDesc.m_pUpperLeft, charDesc.m_bitmap);
	}
}

void FillCharDescs_LeftRight()
{
	// Assume 'A' has no left and right, and
	// left and right is independent of neighboring chars.
	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		cout << "For left and right: " << iChar << "..." << endl;

		SCharDesc& charDesc = g_chars[iChar - g_iMinChar];
		char cChar = (char)iChar;

		charDesc.m_iBufferLeft = CalcD('A', cChar);
		charDesc.m_iBufferRight = CalcD(cChar, 'A');
	}
}

// iMaxChar == exclusive
int CalcTotalWidth(int iStartChar, int iMaxChar)
{
	int iTotalWidth = 0;

	for(int iChar = iStartChar; iChar < iMaxChar; iChar++)
	{
		iTotalWidth += g_chars[iChar].m_bitmap.GetWidth();
	}

	return iTotalWidth;
}

int CalcMaxRowWidth(int nCharPerRow)
{
	int nRows = (g_nCharCount + nCharPerRow - 1) / nCharPerRow;
	int iMaxRowWidth = 0;

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		int iStartChar = iRow * nCharPerRow;
		int iMaxChar = iStartChar + nCharPerRow;

		iMaxChar = min(iMaxChar, g_nCharCount);

		int iRowWidth = CalcTotalWidth(iStartChar, iMaxChar);

		iMaxRowWidth = max(iMaxRowWidth, iRowWidth);
	}

	return iMaxRowWidth;
}

// iMaxChar == exclusive
int CalcMaxHeight(int iStartChar, int iMaxChar)
{
	int iMaxHeight = 0;

	for(int iChar = iStartChar; iChar < iMaxChar; iChar++)
	{
		int iHeight = g_chars[iChar].m_bitmap.GetHeight();

		iMaxHeight = max(iMaxHeight, iHeight);
	}

	return iMaxHeight;
}

int CalcTotalRowsHeight(int nCharPerRow)
{
	int nRows = (g_nCharCount + nCharPerRow - 1) / nCharPerRow;
	int iTotalRowsHeight = 0;

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		int iStartChar = iRow * nCharPerRow;
		int iMaxChar = iStartChar + nCharPerRow;

		iMaxChar = min(iMaxChar, g_nCharCount);

		int iRowHeight = CalcMaxHeight(iStartChar, iMaxChar);

		iTotalRowsHeight += iRowHeight;
	}

	return iTotalRowsHeight;
}

void CreateBigBitmap()
{
	const int nCharPerRow = 10;
	int nRows = (g_nCharCount + nCharPerRow - 1) / nCharPerRow;

	int iRowStartPixel = 0;
	int iNextRowStartPixel = 0;
	int iCurrentColumnPixel = 0;
	int iMaxHeight = 0;

	int iBitmapWidth = CalcMaxRowWidth(nCharPerRow);
	int iBitmapHeight = CalcTotalRowsHeight(nCharPerRow);

	g_bmBigBitmap = CBitmap(iBitmapWidth, iBitmapHeight, 0);

	for(int iRow = 0; iRow < nRows; iRow++)
	{
		iRowStartPixel = iNextRowStartPixel;
		iCurrentColumnPixel = 0;

		for(int iColumn = 0; iColumn < nCharPerRow; iColumn++)
		{
			int iChar = iRow * nCharPerRow + iColumn;

			if (iChar >= g_nCharCount)
			{
				break;
			}

			SCharDesc & charDesc = g_chars[iChar];
			CBitmap & bmChar = charDesc.m_bitmap;
			g_bmBigBitmap.InsertBitmap(iRowStartPixel, iCurrentColumnPixel, bmChar);

			charDesc.m_pInBitmap.x = iCurrentColumnPixel;
			charDesc.m_pInBitmap.y = iRowStartPixel;

			iCurrentColumnPixel += bmChar.GetWidth();

			int iThisCharNextRowPixel = iRowStartPixel + bmChar.GetHeight();

			iNextRowStartPixel = max(iNextRowStartPixel, iThisCharNextRowPixel);
		}
	}
}

void WriteInfoToIniFile()
{
	cout << "Writing info to ini file..." << endl;

	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		int iSourceIndex = iChar - g_iMinChar;
		SCharDesc & charDesc = g_chars[iSourceIndex];

		stringstream ssFormattedIndex;

		// Set formatting.
		ssFormattedIndex.fill('0');
		ssFormattedIndex.width(3);

		ssFormattedIndex << right << iChar;

		string sFormattedIndex = ssFormattedIndex.str();
			
		g_ofOut << endl;
		g_ofOut << "[char" << sFormattedIndex << "]" << endl;
		g_ofOut << "width = " << charDesc.m_bitmap.GetWidth() << endl;
		g_ofOut << "height = " << charDesc.m_bitmap.GetHeight() << endl;
		g_ofOut << "upperLeftX = " << charDesc.m_pUpperLeft.x << endl;
		g_ofOut << "upperLeftY = " << charDesc.m_pUpperLeft.y << endl;
		g_ofOut << "inBitmapX = " << charDesc.m_pInBitmap.x << endl;
		g_ofOut << "inBitmapY = " << charDesc.m_pInBitmap.y << endl;
		g_ofOut << "bufferLeft = " << charDesc.m_iBufferLeft << endl;
		g_ofOut << "bufferRight = " << charDesc.m_iBufferRight << endl;
	}
}

void ReadFontFromBmpIni()
{
	const char * iniFile = "out_chars/arial_black_100.ini";
	const char * bmpFile = "out_chars/arial_black_100.bmp";

	g_bmBigBitmap.Load(bmpFile);

	CIniFile ifIniFile;
	ifIniFile.Load(iniFile);

	for(int iChar = g_iMinChar; iChar < g_iMaxChar; iChar++)
	{
		int iTargetIndex = iChar - g_iMinChar;
		string sBlock = "char" + FormatCharIndex(iChar);

		RECT rSubBitmap;
		ZeroMemory(&rSubBitmap, sizeof(rSubBitmap));

		rSubBitmap.top = ifIniFile.GetValueInt(sBlock, "inBitmapY");
		rSubBitmap.left = ifIniFile.GetValueInt(sBlock, "inBitmapX");
		
		int iCharWidth = ifIniFile.GetValueInt(sBlock, "width");
		int iCharHeight = ifIniFile.GetValueInt(sBlock, "height");
		
		rSubBitmap.bottom = rSubBitmap.top + iCharHeight;
		rSubBitmap.right = rSubBitmap.left + iCharWidth;

		SCharDesc & charDesc = g_chars[iTargetIndex];
		charDesc.m_bitmap = g_bmBigBitmap.GetSubBitmap(rSubBitmap);

		charDesc.m_pInBitmap.x = rSubBitmap.left;
		charDesc.m_pInBitmap.y = rSubBitmap.top;

		charDesc.m_iBufferLeft = ifIniFile.GetValueInt(sBlock, "bufferLeft");
		charDesc.m_iBufferRight = ifIniFile.GetValueInt(sBlock, "bufferRight");

		charDesc.m_pUpperLeft.x = ifIniFile.GetValueInt(sBlock, "upperLeftX");
		charDesc.m_pUpperLeft.y = ifIniFile.GetValueInt(sBlock, "upperLeftY");
	}

	g_iSpaceSize = ifIniFile.GetValueInt("info", "spaceSize");
	g_iFontHeight = ifIniFile.GetValueInt("info", "fontHeight");
	g_sFontName = ifIniFile.GetValueString("info", "fontName");
	g_iWindowsFontHeight = ifIniFile.GetValueInt("info", "fontWindowsHeight");
	g_iLineDistance = ifIniFile.GetValueInt("info", "lineDistance");
	g_iInitialRowOffset = ifIniFile.GetValueInt("info", "initialRowOffset");
}

void CalcAverageHeightWidth()
{
	int iTotalWidth = 0;
	int iTotalHeight = 0;

	for(int iChar = 0; iChar < g_nCharCount; iChar++)
	{
		SCharDesc & charDesc = g_chars[iChar];

		iTotalWidth += charDesc.m_bitmap.GetWidth();
		iTotalHeight += charDesc.m_bitmap.GetHeight();
	}

	g_iAverageWidth = (iTotalWidth - 1) / g_nCharCount + 1;
	g_iAverageHeight = (iTotalHeight - 1) / g_nCharCount + 1;
}

vector<SCharPosition> CalcCharPositions(const string & _sText)
{
	int nLength = _sText.length();
	int iCurrentRow = g_iInitialRowOffset;
	int iCurrentColumn = 0;
	vector<SCharPosition> vCharPos;

	for(int iIndex = 0; iIndex < nLength; iIndex++)
	{
		char c = _sText[iIndex];

		if(c == ' ')
		{
			iCurrentColumn += g_iSpaceSize;
			continue;
		}

		if(c == '\n')
		{
			iCurrentRow += g_iFontHeight + g_iLineDistance;
			iCurrentColumn = 0;
			continue;
		}

		int iIntChar = (int)c;
		bool bValidChar = (iIntChar >= g_iMinChar) && (iIntChar < g_iMaxChar);

		if(!bValidChar)
		{
			c = '?';
			iIntChar = (int)c;
		}

		int iCharIndex = iIntChar - g_iMinChar;
		SCharDesc & charDesc = g_chars[iCharIndex];

		if(iCurrentColumn > 0)
		{
			iCurrentColumn += charDesc.m_iBufferLeft;
		}

		SCharPosition charPos;
		ZeroMemory(&charPos, sizeof(charPos));

		charPos.m_iIndex = iCharIndex;
		charPos.m_pPos.x = iCurrentColumn;
		charPos.m_pPos.y = charDesc.m_pUpperLeft.y + iCurrentRow;
		vCharPos.push_back(charPos);

		iCurrentColumn += charDesc.m_bitmap.GetWidth();
		iCurrentColumn += charDesc.m_iBufferRight;
	}

	return vCharPos;
}

CBitmap DrawStringToBitmap(const string & _sText)
{
	vector<SCharPosition> vCharPos = CalcCharPositions(_sText);
	CBitmap bmOut(0, 0, 0);

	if(_sText.length() == 0)
	{
		return bmOut;
	}

	int iWidth = 0;
	int iHeight = 0;
	for(int i = 0; i < (int)vCharPos.size(); i++)
	{
		const SCharPosition & charPos = vCharPos[i];
		int iCharIndex = charPos.m_iIndex;
		const SCharDesc & charDesc = g_chars[iCharIndex];

		int iCharHeight = charPos.m_pPos.y + charDesc.m_bitmap.GetHeight();
		int iCharWidth = charPos.m_pPos.x + charDesc.m_bitmap.GetWidth();

		iHeight = max(iHeight, iCharHeight);
		iWidth = max(iWidth, iCharWidth);
	}

	bmOut = CBitmap(iWidth, iHeight, 0);

	for(int i = 0; i < (int)vCharPos.size(); i++)
	{
		const SCharPosition & charPos = vCharPos[i];
		int iCharIndex = charPos.m_iIndex;
		const SCharDesc & charDesc = g_chars[iCharIndex];
		POINT pPos = charPos.m_pPos;

		bmOut.InsertBitmap(pPos.y, pPos.x, charDesc.m_bitmap);
	}

	return bmOut;
}

string SanitizeFileNameBase(const string & _sFileBase)
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

void WriteToBmpIni(const string & _sOutName)
{
	string sSanitizedFileBase = SanitizeFileNameBase(_sOutName);
	string sPathAndBase = "out_chars/" + sSanitizedFileBase + ".";
	string sIniFileName = sPathAndBase + "ini";
	string sBmpFileName = sPathAndBase + "bmp";

	g_ofOut.open(sIniFileName.c_str());

	g_ofOut << "[info]" << endl;
	g_ofOut << "name = " << g_sFontName << endl;
	g_ofOut << "minChar = " << g_iMinChar << endl;
	g_ofOut << "maxChar = " << g_iMaxChar << endl;
	g_ofOut << "spaceSize = " << g_iSpaceSize << endl;
	g_ofOut << "fontHeight = " << g_iFontHeight << endl;
	g_ofOut << "windowsFontHeight = " << g_iWindowsFontHeight << endl;
	g_ofOut << "fontName = " << g_sFontName << endl;
	g_ofOut << "lineDistance = " << g_iLineDistance << endl;
	g_ofOut << "initialRowOffset = " << g_iInitialRowOffset << endl;

	WriteInfoToIniFile();
	g_bmBigBitmap.Save(sBmpFileName.c_str());
}

void MoveFontUp()
{
	int iMinY = INT_MAX;

	for(int i = 0; i < g_nCharCount; i++)
	{
		const SCharDesc & charDesc = g_chars[i];

		int iStartY = charDesc.m_pUpperLeft.y;

		iMinY = min(iMinY, iStartY);
	}

	for(int i = 0; i < g_nCharCount; i++)
	{
		SCharDesc & charDesc = g_chars[i];

		charDesc.m_pUpperLeft.y -= iMinY;
	}

	g_iInitialRowOffset = iMinY;
}

void CalcFontHeight()
{
	int iMaxHeight = 0;

	for(int i = 0; i < g_nCharCount; i++)
	{
		const SCharDesc & charDesc = g_chars[i];

		int iHeight = charDesc.m_bitmap.GetHeight() + charDesc.m_pUpperLeft.y;
		iMaxHeight = max(iMaxHeight, iHeight);
	}

	g_iFontHeight = iMaxHeight;
}

void GetHeighestLowestChar(int & _iHeighest, int & _iLowest)
{
	int iMaxHeight = 0;
	
	_iHeighest = -1;

	for(int i = 0; i < g_nCharCount; i++)
	{
		SCharDesc & charDesc = g_chars[i];
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

void CalcFontLineDistance()
{
	RECT rText;
	rText.top = rText.left = 0;
	rText.right = 1920;
	rText.bottom = 1200;

	CBitmap bmAll;

	int iHeightest = 0, iLowest = 0;
	GetHeighestLowestChar(iHeightest, iLowest);

	char cHeighest = (char)(g_iMinChar + iHeightest);
	char cLowest = (char)(g_iMinChar + iLowest);

	cout << "Calculating line distance..." << endl;
	cout << "h: " << cHeighest << ", l: " << cLowest << endl;

	stringstream ss;
	ss << cHeighest << cLowest;

	string sText = ss.str();
	wstring wsText = ConvertStringToWide(sText);
	FillRect(g_hDc, &rText, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawText(g_hDc, wsText.c_str(), wsText.length(), &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

	ExportDcToBitmap(g_hDc, bmAll);
	CBitmap bmSub;
	POINT pPos;
	GetNonBlackData(bmAll, pPos, bmSub);

	int iSingleHeight = bmSub.GetHeight();

	// Built two-line bmp.
	ss << "\n" << cHeighest << cLowest << endl;

	sText = ss.str();
	wsText = ConvertStringToWide(sText);
	FillRect(g_hDc, &rText, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawText(g_hDc, wsText.c_str(), wsText.length(), &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

	ExportDcToBitmap(g_hDc, bmAll);
	GetNonBlackData(bmAll, pPos, bmSub);

	int iDoubleHeight = bmSub.GetHeight();
	int iLineDistance = iDoubleHeight - 2 * iSingleHeight;
	g_iLineDistance = iLineDistance;

	cout << "Line distance: " << iLineDistance << endl;
}

void GenerateFont(const string & _sFontName, int _iFontSize)
{
	HDC hDcDesktop = GetDC(NULL);

	int iWidth = GetDeviceCaps(hDcDesktop, HORZRES);
	int iHeight = GetDeviceCaps(hDcDesktop, VERTRES);

	SelectFontAndSize(_sFontName, _iFontSize);
	SelectObject(g_hDc, GetStockObject(BLACK_BRUSH));

	g_sFontName = _sFontName;
	g_iWindowsFontHeight = _iFontSize;

	FillCharDescs_BitmapUpperLeft();
	MoveFontUp();
	FillCharDescs_LeftRight();
	CalcSpaceSize();
	CalcFontHeight();
	CalcFontLineDistance();

	CreateBigBitmap();

	stringstream ssBaseName;
	ssBaseName << _sFontName << "_" << _iFontSize;

	string sBaseName = ssBaseName.str();
	WriteToBmpIni(sBaseName);
}

int main()
{
	CFontRipper fontRipper;
	fontRipper.RipFont("Arial Black", 200);

	return 0;

	HDC hDcDesktop = GetDC(NULL);

	int iWidth = GetDeviceCaps(hDcDesktop, HORZRES);
	int iHeight = GetDeviceCaps(hDcDesktop, VERTRES);

	g_hDc = CreateCompatibleDC(hDcDesktop);
	HBITMAP hBmp = CreateCompatibleBitmap(g_hDc, iWidth, iHeight);
	SelectObject(g_hDc, hBmp);

	//ReadFontFromBmpIni();



	//GenerateFont("Arial Black", 100);

	/*
	CSingleSizeFont font;
	if(!font.Load("out_chars/arial_black_100"))
		cout << "Load error" << endl;

	CBitmap bmText;
	if(!font.DrawStringToBitmap("I love Fernanda", bmText))
		cout << "Draw error" << endl;

	bmText.Save("fer.bmp");
	*/

	/*
	ReadFontFromBmpIni();
	CalcFontHeight();

	WriteToBmpIni("arial_black_200_5");
	*/

	/*
	CBitmap bmText = DrawStringToBitmap("I love Fernanda\nA lot\nSo so much");
	bmText.Save("fernanda.bmp");
	*/

/*
	POINT pos;
	ZeroMemory(&pos, sizeof(pos));
	CreateStringBitmap("Fernanda", bmText, pos);

	bmText.Save("fernanda2.bmp");
*/

	return 0;
}
