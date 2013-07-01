#include "PacmanGameState.h"
#include "InputManager.h"

#include "Gui.h"

#include <assert.h>

CPacmanGameState::CPacmanGameState()
{
	m_bInited = false;
}

CPacmanGameState::~CPacmanGameState()
{
	assert(!m_bInited);
}

bool CPacmanGameState::Init()
{
	assert(!m_bInited);

	if(!m_board.Init())
	{
		return false;
	}

	float fScreenWidth = CGui::GetScreenRect().m_fRight - CGui::GetScreenRect().m_fLeft;
	float fNewSpiteSize = fScreenWidth / (float)m_board.GetWidth();
	if(!m_board.ResizeSprites(fNewSpiteSize))
	{
		return false;
	}

	POINT startPos[2];
	if(!m_board.FindPacmanStartPoints(startPos, 2))
	{
		return false;
	}

	CInputManager * pInputManager = CInputManager::GetInstance();
	const CInputDevice * pInputDevice = pInputManager->GetDevice(0);
	if(!m_player.Init(pInputDevice))
	{
		return false;
	}

	if(!m_player.ResizeSprites(fNewSpiteSize))
	{
		return false;
	}

	if(!m_player.SetSquare(startPos[0]))
	{
		return false;
	}

	if(!m_board.RemoveDot(startPos[0]))
	{
		return false;
	}

	m_bInited = true;

	return true;
}

bool CPacmanGameState::CleanUp()
{
	assert(m_bInited);

	if(!m_board.CleanUp())
	{
		return false;
	}

	if(!m_player.CleanUp())
	{
		return false;
	}

	m_bInited = false;

	return true;
}

bool CPacmanGameState::Update()
{
	if(!m_board.Update())
	{
		return false;
	}

	if(!m_player.Update())
	{
		return false;
	}

	POINT pPos;
	if(!m_player.GetSquare(pPos))
	{
		return false;
	}

	if(!m_board.RemoveDot(pPos))
	{
		return false;
	}

	return true;
}

bool CPacmanGameState::Render()
{
	if(!m_board.Render())
	{
		return false;
	}

	if(!m_player.Render())
	{
		return false;
	}

	return true;
}
