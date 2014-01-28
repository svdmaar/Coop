#include "D3dDemos.h"

#include "VertexFormats.h"
#include "../../Coop/Bitmap.h"
#include "SingleSizeD3dFont.h"

#include <iostream>
#include <sstream>
using namespace std;

static CSingleSizeD3dFont g_font;
static DWORD g_dwLastFrame = 0;

const int g_iTextureSize = 20;
const int g_iTextureCount = 20;
static CBitmap g_bmSprites[g_iTextureCount];

static bool Init()
{
	HRESULT hr;

	RECT rClient;
	GetClientRect(g_hWindow, &rClient);
	int iClientHeight = rClient.bottom - rClient.top;
	int iClientWidth = rClient.right - rClient.left;

	hr = g_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if(FAILED(hr)) cout << "set cull: " << hr << endl;

	hr = g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr)) cout << "set lightning: " << hr << endl;

	POINT pScreenSize;
	pScreenSize.x = iClientWidth;
	pScreenSize.y = iClientHeight;
	if(!g_font.Init("fonts/Arial_Black_100", pScreenSize, 0x40))
	{
		cout << "Failed to init font" << endl;
	}

	return true;
}

static bool Render()
{
	HRESULT hr;

	hr = g_pD3dDevice->BeginScene();
	if(FAILED(hr)) cout << "begin: " << hr << endl;

	hr = g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if(FAILED(hr)) cout << "clear: " << hr << endl;

	DWORD dwCurrentTime = GetTickCount();
	DWORD dwDiff = dwCurrentTime - g_dwLastFrame;
	int iFrameRate = 0;

	if(dwDiff > 0)
	{
		iFrameRate = 1000 / (dwCurrentTime - g_dwLastFrame);
	}

	stringstream ssText;
	ssText << iFrameRate;

	g_dwLastFrame = dwCurrentTime;

	POINT pPixel;
	pPixel.x = 0;
	pPixel.y = 0;
	if(!g_font.RenderString(ssText.str(), pPixel))
	{
		cout << "Failed to render string" << endl;
	}

	hr = g_pD3dDevice->EndScene();
	if(FAILED(hr)) cout << "end: " << hr << endl;

	hr = g_pD3dDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hr)) cout << "present: " << hr << endl;

	return true;
}

static bool CleanUp()
{

	return true;
}

SD3dDemo GetAlphaBlendingDemo2()
{
	SD3dDemo demo;

	demo.m_sName = "Alpha Blending Demo 2";
	demo.m_pInit = Init;
	demo.m_pRender = Render;
	demo.m_pCleanUp = CleanUp;

	return demo;
}
