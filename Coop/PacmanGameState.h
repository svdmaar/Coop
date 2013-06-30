#pragma once

#include "PacmanBoard.h"

class CPacmanGameState
{
	CPacmanBoard m_board;
	bool m_bInited;

public:
	CPacmanGameState();
	~CPacmanGameState();

	bool Init();
	bool CleanUp();

	bool Update();
	bool Render();

};
