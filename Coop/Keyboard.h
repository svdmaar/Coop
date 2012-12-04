#pragma once

#include "InputDevice.h"

const int g_iDIKeyboardKeyCount = 256;

class CKeyboard : public CInputDevice
{
	LPDIRECTINPUTDEVICE8 m_pDevice;
	BYTE m_state[g_iDIKeyboardKeyCount];

	virtual bool _ButtonIsDown(int _iButtonIndex) const;
	virtual bool _OnStateUpdateRequest();

public:
	CKeyboard();
	virtual ~CKeyboard();

	virtual bool Init(LPDIRECTINPUTDEVICE8 _pDevice);
	virtual bool CleanUp();

	virtual E_INPUTTYPE GetType() const;
	virtual bool HasJoystick1() const;
	virtual bool HasJoystick2() const;
	virtual bool HasDPad() const;
	virtual int GetButtonCount() const;
	virtual bool HasForceFeedback() const;

	virtual float GetXAxis1() const;
	virtual float GetYAxis1() const;
	virtual float GetXAxis2() const;
	virtual float GetYAxis2() const;
	virtual float GetDPadXAxis() const;
	virtual float GetDPadYAxis() const;
	
	std::string GetDownKeyList() const;
};
