#pragma once

#include "../../Coop/Bitmap.h"
#include "../../Coop/DataMap.h"

class CVideoDevice;

class CTexture
{
	struct STextureImpl;
	STextureImpl * m_pImpl;

	CTexture();

public:

	// TODO: use CDataMap<float>
	static CTexture * Create(const CBitmap & _bmColor, const CDataMap<float> & _mAlpha);

	friend class CVideoDevice;
};
