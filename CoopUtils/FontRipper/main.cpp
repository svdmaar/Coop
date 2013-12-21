#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Coop/Bitmap.h"

#include <iostream>
#include <string>

using namespace std;

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

int main()
{
	HDC hDcDesktop = GetDC(NULL);

	cout << hDcDesktop << endl;
	cout << GetDeviceCaps(hDcDesktop, HORZRES) << endl;

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
	rText.right = rText.left + iWidth;
	rText.bottom = rText.top + iHeight;

	SelectObject(hDcComp, GetStockObject(WHITE_BRUSH));

	FillRect(hDcComp, &rText, (HBRUSH)GetStockObject(WHITE_BRUSH));

	int iOutput = DrawText(hDcComp, L"X", -1, &rText, DT_LEFT);
	cout << iOutput << endl;

	ExportDcToBitmap(hDcComp, "out_font_3.bmp");

	return 0;
}
