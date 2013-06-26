#include "PacmanGameState.h"

int CPacmanGameState::_CountDots()
{
	return m_Dots.GetCount(true);
}
