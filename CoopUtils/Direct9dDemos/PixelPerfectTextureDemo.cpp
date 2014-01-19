#include "D3dDemos.h"

#include "VertexFormats.h"
#include "../../Coop/Bitmap.h"

#include <iostream>
using namespace std;

static IDirect3DVertexBuffer9 * g_pVertexBuffer = NULL;
static IDirect3DTexture9 * g_pVideoTexture = NULL;

static const int g_nVertices = 6;
static const int g_iTextureHeight = 1200 / 2;
static const int g_iTextureWidth = 1920 / 2;

static bool Init()
{
	HRESULT hr;

	RECT rClient;
	GetClientRect(g_hWindow, &rClient);
	int iClientHeight = rClient.bottom - rClient.top;
	int iClientWidth = rClient.right - rClient.left;

	float fPixelWidth = 2.0f / (float)iClientWidth;
	float fPixelHeight = 2.0f / (float)iClientHeight;

	hr = g_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if(FAILED(hr)) cout << "set cull: " << hr << endl;

	hr = g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr)) cout << "set lightning: " << hr << endl;

	const UINT uBufferLength = sizeof(STexturedVertex) * g_nVertices;
	hr = g_pD3dDevice->CreateVertexBuffer(uBufferLength, 0, STexturedVertex::FVF, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	if(FAILED(hr)) cout << "create vertex buffer: " << hr << endl;

	STexturedVertex * pBuffer = NULL;
	hr = g_pVertexBuffer->Lock(0, 0, (void **)&pBuffer, 0);
	if(FAILED(hr)) cout << "lock vertex buffer: " << hr << endl;

	const float fZ = 0.5f;

	float fLeft = -1.0f - 0.5f * fPixelWidth;
	float fTop = 1.0f + 0.5f * fPixelHeight;
	float fRight = fLeft + fPixelWidth * (float)g_iTextureWidth;
	float fBottom = fTop - fPixelHeight * (float)g_iTextureHeight;

	// Upper left.
	pBuffer[0].m_fX = fLeft;
	pBuffer[0].m_fY = fTop;
	pBuffer[0].m_fZ = fZ;
	pBuffer[0].m_fU = 0.0f;
	pBuffer[0].m_fV = 0.0f;

	// Lower left.
	pBuffer[1].m_fX = fLeft;
	pBuffer[1].m_fY = fBottom;
	pBuffer[1].m_fZ = fZ;
	pBuffer[1].m_fU = 0.0f;
	pBuffer[1].m_fV = 1.0f;

	// Upper right.
	pBuffer[2].m_fX = fRight;
	pBuffer[2].m_fY = fTop;
	pBuffer[2].m_fZ = fZ;
	pBuffer[2].m_fU = 1.0f;
	pBuffer[2].m_fV = 0.0f;

	// Upper right.
	pBuffer[3] = pBuffer[2];

	// Lower left.
	pBuffer[4] = pBuffer[1];

	// Lower right.
	pBuffer[5].m_fX = fRight;
	pBuffer[5].m_fY = fBottom;
	pBuffer[5].m_fZ = fZ;
	pBuffer[5].m_fU = 1.0f;
	pBuffer[5].m_fV = 1.0f;

	hr = g_pVertexBuffer->Unlock();
	if(FAILED(hr)) cout << "unlock vertex buffer: " << hr << endl;

	IDirect3DTexture9 * pSystemTexture = NULL;
	hr = g_pD3dDevice->CreateTexture(g_iTextureWidth, g_iTextureHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pSystemTexture, NULL);
	if(FAILED(hr)) cout << "create system texture: " << hr << endl;

	D3DLOCKED_RECT lockedRect;
	ZeroMemory(&lockedRect, sizeof(lockedRect));

	const UINT uLevel = 0;
	hr = pSystemTexture->LockRect(uLevel, &lockedRect, NULL, 0);
	if(FAILED(hr)) cout << "lock texture rect: " << hr << endl;

	BYTE * pbBits = (BYTE *)lockedRect.pBits;
	for(int iRow = 0; iRow < g_iTextureHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < g_iTextureWidth; iColumn++)
		{
			bool bRed = ((iRow + iColumn) % 2) == 0;
			DWORD dwPixel = bRed ? 0xff0000 : 0xff00;

			int iTargetIndex = iRow * lockedRect.Pitch + iColumn * sizeof(DWORD);
			DWORD * pTarget = (DWORD *)(pbBits + iTargetIndex);

			*pTarget = dwPixel;
		}
	}

	hr = pSystemTexture->UnlockRect(uLevel);
	if(FAILED(hr)) cout << "unlock texture rect: " << hr << endl;

	hr = g_pD3dDevice->CreateTexture(g_iTextureWidth, g_iTextureHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &g_pVideoTexture, NULL);
	if(FAILED(hr)) cout << "create video texture: " << hr << endl;

	hr = g_pD3dDevice->UpdateTexture(pSystemTexture, g_pVideoTexture);
	if(FAILED(hr)) cout << "update texture: " << hr << endl;

	hr = pSystemTexture->Release();
	if(FAILED(hr)) cout << "release system texture: " << hr << endl;

	return true;
}

static bool Render()
{
	HRESULT hr;

	hr = g_pD3dDevice->BeginScene();
	if(FAILED(hr)) cout << "begin: " << hr << endl;

	hr = g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if(FAILED(hr)) cout << "clear: " << hr << endl;

	hr = g_pD3dDevice->SetFVF(STexturedVertex::FVF);
	if(FAILED(hr)) cout << "set fvf: " << hr << endl;

	hr = g_pD3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(STexturedVertex));
	if(FAILED(hr)) cout << "set stream source: " << hr << endl;

	hr = g_pD3dDevice->SetTexture(0, g_pVideoTexture);
	if(FAILED(hr)) cout << "set texture: " << hr << endl;

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

	hr = g_pVideoTexture->Release();
	if(FAILED(hr)) cout << "Video Texture release: " << hr << endl;

	hr = g_pVertexBuffer->Release();
	if(FAILED(hr)) cout << "Vertex buffer release: " << hr << endl;

	return true;
}

SD3dDemo GetPixelPerfectTextureDemo()
{
	SD3dDemo demo;

	demo.m_sName = "Pixel Perfect Texture Demo";
	demo.m_pInit = Init;
	demo.m_pRender = Render;
	demo.m_pCleanUp = CleanUp;

	return demo;
}
