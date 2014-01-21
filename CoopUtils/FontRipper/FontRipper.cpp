#include "FontRipper.h"

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

void CFontRipper::_CreateStringBitmap(const string & _sText, CBitmap & _bmOut, POINT & _pos)
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

	// TODO: still necessary?
	SetTextColor(m_hDc, 0xffffffL);
	SetBkColor(m_hDc, 0);

	FillRect(m_hDc, &rText, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawText(m_hDc, pwcBuffer, -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

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
	_CreateStringBitmap(ss.str(), bmText, pos);
	int iWWidth = bmText.GetWidth();
	ss.str("");

	ss << _c2;
	_CreateStringBitmap(ss.str(), bmText, pos);
	int iAWidth = bmText.GetWidth();
	ss.str("");

	int iExpectedWidth = iWWidth + iAWidth;

	ss << _c1 << _c2;
	_CreateStringBitmap(ss.str(), bmText, pos);
	int iRealWidth = bmText.GetWidth();

	return iRealWidth - iExpectedWidth;
}

void CFontRipper::_CalcSpaceSize()
{
	CBitmap bmText;
	POINT pos;

	cout << "Calculating space width..." << endl;

	// Assume A has no left and right buffer.
	_CreateStringBitmap("A A", bmText, pos);
	int iWidthWithSpace = bmText.GetWidth();

	_CreateStringBitmap("AA", bmText, pos);
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

		char cChar = (char)iChar;
		wchar_t wcBuffer[4];

		ZeroMemory(wcBuffer, sizeof(wcBuffer));

		char * pcFirstChar = (char *)(void *)wcBuffer;
		*pcFirstChar = cChar;

		FillRect(m_hDc, &rBlack, (HBRUSH)GetStockObject(BLACK_BRUSH));
		DrawText(m_hDc, wcBuffer, -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

		CBitmap bmFull;
		_ExportDcToBitmap(bmFull);

		int iTargetIndex = iChar - g_iMinChar;
		SCharDesc & charDesc = m_vCharDescs[iTargetIndex];

		GetNonBlackData(bmFull, charDesc.m_pUpperLeft, charDesc.m_bitmap);
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

bool CFontRipper::RipFont(const std::string & _sFont, int _iSize)
{

	m_iMinChar = g_iMinChar;
	m_iMaxChar = g_iMaxChar;

	int nChars = m_iMaxChar - m_iMinChar;
	m_vCharDescs.resize(nChars);

	return true;
}