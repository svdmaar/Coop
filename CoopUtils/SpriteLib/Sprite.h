#pragma once

#include "SpriteDesc.h"
#include "../Direct9dDemos/FloatRect.h"

class CTexture;
struct SSpriteImpl;

struct SSpriteRenderArgs
{
	SFloatPoint m_pPosition;
	int m_iDirection;
	float m_fRotation;
	int m_iFrameIndex;
};

class CSprite
{
	SSpriteImpl * m_pImpl;
	CTexture * m_pTexture;

	// Simple copies
	float m_fDir;
	int m_iIndex;

	// Pre-calculated for rendering
	int m_iRenderedIndex;
	int m_iRenderedDir;
	float m_fRenderedRotation;

	bool _CalculatePreRender();

	CSprite();

public:
	bool SetDirection(float _fDir);
	bool SetFrameIndex(int _iIndex);

	bool RenderOld();
	bool Render(const SSpriteRenderArgs & _args);

	void Release();

	static CSprite * CreateOld(const SSpriteDesc & _spriteDesc);
	static CSprite * Create(CTexture * _pTexture, const SSpriteDesc & _spriteDesc);
};
