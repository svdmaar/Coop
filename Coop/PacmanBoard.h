#pragma once

#include "AnimatedSprite.h"
#include "DataMap.h"

struct SPacmanSquare
{
	bool m_bWall;
	int m_iWallIndex;
	bool m_bDotRemaining;
};

class CPacmanBoard
{
	CDataMap<SPacmanSquare> m_squares;
	CAnimatedSprite m_asWalls[15];
	bool m_bInited;

	bool _InitSquares();
	bool _InitWalls();

public:
	CPacmanBoard();
	~CPacmanBoard();

	bool Init();
	bool CleanUp();

	bool Update();
	bool Render();
};
