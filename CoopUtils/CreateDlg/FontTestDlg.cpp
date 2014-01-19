#include "FontTestDlg.h"

static LRESULT CALLBACK FontTestCallbackFunc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   switch(uMessage)
   {
      case WM_CLOSE:
         PostQuitMessage(0);
         return TRUE;
      case WM_PAINT:
         PAINTSTRUCT ps;
         HDC hDc = BeginPaint(hWindow, &ps);

         RECT rText;
         rText.left = rText.top = 0;
         rText.right = rText.bottom = 200;

         SetTextColor(hDc, 0xffffff);
         SetBkColor(hDc, 0);
         //TextOut(hDc, 0, 0, L"Test1\r\nTest2", 10);
         DrawText(hDc, L"Test1\nTest2", 11, &rText, DT_LEFT | DT_TOP);

         EndPaint(hWindow, &ps);
         break;
   }

   return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

void RunDlgFont()
{
   SetupWindow(L"Font test window", WND_WINDOWED_DC, FontTestCallbackFunc);
   
   RunMainLoop();
}
