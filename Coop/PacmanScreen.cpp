#include "PacmanScreen.h"

#include "GraphicsManager.h"
#include "Logger.h"
#include "Gui.h"

#include "MatrixInspector.h"
#include "VertexTypes.h"
#include "ErrorParser.h"

#include <string>

using namespace std;

CPacmanScreen::CPacmanScreen()
{
}

CPacmanScreen::~CPacmanScreen()
{
}

bool CPacmanScreen::Init()
{
	int i;
	SFloatRect rPos;
	CBitmap bm;

	ZeroMemory(&rPos, sizeof(rPos));
	rPos.m_fBottom = 0.0f;
	rPos.m_fTop = 10.0f;
	rPos.m_fLeft = 0.0f;
	rPos.m_fRight = 10.0f;

	if(!CScreen::Init())
	{
		return false;
	}

	if(!bm.Load("bitmap/pacman_dot.bmp"))
	{
		LogError("Failed to load dot bitmap");
		return false;
	}

	if(!m_objDot.Init(rPos, bm))
	{
		LogError("Failed to init dot object");
		return false;
	}

	for(i = 0; i < g_iPacmanBoardHeight * g_iPacmanBoardWidth; i++)
	{
		m_bDots[i] = true;
	}

	return true;
}

bool CPacmanScreen::InitAfterSwap()
{
	if(!CScreen::InitAfterSwap())
	{
		return false;
	}

	if(!SetDefaultGraphicsSettings2D())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::CleanUp()
{
	if(!m_objDot.CleanUp())
	{
		return false;
	}

	if(!CScreen::CleanUp())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::ProcessInput()
{
	return CScreen::ProcessInput();
}

bool CPacmanScreen::Render()
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	if(!_RenderBoard())
	{
		return false;
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::_RenderBoard()
{
	SFloatRect screenRect = CGui::GetScreenRect();
	
	SFloatPoint upperLeft;
	upperLeft.m_fX = screenRect.m_fLeft;
	upperLeft.m_fY = screenRect.m_fTop;

	SFloatPoint offset;
	offset.m_fX = upperLeft.m_fX; // + (float)iColumnIndex;
	offset.m_fY = upperLeft.m_fY; // - (float)iRowIndex;

	/*
	if(!m_objDot.Render())
	{
		return false;
	}
	*/

	CMatrixInspector ins;
	ins.Update();
	D3DXVECTOR3 dbg = ins.ProjectVector(D3DXVECTOR3(1, 0, 0));

	HRESULT hr;
	D3DXMATRIX matIden;
	D3DXMatrixIdentity(&matIden);
	
	IDirect3DDevice9 * pDev = CGraphicsManager::GetInstance()->GetDevice();
/*	hr = pDev->SetTransform(D3DTS_WORLD, &matIden);
	hr = pDev->SetTransform(D3DTS_VIEW, &matIden);
	hr = pDev->SetTransform(D3DTS_PROJECTION, &matIden);*/
	
	ins.Update();
	dbg = ins.ProjectVector(D3DXVECTOR3(0, 1, 0));

	SSimpleVertex triangle[3];
	triangle[0].x = 0.0f;
	triangle[0].y = 0.0f;
	triangle[0].z = 0.0f;
	triangle[0].color = 0xffffffffL;

	triangle[1] = triangle[0];
	triangle[1].x = 10.0f;

	triangle[2] = triangle[0];
	triangle[2].y = 10.0f;

	// debug
	IDirect3DDevice9 * pDevice = pDev;
	hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	hr = pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	hr = pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	hr = pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	hr = pDev->SetFVF(SSimpleVertex::m_fvf);
	hr = pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, triangle, sizeof(SSimpleVertex));
	string s = CErrorParser::ParseError(hr);
	
	offset.m_fX = upperLeft.m_fX; // + (float)iColumnIndex;
	offset.m_fY = upperLeft.m_fY; // - (float)iRowIndex;

	if(!m_objDot.Render())
	{
		return false;
	}

	return true;


	/*
	for(int iRowIndex = 0; iRowIndex < g_iPacmanBoardHeight; iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < g_iPacmanBoardWidth; iColumnIndex++)
		{
			if(m_bDots[iColumnIndex + iRowIndex * g_iPacmanBoardWidth])
			{
				SFloatPoint offset;
				offset.m_fX = upperLeft.m_fX + (float)iColumnIndex;
				offset.m_fY = upperLeft.m_fY - (float)iRowIndex;
				
				if(!m_objDot.Render(offset))
				{
					return false;
				}
			}
		}
	}
	*/

	return true;
}
