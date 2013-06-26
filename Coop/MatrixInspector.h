#pragma once

#include "GraphicsManager.h"

class CMatrixInspector
{
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProjection;
	
	D3DXMATRIX m_matAll;

public:
	CMatrixInspector();

	bool Update();

	D3DXVECTOR3 ProjectVector(const D3DXVECTOR3 & vecInput) const;
};
