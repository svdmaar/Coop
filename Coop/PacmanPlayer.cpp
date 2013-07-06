#include "PacmanPlayer.h"

#include "FrameTimer.h"
#include "Gui.h"

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
	m_nextPos = m_pos;

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
	bool bInput = false;
	E_DPADDIRECTION eDir = m_pInputDevice->GetDPadDirection(bInput);

	if(bInput)
	{
		m_eRequestedDirection = eDir;
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

	m_nextPos = m_pos;

	switch(m_eDirection)
	{
	case DIR_LEFT:
		m_nextPos.m_fX -= fOffset;
		break;
	case DIR_RIGHT:
		m_nextPos.m_fX += fOffset;
		break;
	case DIR_TOP:
		m_nextPos.m_fY += fOffset;
		break;
	case DIR_BOTTOM:
		m_nextPos.m_fY -= fOffset;
		break;
	}
}

bool CPacmanPlayer::ResizeSprites(float _fSize)
{
	if(!m_asLeft.Resize(_fSize))
	{
		return false;
	}
	
	if(!m_asRight.Resize(_fSize))
	{
		return false;
	}
	
	if(!m_asTop.Resize(_fSize))
	{
		return false;
	}
	
	if(!m_asBottom.Resize(_fSize))
	{
		return false;
	}

	return true;
}

bool CPacmanPlayer::SetSquare(const POINT & _pPos)
{
	m_pos = _GetPos(_pPos);

	return true;
}

bool CPacmanPlayer::GetSquare(POINT & _pPos)
{
	_pPos = _GetSquare(m_pos);

	return true;
}

POINT CPacmanPlayer::_GetSquare(const SFloatPoint & _pfPos)
{
	float fSpriteSize = m_asBottom.GetSize().m_fX;
	float fScreenLeft = CGui::GetScreenRect().m_fLeft;
	float fScreenTop = CGui::GetScreenRect().m_fTop;

	float fColumn = (m_pos.m_fX - fScreenLeft - 0.5f * fSpriteSize) / fSpriteSize;
	float fRow = (fScreenTop - m_pos.m_fY - 0.5f * fSpriteSize) / fSpriteSize;

	POINT pPos;
	pPos.x = (int)(fColumn + 0.5f);
	pPos.y = (int)(fRow + 0.5f);

	return pPos;
}

SFloatPoint CPacmanPlayer::_GetPos(const POINT & _pPos)
{
	float fSpriteSize = m_asBottom.GetSize().m_fX;
	float fColumn = (float)_pPos.x;
	float fRow = (float)_pPos.y;
	float fScreenLeft = CGui::GetScreenRect().m_fLeft;
	float fScreenTop = CGui::GetScreenRect().m_fTop;

	SFloatPoint fpPos;
	fpPos.m_fX = fScreenLeft + 0.5f * fSpriteSize + fColumn * fSpriteSize;
	fpPos.m_fY = fScreenTop - 0.5f * fSpriteSize - fRow * fSpriteSize;

	return fpPos;
}

bool CPacmanPlayer::GetTargetSquare(POINT & _pPos)
{
	if(!GetSquare(_pPos))
	{
		return false;
	}

	switch(m_eRequestedDirection)
	{
	case DIR_LEFT:
		_pPos.x--;
		break;
	case DIR_RIGHT:
		_pPos.x++;
		break;
	case DIR_TOP:
		_pPos.y++;
		break;
	case DIR_BOTTOM:
		_pPos.y--;
		break;
	}

	return true;
}

bool CPacmanPlayer::DoMove()
{
	m_pos = m_nextPos;

	return true;
}

bool CPacmanPlayer::StayOnCurrentSquare()
{
	POINT pPos;

	pPos = _GetSquare(m_pos);
	m_pos = _GetPos(pPos);

	return true;
}

bool CPacmanPlayer::AboutToPassSquareCenter()
{
	SFloatPoint fpCurrOffset;
	SFloatPoint fpNextOffset;

	fpCurrOffset = m_pos;
	fpNextOffset = m_nextPos;

	if(fpCurrOffset.m_fX < 0.0f)
	{
		fpCurrOffset.m_fX -= floorf(fpCurrOffset.m_fX);
	}

	if(fpCurrOffset.m_fY < 0.0f)
	{
		fpCurrOffset.m_fY -= floorf(fpCurrOffset.m_fY);
	}

	if(fpNextOffset.m_fX < 0.0f)
	{
		fpNextOffset.m_fX -= floorf(fpNextOffset.m_fX);
	}

	if(fpNextOffset.m_fY < 0.0f)
	{
		fpNextOffset.m_fY -= floorf(fpNextOffset.m_fY);
	}

	fpCurrOffset.m_fX = fmodf(fpCurrOffset.m_fX, 1.0f);
	fpCurrOffset.m_fY = fmodf(fpCurrOffset.m_fY, 1.0f);
	fpNextOffset.m_fX = fmodf(fpNextOffset.m_fX, 1.0f);
	fpNextOffset.m_fY = fmodf(fpNextOffset.m_fY, 1.0f);

	switch(m_eDirection)
	{
	case DIR_LEFT:
		if((fpCurrOffset.m_fX >= 0.5f) && (fpNextOffset.m_fX < 0.5f))
		{
			return true;
		}
		break;
	case DIR_RIGHT:
		if((fpCurrOffset.m_fX < 0.5f) && (fpNextOffset.m_fX >= 0.5f))
		{
			return true;
		}
		break;
	case DIR_TOP:
		if((fpCurrOffset.m_fY < 0.5f) && (fpNextOffset.m_fY >= 0.5f))
		{
			return true;
		}
		break;
	case DIR_BOTTOM:
		if((fpCurrOffset.m_fY < 0.5f) && (fpNextOffset.m_fY >= 0.5f))
		{
			return true;
		}
		break;
	}

	return false;
}

E_DPADDIRECTION CPacmanPlayer::GetRequestedDir() const
{
	return m_eRequestedDirection;
}

bool CPacmanPlayer::UpdateDirection()
{
	m_eDirection = m_eRequestedDirection;

	return true;
}

SFloatPoint CPacmanPlayer::GetPosition() const
{
	return m_pos;
}

