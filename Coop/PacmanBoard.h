#pragma once

#include "AnimatedSprite.h"
#include "DataMap.h"

struct SPacmanSquare
{
	bool m_bWall;
	int m_iWallIndex;
	bool m_bDotRemaining;
	bool m_bPowerDotRemaining;
};

const int g_iWallSpriteCount = 15;

class CPacmanBoard
{
	CBitmap m_bmMap;
	CDataMap<SPacmanSquare> m_squares;
	CAnimatedSprite m_asWalls[g_iWallSpriteCount];
	CAnimatedSprite m_asDot;
	bool m_bInited;

	bool _LoadMapBitmap();
	bool _InitSquares();
	bool _InitWalls();
	bool _InitDot();
	bool _CleanUpSprites();

public:
	CPacmanBoard();
	~CPacmanBoard();

	bool Init();
	bool CleanUp();

	bool Update();
	bool Render();
};
