#include "DlgWithButton.h"

static HWND g_hWndButton;

LRESULT CALLBACK DlgButtonCallbackFunc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   switch(uMessage)
   {
      case WM_CLOSE:
         PostQuitMessage(0);
         return TRUE;
      case WM_CREATE:
         g_hWndButton = CreateWindowEx(NULL, L"BUTTON", L"OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 24, hWindow, NULL, g_hInstance, NULL);
         break;
      case WM_COMMAND:
         if((HWND)lParam == g_hWndButton)
         {
            MessageBox(g_hWindow, L"Clicked", L"Message", MB_OK);
         }
         break;
   }

   return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

void RunDlgButton()
{
   SetupWindow(L"Window with button", WND_DIALOG, DlgButtonCallbackFunc);
   
   RunMainLoop();
}
