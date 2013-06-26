#pragma once

#include "Screen.h"
#include "Objects2D.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "PacmanPlayer.h"

class CTestScreen2D : public CScreen
{
	//CSprite m_sprite1;
	CAnimatedSprite m_asSprite1;
	CPacmanPlayer m_pacmanPlayer1;
	CPacmanPlayer m_pacmanPlayer2;

public:
	CTestScreen2D();
	virtual ~CTestScreen2D();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();

	virtual bool ProcessInput();

	virtual bool Render();
};
