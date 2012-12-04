#pragma once

#include "Screen.h"
#include "Gui.h"
#include "StringSelector.h"

#include <string>
#include <vector>

const int g_iMaxPlayerCount = 4;

class CPlayerSelectScreen : public CScreen
{
	std::vector<std::string> m_vPossPlayerNames;
	CGui m_gui;
	CStringSelector m_playerNameSelectors[g_iMaxPlayerCount];

	static CPlayerSelectScreen *m_pInstance;

	bool _InitNameSelector(int _iIndex);

public:
	CPlayerSelectScreen();
	virtual ~CPlayerSelectScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();

	virtual bool ProcessInput();
	virtual bool Render();

	static CPlayerSelectScreen *GetInstance();
};
