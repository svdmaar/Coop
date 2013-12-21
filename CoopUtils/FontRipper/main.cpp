#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Coop/Bitmap.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int g_iMinChar = 33;
const int g_iMaxChar = 127; // exclusive

void ExportDcToBitmap(HDC hDc, string fileName)
{
	int iWidth = GetDeviceCaps(hDc, HORZRES);
	int iHeight = GetDeviceCaps(hDc, VERTRES);

	CBitmap outBmp(iWidth, iHeight, 0x0);

	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iColumn = 0; iColumn < iWidth; iColumn++)
		{
			COLORREF pixDc = GetPixel(hDc, iColumn, iRow);

			outBmp.SetPixel(iRow, iColumn, pixDc);
		}
	}

	outBmp.Save(fileName);
}

string ConvertWCharToString(const WCHAR * pData)
{
	//WideCharToMultiByte(

	return "";
}

//int CALLBACK FontNameEnumCallback(const ENUMLOGFONT *lpelf, const NEWTEXTMETRIC *lpntm, DWORD FontType, LPARAM lParam)
int CALLBACK FontNameEnumCallback(CONST LOGFONTW * lpelf, CONST TEXTMETRICW *, DWORD, LPARAM)
{
	/*
	void * pParm = (void *)lParam;
	vector<string> & vNamesString = *(vector<string> *)pParm;
	*/

	//cout << lpelf->elfFullName << endl;
	wcout << lpelf->lfFaceName << endl;
	

	//vNamesString.push_back(string(lpelf->elfFullName));


	return TRUE;
}

void ListFontNames(HDC hDc/*, vector<string> vFonts*/)
{
	EnumFontFamilies(hDc, NULL, FontNameEnumCallback, NULL);

}

void SelectArialBlack(HDC hDc)
{
	HFONT font = CreateFont(200, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial Black");
	SelectObject(hDc, font);

}

int main()
{
	HDC hDcDesktop = GetDC(NULL);

	int iWidth = GetDeviceCaps(hDcDesktop, HORZRES);
	int iHeight = GetDeviceCaps(hDcDesktop, VERTRES);

	HDC hDcComp = CreateCompatibleDC(hDcDesktop);

	cout << hDcComp << endl;

	HBITMAP hBmp = CreateCompatibleBitmap(hDcComp, iWidth, iHeight);

	cout << hBmp << endl;

	SelectObject(hDcComp, hBmp);

	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = 0;
	rText.top = 0;
	rText.right = rText.left + iWidth - rText.left;
	rText.bottom = rText.top + iHeight - rText.top;

	SelectArialBlack(hDcComp);
	SelectObject(hDcComp, GetStockObject(WHITE_BRUSH));

	FillRect(hDcComp, &rText, (HBRUSH)GetStockObject(WHITE_BRUSH));

	int iOutput = DrawText(hDcComp, L"Sander WW WA", -1, &rText, DT_LEFT | DT_TOP);
	cout << iOutput << endl;

	ExportDcToBitmap(hDcComp, "out_font_3.bmp");

	//ListFontNames(hDcComp); 

	return 0;
}
