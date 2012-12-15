#include "GraphicsUtils.h"

#include "GraphicsManager.h"
#include "Logger.h"
#include "VertexTypes.h"

bool CGraphicsUtils::CalcRectCenter(const SFloatRect& _rect, float& _fCenterX, float& _fCenterY)
{
	_fCenterX = 0.5f * (_rect.m_fRight + _rect.m_fLeft);
	_fCenterY = 0.5f * (_rect.m_fTop + _rect.m_fBottom);

	return true;
}

bool CGraphicsUtils::CreateColoredFrame(SFloatRect _rPos, IDirect3DVertexBuffer9 **_ppVertexBuffer, DWORD _dwColor)
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	UINT bufferSize = sizeof(SColoredVertex) * 8;
	hr = pDevice->CreateVertexBuffer(bufferSize, 0, SColoredVertex::_fvf, D3DPOOL_DEFAULT, _ppVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for colored frame", hr);
		return false;
	}

	SColoredVertex *pVertices = NULL;
	hr = (*_ppVertexBuffer)->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock colored frame vertex buffer", hr);
		return false;
	}

	// Upper line.
	// Left.
	pVertices[0].x = _rPos.m_fLeft;
	pVertices[0].y = _rPos.m_fTop;
	pVertices[0].z = 0.0f;
	pVertices[0].color = _dwColor;

	// Right.
	pVertices[1].x = _rPos.m_fRight;
	pVertices[1].y = _rPos.m_fTop;
	pVertices[1].z = 0.0f;
	pVertices[1].color = _dwColor;

	// Right line.
	// Top.
	pVertices[2] = pVertices[1];

	// Bottom.
	pVertices[3].x = _rPos.m_fRight;
	pVertices[3].y = _rPos.m_fBottom;
	pVertices[3].z = 0.0f;
	pVertices[3].color = _dwColor;

	// Bottom line.
	// Right.
	pVertices[4] = pVertices[3];

	// Left
	pVertices[5].x = _rPos.m_fLeft;
	pVertices[5].y = _rPos.m_fBottom;
	pVertices[5].z = 0.0f;
	pVertices[5].color = _dwColor;

	// Left line.
	// Bottom.
	pVertices[6] = pVertices[5];

	// Top.
	pVertices[7] = pVertices[0];

	hr = (*_ppVertexBuffer)->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock colored rectangle vertex buffer", hr);
		return false;
	}

	return true;
}
