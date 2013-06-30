#include "PacmanGameState.h"

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

	return true;
}

bool CPacmanGameState::CleanUp()
{
	assert(m_bInited);

	if(!m_board.CleanUp())
	{
		return false;
	}

	return true;
}

bool CPacmanGameState::Update()
{
	if(!m_board.Update())
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

	return true;
}
