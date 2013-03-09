#include "ScreenManager.h"

#include "Logger.h"

#include "TestScreen.h"
#include "MainScreen.h"
#include "PlayerSelectScreen.h"

#include <assert.h>
#include <algorithm>

using namespace std;

CScreenManager::CScreenManager()
{
	assert(m_pInstance == NULL);

	m_bInited = false;
	m_pInstance = this;
}

CScreenManager::~CScreenManager()
{
	assert(!m_bInited);
}

bool CScreenManager::Init()
{
	assert(!m_bInited);
	assert(m_vScreens.empty());

	m_vScreens.push_back(new CTestScreen());
	m_vScreens.push_back(new CMainScreen());
	m_vScreens.push_back(new CPlayerSelectScreen());
	

	assert(!m_vScreens.empty());

	for(ScreenVector::iterator i = m_vScreens.begin(); i != m_vScreens.end(); i++)
	{
		CScreen *pScreen = *i;

		if(!pScreen->Init())
		{
			return false;
		}
	}

	m_pCurrentScreen = m_vScreens.front();
	
	if(!m_pCurrentScreen->InitAfterSwap())
	{
		LogError("Failed to init after swap on first screen");
		return false;
	}

	m_bInited = true;

	return true;
}

bool CScreenManager::RenderScene()
{
	if(!m_pCurrentScreen->ProcessInput())
	{
		return false;
	}

	return m_pCurrentScreen->Render();
}

bool CScreenManager::CleanUp()
{
	assert(m_bInited);

	for(ScreenVector::iterator i = m_vScreens.begin(); i != m_vScreens.end(); i++)
	{
		CScreen *pScreen = *i;

		if(!pScreen->CleanUp())
		{
			return false;
		}

		delete pScreen;
	}

	m_vScreens.clear();

	m_bInited = false;

	return true;
}

bool CScreenManager::SetCurrentScreen(CScreen *_pScreen)
{
	assert(find(m_vScreens.begin(), m_vScreens.end(), _pScreen) != m_vScreens.end());

	m_pCurrentScreen = _pScreen;
	m_pCurrentScreen->InitAfterSwap();

	return true;
}

CScreenManager *CScreenManager::GetInstance()
{
	assert(m_pInstance != NULL);
	assert(m_pInstance->m_bInited);

	return m_pInstance;
}

CScreenManager *CScreenManager::m_pInstance = NULL;
