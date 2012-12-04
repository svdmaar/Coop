#include "PictureBackground.h"

#include "GraphicsManager.h"
#include "Logger.h"
#include "Gui.h"
#include "VertexTypes.h"

#include <assert.h>

using namespace std;

CPictureBackground::CPictureBackground()
{
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
}

CPictureBackground::~CPictureBackground()
{
}

bool CPictureBackground::Init(string _bmpFileName)
{
	HRESULT hr;

	SFloatRect rScreen = CGui::GetScreenRect();
	
	if(!CGuiElement::Init(rScreen))
	{
		return false;
	}

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	CBitmap inBmp(_bmpFileName);

	if(!pGraphicsManager->CreateTexture(inBmp, &m_pTexture))
	{
		LogError("Failed to create texture for background image");
		return false;
	}

	UINT bufferSize = sizeof(SFaceVertex) * 6;
	hr = pDevice->CreateVertexBuffer(bufferSize, 0, SFaceVertex::_fvf, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create background vertex buffer", hr);
		return false;
	}

	SFaceVertex *pVertices = NULL;
	hr = m_pVertexBuffer->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock background vertex buffer", hr);
		return false;
	}

	// Upper right triangle.
	// Upper left.
	pVertices[0].x = rScreen.m_fLeft;
	pVertices[0].y = rScreen.m_fTop;
	pVertices[0].z = 0.0f;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 0.0f;

	// Upper right.
	pVertices[1].x = rScreen.m_fRight;
	pVertices[1].y = rScreen.m_fTop;
	pVertices[1].z = 0.0f;
	pVertices[1].tu = 1.0f;
	pVertices[1].tv = 0.0f;

	// Lower right.
	pVertices[2].x = rScreen.m_fRight;
	pVertices[2].y = rScreen.m_fBottom;
	pVertices[2].z = 0.0f;
	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;

	// Lower left triangle.
	// Upper left.
	pVertices[3] = pVertices[0];

	// Lower left.
	pVertices[4].x = rScreen.m_fLeft;
	pVertices[4].y = rScreen.m_fBottom;
	pVertices[4].z = 0.0f;
	pVertices[4].tu = 0.0f;
	pVertices[4].tv = 1.0f;

	// Lower right.
	pVertices[5] = pVertices[2];

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock background vertex buffer", hr);
		return false;
	}

	return true;
}

bool CPictureBackground::CleanUp()
{
	m_pVertexBuffer->Release();
	m_pTexture->Release();

	if(!CGuiElement::CleanUp())
	{
		return false;
	}

	return true;
}

bool CPictureBackground::Render() const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	hr = pDevice->SetTexture(0, m_pTexture);
	if(FAILED(hr))
	{
		LogError("Failed to set texture for background image");
		return false;
	}

	hr = pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SFaceVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set background image stream source", hr);
		return false;
	}

	hr = pDevice->SetFVF(SFaceVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for background image", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw primitive for background image", hr);
		return false;
	}

	hr = pDevice->SetTexture(0, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to reset texture for background image", hr);
		return false;
	}

	return true;
}
