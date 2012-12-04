#pragma once

#include "InputDevice.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class CJoystick : public CInputDevice
{
	LPDIRECTINPUTDEVICE8 m_pDevice;
	DIJOYSTATE m_state;

	bool _FailedAccess(HRESULT& _hr, bool& _bUseClearState);
	void _ClearState();

	virtual bool _OnStateUpdateRequest();
	virtual bool _ButtonIsDown(int _iButtonIndex) const;

public:
	CJoystick();
	virtual ~CJoystick();

	bool Init(LPDIRECTINPUTDEVICE8 _pDevice);
	virtual bool CleanUp();

	virtual E_INPUTTYPE GetType() const;
	virtual bool HasJoystick1() const;
	virtual bool HasJoystick2() const;
	virtual bool HasDPad() const;
	virtual bool HasForceFeedback() const;
	virtual int GetButtonCount() const;

	virtual float GetXAxis1() const;
	virtual float GetYAxis1() const;
	virtual float GetXAxis2() const;
	virtual float GetYAxis2() const;
	virtual float GetDPadXAxis() const;
	virtual float GetDPadYAxis() const;
	virtual bool DoForceFeedback(float _fTime, float _fForce) const;
	
	// debug
	const DIJOYSTATE *GetJoyState() const;
};
