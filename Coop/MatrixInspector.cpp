#include "MatrixInspector.h"

#include "Logger.h"

CMatrixInspector::CMatrixInspector()
{
	ZeroMemory(&m_matWorld, sizeof(m_matWorld));
	ZeroMemory(&m_matView, sizeof(m_matView));
	ZeroMemory(&m_matProjection, sizeof(m_matProjection));
	ZeroMemory(&m_matAll, sizeof(m_matAll));
}

bool CMatrixInspector::Update()
{
	HRESULT hr;
	CGraphicsManager * pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 * pDev = pGraphicsManager->GetDevice();

	hr = pDev->GetTransform(D3DTS_WORLD, &m_matWorld);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get world transform", hr);
		return false;
	}

	hr = pDev->GetTransform(D3DTS_VIEW, &m_matView);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get view transform", hr);
		return false;
	}

	hr = pDev->GetTransform(D3DTS_PROJECTION, &m_matProjection);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get projection transform", hr);
		return false;
	}

	D3DXMatrixMultiply(&m_matAll, &m_matView, &m_matWorld);
	D3DXMatrixMultiply(&m_matAll, &m_matProjection, &m_matAll);

	return true;
}

D3DXVECTOR3 CMatrixInspector::ProjectVector(const D3DXVECTOR3 & vecInput) const
{
	D3DXVECTOR3 vecOut;

	D3DXVec3TransformCoord(&vecOut, &vecInput, &m_matAll);

	return vecOut;
}
