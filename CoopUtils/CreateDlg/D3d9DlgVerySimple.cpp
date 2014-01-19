#include "D3d9DlgVerySimple.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

using namespace std;

static IDirect3DDevice9 * g_pD3Device = NULL;

static LRESULT CALLBACK DirectX9CallbackFunc(HWND _hWindow, UINT _uMessage, WPARAM _wParam, LPARAM _lParam)
{
	if(_uMessage == WM_CLOSE)
	{
		PostQuitMessage(0);
		return TRUE;
	}

	return DefWindowProc(_hWindow, _uMessage, _wParam, _lParam);
}

struct SColoredVertex
{
	float m_fX, m_fY, m_fZ;
	DWORD m_dwColor;

	const static DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

static void Init()
{
	HRESULT hr;

	hr = g_pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if(FAILED(hr)) cout << "set cull: " << hr << endl;

	hr = g_pD3Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr)) cout << "set lightning: " << hr << endl;
}

static void OnNextFrame()
{
	HRESULT hr;

	hr = g_pD3Device->BeginScene();
	if(FAILED(hr)) cout << "begin: " << hr << endl;

	hr = g_pD3Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if(FAILED(hr)) cout << "clear: " << hr << endl;

	SColoredVertex vertices[3];
	ZeroMemory(vertices, sizeof(vertices));

	const float fZ = 0.5f;
	const DWORD dwColor = 0xffffffl;

	// Upper left.
	vertices[0].m_fX = -0.5f;
	vertices[0].m_fY =  0.5f;
	vertices[0].m_fZ = fZ;
	vertices[0].m_dwColor = dwColor;

	// Lower left.
	vertices[1].m_fX = -0.5f;
	vertices[1].m_fY = -0.5f;
	vertices[1].m_fZ = fZ;
	vertices[1].m_dwColor = dwColor;

	// Upper right.
	vertices[2].m_fX = 0.5f;
	vertices[2].m_fY = 0.5f;
	vertices[2].m_fZ = fZ;
	vertices[2].m_dwColor = dwColor;

	hr = g_pD3Device->SetFVF(SColoredVertex::FVF);
	if(FAILED(hr)) cout << "set fvf: " << hr << endl;

	hr = g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(SColoredVertex));
	if(FAILED(hr)) cout << "draw UP: " << hr << endl;

	hr = g_pD3Device->EndScene();
	if(FAILED(hr)) cout << "end: " << hr << endl;

	hr = g_pD3Device->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hr)) cout << "present: " << hr << endl;
}

static void CleanUp()
{
}

void RunDlgDirectX9VerySimple()
{
	SetupWindow(L"D3D9 test window", WND_WINDOWED_DC, DirectX9CallbackFunc);

	IDirect3D9 * pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(pD3D9 == NULL) cout << "Failed to create Direct3D" << endl;

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
	hr = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParams, &g_pD3Device);
	if(FAILED(hr)) cout << "create device: " << hr << endl;

	Init();
	RunMainLoop(OnNextFrame);

	hr = g_pD3Device->Release();
	if(FAILED(hr)) cout << "release device: " << hr << endl;

	hr = pD3D9->Release();
	if(FAILED(hr)) cout << "release d3d: " << hr << endl;
}
