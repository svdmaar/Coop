#pragma once

#include "SpriteDesc.h"

struct SSpriteImpl;

class CSprite
{
	SSpriteImpl * m_pImpl;

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

	bool Render();

	void Release();

	static CSprite * Create(const SSpriteDesc & _spriteDesc);
};
