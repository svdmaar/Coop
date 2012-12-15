#include "VertexBuffer.h"

#include "GraphicsManager.h"
#include "Logger.h"

#include <assert.h>

CVertexBuffer::CVertexBuffer()
{
	m_bInited = false;
	m_pSystemMemBuffer = NULL;
	m_vertexSize = 0;
	m_iVertexCount = 0;
	m_iPrimitiveCount = 0;
	m_pVideoMemBuffer = NULL;
	m_pTexture = NULL;
	m_ePrimitiveType = D3DPT_FORCE_DWORD; // invalid state.
	m_dwFvf = 0;
}

CVertexBuffer::~CVertexBuffer()
{
	assert(!m_bInited);
}

void CVertexBuffer::_CalcVertexSize()
{
	m_vertexSize = 0;

	if(m_dwFvf & D3DFVF_XYZ)
	{
		m_vertexSize += 3 * sizeof(float);
	}

	if(m_dwFvf & D3DFVF_NORMAL)
	{
		m_vertexSize += 3 * sizeof(float);
	}

	if(m_dwFvf & D3DFVF_DIFFUSE)
	{
		m_vertexSize += 1 * sizeof(DWORD);
	}

	if(m_dwFvf & D3DFVF_TEX1)
	{
		m_vertexSize += 2 * sizeof(float);
	}
}

void CVertexBuffer::_SetPosition(int _iVertexIndex, const D3DXVECTOR3& _vPos)
{
	assert((m_dwFvf | D3DFVF_XYZ) != 0);

	BYTE *pBytes = (BYTE *)_GetSystemMemBuffer();
	BYTE *pVertex = pBytes + _iVertexIndex * m_vertexSize;
	D3DXVECTOR3& vCoor = *(D3DXVECTOR3 *)pVertex;

	vCoor = _vPos;
}

void CVertexBuffer::_SetTexCoor(int _iVertexIndex, const D3DXVECTOR2& _vTexCoor)
{
	assert((m_dwFvf | D3DFVF_XYZ) != 0);
	assert((m_dwFvf | D3DFVF_TEX1) != 0);

	BYTE *pBytes = (BYTE *)_GetSystemMemBuffer();
	BYTE *pVertex = pBytes + _iVertexIndex * m_vertexSize;
	BYTE *pTexCoors = pVertex + 3 * sizeof(float);
	D3DXVECTOR2& vTexCoor = *(D3DXVECTOR2 *)pTexCoors;

	vTexCoor = _vTexCoor;
}

void CVertexBuffer::_SetColor(int _iVertexIndex, DWORD _dwColor)
{
	assert((m_dwFvf | D3DFVF_XYZ) != 0);
	assert((m_dwFvf | D3DFVF_DIFFUSE) != 0);
	
	assert((m_dwFvf & D3DFVF_TEX1) == 0);

	int inVertexOffset = 0;

	if(m_dwFvf | D3DFVF_XYZ)
	{
		inVertexOffset += 3 * (int)sizeof(float);
	}

	if(m_dwFvf & D3DFVF_NORMAL)
	{
		inVertexOffset += 3 * (int)sizeof(float);
	}

	BYTE *pBytes = (BYTE *)_GetSystemMemBuffer();
	BYTE *pVertex = pBytes + _iVertexIndex * m_vertexSize;
	BYTE *pColor = pVertex + inVertexOffset;
	DWORD& dwColor = *(DWORD *)pColor;

	dwColor = _dwColor;
}

bool CVertexBuffer::_Init(unsigned int _iVertexCount, DWORD _dwFvf, D3DPRIMITIVETYPE _ePrimitiveType)
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	assert(!m_bInited);
	assert(m_pSystemMemBuffer == NULL);
	assert(m_pVideoMemBuffer == NULL);

	m_iVertexCount = _iVertexCount;
	m_dwFvf = _dwFvf;
	m_ePrimitiveType = _ePrimitiveType;

	_CalcVertexSize();

	int iBufferSize = (int)(m_vertexSize * m_iVertexCount);
	m_pSystemMemBuffer = (void *)new BYTE[iBufferSize];

	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	hr = pDevice->CreateVertexBuffer(iBufferSize, 0, m_dwFvf, D3DPOOL_DEFAULT, &m_pVideoMemBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer", hr);
		return false;
	}

	m_bInited = true;
	return true;
}

bool CVertexBuffer::_SetTexture(const CBitmap& _bitmap)
{
	assert(m_bInited);
	assert(m_pTexture == NULL);

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	if(!pGraphicsManager->CreateTexture(_bitmap, &m_pTexture))
	{
		return false;
	}

	return true;
}

void *CVertexBuffer::_GetSystemMemBuffer()
{
	return m_pSystemMemBuffer;
}

bool CVertexBuffer::_UploadBuffer()
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	int iBufferSize = (int)(m_vertexSize * m_iVertexCount);
	void *pLockedBuffer = NULL;
	hr = m_pVideoMemBuffer->Lock(0, iBufferSize, &pLockedBuffer, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock buffer", hr);
		return false;
	}

	memcpy(pLockedBuffer, m_pSystemMemBuffer, iBufferSize);

	hr = m_pVideoMemBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock buffer", hr);
		return false;
	}

	return true;
}

bool CVertexBuffer::_SetRectangle(const SFloatRect& _pos, DWORD _dwColor /* = 0 */)
{
	assert(m_iVertexCount == 6);

	D3DXVECTOR3 vTopLeftPos(_pos.m_fLeft, _pos.m_fTop, 0.0f);
	D3DXVECTOR3 vTopRightPos(_pos.m_fRight, _pos.m_fTop, 0.0f);
	D3DXVECTOR3 vBottomLeftPos(_pos.m_fLeft, _pos.m_fBottom, 0.0f);
	D3DXVECTOR3 vBottomRightPos(_pos.m_fRight, _pos.m_fBottom, 0.0f);

	D3DXVECTOR2 vTopLeftTex(0.0f, 0.0f);
	D3DXVECTOR2 vTopRightTex(1.0f, 0.0f);
	D3DXVECTOR2 vBottomLeftTex(0.0f, 1.0f);
	D3DXVECTOR2 vBottomRightTex(1.0f, 1.0f);
	
	_SetPosition(0, vTopLeftPos);
	_SetPosition(1, vTopRightPos);
	_SetPosition(2, vBottomRightPos);
	_SetPosition(3, vTopLeftPos);
	_SetPosition(4, vBottomRightPos);
	_SetPosition(5, vBottomLeftPos);

	if(m_dwFvf & D3DFVF_TEX1)
	{
		_SetTexCoor(0, vTopLeftTex);
		_SetTexCoor(1, vTopRightTex);
		_SetTexCoor(2, vBottomRightTex);
		_SetTexCoor(3, vTopLeftTex);
		_SetTexCoor(4, vBottomRightTex);
		_SetTexCoor(5, vBottomLeftTex);
	}

	if(m_dwFvf & D3DFVF_DIFFUSE)
	{
		for(int iIndex = 0; iIndex < 6; iIndex++)
		{
			_SetColor(iIndex, _dwColor);
		}
	}

	m_iPrimitiveCount = 2;

	return true;
}

bool CVertexBuffer::Render() const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	if(m_pTexture != NULL)
	{
		hr = pDevice->SetTexture(0, m_pTexture);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to set texture", hr);
			return false;
		}
	}

	hr = pDevice->SetStreamSource(0, m_pVideoMemBuffer, 0, m_vertexSize);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source", hr);
		return false;
	}

	hr = pDevice->SetFVF(m_dwFvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(m_ePrimitiveType, 0, m_iPrimitiveCount);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw primitive", hr);
		return false;
	}

	if(m_pTexture != NULL)
	{
		hr = pDevice->SetTexture(0, NULL);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to reset texture", hr);
			return false;
		}
	}

	return true;
}

bool CVertexBuffer::CleanUp()
{
	assert(m_bInited);

	if(m_pVideoMemBuffer != NULL)
	{
		m_pVideoMemBuffer->Release();
		m_pVideoMemBuffer = NULL;
	}

	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if(m_pSystemMemBuffer != NULL)
	{
		delete [] m_pSystemMemBuffer;
		m_pSystemMemBuffer = NULL;
	}

	m_bInited = false;

	return true;
}

CColoredVertexBuffer::CColoredVertexBuffer()
{
}

CColoredVertexBuffer::~CColoredVertexBuffer()
{
}

bool CColoredVertexBuffer::Init(const SFloatRect& _pos, DWORD _dwColor)
{
	if(!_Init(6, SColoredVertex::_fvf, D3DPT_TRIANGLELIST))
	{
		return false;
	}

	if(!_SetRectangle(_pos, _dwColor))
	{
		return false;
	}

	if(!_UploadBuffer())
	{
		return false;
	}

	return true;
}

CTexturedVertexBuffer::CTexturedVertexBuffer()
{
}

CTexturedVertexBuffer::~CTexturedVertexBuffer()
{
}

bool CTexturedVertexBuffer::Init(const CBitmap& _bitmap, const SFloatRect& _pos)
{
	if(!_Init(6, STexturedVertex::_fvf, D3DPT_TRIANGLELIST))
	{
		return false;
	}

	if(!_SetRectangle(_pos))
	{
		return false;
	}

	if(!_SetTexture(_bitmap))
	{
		return false;
	}

	if(!_UploadBuffer())
	{
		return false;
	}

	return true;
}


/*
 * Supported FVFs (flexible vertex formats):
 *
 * FVF bits set   :  vertex struct members          :  added size*
 * 
 * D3DFVF_XYZ     :  float x, y, and z coordinates  :  12
 * D3DFVF_NORMAL  :  float nx, ny, and nz dir       :  12
 * D3DFVF_DIFFUSE :  DWORD color                    :  4
 * D3DFVF_TEX1    :  float u and v coordinates      :  8
 *
 * *sizeof(float) == sizeof(DWORD) == 4
 */

// TODO: unneeded setting of FVFs on device causes extra delay?
