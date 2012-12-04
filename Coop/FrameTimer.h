#pragma once

#include <Windows.h>

const int g_iFrameTimingCount = 128;

class CFrameTimer
{
	DWORD m_dwTimings[g_iFrameTimingCount];
	int m_iBufferIndex;

	static CFrameTimer *m_pInstance;

public:
	CFrameTimer();

	DWORD GetCurrTime();
	DWORD GetFrameRate();
	void OnNewFrame();

	static CFrameTimer *GetInstance();
};
