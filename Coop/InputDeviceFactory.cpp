#include "InputDeviceFactory.h"

#include <assert.h>
#include <algorithm>

#include "Joystick.h"
#include "Logger.h"

using namespace std;

CInputDeviceFactory::CInputDeviceFactory()
{
}

bool CInputDeviceFactory::AlsoCreateType(E_INPUTTYPE _type)
{
	assert(find(m_vCreatedTypes.begin(), m_vCreatedTypes.end(), _type) == m_vCreatedTypes.end());

	m_vCreatedTypes.push_back(_type);

	return true;
}

bool CInputDeviceFactory::CreateDevice(LPDIRECTINPUT8 _pDirectInput, const DIDEVICEINSTANCE& _deviceInstance, CInputDevice ** _ppInputDevice)
{
	HRESULT hr;
	int iDeviceType = _deviceInstance.dwDevType & 0xff;

	*_ppInputDevice = NULL;

	switch(iDeviceType)
	{
		case DI8DEVTYPE_JOYSTICK:
		case DI8DEVTYPE_GAMEPAD:
			{
				if(find(m_vCreatedTypes.begin(), m_vCreatedTypes.end(), INTYPE_JOYSTICK) != m_vCreatedTypes.end())
				{
					LPDIRECTINPUTDEVICE8 pdiDevice = NULL;
					hr = _pDirectInput->CreateDevice(_deviceInstance.guidInstance, &pdiDevice, NULL);
					if(FAILED(hr))
					{
						LogErrorHr("Failed to create device", hr);
						return false;
					}

					CJoystick *pJoystick = new CJoystick();

					if(!pJoystick->Init(pdiDevice))
					{
						return false;
					}

					*_ppInputDevice = pJoystick;
				}
			}
	}

	return true;
}
