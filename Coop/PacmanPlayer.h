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
	SFloatPoint m_nextPos;
	const CInputDevice * m_pInputDevice;
	E_DPADDIRECTION m_eDirection;
	E_DPADDIRECTION m_eRequestedDirection;
	bool m_bInited;

	void _AddDirection();
	bool _RenderCorrectDirection();
	POINT _GetSquare(const SFloatPoint & _pfPos);
	SFloatPoint _GetPos(const POINT & _pPos);
	SFloatPoint _GetDirection();

public:
	CPacmanPlayer();
	~CPacmanPlayer();

	bool Init(const CInputDevice * _pInputDevice);
	bool CleanUp();

	bool Update();
	bool Render();

	bool ResizeSprites(float _fSize);
	
	bool SetSquare(const POINT & _pPos);
	bool GetSquare(POINT & _pPos);
	bool GetTargetSquare(POINT & _pPos);

	bool DoMove();
	bool StayOnCurrentSquare();
	bool AboutToPassSquareCenter();

	E_DPADDIRECTION GetRequestedDir() const;
	bool UpdateDirection();

	SFloatPoint GetPosition() const;
};
