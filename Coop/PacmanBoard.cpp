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

	if(!_InitDot())
	{
		return false;
	}

	m_bInited = true;

	return true;
}

bool CPacmanBoard::CleanUp()
{
	assert(m_bInited);

	if(!_CleanUpSprites())
	{
		return false;
	}

	m_bInited = false;
	
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
	float fSquareSize = m_asWalls[0].GetSize().m_fX;

	SFloatPoint pUpperLeft;
	pUpperLeft.m_fX = -fSquareSize * ((float)iWidth) / 2.0f;
	pUpperLeft.m_fY = fSquareSize * ((float)iHeight) / 2.0f;

	pUpperLeft.m_fX += fSquareSize / 2.0f;
	pUpperLeft.m_fY += fSquareSize / 2.0f;

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
				int iWallIndex = square.m_iWallIndex;
				if(!m_asWalls[iWallIndex].Render(0, pPos))
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
				square.m_iWallIndex = _GetWallSpriteIndex(iRowIndex, iColumnIndex);
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

DWORD CPacmanBoard::_GetPixelSafe(int _iRowIndex, int _iColumnIndex, DWORD _dwDefault)
{
	if((_iRowIndex >= 0) && (_iRowIndex < m_bmMap.GetHeight()) &&
		(_iColumnIndex >= 0) && (_iColumnIndex < m_bmMap.GetWidth()))
	{
		return m_bmMap.GetPixel(_iRowIndex, _iColumnIndex);		
	}

	return _dwDefault;
}

int CPacmanBoard::_GetWallSpriteIndex(int _iRowIndex, int _iColumnIndex)
{
	DWORD dwThis = m_bmMap.GetPixel(_iRowIndex, _iColumnIndex);
	DWORD dwLeft = _GetPixelSafe(_iRowIndex, _iColumnIndex - 1, 0xffffffL);
	DWORD dwRight = _GetPixelSafe(_iRowIndex, _iColumnIndex + 1, 0xffffffL);
	DWORD dwTop = _GetPixelSafe(_iRowIndex - 1, _iColumnIndex, 0xffffffL);
	DWORD dwBottom = _GetPixelSafe(_iRowIndex + 1, _iColumnIndex, 0xffffffL);

	assert(dwThis == 0);

	int iIndex = 0;

	if(dwTop == 0)
	{
		iIndex += 1;
	}

	if(dwRight == 0)
	{
		iIndex += 2;
	}

	if(dwBottom == 0)
	{
		iIndex += 4;
	}

	if(dwLeft == 0)
	{
		iIndex += 8;
	}

	iIndex = 15 - iIndex;
	return iIndex;
}

int CPacmanBoard::GetWidth() const
{
	return m_squares.GetWidth();
}

int CPacmanBoard::GetHeight() const
{
	return m_squares.GetHeight();
}

bool CPacmanBoard::ResizeSprites(float _fSize)
{
	for(int i = 0; i < g_iWallSpriteCount; i++)
	{
		if(!m_asWalls[i].Resize(_fSize))
		{
			return false;
		}
	}

	if(!m_asDot.Resize(_fSize))
	{
		return false;
	}

	return true;
}

void CPacmanBoard::_AddSpotToVector(int _iRowIndex, int _iColumnIndex)
{
	int iBitmapCenterRow = m_bmMap.GetHeight() / 2;
	int iBitmapCenterColumn = m_bmMap.GetWidth() / 2;


}

bool CPacmanBoard::FindPacmanStartPoints(POINT * _pPoints, int _iCount) const
{
	_pPoints[0].x = 1;
	_pPoints[0].y = 1;

	if(_iCount > 1)
	{
		_pPoints[1].x = GetWidth() - 2;
		_pPoints[1].y = GetHeight() - 2;
	}
	
	return true;
}

bool CPacmanBoard::RemoveDot(const POINT & _pPos)
{
	m_squares.GetValueRef(_pPos.y, _pPos.x).m_bDotRemaining = false;

	return true;
}

bool CPacmanBoard::HasWall(const POINT & _pPos) const
{
	SPacmanSquare square = m_squares.GetValue(_pPos.y, _pPos.x);

	return square.m_bWall;
}
