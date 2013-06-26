#pragma once

#include "Screen.h"
#include "FloatPoint.h"
#include "Objects2D.h"

const int g_iPacmanBoardWidth = 30;
const int g_iPacmanBoardHeight = 20;

class CPacmanScreen : public CScreen
{
	// Todo: CDataMap<bool> instead.
	bool m_bDots[g_iPacmanBoardWidth * g_iPacmanBoardHeight];
	SFloatPoint m_pPlayerPos;
	CImageObject m_objDot;

	bool _RenderBoard();

public:
	CPacmanScreen();
	virtual ~CPacmanScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();
	virtual bool ProcessInput();
	virtual bool Render();
};
