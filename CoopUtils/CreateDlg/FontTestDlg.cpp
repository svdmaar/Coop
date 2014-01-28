#include "FontTestDlg.h"

#include <string>

using namespace std;

static LRESULT CALLBACK FontTestCallbackFunc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = 0;
	HDC hDc = 0;
	wstring sText;

	switch(uMessage)
	{
	case WM_CREATE:
		hFont = CreateFont(100, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial Black");
		hDc = GetDC(hWindow);

		SelectObject(hDc, hFont);
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		hDc = BeginPaint(hWindow, &ps);

		RECT rText;
		rText.left = rText.top = 0;
		rText.right = rText.bottom = 1000;

		SetTextColor(hDc, 0xffffff);
		SetBkColor(hDc, 0);
		
		sText = L"I love Fernanda\nSo so much\nA lot, a lot";
		DrawText(hDc, sText.c_str(), sText.length(), &rText, DT_LEFT | DT_TOP);

		EndPaint(hWindow, &ps);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;
	}

	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

void RunDlgFont()
{
	SetupWindow(L"Font test window", WND_WINDOWED_DC, FontTestCallbackFunc);

	RunMainLoop();
}
