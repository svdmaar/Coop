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
	std::vector<POINT> m_vPacmanSpots;
	std::vector<POINT> m_vGhostsSpots;
	bool m_bInited;

	bool _LoadMapBitmap();
	bool _InitSquares();
	bool _InitWalls();
	bool _InitDot();
	bool _CleanUpSprites();
	DWORD _GetPixelSafe(int _iRowIndex, int _iColumnIndex, DWORD _dwDefault);
	int _GetWallSpriteIndex(int _iRowIndex, int _iColumnIndex);
	void _AddSpotToVector(int _iRowIndex, int _iColumnIndex);

public:
	CPacmanBoard();
	~CPacmanBoard();

	bool Init();
	bool CleanUp();

	bool Update();
	bool Render();

	int GetWidth() const;
	int GetHeight() const;

	bool ResizeSprites(float _fSize);
	bool FindPacmanStartPoints(POINT * _pPoints, int _iCount) const;
	bool RemoveDot(const POINT & _pPos);
};
