#pragma once

#include <d3dx9.h>

class CCamera
{
	bool m_bInited;
	float m_fRotationAngle;
	float m_fUpAngle;
	float m_fDistance;
	D3DXVECTOR3 m_vLookAt;

	static CCamera *m_pInstance;

public:
	CCamera();
	~CCamera();

	bool Init();
	bool CleanUp();

	float GetRotationAngle();
	float GetUpAngle();
	float GetDistance();
	D3DXVECTOR3 GetLookAt();

	void SetRotationAngle(float _fRotationAngle);
	void SetUpAngle(float _fUpAngle);
	void SetDistance(float _fDistance);
	void SetLookAt(const D3DXVECTOR3& _vLookAt);

	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();

	static CCamera *GetInstance();
};
