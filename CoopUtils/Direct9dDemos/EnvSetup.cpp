#include "EnvSetup.h"

#include <iostream>
using namespace std;

const static wchar_t * g_className = L"class";
const static wchar_t * g_windowCaption = L"Direct3D Demo";

static IDirect3D9 * g_pDirect3d9 = NULL;

IDirect3DDevice9 * g_pD3dDevice = NULL;
HWND g_hWindow = 0;

static RECT CalcDesktopCenterWindow(DWORD _dwStyle)
{
	HWND hWndDesktop = GetDesktopWindow();

	RECT rDesktop;
	GetWindowRect(hWndDesktop, &rDesktop);

	RECT rClient;
	rClient.left = rDesktop.right / 4;
	rClient.top = rDesktop.bottom / 4;
	rClient.right = 3 * rClient.left;
	rClient.bottom = 3 * rClient.top;

	AdjustWindowRect(&rClient, _dwStyle, FALSE);

	return rClient;
}

static LRESULT CALLBACK WindowProc(HWND _hWindow, UINT _uMessage, WPARAM _wParam, LPARAM _lParam)
{
	if(_uMessage == WM_CLOSE)
	{
		PostQuitMessage(0);
		return TRUE;
	}

	return DefWindowProc(_hWindow, _uMessage, _wParam, _lParam);
}

bool SetupWindow()
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));

	int iBrush = BLACK_BRUSH;

	DWORD dwStyle = WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
	RECT rClient = CalcDesktopCenterWindow(dwStyle);

	HINSTANCE hInstance = GetModuleHandle(NULL);

	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpszClassName = g_className;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(iBrush);

	ATOM classAtom = RegisterClassEx(&windowClass);

	g_hWindow = CreateWindow(g_className, g_windowCaption, dwStyle, rClient.left, 
		rClient.top, rClient.right - rClient.left, rClient.bottom - rClient.top, NULL, NULL, hInstance, NULL);

	g_pDirect3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pDirect3d9 == NULL) cout << "Failed to create Direct3D" << endl;

	HDC hDc = GetDC(g_hWindow);
	int iWidth = GetDeviceCaps(hDc, HORZSIZE);
	int iHeight = GetDeviceCaps(hDc, VERTSIZE);

	D3DPRESENT_PARAMETERS presentParams;
	ZeroMemory(&presentParams, sizeof(presentParams));

	presentParams.Windowed = TRUE;
	presentParams.SwapEffect = D3DSWAPEFFECT_COPY;
	presentParams.EnableAutoDepthStencil = TRUE;
	presentParams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT hr;
	hr = g_pDirect3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParams, &g_pD3dDevice);
	if(FAILED(hr)) cout << "create device: " << hr << endl;

	return true;
}

bool CleanUpWindow()
{
	HRESULT hr;

	hr = g_pD3dDevice->Release();
	if(FAILED(hr)) cout << "release device: " << hr << endl;

	hr = g_pDirect3d9->Release();
	if(FAILED(hr)) cout << "release d3d: " << hr << endl;

	return true;
}
