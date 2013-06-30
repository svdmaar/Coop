#include "PacmanBoard.h"

#include "Logger.h"

#include <assert.h>

CPacmanBoard::CPacmanBoard()
{
	m_bInited = false;
}

CPacmanBoard::~CPacmanBoard()
{
	assert(!m_bInited);
}

bool CPacmanBoard::Init()
{
	assert(!m_bInited);

	if(!_LoadMapBitmap())
	{
		return false;
	}

	if(!_InitSquares())
	{
		return false;
	}

	if(!_InitWalls())
	{
		return false;
	}

	return true;
}

bool CPacmanBoard::CleanUp()
{
	assert(m_bInited);

	if(!_CleanUpSprites())
	{
		return false;
	}

	return true;
}

bool CPacmanBoard::Update()
{
	return true;
}

bool CPacmanBoard::Render()
{
	int iWidth = m_squares.GetWidth();
	int iHeight = m_squares.GetHeight();
	const float fSquareSize = 8.0f;

	SFloatPoint pUpperLeft;
	pUpperLeft.m_fX = -fSquareSize * ((float)iWidth) / 2.0f;
	pUpperLeft.m_fY = fSquareSize * ((float)iHeight) / 2.0f;

	for(int iRowIndex = 0; iRowIndex < iHeight; iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < iWidth; iColumnIndex++)
		{
			SPacmanSquare square = m_squares.GetValue(iRowIndex, iColumnIndex);
			SFloatPoint pPos;
			
			pPos.m_fX = pUpperLeft.m_fX + fSquareSize * (float)iColumnIndex;
			pPos.m_fY = pUpperLeft.m_fY - fSquareSize * (float)iRowIndex;

			if(square.m_bWall)
			{
				if(!m_asWalls[0].Render(0, pPos))
				{
					return false;
				}
			}
			else if(square.m_bDotRemaining)
			{
				if(!m_asDot.Render(0, pPos))
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool CPacmanBoard::_LoadMapBitmap()
{
	const char * fileName = "C:\\Temp\\pacman board.bmp";
	if(!m_bmMap.Load(fileName))
	{
		LogError("Failed to load pacman map bitmap");
		return false;
	}

	return true;
}

bool CPacmanBoard::_InitSquares()
{
	int iWidth = m_bmMap.GetWidth();
	int iHeight = m_bmMap.GetHeight();

	m_squares.Resize(iWidth, iHeight);

	for(int iRowIndex = 0; iRowIndex < iHeight; iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < iWidth; iColumnIndex++)
		{
			DWORD dwPixel = m_bmMap.GetPixel(iRowIndex, iColumnIndex);
			SPacmanSquare square;
			ZeroMemory(&square, sizeof(square));

			if(dwPixel != 0)
			{
				square.m_bDotRemaining = true;
				square.m_bWall = false;
				square.m_iWallIndex = 0;
			}
			else
			{
				square.m_bDotRemaining = false;
				square.m_bWall = true;
				square.m_iWallIndex = 0;
			}

			m_squares.SetValue(iRowIndex, iColumnIndex, square);
		}
	}

	return true;
}

bool CPacmanBoard::_InitWalls()
{
	const int iWallCount = g_iWallSpriteCount;
	const int iSpritesPerRow = 8;
	const int iSpriteSize = 32;
	const int iSpriteStartRow = 5;
	const char * fileName = "c:\\temp\\pacman.bmp";

	for(int iWallIndex = 0; iWallIndex < iWallCount; iWallIndex++)
	{
		int iSpriteRow = iSpriteStartRow + iWallIndex / iSpritesPerRow;
		int iSpriteColumn = iWallIndex % iSpritesPerRow;

		RECT rSprite;
		rSprite.left = iSpriteColumn * iSpriteSize;
		rSprite.top = iSpriteRow * iSpriteSize;
		rSprite.right = rSprite.left + iSpriteSize;
		rSprite.bottom = rSprite.top + iSpriteSize;

		if(!m_asWalls[iWallIndex].Init(fileName, rSprite, 1))
		{
			return false;
		}
	}

	return true;
}

bool CPacmanBoard::_InitDot()
{
	const char * fileName = "c:\\temp\\pacman.bmp";
	const int iSpriteSize = 32;
	const int iSpriteRow = 4;
	const int iSpriteColumn = 6;

	RECT rSprite;
	rSprite.left = iSpriteColumn * iSpriteSize;
	rSprite.top = iSpriteRow * iSpriteSize;
	rSprite.right = rSprite.left + iSpriteSize;
	rSprite.bottom = rSprite.top + iSpriteSize;

	return m_asDot.Init(fileName, rSprite, 1);
}

bool CPacmanBoard::_CleanUpSprites()
{
	for(int iWall = 0; iWall < g_iWallSpriteCount; iWall++)
	{
		if(!m_asWalls[iWall].CleanUp())
		{
			return false;
		}
	}

	if(!m_asDot.CleanUp())
	{
		return false;
	}

	return true;
}
