#pragma once

#include "../../Coop/Bitmap.h"

struct SCharDesc
{
	CBitmap m_bitmap;
	POINT m_pUpperLeft;
	int m_iBufferLeft;
	int m_iBufferRight;
	POINT m_pInBitmap;
};
