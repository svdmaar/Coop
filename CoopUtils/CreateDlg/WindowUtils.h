#pragma once

#include <Windows.h>

extern HWND g_hWindow;
extern HINSTANCE g_hInstance;

enum E_WINDOWSTYLE
{
   WND_FULLSCREEN,
   WND_DIALOG,
   WND_WINDOWED_DC,
};

bool SetupWindow(const wchar_t * _caption, E_WINDOWSTYLE _eStyle, WNDPROC _lpProc);

bool RunMainLoop(void (*_proc)(void) = NULL);
