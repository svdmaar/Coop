#include "Objects2D.h"

#include "VertexTypes.h"
#include "Logger.h"

#include <cassert>

CObject2D::CObject2D()
{
	m_bInited = false;
	ZeroMemory(&m_desc, sizeof(m_desc));
	ZeroMemory(&m_pos, sizeof(m_pos));
}

CObject2D::~CObject2D()
{
	assert(!m_bInited);
}

bool CObject2D::_Init(const SObject2DDesc& _desc, const SFloatRect _pos)
{
	m_bInited = true;
	m_desc = _desc;
	m_pos = _pos;

	return true;
}

bool CObject2D::_CleanUp()
{
	m_bInited = false;

	return true;
}

Object2DType CObject2D::_GetType() const
{
	return m_desc.m_eType;
}

const SObject2DDesc& CObject2D::_GetDesc() const
{
	return m_desc;
}

const SFloatRect& CObject2D::_GetPos() const
{
	return m_pos;
}

bool CObject2D::IsInited() const
{
	return m_bInited;
}

bool CObject2D::_RenderVertexBuffer(IDirect3DVertexBuffer9 *_pVertexBuffer, size_t _vertexSize, DWORD _fvf, D3DPRIMITIVETYPE _type, UINT _iCount) const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	hr = pDevice->SetStreamSource(0, _pVertexBuffer, 0, _vertexSize);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source", hr);
		return false;
	}

	hr = pDevice->SetFVF(_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(_type, 0, _iCount);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw primitive", hr);
		return false;
	}

	return true;
}

CRectObject::CRectObject() : CObject2D()
{
	m_pInnerBuffer = NULL;
	m_pFrameBuffer = NULL;
}

CRectObject::~CRectObject()
{
	assert(!IsInited());
}

bool CRectObject::Init(const SObject2DDesc& _desc, const SFloatRect& _pos)
{
	if(!CObject2D::_Init(_desc, _pos))
	{
		return false;
	}

	if(_desc.m_eType & otInner)
	{
		if(!_CreateInnerBuffer())
		{
			return false;
		}
	}

	return true;
}

bool CRectObject::Render() const
{
	if(_GetType() & otInner)
	{
		if(!_RenderVertexBuffer(m_pInnerBuffer, sizeof(SColoredVertex), SColoredVertex::_fvf, D3DPT_TRIANGLELIST, 2))
		{
			return false;
		}
	}

	if(_GetType() & otFrame)
	{
		if(!_RenderVertexBuffer(m_pFrameBuffer, sizeof(SColoredVertex), SColoredVertex::_fvf, D3DPT_LINELIST, 4))
		{
			return false;
		}
	}

	return true;
}

bool CRectObject::CleanUp()
{
	assert(IsInited());

	if(m_pInnerBuffer != NULL)
	{
		m_pInnerBuffer->Release();
		m_pInnerBuffer = NULL;
	}

	if(m_pFrameBuffer != NULL)
	{
		m_pFrameBuffer->Release();
		m_pFrameBuffer = NULL;
	}

	if(!CObject2D::_CleanUp())
	{
		return false;
	}

	return true;
}

bool CRectObject::_CreateInnerBuffer()
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	UINT bufferSize = sizeof(SColoredVertex) * 6;
	hr = pDevice->CreateVertexBuffer(bufferSize, 0, SColoredVertex::_fvf, D3DPOOL_DEFAULT, &m_pInnerBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for colored rectangle", hr);
		return false;
	}

	SColoredVertex *pVertices = NULL;
	hr = m_pInnerBuffer->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock colored rectangle vertex buffer", hr);
		return false;
	}

	SFloatRect pos = _GetPos();

	// Upper right triangle.
	// Upper left.
	pVertices[0].x = pos.m_fLeft;
	pVertices[0].y = pos.m_fTop;
	pVertices[0].z = 0.0f;
	pVertices[0].color = _GetDesc().m_dwInnerColor;

	// Upper right.
	pVertices[1].x = pos.m_fRight;
	pVertices[1].y = pos.m_fTop;
	pVertices[1].z = 0.0f;
	pVertices[1].color = _GetDesc().m_dwInnerColor;

	// Lower right.
	pVertices[2].x = pos.m_fRight;
	pVertices[2].y = pos.m_fBottom;
	pVertices[2].z = 0.0f;
	pVertices[2].color = _GetDesc().m_dwInnerColor;

	// Lower left triangle.
	// Upper left.
	pVertices[3] = pVertices[0];

	// Lower left.
	pVertices[4].x = pos.m_fLeft;
	pVertices[4].y = pos.m_fBottom;
	pVertices[4].z = 0.0f;
	pVertices[4].color = _GetDesc().m_dwInnerColor;

	// Lower right.
	pVertices[5] = pVertices[2];

	hr = m_pInnerBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock colored rectangle vertex buffer", hr);
		return false;
	}

	return true;
}

bool CRectObject::_CreateOuterBuffer()
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	UINT bufferSize = sizeof(SColoredVertex) * 8;
	hr = pDevice->CreateVertexBuffer(bufferSize, 0, SColoredVertex::_fvf, D3DPOOL_DEFAULT, &m_pFrameBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for colored frame", hr);
		return false;
	}

	SColoredVertex *pVertices = NULL;
	hr = m_pFrameBuffer->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock colored frame vertex buffer", hr);
		return false;
	}

	SFloatRect pos = _GetPos();

	// Upper line.
	// Left.
	pVertices[0].x = pos.m_fLeft;
	pVertices[0].y = pos.m_fTop;
	pVertices[0].z = 0.0f;
	pVertices[0].color = _GetDesc().m_dwInnerColor;

	// Right.
	pVertices[1].x = pos.m_fRight;
	pVertices[1].y = pos.m_fTop;
	pVertices[1].z = 0.0f;
	pVertices[1].color = _GetDesc().m_dwInnerColor;

	// Right line.
	// Top.
	pVertices[2] = pVertices[1];

	// Bottom.
	pVertices[3].x = pos.m_fRight;
	pVertices[3].y = pos.m_fBottom;
	pVertices[3].z = 0.0f;
	pVertices[3].color = _GetDesc().m_dwInnerColor;

	// Bottom line.
	// Right.
	pVertices[4] = pVertices[3];

	// Left
	pVertices[5].x = pos.m_fLeft;
	pVertices[5].y = pos.m_fBottom;
	pVertices[5].z = 0.0f;
	pVertices[5].color = _GetDesc().m_dwInnerColor;

	// Left line.
	// Bottom.
	pVertices[6] = pVertices[5];

	// Top.
	pVertices[7] = pVertices[0];

	hr = m_pFrameBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock colored rectangle vertex buffer", hr);
		return false;
	}

	return true;
}

CImageObject::CImageObject() : CObject2D()
{
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
}

CImageObject::~CImageObject()
{
}

bool CImageObject::Init(const SFloatRect& _pos, const CBitmap& _bitmap)
{
	SObject2DDesc desc;
	
	ZeroMemory(&desc, sizeof(desc));
	desc.m_eType = otAll;

	if(!CObject2D::_Init(desc, _pos))
	{
		return false;
	}

	if(!_CreateBuffer())
	{
		return false;
	}

	if(!_CreateTexture(_bitmap))
	{
		return false;
	}

	return true;
}

bool CImageObject::Render() const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	hr = pDevice->SetTexture(0, m_pTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set texture", hr);
		return false;
	}

	if(!_RenderVertexBuffer(m_pVertexBuffer, sizeof(SFaceVertex), SFaceVertex::_fvf, D3DPT_TRIANGLELIST, 2))
	{
		return false;
	}

	hr = pDevice->SetTexture(0, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to reset texture", hr);
		return false;
	}

	return true;
}

bool CImageObject::Render(const SFloatPoint & _offset) const
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	D3DXMATRIX orgMatrix, translateMatrix;

	hr = pDevice->GetTransform(D3DTS_WORLD, &orgMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get transform", hr);
		return false;
	}

	D3DXMatrixTranslation(&translateMatrix, _offset.m_fX, _offset.m_fY, 0.0f);

	hr = pDevice->SetTransform(D3DTS_WORLD, &translateMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to multiply transform", hr);
		return false;
	}

	if(!Render())
	{
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_WORLD, &orgMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to restore transform", hr);
		return false;
	}

	return true;
}

bool CImageObject::CleanUp()
{
	if(!CObject2D::_CleanUp())
	{
		return false;
	}

	m_pTexture->Release();
	m_pVertexBuffer->Release();

	return true;
}

bool CImageObject::_CreateBuffer()
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

	SFloatRect pos = _GetPos();

	// Upper right triangle.
	// Upper left.
	pVertices[0].x = pos.m_fLeft;
	pVertices[0].y = pos.m_fTop;
	pVertices[0].z = 0.0f;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 0.0f;

	// Upper right.
	pVertices[1].x = pos.m_fRight;
	pVertices[1].y = pos.m_fTop;
	pVertices[1].z = 0.0f;
	pVertices[1].tu = 1.0f;
	pVertices[1].tv = 0.0f;

	// Lower right.
	pVertices[2].x = pos.m_fRight;
	pVertices[2].y = pos.m_fBottom;
	pVertices[2].z = 0.0f;
	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;

	// Lower left triangle.
	// Upper left.
	pVertices[3] = pVertices[0];

	// Lower left.
	pVertices[4].x = pos.m_fLeft;
	pVertices[4].y = pos.m_fBottom;
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

bool CImageObject::_CreateTexture(const CBitmap& _bitmap)
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	if(!pGraphicsManager->CreateTexture(_bitmap, &m_pTexture))
	{
		LogError("Failed to create texture for image rectangle");
		return false;
	}

	return true;
}
