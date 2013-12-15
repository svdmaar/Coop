#pragma once

#include "Screen.h"
#include "EshFile.h"
#include "AudioFile.h"
#include "AudioManager.h"
#include "3dsFileRenderer.h"

class CTestScreen : public CScreen
{
	//CEshFile m_box;
	C3dsFileRenderer m_box;
	CAudioFile m_sounds[12];

	static CTestScreen *m_pInstance;

	bool _RenderGUI();

public:
	CTestScreen();
	virtual ~CTestScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();

	virtual bool ProcessInput();
	virtual bool Render();

	static CTestScreen *GetInstance();
};
