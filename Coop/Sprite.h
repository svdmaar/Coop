#pragma once

#include "Objects2D.h"
#include "VertexTypes.h"
#include "FloatPoint.h"

#include <string>

class CSprite
{
	bool m_bInited;
	std::string m_sName;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DTexture9 *m_pTexture;
	SFloatPoint m_fpSize;

	bool _CreateBuffer();
	bool _CreateTexture(const CBitmap& _bitmap);
	bool _CreateBufferAndTexture(const CBitmap& _bitmap);
	void _FillRectVertices(SFaceVertex * _pVertices);

public:
	CSprite(const std::string& _sName);
	CSprite();
	~CSprite();

	bool Init();
	bool Init(const CBitmap & _bitmap);
	bool Render(const SFloatPoint & _pos) const;
	bool CleanUp();
	bool Resize(float _fSize);
	SFloatPoint GetSize() const;
};
