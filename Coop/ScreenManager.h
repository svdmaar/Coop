#pragma once

#include "Screen.h"

#include <vector>
#include <map>

class CScreenManager
{
	bool m_bInited;

	ScreenVector m_vScreens;
	CScreen *m_pCurrentScreen;

	static CScreenManager *m_pInstance;

public:
	CScreenManager();
	~CScreenManager();

	bool Init();
	bool CleanUp();

	bool RenderScene();
	bool SetCurrentScreen(CScreen *_pScreen);

	static CScreenManager *GetInstance();
};
