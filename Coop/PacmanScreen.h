#pragma once

#include "Screen.h"
#include "FloatPoint.h"
#include "Objects2D.h"
#include "PacmanGameState.h"

const int g_iPacmanBoardWidth = 30;
const int g_iPacmanBoardHeight = 20;

class CPacmanScreen : public CScreen
{
	CPacmanGameState m_gameState;

public:
	CPacmanScreen();
	virtual ~CPacmanScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();
	virtual bool ProcessInput();
	virtual bool Render();
};
