#include "FrameTimer.h"

CFrameTimer::CFrameTimer()
{
	m_iBufferIndex = 0;
	ZeroMemory(m_dwTimings, sizeof(m_dwTimings));
	m_dwTimings[0] = GetTickCount();
	m_dwDeltaT = 0;

	m_pInstance = this;
}

DWORD CFrameTimer::GetCurrTime()
{
	return m_dwTimings[m_iBufferIndex];
}

DWORD CFrameTimer::GetDeltaT()
{
	return m_dwDeltaT;
}

DWORD CFrameTimer::GetFrameRate()
{
	DWORD dwCurrentTime = m_dwTimings[m_iBufferIndex];
	DWORD dwPastTime = m_dwTimings[(m_iBufferIndex + 1) % g_iFrameTimingCount];
	return 1000 * g_iFrameTimingCount / (dwCurrentTime - dwPastTime);
}

void CFrameTimer::OnNewFrame()
{
	DWORD dwLastTime = GetCurrTime();
	
	m_iBufferIndex = (m_iBufferIndex + 1) % g_iFrameTimingCount;
	m_dwTimings[m_iBufferIndex] = GetTickCount();
	m_dwDeltaT = m_dwTimings[m_iBufferIndex] - dwLastTime;
}

CFrameTimer *CFrameTimer::GetInstance()
{
	return m_pInstance;
}

CFrameTimer *CFrameTimer::m_pInstance = NULL;
