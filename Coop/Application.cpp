#include "Application.h"

#include "AudioFile.h"

#include <assert.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

CApplication::CApplication(HINSTANCE _hInstance) : m_logger()
{
	m_hInstance = _hInstance;
	m_pInstance = this;
}

CApplication *CApplication::m_pInstance = NULL; 

CApplication *CApplication::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

int CApplication::Main(string _args)
{
	if(!_Init())
	{
		LogError("Failed to init application");
		return -1;
	}

	while(!m_graphicsManager.ShouldQuit())
	{
		if(!m_graphicsManager.ProcessWindowMessages())
		{
			LogError("Failed to process windows messages.");
			break;
		}

		if(!m_inputManager.OnNewFrame())
		{
			LogError("Input manager failed to act on new frame.");
			break;
		}

		if(!m_screenManager.RenderScene())
		{
			LogError("Failed to render scene.");
			break;
		}
	}

	if(!_CleanUp())
	{
		LogError("Failed to clean up application.");
		return -1;
	}
	
	return 0;
}

bool CApplication::_Init()
{
	if(!m_graphicsManager.Init())
	{
		return false;
	}

	if(!m_inputManager.Init(2))
	{
		return false;
	}

	if(!m_appState.Init())
	{
		return false;
	}

	if(!m_audioManager.Init())
	{
		return false;
	}

	if(!m_basicObjects.Init())
	{
		return false;
	}

	if(!m_camera.Init())
	{
		return false;
	}

	if(!m_screenManager.Init())
	{
		return false;
	}

	return true;
}

bool CApplication::_CleanUp()
{
	if(!m_screenManager.CleanUp())
	{
		LogError("Failed to clean up screen manager");
		return false;
	}

	if(!m_camera.CleanUp())
	{
		LogError("Failed to clean up camera manager");
		return false;
	}

	if(!m_basicObjects.CleanUp())
	{
		LogError("Failed to clean up basic objects");
		return false;
	}

	if(!m_audioManager.CleanUp())
	{
		LogError("Failed to clean up audio manager");
		return false;
	}

	if(!m_appState.CleanUp())
	{
		LogError("Failed to clean up app state");
		return false;
	}

	if(!m_inputManager.CleanUp())
	{
		LogError("Failed to clean up input manager");
		return false;
	}

	if(!m_graphicsManager.CleanUp())
	{
		LogError("Failed to clean up graphics manager");
		return false;
	}

	return true;
}
