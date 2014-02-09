#pragma once

#include "../../Coop/Bitmap.h"

#include <vector>

class CTexture;

// What to do when accessing frame index out-of-bounds
enum E_SPRITEWRAPMODE
{
	// gives assertion error.
	SPRITEWRAP_ERROR = 0,

	// Go back to first frame.
	SPRITEWRAP_MODULO,

	// Render min or max frame, whichever one is closest.
	SPRITEWRAP_CLAMP,

	// Go left-right through frames.
	SPRITEWRAP_ZIGZAG,
};

struct SDirectionSpriteSet
{
	float m_fDir; // In radians.
	std::vector<CBitmap> m_frames;
	std::vector<RECT> m_rSubRects;
};

struct SSpriteDesc
{
	std::vector<SDirectionSpriteSet> m_spriteSets;
	E_SPRITEWRAPMODE m_eWrapMode;
	CTexture * m_pTexture;
};
