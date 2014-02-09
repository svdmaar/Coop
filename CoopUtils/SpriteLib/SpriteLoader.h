#pragma once

#include "Sprite.h"
#include "../IniFileReader/IniFile.h"
#include "../../Coop/Bitmap.h"

#include <string>

// TODO: register several ini/bmp files, storing all names.
class CSpriteLoader
{
	CIniFile m_iniFile;
	CBitmap m_bitmap;

	bool m_bInited;

	static std::string _FormatSpriteBlockName(int _iIndex);
	bool _FillSpriteDesc(const std::string & _sName, SSpriteDesc & _desc);

	// Todo: keep track of textures and multiple bmp/ini files.

public:
	CSpriteLoader();
	~CSpriteLoader();

	bool Init(const std::string & _sFilenameBase);
	CSprite * LoadSpriteOld(const std::string & _sName);

	CSprite * LoadSprite(const std::string & _sName);

};
