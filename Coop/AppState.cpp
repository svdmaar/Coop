#include "AppState.h"

#include <assert.h>

CAppState::CAppState()
{
	m_pMainInputDevice = NULL;

	assert(m_pInstance == NULL);

	m_pInstance = this;
}

CAppState::~CAppState()
{
}

const CInputDevice *CAppState::GetMainInputDevice()
{
	return m_pMainInputDevice;
}

void CAppState::SetMainInputDevice(const CInputDevice *_pMainInputDevice)
{
	assert(m_pMainInputDevice == NULL);

	m_pMainInputDevice = _pMainInputDevice;
}

CAppState *CAppState::GetInstance()
{
	return m_pInstance;
}

bool CAppState::Init()
{
	return true;
}

bool CAppState::CleanUp()
{
	return true;
}

CAppState *CAppState::m_pInstance = NULL;
