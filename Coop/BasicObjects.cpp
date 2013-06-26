#include "BasicObjects.h"

#include "Logger.h"
#include "GraphicsManager.h"

#include <assert.h>

const int g_iGridSize = 8;

CBasicObjects::CBasicObjects()
{
	m_bInited = false;
	m_pBoxVB = NULL;
	m_pGridVB = NULL;
	m_pPolesVB = NULL;
	m_pRectVB = NULL;
	m_pNormalBox = NULL;

	m_pInstance = this;
}

CBasicObjects::~CBasicObjects()
{
	assert(!m_bInited);
}

bool CBasicObjects::Init()
{
	assert(!m_bInited);

	if(!_FillBoxVB())
	{
		return false;
	}

	if(!_FillGridVB())
	{
		return false;
	}

	if(!_FillPolesVB())
	{
		return false;
	}

	if(!_FillRectVB())
	{
		return false;
	}

	if(!_FillBoxVBNormal())
	{
		return false;
	}

	m_bInited = true;

	return true;
}

bool CBasicObjects::CleanUp()
{
	HRESULT hr;

	assert(m_bInited);

	hr = m_pBoxVB->Release();
	if(FAILED(hr))
	{
		LogError("Failed to release box vb");
		return false;		
	}

	hr = m_pGridVB->Release();
	if(FAILED(hr))
	{
		LogError("Failed to release grid vb");
		return false;		
	}

	hr = m_pPolesVB->Release();
	if(FAILED(hr))
	{
		LogError("Failed to release poles vb");
		return false;		
	}

	hr = m_pRectVB->Release();
	if(FAILED(hr))
	{
		LogError("Failed to release rect vb");
		return false;		
	}

	hr = m_pNormalBox->Release();
	if(FAILED(hr))
	{
		LogError("Failed to release normal box vb");
		return false;		
	}

	m_bInited = false;

	return true;
}

bool CBasicObjects::IsInited()
{
	return m_bInited;
}

bool CBasicObjects::RenderBox()
{
	HRESULT hr;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pBoxVB, 0, sizeof(SFaceVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to box vb", hr);
		return false;
	}

	hr = pDevice->SetFVF(SFaceVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for box", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 * 6);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for box", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::RenderGrid()
{
	HRESULT hr;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pGridVB, 0, sizeof(SLineVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to grid vb", hr);
		return false;
	}

	hr = pDevice->SetFVF(SLineVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF to grid", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 2 * (g_iGridSize + 1));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw grid", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::RenderPoles()
{
	HRESULT hr;
	const int iPoleCount = (g_iGridSize + 1) * (g_iGridSize + 1);

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pPolesVB, 0, sizeof(SLineVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to grid vb", hr);
		return false;
	}

	hr = pDevice->SetFVF(SLineVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to grid vb", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_LINELIST, 0, iPoleCount);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to grid vb", hr);
		return false;
	}

	return false;
}

bool CBasicObjects::RenderRect()
{
	HRESULT hr;
	const int iCornerCount = 6;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pRectVB, 0, sizeof(SFaceVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to rect vb", hr);
		return false;
	}

	hr = pDevice->SetFVF(SFaceVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for rect", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, iCornerCount);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw rect", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::RenderBoxNormal()
{
	HRESULT hr;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pNormalBox, 0, sizeof(SNormalVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to box normal vb", hr);
		return false;
	}

	hr = pDevice->SetFVF(SNormalVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for box normal vb", hr);
		return false;
	}

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 * 6);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw box normal", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::_FillBoxVB()
{
	HRESULT hr;
	const int iTriangleCount = 2 * 6;
	SFaceVertex boxCorners[2][2][2];
	SFaceVertex boxTriangles[iTriangleCount * 3];

	DWORD dwBoxColor = D3DCOLOR_XRGB(255, 0, 255);
	int iX, iY, iZ;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	// fill corners
	for(iX = 0; iX < 2; iX++)
	{
		for(iY = 0; iY < 2; iY++)
		{
			for(iZ = 0; iZ < 2; iZ++)
			{
				boxCorners[iX][iY][iZ].x = (float)iX;
				boxCorners[iX][iY][iZ].y = (float)iY;
				boxCorners[iX][iY][iZ].z = (float)iZ;
			}
		}
	}

	SFaceVertex * pBoxPoint = boxTriangles;

	memset(boxTriangles, 0, sizeof(boxTriangles));

	// fill triangles
	// bottom
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;

	// back
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;

	// left
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;

	// front
	*pBoxPoint = boxCorners[0][1][0]; _SetTextureCoors(pBoxPoint, 0.0f, 0.0f); pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; _SetTextureCoors(pBoxPoint, 0.0f, 1.0f); pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; _SetTextureCoors(pBoxPoint, 1.0f, 0.0f); pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; _SetTextureCoors(pBoxPoint, 0.0f, 1.0f); pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; _SetTextureCoors(pBoxPoint, 1.0f, 1.0f); pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; _SetTextureCoors(pBoxPoint, 1.0f, 0.0f); pBoxPoint++;

	// right
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;

	// top
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;

	if(m_pBoxVB != NULL)
	{
		LogError("Expected Box VB to be NULL");
		return false;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(boxTriangles), 0, SFaceVertex::_fvf, D3DPOOL_DEFAULT, &m_pBoxVB, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer", hr);
		return false;
	}

	VOID * pDevBox = NULL;
	hr = m_pBoxVB->Lock(0, sizeof(boxTriangles), (void**)&pDevBox, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer", hr);
		return false;
	}

	memcpy(pDevBox, boxTriangles, sizeof(boxTriangles));
	hr = m_pBoxVB->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock vertex buffer", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::_FillGridVB()
{
	HRESULT hr;

	SLineVertex lines[(g_iGridSize + 1) * 2 * 2];
	memset(lines, 0, sizeof(lines));
	const float fGridStart = ((float)g_iGridSize) / 2.0f;
	const float fGridSize = (float)g_iGridSize;

	DWORD dwGridColor = 0xff0000ff;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	for(int iGridLineIndex = 0; iGridLineIndex <= g_iGridSize; iGridLineIndex++)
	{
		// parallel to y axis
		lines[4 * iGridLineIndex + 0].x = -fGridStart + (float)(iGridLineIndex);
		lines[4 * iGridLineIndex + 0].y = -fGridStart;
		lines[4 * iGridLineIndex + 0].color = dwGridColor;

		lines[4 * iGridLineIndex + 1].x = -fGridStart + (float)(iGridLineIndex);
		lines[4 * iGridLineIndex + 1].y =  fGridStart;
		lines[4 * iGridLineIndex + 1].color = dwGridColor;

		// parallel to x axis
		lines[4 * iGridLineIndex + 2].x = -fGridStart;
		lines[4 * iGridLineIndex + 2].y = -fGridStart + (float)(iGridLineIndex);
		lines[4 * iGridLineIndex + 2].color = dwGridColor;

		lines[4 * iGridLineIndex + 3].x =  fGridStart;
		lines[4 * iGridLineIndex + 3].y = -fGridStart + (float)(iGridLineIndex);
		lines[4 * iGridLineIndex + 3].color = dwGridColor;
	}

	if(m_pGridVB != NULL)
	{
		LogError("Expected Box VB to be NULL");
		return false;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(lines), 0, SLineVertex::_fvf, D3DPOOL_DEFAULT, &m_pGridVB, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for grid", hr);
		return false;
	}

	VOID * pDevGrid = NULL;
	hr = m_pGridVB->Lock(0, sizeof(lines), (void**)&pDevGrid, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer for grid", hr);
		return false;
	}

	memcpy(pDevGrid, lines, sizeof(lines));
	hr = m_pGridVB->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock vertex buffer for grid", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::_FillPolesVB()
{
	HRESULT hr;
	const int iPoleCount = (g_iGridSize + 1) * (g_iGridSize + 1);
	DWORD dwColor = 0xff00ff00;

	SLineVertex lines[2 * iPoleCount];
	memset(lines, 0, sizeof(lines));

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	float fCoorMax = ((float)g_iGridSize) / 2.0f;

	for(int iX = 0; iX <= g_iGridSize; iX++)
	{
		for(int iY = 0; iY <= g_iGridSize; iY++)
		{
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 0].x = -fCoorMax + (float)iX;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 0].y = -fCoorMax + (float)iY;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 0].z = 0.0f;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 0].color = dwColor;

			lines[2 * (iX + iY * (g_iGridSize + 1)) + 1].x = -fCoorMax + (float)iX;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 1].y = -fCoorMax + (float)iY;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 1].z = 1.0f;
			lines[2 * (iX + iY * (g_iGridSize + 1)) + 1].color = dwColor;
		}
	}

	if(m_pPolesVB != NULL)
	{
		LogError("Expected Box VB to be NULL");
		return false;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(lines), 0, SLineVertex::_fvf, D3DPOOL_DEFAULT, &m_pPolesVB, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for poles", hr);
		return false;
	}

	VOID * pDevPoles = NULL;
	hr = m_pPolesVB->Lock(0, sizeof(lines), (void**)&pDevPoles, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer for poles", hr);
		return false;
	}

	memcpy(pDevPoles, lines, sizeof(lines));

	hr = m_pPolesVB->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer for poles", hr);
		return false;
	}

	return true;
}

bool CBasicObjects::_FillRectVB()
{
	HRESULT hr;
	DWORD dwColor = 0xff00ff00;

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	SFaceVertex corners[3 * 2];
	memset(corners, 0, sizeof(corners));

	SFaceVertex baseCorners[4];
	memset(baseCorners, 0, sizeof(baseCorners));

	// lower left
	baseCorners[0].x = 0.0f;
	baseCorners[0].y = 0.0f;
	baseCorners[0].z = 0.0f;
	baseCorners[0].tu = 0.0f;
	baseCorners[0].tv = 1.0f;

	// upper left
	baseCorners[1].x = 0.0f;
	baseCorners[1].y = 1.0f;
	baseCorners[1].z = 0.0f;
	baseCorners[1].tu = 0.0f;
	baseCorners[1].tv = 0.0f;

	// upper left
	baseCorners[2].x = 1.0f;
	baseCorners[2].y = 1.0f;
	baseCorners[2].z = 0.0f;
	baseCorners[2].tu = 1.0f;
	baseCorners[2].tv = 0.0f;

	// lower right
	baseCorners[3].x = 1.0f;
	baseCorners[3].y = 0.0f;
	baseCorners[3].z = 0.0f;
	baseCorners[3].tu = 1.0f;
	baseCorners[3].tv = 1.0f;

	corners[0] = baseCorners[0];
	corners[1] = baseCorners[1];
	corners[2] = baseCorners[2];
	corners[3] = baseCorners[2];
	corners[4] = baseCorners[3];
	corners[5] = baseCorners[0];

	if(m_pRectVB != NULL)
	{
		LogError("Expected rect VB to be NULL");
		return false;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(corners), 0, SFaceVertex::_fvf, D3DPOOL_DEFAULT, &m_pRectVB, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for rect", hr);
		return false;
	}

	VOID * pDevRect = NULL;
	hr = m_pRectVB->Lock(0, sizeof(corners), (void**)&pDevRect, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer for rect", hr);
		return false;
	}

	memcpy(pDevRect, corners, sizeof(corners));

	hr = m_pRectVB->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer for rect", hr);
		return false;
	}

	return true;
}


bool CBasicObjects::_FillBoxVBNormal()
{
	HRESULT hr;
	const int iTriangleCount = 2 * 6;
	SNormalVertex boxCorners[2][2][2];
	SNormalVertex boxTriangles[iTriangleCount * 3];

	memset(boxCorners, 0, sizeof(boxCorners));
	memset(boxTriangles, 0, sizeof(boxTriangles));

	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	int iX, iY, iZ;

	// fill corners
	for(iX = 0; iX < 2; iX++)
	{
		for(iY = 0; iY < 2; iY++)
		{
			for(iZ = 0; iZ < 2; iZ++)
			{
				boxCorners[iX][iY][iZ].x = (float)iX;
				boxCorners[iX][iY][iZ].y = (float)iY;
				boxCorners[iX][iY][iZ].z = (float)iZ;
			}
		}
	}

	SNormalVertex * pBoxPoint = boxTriangles;

	// fill triangles
	// bottom
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, 0.0f, 0.0f, -1.0f);

	// back
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, 0.0f, -1.0f, 0.0f);

	// left
	*pBoxPoint = boxCorners[0][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, -1.0f, 0.0f, 0.0f);

	// front
	*pBoxPoint = boxCorners[0][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, 0.0f, 1.0f, 0.0f);

	// right
	*pBoxPoint = boxCorners[1][1][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][0]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, 1.0f, 0.0f, 0.0f);

	// top
	*pBoxPoint = boxCorners[0][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][0][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[1][1][1]; pBoxPoint++;
	*pBoxPoint = boxCorners[0][1][1]; pBoxPoint++;
	_SetPrevFaceNormals(pBoxPoint, 0.0f, 0.0f, 1.0f);

	if(m_pNormalBox != NULL)
	{
		LogError("Expected normal box VB to be NULL");
		return false;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(boxTriangles), 0, SNormalVertex::_fvf, D3DPOOL_DEFAULT, &m_pNormalBox,
		NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for normal box", hr);
		return false;
	}

	VOID * pDevBox = NULL;
	hr = m_pNormalBox->Lock(0, sizeof(boxTriangles), (void**)&pDevBox, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for normal box", hr);
		return false;
	}

	memcpy(pDevBox, boxTriangles, sizeof(boxTriangles));
	hr = m_pNormalBox->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for normal box", hr);
		return false;
	}

	return true;
}

void CBasicObjects::_SetTextureCoors(SFaceVertex *_pVertex, float _fTu, float _fTv)
{
	_pVertex->tu = _fTu;
	_pVertex->tv = _fTv;
}

void CBasicObjects::_SetPrevFaceNormals(SNormalVertex *_pTooLate, float _nx, float _ny, float _nz)
{
	SNormalVertex *pBuffer = _pTooLate - 6;

	for(int i = 0; i < 6; i++)
	{
		pBuffer[i].nx = _nx;
		pBuffer[i].ny = _ny;
		pBuffer[i].nz = _nz;
	}
}

CBasicObjects *CBasicObjects::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

CBasicObjects *CBasicObjects::m_pInstance = NULL;
