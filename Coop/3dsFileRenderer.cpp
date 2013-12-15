#include "3dsFileRenderer.h"

#include "Logger.h"
#include "VertexTypes.h"

C3dsFileRenderer::C3dsFileRenderer()
{
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
}

bool C3dsFileRenderer::Init()
{
	m_file.Load("D:\\projects\\3dsReader\\3dsReader\\die\\die.3ds");

	return true;
}

bool C3dsFileRenderer::CreateVertexBuffer()
{
	CGraphicsManager * pGraphicsManager = CGraphicsManager::GetInstance();

	CBitmap bitmap("D:\\projects\\3dsReader\\3dsReader\\die\\die_text.bmp");

	if(!pGraphicsManager->CreateTexture(bitmap, &m_pTexture))
	{
		LogError("Failed to create texture for die");
		return false;
	}

	IDirect3DDevice9 * pDevice = pGraphicsManager->GetDevice();

	int nVertices = m_file.GetTriangleCount() * 3;
	size_t bufferSize = sizeof(STexturedVertex) * nVertices;
	HRESULT hr = pDevice->CreateVertexBuffer(bufferSize, 0, STexturedVertex::_fvf, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer", hr);
		return false;
	}

	STexturedVertex * pLockedData = NULL;
	hr = m_pVertexBuffer->Lock(0, bufferSize, (void **)&pLockedData, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock vertex buffer", hr);
		return false;
	}

	int nTriangles = m_file.GetTriangleCount();
	for(int i = 0; i < nTriangles; i++)
	{
		S3dsCompleteTrianlge2 triangle = m_file.GetTriangle(i);

		for(int j = 0; j < 3; j++)
		{
			int iVertexIndex = i * 3 + j;

			pLockedData[iVertexIndex].x = triangle.m_fPos[j][0];
			pLockedData[iVertexIndex].y = triangle.m_fPos[j][1];
			pLockedData[iVertexIndex].z = triangle.m_fPos[j][2];

			pLockedData[iVertexIndex].tu = triangle.m_fTextCoor[j][0];
			pLockedData[iVertexIndex].tv = triangle.m_fTextCoor[j][1];
		}
	}

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock vertex buffer", hr);
		return false;
	}

	return true;
}

bool C3dsFileRenderer::Render()
{
	HRESULT hr;
	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(STexturedVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to 3ds", hr);
		return false;
	}

	hr = pDevice->SetFVF(STexturedVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for 3ds", hr);
		return false;
	}

	hr = pDevice->SetTexture(0, m_pTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set texture for 3ds", hr);
		return false;
	}

	int nTriangles = m_file.GetTriangleCount() * 3;
	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nTriangles);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw 3ds primitive", hr);
		return false;
	}

	hr = pDevice->SetTexture(0, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to reset texture for 3ds", hr);
		return false;
	}

	return true;
}

bool C3dsFileRenderer::CleanUp()
{
	if(m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	return true;
}
