#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct SCharPosition
{
	POINT m_pPos;
	int m_iIndex;	// 0 up to g_nCharCount (exclusive)
};
