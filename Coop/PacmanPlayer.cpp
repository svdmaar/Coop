#include "PacmanPlayer.h"
#include "FrameTimer.h"

#include <assert.h>

CPacmanPlayer::CPacmanPlayer()
{
	m_bInited = false;
}

CPacmanPlayer::~CPacmanPlayer()
{
	assert(!m_bInited);
}

bool CPacmanPlayer::Init(const CInputDevice * _pInputDevice)
{
	const int iSpriteSize = 32;
	const int iFrameCount = 8;
	const char * fileName = "D:\\projects\\Coop\\bin\\final\\Debug\\bitmap\\pacman.bmp";

	assert(!m_bInited);

	m_pos.m_fX = m_pos.m_fY = 0.0f;

	RECT rSpriteRect;
	rSpriteRect.left = 0;
	rSpriteRect.top = 0;
	rSpriteRect.bottom = iSpriteSize;
	rSpriteRect.right = iSpriteSize;
	if(!m_asLeft.Init(fileName, rSpriteRect, iFrameCount))
	{
		return false;
	}

	rSpriteRect.top += iSpriteSize;
	rSpriteRect.bottom += iSpriteSize;
	if(!m_asRight.Init(fileName, rSpriteRect, iFrameCount))
	{
		return false;
	}

	rSpriteRect.top += iSpriteSize;
	rSpriteRect.bottom += iSpriteSize;
	if(!m_asTop.Init(fileName, rSpriteRect, iFrameCount))
	{
		return false;
	}

	rSpriteRect.top += iSpriteSize;
	rSpriteRect.bottom += iSpriteSize;
	if(!m_asBottom.Init(fileName, rSpriteRect, iFrameCount))
	{
		return false;
	}

	m_pInputDevice = _pInputDevice;
	m_eDirection = DIR_RIGHT;

	m_bInited = true;
	return true;
}

bool CPacmanPlayer::CleanUp()
{
	assert(m_bInited);

	if(!m_asLeft.CleanUp())
	{
		return false;
	}

	if(!m_asRight.CleanUp())
	{
		return false;
	}

	if(!m_asTop.CleanUp())
	{
		return false;
	}

	if(!m_asBottom.CleanUp())
	{
		return false;
	}

	m_bInited = false;
	return true;
}

bool CPacmanPlayer::Update()
{
	/*
	CFrameTimer * pFrameTimer = CFrameTimer::GetInstance();

	float fDeltaT = (float)pFrameTimer->GetDeltaT();
	fDeltaT /= 50.0f;

	m_pos.m_fX += fDeltaT;
	*/

	bool bInput = false;
	E_DPADDIRECTION eDir = m_pInputDevice->GetDPadDirection(bInput);

	if(bInput)
	{
		m_eDirection = eDir;
	}

	_AddDirection();

	return true;
}

bool CPacmanPlayer::_RenderCorrectDirection()
{
	float fFrameIndex = 0.0f;

	if((m_eDirection == DIR_LEFT) || (m_eDirection == DIR_RIGHT))
	{
		fFrameIndex = fabsf(fmodf(m_pos.m_fX, 8.0f));
	}
	else
	{
		fFrameIndex = fabsf(fmodf(m_pos.m_fY, 8.0f));
	}

	fFrameIndex *= 2.0f;
	int iFrameIndex = (int)(fFrameIndex + 0.5f) % 16;

	if(iFrameIndex > 7)
	{
		iFrameIndex = 15 - iFrameIndex;
	}

	switch(m_eDirection)
	{
	case DIR_LEFT:
		if(!m_asLeft.Render(iFrameIndex, m_pos))
		{
			return false;
		}
		break;
	case DIR_RIGHT:
		if(!m_asRight.Render(iFrameIndex, m_pos))
		{
			return false;
		}
		break;
	case DIR_TOP:
		if(!m_asTop.Render(iFrameIndex, m_pos))
		{
			return false;
		}
		break;
	case DIR_BOTTOM:
		if(!m_asBottom.Render(iFrameIndex, m_pos))
		{
			return false;
		}
		break;
	}

	return true;
}

bool CPacmanPlayer::Render()
{
	if(!_RenderCorrectDirection())
	{
		return false;
	}

	return true;
}

void CPacmanPlayer::_AddDirection()
{
	const float fAddedFactor = 1.0f / 50.0f;
	
	CFrameTimer * pFrameTimer = CFrameTimer::GetInstance();
	float fOffset = fAddedFactor * (float)pFrameTimer->GetDeltaT();

	switch(m_eDirection)
	{
	case DIR_LEFT:
		m_pos.m_fX -= fOffset;
		break;
	case DIR_RIGHT:
		m_pos.m_fX += fOffset;
		break;
	case DIR_TOP:
		m_pos.m_fY += fOffset;
		break;
	case DIR_BOTTOM:
		m_pos.m_fY -= fOffset;
		break;
	}
}
