#include "Keyboard.h"

#include "GraphicsManager.h"
#include "Logger.h"

#include <assert.h>
#include <sstream>

using namespace std;

CKeyboard::CKeyboard()
{
	m_pDevice = NULL;
	
	ZeroMemory(m_state, sizeof(m_state));
}

CKeyboard::~CKeyboard()
{
}

bool CKeyboard::Init(LPDIRECTINPUTDEVICE8 _pDevice)
{
	HRESULT hr;

	if(!CInputDevice::Init())
	{
		return false;
	}

	m_pDevice = _pDevice;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	hr = m_pDevice->SetCooperativeLevel(pGraphicsManager->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set cooperative level", hr);
		return false;
	}

	hr = m_pDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set data format", hr);
		return false;
	}
	
	hr = m_pDevice->Acquire();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to acquire device", hr);
		return false;
	}

	return true;
}

bool CKeyboard::CleanUp()
{
	m_pDevice->Unacquire();

	m_pDevice->Release();
	m_pDevice = NULL;

	if(!CInputDevice::CleanUp())
	{
		return false;
	}

	return true;
}

E_INPUTTYPE CKeyboard::GetType() const
{
	return INTYPE_KEYBOARD;
}

bool CKeyboard::HasJoystick1() const
{
	return false;
}

bool CKeyboard::HasJoystick2() const
{
	return false;
}

bool CKeyboard::HasDPad() const
{
	return true;
}

bool CKeyboard::HasForceFeedback() const
{
	return false;
}

int CKeyboard::GetButtonCount() const
{
	return g_iDIKeyboardKeyCount;
}

bool CKeyboard::_ButtonIsDown(int _iButtonIndex) const
{
	assert((_iButtonIndex >= 0) && (_iButtonIndex < g_iDIKeyboardKeyCount));

	return m_state[_iButtonIndex] != 0;
}

float CKeyboard::GetXAxis1() const
{
	return 0.0f;
}

float CKeyboard::GetYAxis1() const
{
	return 0.0f;
}

float CKeyboard::GetXAxis2() const
{
	return 0.0f;
}

float CKeyboard::GetYAxis2() const
{
	return 0.0f;
}

float CKeyboard::GetDPadXAxis() const
{
	return 0.0f;
}

float CKeyboard::GetDPadYAxis() const
{
	return 0.0f;
}

bool CKeyboard::_OnStateUpdateRequest()
{
	HRESULT hr;

	hr = m_pDevice->Poll();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to poll device", hr);
		return false;
	}

	hr = m_pDevice->GetDeviceState(sizeof(m_state), &m_state);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get device state", hr);
		return false;
	}

	return true;
}

string CKeyboard::GetDownKeyList() const
{
	stringstream out;
	bool bFirst = true;

	for(int i = 0; i < g_iDIKeyboardKeyCount; i++)
	{
		if(m_state[i] != 0)
		{
			if(!bFirst)
			{
				out << " ";
			}
			else
			{
				bFirst = false;
			}

			out << i;
		}
	}

	return out.str();	
}
