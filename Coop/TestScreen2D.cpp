#include "TestScreen2D.h"

#include "GraphicsManager.h"
#include "FrameTimer.h"
#include "InputManager.h"

CTestScreen2D::CTestScreen2D()
{
}

CTestScreen2D::~CTestScreen2D()
{
}

bool CTestScreen2D::Init()
{
	if(!CScreen::Init())
	{
		return false;
	}

	if(!SetDefaultGraphicsSettings2D())
	{
		return false;
	}

	/*
	if(!m_sprite1.Init())
	{
		return false;
	}
	*/

	RECT rSub;
	rSub.left = rSub.top = 0;
	rSub.right = rSub.bottom = 32;
	if(!m_asSprite1.Init("D:\\projects\\Coop\\bin\\final\\Debug\\bitmap\\pacman.bmp", rSub, 8))
	{
		return false;
	}

	CInputManager * pInputManager = CInputManager::GetInstance();

	const CInputDevice * pInputDevice = pInputManager->GetDevice(0);
	if(!m_pacmanPlayer1.Init(pInputDevice))
	{
		return false;
	}

	pInputDevice = pInputManager->GetDevice(1);
	if(!m_pacmanPlayer2.Init(pInputDevice))
	{
		return false;
	}

	return true;
}

bool CTestScreen2D::InitAfterSwap()
{
	if(!CScreen::InitAfterSwap())
	{
		return false;
	}

	return true;
}

bool CTestScreen2D::CleanUp()
{
	if(!m_asSprite1.CleanUp())
	{
		return false;
	}

	if(!m_pacmanPlayer1.CleanUp())
	{
		return false;
	}

	if(!m_pacmanPlayer2.CleanUp())
	{
		return false;
	}

	if(!CScreen::CleanUp())
	{
		return false;
	}

	return true;
}

bool CTestScreen2D::ProcessInput()
{
	return true;
}

bool CTestScreen2D::Render()
{
	CGraphicsManager * pGraphicsManager = CGraphicsManager::GetInstance();
	CFrameTimer * pFrameTimer = CFrameTimer::GetInstance();	

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	const int iRowCount = 10;
	const int iColumnCount = 20;
	const float fDotSize = 8.0f;

	/*
	for(int iRowIndex = 0; iRowIndex < iRowCount; iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < iColumnCount; iColumnIndex++)
		{
			SFloatPoint fpPos;
			fpPos.m_fX = fDotSize * (float)(iColumnIndex - iColumnCount / 2) + 0.5f * fDotSize;
			fpPos.m_fY = -fDotSize * (float)(iRowIndex - iRowCount / 2) - 0.5f * fDotSize;

			if(!m_asSprite1.Render(0, fpPos))
			{
				return false;
			}
		}
	}
	*/

	/*
	SFloatPoint fpPos;
	fpPos.m_fX = 0.0f;
	fpPos.m_fY = 0.0f;

	int iFrameIndex = (pFrameTimer->GetCurrTime() / 100) % 16;
	if(iFrameIndex > 7)
	{
		iFrameIndex = 15 - iFrameIndex;
	}

	if(!m_asSprite1.Render(iFrameIndex, fpPos))
	{
		return false;
	}

	fpPos.m_fX = 8.0f;
	if(!m_asSprite1.Render(iFrameIndex, fpPos))
	{
		return false;
	}
	*/

	if(!m_pacmanPlayer1.Update())
	{
		return false;
	}

	if(!m_pacmanPlayer1.Render())
	{
		return false;
	}

	if(!m_pacmanPlayer2.Update())
	{
		return false;
	}

	if(!m_pacmanPlayer2.Render())
	{
		return false;
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}
