#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <string>

const int g_iMaxButtonCount = 256;

enum E_INPUTTYPE
{
	INTYPE_KEYBOARD,
	INTYPE_JOYSTICK
};

enum E_DPADDIRECTION
{
	DIR_LEFT = 0,
	DIR_RIGHT = 1,
	DIR_TOP = 2,
	DIR_BOTTOM = 3,
};

class CInputDevice
{
	bool m_bInited;
	float m_fXAxis1;
	float m_fYAxis1;
	float m_fXAxis2;
	float m_fYAxis2;
	float m_fDPadX;
	float m_fDPadY;
	DWORD m_dwButtonDownTime[g_iMaxButtonCount];
	bool m_bButtonJustPressed[g_iMaxButtonCount];
	bool m_bButtonIsDown[g_iMaxButtonCount];
	bool m_bDPadDown[4];
	bool m_bDPadJustPressed[4];
	bool m_bJoystick1InDir[4];
	bool m_bJoystick1JustInDir[4];

	void _UpdateJoystickDir(bool _bDownNow, bool & _bDownPrev, bool & _bJustDown);

	virtual bool _OnStateUpdateRequest() = 0;
	virtual bool _ButtonIsDown(int _iButtonIndex) const = 0;

public:
	CInputDevice();
	virtual ~CInputDevice();

	bool Init();
	virtual bool CleanUp();

	std::string GetStateString() const;

	virtual E_INPUTTYPE GetType() const = 0;
	virtual bool HasJoystick1() const = 0;
	virtual bool HasJoystick2() const = 0;
	virtual bool HasDPad() const = 0;
	virtual int GetButtonCount() const = 0;
	virtual bool HasForceFeedback() const = 0;

	virtual float GetXAxis1() const = 0;
	virtual float GetYAxis1() const = 0;
	virtual float GetXAxis2() const = 0;
	virtual float GetYAxis2() const = 0;
	virtual float GetDPadXAxis() const = 0;
	virtual float GetDPadYAxis() const = 0;
	virtual bool DoForceFeedback(float _fTime, float _fForce) const;
	
	bool OnStateUpdateRequest();
	bool ButtonIsJustPressed(int _iButtonIndex) const;
	bool DPadIsJustPressed(E_DPADDIRECTION _eDir) const;
	bool ButtonIsDown(int _iButtonIndex) const;
	bool JustReceivedAnyInput() const;
	E_DPADDIRECTION GetDPadDirection(bool & _bInput) const;
};

typedef std::vector<CInputDevice *> InputDeviceVector;
