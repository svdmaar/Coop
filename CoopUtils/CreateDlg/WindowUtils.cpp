#include "WindowUtils.h"

HWND g_hWindow = 0;

const static wchar_t * g_className = L"class";

static RECT CalcDesktopCenterWindow(DWORD _dwStyle, DWORD _dwStyleEx)
{
	HWND hWndDesktop = GetDesktopWindow();

	RECT rDesktop;
	GetWindowRect(hWndDesktop, &rDesktop);

	RECT rClient;
	rClient.left = rDesktop.right / 4;
	rClient.top = rDesktop.bottom / 4;
	rClient.right = 3 * rClient.left;
	rClient.bottom = 3 * rClient.top;

	AdjustWindowRectEx(&rClient, _dwStyle, FALSE, _dwStyleEx);
	return rClient;
}

bool SetupWindow(const wchar_t * _caption, E_WINDOWSTYLE _eStyle, WNDPROC _lpProc)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));

	int iBrush = BLACK_BRUSH;
	if(_eStyle == WND_DIALOG)
	{
		iBrush = LTGRAY_BRUSH;
	}

	DWORD dwStyle = WS_VISIBLE;
	DWORD dwStyleEx = 0;
	RECT rClient;

	switch(_eStyle)
	{
	case WND_FULLSCREEN:

		break;
	case WND_DIALOG:

		break;
	case WND_WINDOWED_DC:
		dwStyle = WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
		rClient = CalcDesktopCenterWindow(dwStyle, dwStyleEx);
		break;
	}

	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpszClassName = g_className;
	windowClass.lpfnWndProc = _lpProc;
	windowClass.hInstance = g_hInstance;
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(iBrush);

	ATOM classAtom = RegisterClassEx(&windowClass);

	g_hWindow = CreateWindowEx(dwStyleEx, g_className, _caption, dwStyle, rClient.left, 
		rClient.top, rClient.right - rClient.left, rClient.bottom - rClient.top, NULL, NULL, g_hInstance, NULL);

	return true;
}

bool RunMainLoop(void (*_proc)(void) /* = NULL */)
{
	while(true)
	{
		MSG message;
		BOOL bMessageWaiting = PeekMessage(&message, g_hWindow, 0, 0, PM_REMOVE);

		if(bMessageWaiting)
		{
			if(message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if(_proc != NULL)
		{
			_proc();
		}
	}

	return true;
}

// Regular Window styles (fourth arg of CreateWindowEx):
// WS_BORDER - The window has a thin-line border.
// WS_CAPTION - The window has a title bar (includes the WS_BORDER style).
// WS_DLGFRAME - The window has a border of a style typically used with dialog boxes.  (Can't have titlebar).
// WS_HSCROLL, WS_VSCROLL - The window has a horizontal/vertical scroll bar.
// WS_MAXIMIZE - The window is initially maximized.
// WS_MAXIMIZEBOX, WS_MINIMIZEBOX - The window has a maximize/minimize button (requires WS_SYSMENU).
// WS_SIZEBOX - The window has a sizing border. 
// WS_SYSMENU - The window has a window menu on its title bar (requires WS_CAPTION).
// WS_VISIBLE - The window is initially visible.

// Extended window styles (first arg):
// WS_EX_CLIENTEDGE - The window has a border with a sunken edge.
// WS_EX_DLGMODALFRAME - The window has a double border.
// WS_EX_WINDOWEDGE - The window has a border with a raised edge.
