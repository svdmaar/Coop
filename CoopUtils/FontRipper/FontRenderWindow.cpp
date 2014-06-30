#include "FontRenderWindow.h"

#include <iostream>

using namespace std;

const static wchar_t * g_className = L"class";
const static wchar_t * g_caption = L"font-rip";

const int g_iTextRectSize = 512;

LRESULT CALLBACK CFontRenderWindow::FontTestCallbackFunc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = 0;
	HDC hDc = 0;
   wchar_t * pText;

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
		
		pText = L"T";
		DrawText(hDc, pText, 1, &rText, DT_LEFT | DT_TOP);

		EndPaint(hWindow, &ps);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;
	}

	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

bool CFontRenderWindow::Create()
{
   WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));

	int iBrush = BLACK_BRUSH;

	DWORD dwStyle = WS_VISIBLE;
	DWORD dwStyleEx = 0;
	RECT rClient;

	dwStyle = WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
 	rClient.left = 100;
	rClient.top = 100;
	rClient.right = 600;
	rClient.bottom = 800;

	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpszClassName = g_className;
	windowClass.lpfnWndProc = FontTestCallbackFunc;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.hCursor = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(iBrush);

	ATOM classAtom = RegisterClassEx(&windowClass);

   m_hWindow = CreateWindowEx(dwStyleEx, g_className, g_caption, dwStyle, rClient.left, 
		rClient.top, rClient.right - rClient.left, rClient.bottom - rClient.top, NULL, NULL, GetModuleHandle(NULL), NULL);

   _ProcessMessages();

	return true;
}

void CFontRenderWindow::_ProcessMessages()
{
   while(true)
	{
		MSG message;

		BOOL bMessageWaiting = PeekMessage(&message, m_hWindow, 0, 0, PM_REMOVE);
      if (!bMessageWaiting)
      {
         break;
      }

		if(bMessageWaiting)
		{
			if(message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

CBitmap CFontRenderWindow::RenderString(const std::string& _sText)
{
   HDC hDc = GetDC(m_hWindow);

   _ClearDc(hDc);

   wstring wsText = _ConvertStringToWide(_sText);

	RECT rText;
	ZeroMemory(&rText, sizeof(rText));

	rText.left = rText.top = 0;
	rText.right = rText.bottom = g_iTextRectSize;

	SetTextColor(hDc, 0xffffff);
	SetBkColor(hDc, 0);
		
   DrawText(hDc, wsText.c_str(), -1, &rText, DT_LEFT | DT_TOP | DT_NOPREFIX);

   CBitmap bmOut;
   _ExportDcToBitmap(hDc, bmOut);

   return bmOut;
}

void CFontRenderWindow::_ExportDcToBitmap(HDC _hDc, CBitmap & _bmOut)
{
   RECT rClient;
   GetClientRect(m_hWindow, &rClient);

	//int iWidth = GetDeviceCaps(_hDc, HORZRES);
   int iWidth = rClient.right - rClient.left;
	//int iHeight = GetDeviceCaps(_hDc, VERTRES);
   int iHeight = rClient.bottom - rClient.top;

	_bmOut = CBitmap(iWidth, iHeight, 0);

	for(int iRow = 0; iRow < iHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < iWidth; iColumn++)
		{
			COLORREF pixDc = GetPixel(_hDc, iColumn, iRow);

			_bmOut.SetPixel(iRow, iColumn, pixDc);
		}

      _ProcessMessages();
	}
}

wstring CFontRenderWindow::_ConvertStringToWide(const string & _sText)
{
	int nLength = _sText.length();
	wchar_t * pwcBuffer = new wchar_t[nLength + 1];

	ZeroMemory(pwcBuffer, sizeof(wchar_t) * (nLength + 1));
	char * pcBuffer = (char *)pwcBuffer;

	for(int i = 0; i < nLength; i++)
	{
		int iTargetIndex = i * 2;
		pcBuffer[iTargetIndex] = _sText[i];
	}

	wstring wsOut(pwcBuffer);
	delete [] pwcBuffer;

	return wsOut;
}

void CFontRenderWindow::_ClearDc(HDC _hDc)
{
   //int nWidth = GetDeviceCaps(_hDc, HORZSIZE);
   //int nHeight = GetDeviceCaps(_hDc, VERTSIZE);
   int nWidth = 1920;
   int nHeight = 1200;

   HBRUSH backBrush = CreateSolidBrush(0);
   HBRUSH oldBrush = (HBRUSH)SelectObject(_hDc, backBrush);

   RECT rBack;
   rBack.left = rBack.top = 0;
   rBack.right = nWidth;
   rBack.bottom = nHeight;

   Rectangle(_hDc, 0, 0, nWidth, nHeight);

   SelectObject(_hDc, oldBrush);
   DeleteObject(oldBrush);
}

CBitmap CFontRenderWindow::RenderGreyString(const std::string& _sText, POINT& _pos)
{
   CBitmap bmAll = RenderString(_sText);

   CBitmap bmOut;
   GetNonBlackData(bmAll, _pos, bmOut);

   for (int i = 0; i < bmOut.GetHeight(); i++)
   {
      for (int j = 0; j < bmOut.GetWidth(); j++)
      {
         DWORD dwPixel = bmOut.GetPixel(i, j);

         int iRed = 0, iGreen = 0, iBlue = 0;
         CBitmap::DecodeColor(dwPixel, iRed, iGreen, iBlue);

         int iGrey = (iRed + iGreen + iBlue) / 3;

         dwPixel = CBitmap::EncodeColor(iGrey, iGrey, iGrey);
         bmOut.SetPixel(i, j, dwPixel);
      }
   }

   return bmOut;
}

void CFontRenderWindow::SelectFontAndSize(const string & _sFontName, int _iSize)
{
   HDC hDc = GetDC(m_hWindow);

	SetTextColor(hDc, 0xffffL);
	SetBkColor(hDc, 0);

	wstring wsFontName = _ConvertStringToWide(_sFontName);
	const wchar_t * pwcFontName = wsFontName.c_str();

	HFONT font = CreateFont(_iSize, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, pwcFontName);
	SelectObject(hDc, font);
}
