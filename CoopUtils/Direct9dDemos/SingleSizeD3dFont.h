#pragma once

#include "../FontRipper/SingleSizeFont.h"
#include "FloatRect.h"

#include <d3d9.h>
#include <d3dx9.h>

class CSingleSizeD3dFont : public CSingleSizeFont
{
	IDirect3DVertexBuffer9 * m_pVertexBuffer;
	IDirect3DTexture9 * m_pVideoTexture;
	POINT m_pScreenSize;

	bool _CalcUpperLeftCoor(const POINT & _pPixel, SFloatPoint & _pCoor) const;
	bool _CalcCoorstForPixels(const RECT & _rPixels, SFloatRect & _rCoors) const;

public:
	CSingleSizeD3dFont();
	~CSingleSizeD3dFont();

	bool Init(const std::string & _sFilenameBase, const POINT & _pScreenSize);
	bool RenderString(const std::string & _sText, const POINT & _pPixel);
};
