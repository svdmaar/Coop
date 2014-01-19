#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool SetupWindow();
bool CleanUpWindow();

extern IDirect3DDevice9 * g_pD3dDevice;
extern HWND g_hWindow;
