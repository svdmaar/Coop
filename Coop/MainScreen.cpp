#include "MainScreen.h"

#include "PictureBackground.h"
#include "GraphicsManager.h"
#include "Logger.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "PlayerSelectScreen.h"

#include <d3dx9.h>
#include <assert.h>

using namespace std;

CMainScreen::CMainScreen() : CScreen()
{
	assert(m_pInstance == NULL);

	m_pInstance = this;
}

CMainScreen::~CMainScreen()
{
}

bool CMainScreen::Render()
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	if(!m_gui.Render())
	{
		return false;
	}

	if(!m_texturedVertexBuffer.Render())
	{
		return false;
	}

	if(!m_coloredVertexBuffer.Render())
	{
		return false;
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}

SFloatRect CMainScreen::_CalcButtonPos(int _iButtonIndex)
{
	SFloatRect output;
	SFloatRect screenRect = m_gui.GetScreenRect();

	const float fButtonCount = 5.0f;

	const float fLeft = screenRect.m_fRight / 4.0f;
	const float fRight = 3.0f * fLeft;
	const float fHeight = 10.0f;
	const float fVertDist = fHeight / 2.0f;

	const float fFirstTop = (fButtonCount * fHeight + (fButtonCount - 1.0f) * fVertDist) / 2.0f;
	
	float fButtonIndex = (float)_iButtonIndex;

	output.m_fLeft = fLeft;
	output.m_fRight = fRight;
	output.m_fTop = fFirstTop - fButtonIndex * (fHeight + fVertDist);
	output.m_fBottom = output.m_fTop - fHeight;

	return output;
}

bool CMainScreen::Init()
{
	if(!CScreen::Init())
	{
		return false;
	}

	if(!m_gui.Init())
	{
		return false;
	}

	if(!m_background.Init("bitmap/main_background.bmp"))
	{
		return false;
	}

	CBitmap bm("bitmap/coop_logo.bmp");
	SFloatRect rect;

	rect.m_fLeft = -50.0f;
	rect.m_fRight = -40.0f;
	rect.m_fTop = 20.0f;
	rect.m_fBottom = 10.0f;

	if(!m_texturedVertexBuffer.Init(bm, rect))
	{
		return false;
	}

	rect.m_fTop -= 20.0f;
	rect.m_fBottom -= 20.0f;

	if(!m_coloredVertexBuffer.Init(rect, 0xff00ff00l))
	{
		return true;
	}

	// Init buttons.
	if(!m_btnOne.Init(_CalcButtonPos(0), "Single Game")) return false;
	if(!m_btnTwo.Init(_CalcButtonPos(1), "Tournament")) return false;
	if(!m_btnThree.Init(_CalcButtonPos(2), "Highscores")) return false;
	if(!m_btnFour.Init(_CalcButtonPos(3), "Settings")) return false;
	if(!m_btnFive.Init(_CalcButtonPos(4), "Exit")) return false;
	
	if(!m_gui.AddElement(&m_background))
	{
		return false;
	}

	if(!m_gui.AddElement(&m_btnOne)) return false;
	if(!m_gui.AddElement(&m_btnTwo)) return false;
	if(!m_gui.AddElement(&m_btnThree)) return false;
	if(!m_gui.AddElement(&m_btnFour)) return false;
	if(!m_gui.AddElement(&m_btnFive)) return false;

	if(!m_gui.SetSelected(&m_btnOne)) return false;

	return true;
}

bool CMainScreen::InitAfterSwap()
{
	/*
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	D3DXMATRIX identityTransform, projTransform;
	D3DXMatrixIdentity(&identityTransform);
	D3DXMatrixOrthoLH(&projTransform, 100.0f * pGraphicsManager->GetAspectRatio(), 100.0f, -1.0f, 1.0f);

	hr = pDevice->SetTransform(D3DTS_WORLD, &identityTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set world transform", hr);
		return false;
	}
		
	hr = pDevice->SetTransform(D3DTS_VIEW, &identityTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set view transform", hr);
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_PROJECTION, &projTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set projection transform", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set cull mode render state", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enable z", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set z write enabled", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lighting to false", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha blending to true", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha source render state", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha dest render state", hr);
		return false;
	}

	return true;
	*/

	return SetDefaultGraphicsSettings2D();
}

bool CMainScreen::CleanUp()
{
	if(!m_coloredVertexBuffer.CleanUp())
	{
		return false;
	}

	if(!m_texturedVertexBuffer.CleanUp())
	{
		return false;
	}

	if(!m_gui.CleanUp())
	{
		return false;
	}

	if(!m_background.CleanUp())
	{
		return false;
	}

	if(!m_btnOne.CleanUp()) return false;
	if(!m_btnTwo.CleanUp()) return false;
	if(!m_btnThree.CleanUp()) return false;
	if(!m_btnFour.CleanUp()) return false;
	if(!m_btnFive.CleanUp()) return false;

	if(!CScreen::CleanUp())
	{
		return false;
	}

	return true;
}

bool CMainScreen::ProcessInput()
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if(!m_gui.ProcessInput())
	{
		LogError("GUI failed to process input.");
		return false;
	}

	CGuiEvent guiEvent;
	while(m_gui.GetEvent(guiEvent))
	{
		if((guiEvent.GetElement() == &m_btnFive) && (guiEvent.GetEvent() == EVENT_BUTTONPRESSED))
		{
			CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
			pGraphicsManager->SetShouldQuit();
		}
		else if((guiEvent.GetElement() == &m_btnOne) && (guiEvent.GetEvent() == EVENT_BUTTONPRESSED))
		{
			CScreenManager *pScreenManager = CScreenManager::GetInstance();
			pScreenManager->SetCurrentScreen(CPlayerSelectScreen::GetInstance());
		}
	}

	return true;
}

CMainScreen *CMainScreen::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

CMainScreen *CMainScreen::m_pInstance = NULL;
