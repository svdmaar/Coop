#pragma once

#include "Objects2D.h"

#include <string>

// TODO: read from extra text file if it is an animated sprite.
class CSprite
{
	bool m_bInited;
	std::string m_sName;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DTexture9 *m_pTexture;

	bool _CreateBuffer(const SFloatPoint& _fpSize);
	bool _CreateTexture(const CBitmap& _bitmap);
	bool _CreateBufferAndTexture(const CBitmap& _bitmap);

public:
	CSprite(const std::string& _sName);
	CSprite();
	~CSprite();

	bool Init();
	bool Init(const CBitmap & _bitmap);
	bool Render(const SFloatPoint & _pos) const;
	bool CleanUp();
};
