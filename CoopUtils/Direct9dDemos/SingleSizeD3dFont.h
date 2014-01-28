#pragma once

#include "../FontRipper/SingleSizeFont.h"
#include "FloatRect.h"
#include "VertexFormats.h"

#include <d3d9.h>
#include <d3dx9.h>

class CSingleSizeD3dFont : public CSingleSizeFont
{
	IDirect3DVertexBuffer9 * m_pVertexBuffer;
	IDirect3DTexture9 * m_pVideoTexture;
	POINT m_pScreenSize;
	SFloatPoint m_pUpperLeft;
	SFloatPoint m_pPixelSize;
	DWORD m_dwColor;

	bool _CalcUpperLeftCoor(const POINT & _pPixel, SFloatPoint & _pCoor) const;
	bool _CalcCoorsForPixels(const RECT & _rPixels, SFloatRect & _rCoors) const;
	bool _InitUpperLeftCoor();
	bool _CalcTextureCoors(const RECT & _rTexels, SFloatRect & _rTexCoors) const;
	bool _FillVertexBuffer(STexturedVertex * _pVertices) const;
	bool _CalcRelCoorsForChar(const SCharDesc & _charDesc, SFloatRect & _rCoors) const;
	DWORD _ColorPixel(DWORD _dwPixel) const;

public:
	CSingleSizeD3dFont();
	~CSingleSizeD3dFont();

	bool Init(const std::string & _sFilenameBase, const POINT & _pScreenSize, DWORD _dwColor);
	bool RenderString(const std::string & _sText, const POINT & _pPixel);
};
