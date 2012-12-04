#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>

#include "Joystick.h"
#include "Keyboard.h"

class CInputManager
{
	bool m_bInited;
	LPDIRECTINPUT8 m_pDirectInput;
	InputDeviceVector m_vDevices;
	CKeyboard m_keyboard;

	static CInputManager *m_pInstance;

public:
	CInputManager();

	bool Init(int _iKeyboardSectorCount);
	bool CleanUp();

	const CJoystick *GetJoystick();
	const CKeyboard *GetKeyboard();

	bool OnNewFrame();
	int GetDeviceCount();
	const CInputDevice *GetDevice(int _iIndex);
	const CInputDevice *GetFirstDeviceWithInput();

	static CInputManager *GetInstance();
};
