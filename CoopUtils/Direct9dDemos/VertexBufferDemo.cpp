#include "D3dDemos.h"

#include "VertexFormats.h"

#include <iostream>
using namespace std;

static IDirect3DVertexBuffer9 * g_pVertexBuffer = NULL;

static const int g_nVertices = 6;

static bool Init()
{
	HRESULT hr;

	hr = g_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if(FAILED(hr)) cout << "set cull: " << hr << endl;

	hr = g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr)) cout << "set lightning: " << hr << endl;

	const UINT uBufferLength = sizeof(SColoredVertex) * g_nVertices;
	hr = g_pD3dDevice->CreateVertexBuffer(uBufferLength, 0, SColoredVertex::FVF, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	if(FAILED(hr)) cout << "create vertex buffer: " << hr << endl;

	SColoredVertex * pBuffer = NULL;
	hr = g_pVertexBuffer->Lock(0, 0, (void **)&pBuffer, 0);
	if(FAILED(hr)) cout << "lock vertex buffer: " << hr << endl;

	const float fZ = 0.5f;
	const DWORD dwColor = 0xffffffl;

	// Upper left.
	pBuffer[0].m_fX = -0.5f;
	pBuffer[0].m_fY =  0.5f;
	pBuffer[0].m_fZ = fZ;
	pBuffer[0].m_dwColor = dwColor;

	// Lower left.
	pBuffer[1].m_fX = -0.5f;
	pBuffer[1].m_fY = -0.5f;
	pBuffer[1].m_fZ = fZ;
	pBuffer[1].m_dwColor = dwColor;

	// Upper right.
	pBuffer[2].m_fX = 0.5f;
	pBuffer[2].m_fY = 0.5f;
	pBuffer[2].m_fZ = fZ;
	pBuffer[2].m_dwColor = dwColor;

	// Upper right.
	pBuffer[3].m_fX = 0.5f;
	pBuffer[3].m_fY = 0.5f;
	pBuffer[3].m_fZ = fZ;
	pBuffer[3].m_dwColor = dwColor;

	// Lower left.
	pBuffer[4].m_fX = -0.5f;
	pBuffer[4].m_fY = -0.5f;
	pBuffer[4].m_fZ = fZ;
	pBuffer[4].m_dwColor = dwColor;

	// Lower right.
	pBuffer[5].m_fX =  0.5f;
	pBuffer[5].m_fY = -0.5f;
	pBuffer[5].m_fZ = fZ;
	pBuffer[5].m_dwColor = dwColor;

	hr = g_pVertexBuffer->Unlock();
	if(FAILED(hr)) cout << "unlock vertex buffer: " << hr << endl;

	return true;
}

static bool Render()
{
	HRESULT hr;

	hr = g_pD3dDevice->BeginScene();
	if(FAILED(hr)) cout << "begin: " << hr << endl;

	hr = g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if(FAILED(hr)) cout << "clear: " << hr << endl;

	hr = g_pD3dDevice->SetFVF(SColoredVertex::FVF);
	if(FAILED(hr)) cout << "set fvf: " << hr << endl;

	hr = g_pD3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(SColoredVertex));
	if(FAILED(hr)) cout << "set strem source: " << hr << endl;

	hr = g_pD3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	if(FAILED(hr)) cout << "draw primitive: " << hr << endl;

	hr = g_pD3dDevice->EndScene();
	if(FAILED(hr)) cout << "end: " << hr << endl;

	hr = g_pD3dDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hr)) cout << "present: " << hr << endl;

	return true;
}

static bool CleanUp()
{
	HRESULT hr;

	hr = g_pVertexBuffer->Release();
	if(FAILED(hr)) cout << "Vertex buffer release: " << hr << endl;

	return true;
}

SD3dDemo GetVertexBufferDemo()
{
	SD3dDemo demo;

	demo.m_sName = "Vertex Buffer Demo";
	demo.m_pInit = Init;
	demo.m_pRender = Render;
	demo.m_pCleanUp = CleanUp;

	return demo;
}
