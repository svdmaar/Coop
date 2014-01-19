#include "D3dDemos.h"

#include "VertexFormats.h"

#include <iostream>
using namespace std;

static bool Init()
{
	HRESULT hr;

	hr = g_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if(FAILED(hr)) cout << "set cull: " << hr << endl;

	hr = g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr)) cout << "set lightning: " << hr << endl;

	return true;
}

static bool Render()
{
	HRESULT hr;

	hr = g_pD3dDevice->BeginScene();
	if(FAILED(hr)) cout << "begin: " << hr << endl;

	hr = g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
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

	hr = g_pD3dDevice->SetFVF(SColoredVertex::FVF);
	if(FAILED(hr)) cout << "set fvf: " << hr << endl;

	hr = g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(SColoredVertex));
	if(FAILED(hr)) cout << "draw UP: " << hr << endl;

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

SD3dDemo GetSimpleDemo()
{
	SD3dDemo demo;

	demo.m_sName = "Simple Demo";
	demo.m_pInit = Init;
	demo.m_pRender = Render;
	demo.m_pCleanUp = CleanUp;

	return demo;
}
