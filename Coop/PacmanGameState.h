#pragma once

#include "PacmanBoard.h"
#include "PacmanPlayer.h"

class CPacmanGameState
{
	CPacmanBoard m_board;
	CPacmanPlayer m_player;
	bool m_bInited;

	bool _UpdatePlayers();

public:
	CPacmanGameState();
	~CPacmanGameState();

	bool Init();
	bool CleanUp();

	bool Update();
	bool Render();

	bool ResizeSprites(float _fSize);
};
