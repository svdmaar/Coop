#include "KeyboardSector.h"

#include <assert.h>

CKeyboardSector::CKeyboardSector()
{
	m_pKeyboard = NULL;
	
	ZeroMemory(&m_keys, sizeof(m_keys));
}

CKeyboardSector::~CKeyboardSector()
{
}

bool CKeyboardSector::Init(const CKeyboard *_pKeyboard, SKeySetup _keySetup)
{
	if(!CInputDevice::Init())
	{
		return false;
	}

	m_pKeyboard = _pKeyboard;
	m_keys = _keySetup;

	return true;
}

bool CKeyboardSector::CleanUp()
{
	return true;
}

E_INPUTTYPE CKeyboardSector::GetType() const
{
	return INTYPE_KEYBOARD;
}

bool CKeyboardSector::HasJoystick1() const
{
	return false;
}

bool CKeyboardSector::HasJoystick2() const
{
	return false;
}

bool CKeyboardSector::HasDPad() const
{
	return true;
}

bool CKeyboardSector::HasForceFeedback() const
{
	return false;
}

int CKeyboardSector::GetButtonCount() const
{
	return g_iKeyboardClusterKeyCount;
}

bool CKeyboardSector::_GetActualKeyState(int _iActualKeyIndex) const
{
	if(_iActualKeyIndex == -1)
	{
		// no key defined
		return false;
	}

	return m_pKeyboard->ButtonIsDown(_iActualKeyIndex);
}

bool CKeyboardSector::_ButtonIsDown(int _iButtonIndex) const
{
	assert((_iButtonIndex >= 0) && (_iButtonIndex < 4));

	int iActualKeyIndex = m_keys.m_iButtons[_iButtonIndex];

	if(iActualKeyIndex == -1)
	{
		// no key defined
		return false;
	}

	return m_pKeyboard->ButtonIsDown(iActualKeyIndex);
}

float CKeyboardSector::GetXAxis1() const
{
	return 0.0f;
}

float CKeyboardSector::GetYAxis1() const
{
	return 0.0f;
}

float CKeyboardSector::GetXAxis2() const
{
	return 0.0f;
}

float CKeyboardSector::GetYAxis2() const
{
	return 0.0f;
}

float CKeyboardSector::GetDPadXAxis() const
{
	if(_GetActualKeyState(m_keys.m_iRight))
	{
		return 1.0f;
	}

	if(_GetActualKeyState(m_keys.m_iLeft))
	{
		return -1.0f;
	}

	return 0.0f;
}

float CKeyboardSector::GetDPadYAxis() const
{
	if(_GetActualKeyState(m_keys.m_iUp))
	{
		return 1.0f;
	}

	if(_GetActualKeyState(m_keys.m_iDown))
	{
		return -1.0f;
	}

	return 0.0f;
}

bool CKeyboardSector::_OnStateUpdateRequest()
{
	return true;
}

CKeyboardSector::SKeySetup CKeyboardSector::GetDefaultCluster1()
{
	SKeySetup out;
	ZeroMemory(&out, sizeof(out));
	memset(out.m_iButtons, -1, sizeof(out.m_iButtons));

	out.m_iUp = DIK_UP;
	out.m_iDown = DIK_DOWN;
	out.m_iLeft = DIK_LEFT;
	out.m_iRight = DIK_RIGHT;
	out.m_iButtons[0] = DIK_RETURN;
	out.m_iButtons[1] = DIK_BACKSLASH;

	return out;
}

CKeyboardSector::SKeySetup CKeyboardSector::GetDefaultCluster2()
{
	SKeySetup out;
	ZeroMemory(&out, sizeof(out));
	memset(out.m_iButtons, -1, sizeof(out.m_iButtons));

	out.m_iUp = DIK_W;
	out.m_iDown = DIK_S;
	out.m_iLeft = DIK_A;
	out.m_iRight = DIK_D;
	out.m_iButtons[0] = DIK_TAB;
	out.m_iButtons[1] = DIK_GRAVE;

	return out;
}

CKeyboardSector::SKeySetup CKeyboardSector::GetDefaultCluster(int _iIndex)
{
	assert(_iIndex >= 0);
	assert(_iIndex < 2);

	if(_iIndex == 0)
	{
		return GetDefaultCluster1();
	}

	return GetDefaultCluster2();
}