#pragma once

#include "FloatRect.h"

#include <d3d9.h>

class CGraphicsUtils
{

public:
	static bool CreateColoredRectangle(SFloatRect _rPos, IDirect3DVertexBuffer9 **_ppVertexBuffer, DWORD _dwColor);
	static bool RenderVertexBuffer(IDirect3DVertexBuffer9 *_pVertexBuffer, size_t _vertexSize, DWORD _fvf, D3DPRIMITIVETYPE _type, UINT _iCount);
	static bool CalcRectCenter(const SFloatRect& _rect, float& _fCenterX, float& _fCenterY);
	static bool CreateColoredFrame(SFloatRect _rPos, IDirect3DVertexBuffer9 **_ppVertexBuffer, DWORD _dwColor);
};
