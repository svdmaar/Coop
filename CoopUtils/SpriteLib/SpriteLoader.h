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

public:
	CSpriteLoader();
	~CSpriteLoader();

	bool Init(const std::string & _sFilenameBase);
	CSprite * LoadSprite(const std::string & _sName);



};
