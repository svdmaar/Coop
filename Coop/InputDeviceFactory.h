#pragma once

#include "InputDevice.h"

#include <vector>

class CInputDeviceFactory
{
	std::vector<E_INPUTTYPE> m_vCreatedTypes;

public:
	CInputDeviceFactory();

	bool AlsoCreateType(E_INPUTTYPE _type);

	bool CreateDevice(LPDIRECTINPUT8 _pDirectInput, const DIDEVICEINSTANCE& _deviceInstance, CInputDevice ** _ppInputDevice);
};
