#include "InputDevice.h"

#include <assert.h>
#include <sstream>

using namespace std;

CInputDevice::CInputDevice()
{
	m_bInited = false;
	m_fXAxis1 = 0.0f;
	m_fYAxis1 = 0.0f;
	m_fXAxis2 = 0.0f;
	m_fYAxis2 = 0.0f;
	m_fDPadX = 0.0f;
	m_fDPadY = 0.0f;

	ZeroMemory(m_bButtonIsDown, sizeof(m_bButtonIsDown));
	
	ZeroMemory(m_dwButtonDownTime, sizeof(m_dwButtonDownTime));
	ZeroMemory(m_bButtonJustPressed, sizeof(m_bButtonJustPressed));
	ZeroMemory(m_bDPadJustPressed, sizeof(m_bDPadJustPressed));
	ZeroMemory(m_bJoystick1JustInDir, sizeof(m_bJoystick1JustInDir));
}

CInputDevice::~CInputDevice()
{
	assert(!m_bInited);
}

bool CInputDevice::Init()
{
	m_bInited = true;

	return true;
}

bool CInputDevice::CleanUp()
{
	m_bInited = false;

	return true;
}

string CInputDevice::GetStateString() const
{
	stringstream out;
	float fX = 0.0f, fY = 0.0f;
	bool bFirst = true;

	if(HasJoystick1())
	{
		fX = GetXAxis1();
		fY = GetYAxis1();

		out << "x1: " << fX << ", y1: " << fY;
		bFirst = false;
	}

	if(HasJoystick2())
	{
		fX = GetXAxis2();
		fY = GetYAxis2();

		if(!bFirst)
		{
			out << ", ";
		}

		out << "x2: " << fX << ", y2: " << fY;
		bFirst = false;
	}
	
	if(HasDPad())
	{
		fX = GetDPadXAxis();
		fY = GetDPadYAxis();

		if(!bFirst)
		{
			out << ", ";
		}

		out << "xd: " << fX << ", yd: " << fY;
		bFirst = false;
	}

	for(int iButtonIndex = 0; iButtonIndex < GetButtonCount(); iButtonIndex++)
	{
		if(ButtonIsDown(iButtonIndex))
		{
			out << ", " << iButtonIndex;
		}
	}

	return out.str();
}

bool CInputDevice::ButtonIsJustPressed(int _iButtonIndex) const
{
	assert(_iButtonIndex >= 0);
	assert(_iButtonIndex < g_iMaxButtonCount);

	return m_bButtonJustPressed[_iButtonIndex];
}

bool CInputDevice::DPadIsJustPressed(E_DPADDIRECTION _eDir) const
{
	int iArrayIndex = (int)_eDir;

	assert(iArrayIndex >= 0);
	assert(iArrayIndex < 4);

	return m_bDPadJustPressed[iArrayIndex];
}

void CInputDevice::_UpdateJoystickDir(bool _bDownNow, bool& _bDownPrev, bool& _bJustDown)
{
	_bJustDown = _bDownNow && !_bDownPrev;
	_bDownPrev = _bDownNow;
}

bool CInputDevice::OnStateUpdateRequest()
{
	if(!_OnStateUpdateRequest())
	{
		return false;
	}

	for(int iButtonIndex = 0; iButtonIndex < GetButtonCount(); iButtonIndex++)
	{
		bool bButtonIsDown = _ButtonIsDown(iButtonIndex);

		m_bButtonJustPressed[iButtonIndex] = false;

		if(bButtonIsDown && !m_bButtonIsDown[iButtonIndex])
		{
			m_bButtonJustPressed[iButtonIndex] = true;
		}

		m_bButtonIsDown[iButtonIndex] = bButtonIsDown;
	}

	float fDPadX = GetDPadXAxis();
	float fDPadY = GetDPadYAxis();
	float fJoystickX = GetXAxis1();
	float fJoystickY = GetYAxis1();
	
	// D-pad.
	_UpdateJoystickDir(fDPadX < 0.0f, m_bDPadDown[DIR_LEFT], m_bDPadJustPressed[DIR_LEFT]);
	_UpdateJoystickDir(fDPadX > 0.0f, m_bDPadDown[DIR_RIGHT], m_bDPadJustPressed[DIR_RIGHT]);
	_UpdateJoystickDir(fDPadY < 0.0f, m_bDPadDown[DIR_BOTTOM], m_bDPadJustPressed[DIR_BOTTOM]);
	_UpdateJoystickDir(fDPadY > 0.0f, m_bDPadDown[DIR_TOP], m_bDPadJustPressed[DIR_TOP]);

	// Joystick 1.
	_UpdateJoystickDir(fJoystickX < 0.0f, m_bJoystick1InDir[DIR_LEFT], m_bJoystick1JustInDir[DIR_LEFT]);
	_UpdateJoystickDir(fJoystickX > 0.0f, m_bJoystick1InDir[DIR_RIGHT], m_bJoystick1JustInDir[DIR_RIGHT]);
	_UpdateJoystickDir(fJoystickY < 0.0f, m_bJoystick1InDir[DIR_BOTTOM], m_bJoystick1JustInDir[DIR_BOTTOM]);
	_UpdateJoystickDir(fJoystickY > 0.0f, m_bJoystick1InDir[DIR_TOP], m_bJoystick1JustInDir[DIR_TOP]);

	return true;
}

bool CInputDevice::ButtonIsDown(int _iButtonIndex) const
{
	assert(_iButtonIndex >= 0);
	assert(_iButtonIndex < g_iMaxButtonCount);

	return m_bButtonIsDown[_iButtonIndex];
}

bool CInputDevice::DoForceFeedback(float _fTime, float _fForce) const
{
	return false;
}

bool CInputDevice::JustReceivedAnyInput() const
{
	for(int i = 0; i < g_iMaxButtonCount; i++)
	{
		if(m_bButtonIsDown[i])
		{
			return true;
		}
	}

	if(GetXAxis1() != 0.0f)
	{
		return true;
	}

	if(GetYAxis1() != 0.0f)
	{
		return true;
	}

	if(GetXAxis2() != 0.0f)
	{
		return true;
	}

	if(GetYAxis2() != 0.0f)
	{
		return true;
	}

	if(GetDPadXAxis() != 0.0f)
	{
		return true;
	}

	if(GetDPadYAxis() != 0.0f)
	{
		return true;
	}

	return false;
}
