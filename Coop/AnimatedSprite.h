#pragma once

#include "Sprite.h"

#include <vector>
#include <string>

class CAnimatedSprite
{
	std::vector<CSprite *> m_vFrames;
	bool m_bInited;

public:
	CAnimatedSprite();
	~CAnimatedSprite();

	bool Init(const std::string & _sFileName, const RECT & _firstFrame, int _nFrames);
	bool Init(const CBitmap & _bitmap);
	bool Init(const std::string & _sName, int _iRow);
	bool CleanUp();
	bool Render(int iFrameIndex, const SFloatPoint & _pos);

	int GetFrameCount() const;
	bool Resize(float _fSize);
	SFloatPoint GetSize() const;
};
