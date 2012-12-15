#pragma once

#include "FloatRect.h"

#include <d3d9.h>

class CGraphicsUtils
{

public:
	static bool CalcRectCenter(const SFloatRect& _rect, float& _fCenterX, float& _fCenterY);
	static bool CreateColoredFrame(SFloatRect _rPos, IDirect3DVertexBuffer9 **_ppVertexBuffer, DWORD _dwColor);
};
