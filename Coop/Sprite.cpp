#include "Sprite.h"

#include "GraphicsManager.h"
#include "Logger.h"
#include "VertexTypes.h"

#include <cassert>

using namespace std;

const int g_iPixelsPerUnit = 4;
static const DWORD g_dwBackgroundColor = 0xff00ff; // TODO: move sprite consts to own header file

CSprite::CSprite(const std::string& _sName)
{
	m_bInited = false;
	m_sName = _sName;

	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
}

CSprite::CSprite()
{
	m_bInited = false;
	m_sName = "";

	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
}

CSprite::~CSprite()
{
	assert(!m_bInited);
}

bool CSprite::_CreateBuffer(const SFloatPoint& _fpSize)
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	UINT bufferSize = sizeof(SFaceVertex) * 6;
	hr = pDevice->CreateVertexBuffer(bufferSize, 0, SFaceVertex::_fvf, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for image rectangle", hr);
		return false;
	}

	SFaceVertex *pVertices = NULL;
	hr = m_pVertexBuffer->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock image rectangle vertex buffer", hr);
		return false;
	}

	SFloatPoint fpHalfSize;
	fpHalfSize.m_fX = 0.5f * _fpSize.m_fX;
	fpHalfSize.m_fY = 0.5f * _fpSize.m_fY;

	// Upper right triangle.
	// Upper left.
	pVertices[0].x = -fpHalfSize.m_fX;
	pVertices[0].y = fpHalfSize.m_fY;
	pVertices[0].z = 0.0f;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 0.0f;

	// Upper right.
	pVertices[1].x = fpHalfSize.m_fX;
	pVertices[1].y = fpHalfSize.m_fY;
	pVertices[1].z = 0.0f;
	pVertices[1].tu = 1.0f;
	pVertices[1].tv = 0.0f;

	// Lower right.
	pVertices[2].x = fpHalfSize.m_fX;
	pVertices[2].y = -fpHalfSize.m_fX;
	pVertices[2].z = 0.0f;
	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;

	// Lower left triangle.
	// Upper left.
	pVertices[3] = pVertices[0];

	// Lower left.
	pVertices[4].x = -fpHalfSize.m_fX;
	pVertices[4].y = -fpHalfSize.m_fX;
	pVertices[4].z = 0.0f;
	pVertices[4].tu = 0.0f;
	pVertices[4].tv = 1.0f;

	// Lower right.
	pVertices[5] = pVertices[2];

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock image rectangle vertex buffer", hr);
		return false;
	}

	return true;
}

bool CSprite::_CreateTexture(const CBitmap& _bitmap)
{
	HRESULT hr;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pD3Device = pGraphicsManager->GetDevice();

	int iWidth = _bitmap.GetWidth();
	int iHeight = _bitmap.GetHeight();

	IDirect3DTexture9 *pSystemTexture = NULL;

	hr = pD3Device->CreateTexture((UINT)iWidth, (UINT)iHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,
		&pSystemTexture, NULL);
	if(FAILED(hr))
	{
		LogError("Failed to create system texture");
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	memset(&lockedRect, 0, sizeof(lockedRect));

	hr = pSystemTexture->LockRect(0, &lockedRect, NULL, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock system texture rect", hr);
		return false;
	}

	DWORD *pTexLocation = (DWORD *)lockedRect.pBits;
	for(int iRowIndex = 0; iRowIndex < iHeight; iRowIndex++)
	{
		memcpy(pTexLocation, _bitmap.GetRow(iRowIndex), sizeof(DWORD) * iWidth);

		for(int iColumnIndex = 0; iColumnIndex < iWidth; iColumnIndex++)
		{
			DWORD dwPixel = pTexLocation[iColumnIndex];

			if(dwPixel != g_dwBackgroundColor)
			{
				dwPixel |= 0xff000000l;
				pTexLocation[iColumnIndex] = dwPixel;
			}
		}

		BYTE *pTextureByteLocation = (BYTE *)pTexLocation;
		pTextureByteLocation += lockedRect.Pitch;
		
		pTexLocation = (DWORD *)pTextureByteLocation;
	}

	hr = pSystemTexture->UnlockRect(0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock system texture rect", hr);
		return false;
	}

	hr = pD3Device->CreateTexture(iWidth, iHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create video texture", hr);
		return false;
	}

	hr = pD3Device->UpdateTexture(pSystemTexture, m_pTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to update video texture", hr);
		return false;
	}

	pSystemTexture->Release();

	return true;
}

bool CSprite::_CreateBufferAndTexture(const CBitmap& _bitmap)
{
	SFloatPoint fpSize;
	fpSize.m_fX = _bitmap.GetWidth() / (float)g_iPixelsPerUnit;
	fpSize.m_fY = _bitmap.GetHeight() / (float)g_iPixelsPerUnit;

	if(!_CreateBuffer(fpSize))
	{
		return false;
	}

	if(!_CreateTexture(_bitmap))
	{
		return false;
	}

	return true;
}

bool CSprite::Init()
{
	assert(!m_bInited);
	assert(m_sName != "");

	string sFilename = "bitmap/" + m_sName + ".bmp";
	
	CBitmap bmText;
	if(!bmText.Load(sFilename))
	{
		LogError("Failed to read bitmap for sprite: " + m_sName);
		return false;
	}

	if(!_CreateBufferAndTexture(bmText))
	{
		LogError("Failed to create buffer and texture for sprite: " + m_sName);
		return false;
	}

	m_bInited = true;

	return true;
}

bool CSprite::Init(const CBitmap & _bitmap)
{
	assert(!m_bInited);
	assert(m_sName == "");

	if(!_CreateBufferAndTexture(_bitmap))
	{
		LogError("Failed to create buffer and texture for bitmap sprite");
		return false;
	}

	m_bInited = true;

	return true;
}

bool CSprite::Render(const SFloatPoint & _pos) const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	D3DXMATRIX matWorld, matTranslation;

	hr = pDevice->GetTransform(D3DTS_WORLD, &matWorld);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get world transform", hr);
		return false;
	}

	D3DXMatrixTranslation(&matTranslation, _pos.m_fX, _pos.m_fY, 0.0f);
	hr = pDevice->MultiplyTransform(D3DTS_WORLD, &matTranslation);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to multiply world transform", hr);
		return false;
	}

	hr = pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SFaceVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source", hr);
		return false;
	}

	hr = pDevice->SetFVF(SFaceVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF", hr);
		return false;
	}

	hr = pDevice->SetTexture(0, m_pTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set texture", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw primitive", hr);
		return false;
	}

	hr = pDevice->SetTexture(0, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to reset texture", hr);
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to restore world transform", hr);
		return false;
	}

	return true;
}

bool CSprite::CleanUp()
{
	assert(m_bInited);

	m_pVertexBuffer->Release();
	m_pTexture->Release();

	m_bInited = false;

	return true;
}
