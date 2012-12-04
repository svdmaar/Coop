#pragma once

#include "Font.h"
#include "FrameTimer.h"

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class CGraphicsManager
{
	bool m_bInited;
	HWND m_hWnd;
	bool m_bFullScreen;
	int m_iWindowWidth;
	int m_iWindowHeight;
	IDirect3DDevice9 *m_pD3Device;
	IDirect3D9 *m_pDirect3D9;
	bool m_bShouldQuit;

	CFont m_arial;
	CFrameTimer m_frameTimer;

	static LRESULT WINAPI _WindowProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam);

	static CGraphicsManager *m_pInstance;

public:
	CGraphicsManager();
	~CGraphicsManager();

	bool Init();
	bool CleanUp();
	bool IsInited();

	HWND GetWindowHandle();
	int GetWindowWidth();
	int GetWindowHeight();
	IDirect3DDevice9 *GetDevice();
	IDirect3D9 *GetDirect3D();
	bool IsFullscreen();
	bool ShouldQuit();
	void SetShouldQuit();

	const CFont *GetArial();
	bool CheckDeviceLostStatus();
	bool CreateTexture(const CBitmap& _bitmap, IDirect3DTexture9 ** _ppTexture);
	float GetAspectRatio();

	bool ProcessWindowMessages();
	bool MakeScreenshot();
	bool BeginScene();
	bool EndScene();

	static CGraphicsManager *GetInstance();
};
