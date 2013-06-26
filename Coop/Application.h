#pragma once

#include "Logger.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "BasicObjects.h"
#include "Camera.h"
#include "ScreenManager.h"
#include "AudioManager.h"
#include "AppState.h"
#include "TextureManager.h"

#include <Windows.h>
#include <string>

class CApplication
{
	HINSTANCE m_hInstance;
	
	CLogger m_logger;
	CInputManager m_inputManager;
	CGraphicsManager m_graphicsManager;
	CBasicObjects m_basicObjects;
	CCamera m_camera;
	CScreenManager m_screenManager;
	CAudioManager m_audioManager;
	CAppState m_appState;
	CTextureManager m_textureManager;

	bool _Init();
	bool _CleanUp();

	static CApplication * m_pInstance;

public:
	CApplication(HINSTANCE _hInstance);

	int Main(std::string _args);
	HINSTANCE GetHInstance() { return m_hInstance; }

	static CApplication * GetInstance();
};
