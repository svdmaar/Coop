#include "PlayerSelectScreen.h"

#include "GraphicsManager.h"
#include "InputManager.h"
#include "Logger.h"

#include <assert.h>

using namespace std;

CPlayerSelectScreen::CPlayerSelectScreen() : CScreen()
{
	assert(m_pInstance == NULL);

	m_pInstance = this;
}

CPlayerSelectScreen::~CPlayerSelectScreen()
{

}

bool CPlayerSelectScreen::Init()
{
	if(!CScreen::Init())
	{
		return false;
	}

	m_vPossPlayerNames.clear();

	m_vPossPlayerNames.push_back("Player1");
	m_vPossPlayerNames.push_back("Player2");
	m_vPossPlayerNames.push_back("Player3");
	m_vPossPlayerNames.push_back("Player4");
	m_vPossPlayerNames.push_back("Player5");
	m_vPossPlayerNames.push_back("Player6");
	m_vPossPlayerNames.push_back("Player7");

	if(!m_gui.Init())
	{
		return false;
	}

	for(int i = 0; i < g_iMaxPlayerCount; i++)
	{
		if(!_InitNameSelector(i))
		{
			return false;
		}
	}

	return true;
}

bool CPlayerSelectScreen::InitAfterSwap()
{
	return SetDefaultGraphicsSettings2D();
}

bool CPlayerSelectScreen::CleanUp()
{
	if(!CScreen::CleanUp())
	{
		return false;
	}

	for(int i = 0; i < g_iMaxPlayerCount; i++)
	{
		if(!m_playerNameSelectors[i].CleanUp())
		{
			return false;
		}
	}

	if(!m_gui.CleanUp())
	{
		return false;
	}

	return true;
}

bool CPlayerSelectScreen::ProcessInput()
{
	return true;
}

bool CPlayerSelectScreen::_InitNameSelector(int _iIndex)
{
	SFloatRect rPos, rScreen;

	rScreen = m_gui.GetScreenRect();

	float fScreenWidth = rScreen.m_fRight - rScreen.m_fLeft;

	float fSelectorWidth = fScreenWidth / 4.0f;
	float fLeftLeft = rScreen.m_fLeft + fScreenWidth / 8.0f;
	float fRightLeft = rScreen.m_fRight - 3.0f * fScreenWidth / 8.0f;
	float fLeftRight = fLeftLeft + fSelectorWidth;
	float fRightRight = fRightLeft + fSelectorWidth;

	float fScreenHeight = rScreen.m_fTop - rScreen.m_fBottom;
	float fSelectorHeight = fScreenHeight / 4.0f;
	float fTopTop = rScreen.m_fTop - fScreenHeight / 8.0f;
	float fTopBottom = fTopTop - fSelectorHeight;
	float fBottomTop = rScreen.m_fBottom + 3.0f * fScreenHeight / 8.0f;
	float fBottomBottom = fBottomTop - fSelectorHeight;

	float fCorrFactor = fSelectorHeight / 4.0f;

	fTopTop -= fCorrFactor;
	fBottomTop -= fCorrFactor;
	fTopBottom += fCorrFactor;
	fBottomBottom += fCorrFactor;
	
	if((_iIndex % 2) == 0)
	{
		rPos.m_fLeft = fLeftLeft;
		rPos.m_fRight = fLeftRight;
	}
	else
	{
		rPos.m_fLeft = fRightLeft;
		rPos.m_fRight = fRightRight;
	}


	if((_iIndex / 2) == 0)
	{
		rPos.m_fTop = fTopTop;
		rPos.m_fBottom = fTopBottom;
	}
	else
	{
		rPos.m_fTop = fBottomTop;
		rPos.m_fBottom = fBottomBottom;
	}

	m_playerNameSelectors[_iIndex].Init(rPos, &m_vPossPlayerNames);

	if(!m_gui.AddElement(&m_playerNameSelectors[_iIndex]))
	{
		return false;
	}

	return true;
}

bool CPlayerSelectScreen::Render()
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	if(!m_gui.Render())
	{
		return false;
	}

	for(int iDevIndex = 0; iDevIndex < g_iMaxPlayerCount; iDevIndex++)
	{
		const CInputDevice *pInputDevice = pInputManager->GetDevice(iDevIndex);
		int iAddedIndex = 0;

		if(pInputDevice->DPadIsJustPressed(DIR_TOP))
		{
			iAddedIndex = -1;
		}
		else if(pInputDevice->DPadIsJustPressed(DIR_BOTTOM))
		{
			iAddedIndex = 1;
		}

		m_playerNameSelectors[iDevIndex].OnDPadInput(iAddedIndex);

		for(int iButtonIndex = 0; iButtonIndex < 2; iButtonIndex++)
		{
			if(pInputDevice->ButtonIsJustPressed(iButtonIndex))
			{
				m_playerNameSelectors[iDevIndex].OnButtonPressed(iButtonIndex);
			}
		}
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}

CPlayerSelectScreen *CPlayerSelectScreen::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

CPlayerSelectScreen *CPlayerSelectScreen::m_pInstance = NULL;
