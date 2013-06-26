#pragma once

#include "AnimatedSprite.h"
#include "FloatPoint.h"
#include "InputDevice.h"

class CPacmanPlayer
{
	CAnimatedSprite m_asLeft;
	CAnimatedSprite m_asRight;
	CAnimatedSprite m_asTop;
	CAnimatedSprite m_asBottom;
	SFloatPoint m_pos;
	const CInputDevice * m_pInputDevice;
	E_DPADDIRECTION m_eDirection;
	bool m_bInited;

	void _AddDirection();
	bool _RenderCorrectDirection();

public:
	CPacmanPlayer();
	~CPacmanPlayer();

	bool Init(const CInputDevice * _pInputDevice);
	bool CleanUp();

	bool Update();
	bool Render();
};
