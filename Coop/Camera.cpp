#include "Camera.h"

#include "GraphicsManager.h"

#include <assert.h>
#include <d3dx9.h>

CCamera::CCamera()
{
	m_bInited = false;
	m_fRotationAngle = 0.0f;
	m_fUpAngle = 0.0f;
	m_fDistance = 0.0f;
	m_vLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pInstance = this;
}

CCamera::~CCamera()
{
	assert(!m_bInited);
}

bool CCamera::Init()
{
	m_fDistance = 10.0f;
	m_fRotationAngle = D3DX_PI / 4.0f;
	m_fUpAngle = D3DX_PI / 4.0f;

	m_bInited = true;
	
	return true;
}

bool CCamera::CleanUp()
{
	m_bInited = false;

	return true;
}

float CCamera::GetRotationAngle()
{
	return m_fRotationAngle;
}

float CCamera::GetUpAngle()
{
	return m_fUpAngle;
}

float CCamera::GetDistance()
{
	return m_fDistance;
}

D3DXVECTOR3 CCamera::GetLookAt()
{
	return m_vLookAt;
}

void CCamera::SetRotationAngle(float _fRotationAngle)
{
	m_fRotationAngle = _fRotationAngle;
}

void CCamera::SetUpAngle(float _fUpAngle)
{
	m_fUpAngle = _fUpAngle;
}

void CCamera::SetDistance(float _fDistance)
{
	m_fDistance = _fDistance;
}

void CCamera::SetLookAt(const D3DXVECTOR3& _vLookAt)
{
	m_vLookAt = _vLookAt;
}

D3DXMATRIX CCamera::GetViewMatrix()
{
	D3DXMATRIX out;

	float fDistanceXY = m_fDistance * cosf(m_fUpAngle);

	float fX = -cosf(m_fRotationAngle) * fDistanceXY;
	float fY =  sinf(m_fRotationAngle) * fDistanceXY;
	float fZ = m_fDistance * sin(m_fUpAngle);

	D3DXVECTOR3 vEyePt(fX, fY, fZ);
	vEyePt += m_vLookAt;

	D3DXVECTOR3 vUpVec;

	if(m_fUpAngle <= (0.25f * D3DX_PI))
	{
		vUpVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		vUpVec = -D3DXVECTOR3(fX, fY, 0.0f);
	}

    D3DXMatrixLookAtLH(&out, &vEyePt, &m_vLookAt, &vUpVec);

	return out;
}

D3DXMATRIX CCamera::GetProjectionMatrix()
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	
	int iWindowWidth = pGraphicsManager->GetWindowWidth();
	int iWindowHeight = pGraphicsManager->GetWindowHeight();

	float fAspect = (float)iWindowWidth / (float)iWindowHeight;

	D3DXMATRIX out;
	D3DXMatrixPerspectiveFovLH(&out, D3DX_PI / 4, fAspect, 1.0f, 100.0f);

	return out;
}

CCamera *CCamera::GetInstance()
{
	assert(m_pInstance->m_bInited);

	return m_pInstance;
}

CCamera *CCamera::m_pInstance = NULL;
