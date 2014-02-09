#pragma once

#include "Sprite.h"
#include "../Direct9dDemos/FloatPoint.h"

class CRenderedSprite
{
	CSprite * m_pSprite;

	// Simple values to get/set.
	int m_iFrameIndex;
	float m_fDirection;
	SFloatPoint m_pPosition;

	// Struct used to render.
	SSpriteRenderArgs m_renderArgs;

	bool _CalculatePreRender();

public:
	CRenderedSprite(CSprite * _pSprite);

	void SetFrameIndex(int _iIndex);
	int GetFrameIndex() const;
	void SetDirection(float _fDirection);
	float GetDirection() const;
	void SetPosition(const SFloatPoint & _pPosition);
	SFloatPoint GetPosition() const;

	bool Render();
};
