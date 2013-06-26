#include "CreatePacmanSprites.h"

#include "Bitmap.h"

#include <d3dx9math.h>
#include <assert.h>

#include <cmath>
using namespace std;

static const int g_iBitmapSize = 256;
static const int g_iSpriteSize = 32;
static const int g_iSpritePerRow = g_iBitmapSize / g_iSpriteSize;
static const DWORD g_dwBackgroundColor = 0xff00ff;
static const DWORD g_dwSpriteColor = 0xffff00;

static void CreatePacmanSprites(CBitmap & _bmOut)
{
	const int iSpriteCount = g_iBitmapSize / g_iSpriteSize;
	const float fMaxMouthAngle = 0.25f * D3DX_PI;
	const float fMouthCenterX = -0.2f;

	for(int iSpriteIndex = 0; iSpriteIndex < iSpriteCount; iSpriteIndex++)
	{
		float fMouthAngle = ((float)iSpriteIndex) * fMaxMouthAngle / (float)(iSpriteCount - 1);

		for(int iRowIndex = 0; iRowIndex < g_iSpriteSize; iRowIndex++)
		{
			for(int iColumnIndex = 0; iColumnIndex < g_iSpriteSize; iColumnIndex++)
			{
				int iInBmpColumnIndex = iColumnIndex + iSpriteIndex * g_iSpriteSize;

				float fNormColumnIndex = ((float)iColumnIndex) / ((float)g_iSpriteSize);
				float fNormRowIndex = ((float)iRowIndex) / ((float)g_iSpriteSize);
				float fX = fNormColumnIndex - 0.5f;
				float fY = fNormRowIndex - 0.5f;
				float fDistance = sqrtf(fX * fX + fY * fY);

				bool bColored = fDistance <= 0.5f;

				if(bColored && (fX >= fMouthCenterX))
				{
					float fAngle = fabsf(atanf(fY / (fX - fMouthCenterX)));

					bColored = fAngle >= fMouthAngle;
				}

				if (bColored)
				{
					_bmOut.SetPixel(iRowIndex, iInBmpColumnIndex, g_dwSpriteColor);
				}
			}
		}
	}
}

static void MirrorAndCloneGhosts(CBitmap & _bmOut)
{
	const char * inputFile = "c:\\temp\\pacman ghost2.bmp";
	const int nGhosts = 5;

	CBitmap bmInput = CBitmap(inputFile);
	DWORD dwGhostBackground = bmInput.GetPixel(0, 0);
	int iRowIndex, iColumnIndex;

	CBitmap bmGhost(g_iSpriteSize, g_iSpriteSize, g_dwBackgroundColor);

	for(iRowIndex = 0; iRowIndex < g_iSpriteSize; iRowIndex++)
	{
		for(iColumnIndex = 0; iColumnIndex < g_iSpriteSize / 2; iColumnIndex++)
		{
			DWORD dwPixel = bmInput.GetPixel(iRowIndex, iColumnIndex);

			if(dwPixel != dwGhostBackground)
			{
				if(dwPixel == 0x0)
				{
					dwPixel = g_dwSpriteColor;
				}

				bmGhost.SetPixel(iRowIndex, iColumnIndex, dwPixel);
				bmGhost.SetPixel(iRowIndex, g_iSpriteSize - 1 - iColumnIndex, dwPixel);
			}
		}
	}

	for(int iGhostIndex = 0; iGhostIndex < nGhosts; iGhostIndex++)
	{
		iRowIndex = g_iSpriteSize;
		iColumnIndex = iGhostIndex * g_iSpriteSize;

		_bmOut.InsertBitmap(iRowIndex, iColumnIndex, bmGhost);
	}
}

static void AddDots(CBitmap & _bmOut)
{
	const char * normalDotFileName = "C:\\temp\\pacman dot.bmp";
	const char * powerDotFileName = "C:\\temp\\pacman power dot.bmp";
	const int iRowIndex = g_iSpriteSize;
	const int iStartColumnIndex = 6 * g_iSpriteSize;

	CBitmap bmNormalDot(normalDotFileName);	
	CBitmap bmPowerDot(powerDotFileName);

	_bmOut.InsertBitmap(iRowIndex, iStartColumnIndex, bmNormalDot);
	_bmOut.InsertBitmap(iRowIndex, iStartColumnIndex + g_iSpriteSize, bmPowerDot);
}

static void AddRect(CBitmap & _bmOut, POINT offset, RECT _rect)
{
	for(int iRowIndex = _rect.top; iRowIndex < _rect.bottom; iRowIndex++)
	{
		for(int iColumnIndex = _rect.left; iColumnIndex < _rect.right; iColumnIndex++)
		{
			_bmOut.SetPixel(iRowIndex + offset.y, iColumnIndex + offset.x, g_dwSpriteColor);
		}
	}
}

static void AddWallSprites(CBitmap & _bmOut)
{
	const int iWallWidth = 6;
	const int iStartSpriteRow = 2;

	for(int iSprite = 0; iSprite < 16; iSprite++)
	{
		int iSpriteRow = iStartSpriteRow + iSprite / g_iSpritePerRow;
		int iSpriteColumn = iSprite % g_iSpritePerRow;
		CBitmap bmSprite(g_iSpriteSize, g_iSpriteSize, g_dwBackgroundColor);
		int iWallBits = ~iSprite & 0xf;

		POINT offset;
		offset.x = iSpriteColumn * g_iSpriteSize;
		offset.y = iSpriteRow * g_iSpriteSize;

		// top
		if(iWallBits & (1 << 0))
		{
			RECT rect;
			rect.left = (g_iSpriteSize - iWallWidth) / 2;
			rect.top = 0;
			rect.right = (g_iSpriteSize + iWallWidth) / 2;
			rect.bottom = (g_iSpriteSize + iWallWidth) / 2;

			AddRect(_bmOut, offset, rect);
		}

		// right
		if(iWallBits & (1 << 1))
		{
			RECT rect;
			rect.left = (g_iSpriteSize - iWallWidth) / 2;
			rect.top = (g_iSpriteSize - iWallWidth) / 2;
			rect.right = g_iSpriteSize;
			rect.bottom = (g_iSpriteSize + iWallWidth) / 2;

			AddRect(_bmOut, offset, rect);
		}

		// bottom
		if(iWallBits & (1 << 2))
		{
			RECT rect;
			rect.left = (g_iSpriteSize - iWallWidth) / 2;
			rect.top = (g_iSpriteSize - iWallWidth) / 2;
			rect.right = (g_iSpriteSize + iWallWidth) / 2;
			rect.bottom = g_iSpriteSize;

			AddRect(_bmOut, offset, rect);
		}

		// left
		if(iWallBits & (1 << 3))
		{
			RECT rect;
			rect.left = 0;
			rect.top = (g_iSpriteSize - iWallWidth) / 2;
			rect.right = (g_iSpriteSize + iWallWidth) / 2;
			rect.bottom = (g_iSpriteSize + iWallWidth) / 2;

			AddRect(_bmOut, offset, rect);
		}
	}
}

static void RotateBitmap(const CBitmap & _bmIn, CBitmap & _bmOut)
{
	assert(_bmIn.GetWidth() == _bmIn.GetHeight());

	_bmOut = CBitmap(_bmIn);

	for(int iRowIndex = 0; iRowIndex < _bmIn.GetHeight(); iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < _bmIn.GetWidth(); iColumnIndex++)
		{
			int iSourceRowIndex = _bmIn.GetWidth() - 1 - iColumnIndex;
			int iSourceColumnIndex = iRowIndex;

			DWORD dwPixel = _bmIn.GetPixel(iSourceRowIndex, iSourceColumnIndex);
			_bmOut.SetPixel(iRowIndex, iColumnIndex, dwPixel);
		}
	}
}

static void AddRotatedPacmanSprites(CBitmap & _bmOut)
{
	/*
	RECT rRest;
	rRest.left = 0;
	rRest.top = 32;
	rRest.right = _bmOut.GetWidth();
	rRest.bottom = 32 * 4;

	CBitmap bmRestImage = _bmOut.GetSubBitmap(rRest);
	_bmOut.InsertBitmap(32 * 4, 0, bmRestImage);
	*/

	/*
	RECT rPacmans;
	rPacmans.left = 0;
	rPacmans.top = 0;
	rPacmans.right = _bmOut.GetWidth();
	rPacmans.bottom = 32;

	CBitmap bmPacmans = _bmOut.GetSubBitmap(rPacmans);

	_bmOut.InsertBitmap(1 * 32, 0, bmPacmans);
	_bmOut.InsertBitmap(2 * 32, 0, bmPacmans);
	_bmOut.InsertBitmap(3 * 32, 0, bmPacmans);
	*/

	RECT rPacman;
	rPacman.left = 0;
	rPacman.top = 3 * 32;
	rPacman.right = 32;
	rPacman.bottom = rPacman.top + 32;

	for(int iPacman = 0; iPacman < 8; iPacman++)
	{
		rPacman.left = iPacman * 32;
		rPacman.right = rPacman.left + 32;

		CBitmap bmPacman = _bmOut.GetSubBitmap(rPacman);
		CBitmap bmRotated;

		RotateBitmap(bmPacman, bmRotated);
		bmPacman = bmRotated;

		/*
		RotateBitmap(bmPacman, bmRotated);
		bmPacman = bmRotated;

		RotateBitmap(bmPacman, bmRotated);
		bmPacman = bmRotated;
		*/

		_bmOut.InsertBitmap(3 * 32, rPacman.left, bmPacman);
	}

}

void CreateAllPacmanSprites()
{
	const char * bitmapName = "c:\\temp\\pacman.bmp";
	CBitmap bmOut(bitmapName);

	AddRotatedPacmanSprites(bmOut);

	/*
	CreatePacmanSprites(bmOut);
	MirrorAndCloneGhosts(bmOut);
	AddDots(bmOut);
	AddWallSprites(bmOut);
	*/

	bmOut.Save(bitmapName);
}
