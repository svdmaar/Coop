#include "SpriteLoader.h"

#include "../IniFileReader/IniFile.h"
#include "../../Coop/Bitmap.h"
#include "Texture.h"

#include <sstream>

using namespace std;

CSpriteLoader::CSpriteLoader()
{
	m_bInited = false;
}

CSpriteLoader::~CSpriteLoader()
{
}

bool CSpriteLoader::Init(const std::string & _sFilenameBase)
{
	if(!m_iniFile.Load(_sFilenameBase + ".ini"))
	{
		return false;
	}

	if(!m_bitmap.Load(_sFilenameBase + ".bmp"))
	{
		return false;
	}

	m_bInited = true;
	return true;
}

CSprite * CSpriteLoader::LoadSpriteOld(const std::string & _sName)
{
	SSpriteDesc spriteDesc;

	spriteDesc.m_eWrapMode = SPRITEWRAP_ERROR;

	int nSprites = m_iniFile.GetValueInt("info", "count");
	
	int iSpriteIndex = -1;
	string sBlock;

	for(iSpriteIndex = 0; iSpriteIndex < nSprites; iSpriteIndex++)
	{
		sBlock = _FormatSpriteBlockName(iSpriteIndex);
		string sName = m_iniFile.GetValueString(sBlock, "name");

		if(sName == _sName)
		{
			break;
		}
	}

	if(iSpriteIndex == nSprites)
	{
		return NULL;
	}

	int iGridSize = m_iniFile.GetValueInt("grid", "size");
	int nGridPerRow = m_bitmap.GetWidth() / iGridSize;
	int iStartGridRow = m_iniFile.GetValueInt(sBlock, "startRow");
	int iStartGridColumn = m_iniFile.GetValueInt(sBlock, "startColumn");

	int nDirections = m_iniFile.GetValueInt(sBlock, "directions");

	if(nDirections != 0)
	{
		int nFramesPerDir = m_iniFile.GetValueInt(sBlock, "framesPerDir");

		for(int iDirection = 0; iDirection < nDirections; iDirection++)
		{
			SDirectionSpriteSet dirSpriteSet;
				
			stringstream ssDirKey;
			ssDirKey << "direction" << iDirection << "Dir";
			int iDir = m_iniFile.GetValueInt(sBlock, ssDirKey.str());
				
			dirSpriteSet.m_fDir = ((float)iDir) * 3.1415f / 180.0f;

			for(int iFrame = 0; iFrame < nFramesPerDir; iFrame++)
			{
				int iTotalGridFrameOffset = iStartGridColumn + iStartGridRow * nGridPerRow + iDirection * nFramesPerDir + iFrame;
				int iGridRow = iTotalGridFrameOffset / nGridPerRow;
				int iGridColumn = iTotalGridFrameOffset % nGridPerRow;

				RECT rSubBitmap;
				rSubBitmap.left = iGridColumn * iGridSize;
				rSubBitmap.top = iGridRow * iGridSize;
				rSubBitmap.right = rSubBitmap.left + iGridSize;
				rSubBitmap.bottom = rSubBitmap.top + iGridSize;
				
				CBitmap bmFrame = m_bitmap.GetSubBitmap(rSubBitmap);
				dirSpriteSet.m_frames.push_back(bmFrame);
			}

			spriteDesc.m_spriteSets.push_back(dirSpriteSet);
		}
	}

	string sWrapMode = m_iniFile.GetValueString(sBlock, "wrapMode");

	if(sWrapMode == "zigzag")
	{
		spriteDesc.m_eWrapMode = SPRITEWRAP_ZIGZAG;
	}

	CSprite * pOutput = CSprite::CreateOld(spriteDesc);
	return pOutput;
}

string CSpriteLoader::_FormatSpriteBlockName(int _iIndex)
{
	stringstream ssOut;

	ssOut << "sprite";

	if (_iIndex < 10)
	{
		ssOut << "0";
	}

	ssOut << _iIndex;
	
	return ssOut.str();
}

CSprite * CSpriteLoader::LoadSprite(const std::string & _sName)
{
	CSprite * pSprite = NULL;
	
	POINT pBackground = m_iniFile.GetValuePoint("color", "backgroundPixel");
	DWORD dwBackground = m_bitmap.GetPixel(pBackground.y, pBackground.x);

	CDataMap<float> mAlpha(m_bitmap.GetWidth(), m_bitmap.GetHeight());
	for(int iRow = 0; iRow < m_bitmap.GetHeight(); iRow++)
	{
		for(int iColumn = 0; iColumn < m_bitmap.GetWidth(); iColumn++)
		{
			DWORD dwPixel = m_bitmap.GetPixel(iRow, iColumn);
			float fStoredAlpha = 1.0f;
			if(dwPixel == dwBackground)
			{
				fStoredAlpha = 0.0f;
			}

			mAlpha.SetValue(iRow, iColumn, fStoredAlpha);
		}
	}
	
	CTexture * pTexture = CTexture::Create(m_bitmap, mAlpha);

	SSpriteDesc desc;
	_FillSpriteDesc(_sName, desc);

	pSprite = CSprite::Create(pTexture, desc);

	return pSprite;
}

bool CSpriteLoader::_FillSpriteDesc(const std::string & _sName, SSpriteDesc & _desc)
{
	_desc.m_eWrapMode = SPRITEWRAP_ERROR;

	int nSprites = m_iniFile.GetValueInt("info", "count");
	
	int iSpriteIndex = -1;
	string sBlock;

	for(iSpriteIndex = 0; iSpriteIndex < nSprites; iSpriteIndex++)
	{
		sBlock = _FormatSpriteBlockName(iSpriteIndex);
		string sName = m_iniFile.GetValueString(sBlock, "name");

		if(sName == _sName)
		{
			break;
		}
	}

	if(iSpriteIndex == nSprites)
	{
		return NULL;
	}

	int iGridSize = m_iniFile.GetValueInt("grid", "size");
	int nGridPerRow = m_bitmap.GetWidth() / iGridSize;
	int iStartGridRow = m_iniFile.GetValueInt(sBlock, "startRow");
	int iStartGridColumn = m_iniFile.GetValueInt(sBlock, "startColumn");

	int nDirections = m_iniFile.GetValueInt(sBlock, "directions");

	if(nDirections != 0)
	{
		int nFramesPerDir = m_iniFile.GetValueInt(sBlock, "framesPerDir");

		for(int iDirection = 0; iDirection < nDirections; iDirection++)
		{
			SDirectionSpriteSet dirSpriteSet;
				
			stringstream ssDirKey;
			ssDirKey << "direction" << iDirection << "Dir";
			int iDir = m_iniFile.GetValueInt(sBlock, ssDirKey.str());
				
			dirSpriteSet.m_fDir = ((float)iDir) * 3.1415f / 180.0f;

			for(int iFrame = 0; iFrame < nFramesPerDir; iFrame++)
			{
				int iTotalGridFrameOffset = iStartGridColumn + iStartGridRow * nGridPerRow + iDirection * nFramesPerDir + iFrame;
				int iGridRow = iTotalGridFrameOffset / nGridPerRow;
				int iGridColumn = iTotalGridFrameOffset % nGridPerRow;

				RECT rSubBitmap;
				rSubBitmap.left = iGridColumn * iGridSize;
				rSubBitmap.top = iGridRow * iGridSize;
				rSubBitmap.right = rSubBitmap.left + iGridSize;
				rSubBitmap.bottom = rSubBitmap.top + iGridSize;
				
				CBitmap bmFrame = m_bitmap.GetSubBitmap(rSubBitmap);
				dirSpriteSet.m_frames.push_back(bmFrame);

				dirSpriteSet.m_rSubRects.push_back(rSubBitmap);
			}

			_desc.m_spriteSets.push_back(dirSpriteSet);
		}
	}

	string sWrapMode = m_iniFile.GetValueString(sBlock, "wrapMode");

	// TODO: the rest.
	if(sWrapMode == "zigzag")
	{
		_desc.m_eWrapMode = SPRITEWRAP_ZIGZAG;
	}

	return true;
}
