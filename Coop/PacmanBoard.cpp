#include "PacmanBoard.h"

#include <assert.h>

CPacmanBoard::CPacmanBoard()
{
	m_bInited = false;
}

CPacmanBoard::~CPacmanBoard()
{
	assert(!m_bInited);
}

bool CPacmanBoard::Init()
{
	assert(!m_bInited);

	if(!_InitSquares())
	{
		return false;
	}

	if(!_InitWalls())
	{
		return false;
	}

	return true;
}

bool CPacmanBoard::CleanUp()
{
	assert(m_bInited);

	return true;
}

bool CPacmanBoard::Update()
{
	return true;
}

bool CPacmanBoard::Render()
{
	return true;
}

bool CPacmanBoard::_InitSquares()
{
	return true;
}

bool CPacmanBoard::_InitWalls()
{
	return true;
}
