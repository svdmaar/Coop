#include "D3dDemos.h"

#include "VertexFormats.h"
#include "../../Coop/Bitmap.h"
#include "SingleSizeD3dFont.h"

#include <iostream>
#include <sstream>
using namespace std;

CSingleSizeD3dFont g_font;

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
	if(!g_font.Init("fonts/Arial_Black_100", pScreenSize))
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

	static int nFrames = 0;
	stringstream ssText;
	ssText << "I love Fernanda" << endl;
	ssText << nFrames;

	nFrames++;

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

SD3dDemo GetFirstFontDemo()
{
	SD3dDemo demo;

	demo.m_sName = "First Font Demo";
	demo.m_pInit = Init;
	demo.m_pRender = Render;
	demo.m_pCleanUp = CleanUp;

	return demo;
}
