#include "InputManager.h"

#include "Application.h"
#include "Logger.h"
#include "GraphicsManager.h"
#include "InputDeviceFactory.h"
#include "KeyboardSector.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <assert.h>

using namespace std;

typedef vector<DIDEVICEINSTANCE> DIDeviceInstanceVector;

static BOOL __stdcall DirectInputDeviceCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DIDeviceInstanceVector *pDeviceVector = (vector<DIDEVICEINSTANCE> *)pvRef;
	pDeviceVector->push_back(*lpddi);

	return TRUE;
}

CInputManager::CInputManager()
{
	m_bInited = false;
	m_pDirectInput = NULL;

	m_pInstance = this;
}

bool CInputManager::OnNewFrame()
{
	if(!m_keyboard.OnStateUpdateRequest())
	{
		return false;
	}

	for(InputDeviceVector::iterator i = m_vDevices.begin(); i != m_vDevices.end(); i++)
	{
		CInputDevice *pDevice = *i;

		if(!pDevice->OnStateUpdateRequest())
		{
			LogError("Failed to update device");
			return false;
		}
	}

	return true;
}

bool CInputManager::Init(int _iKeyboardSectorCount)
{
	HINSTANCE hInstance = CApplication::GetInstance()->GetHInstance();
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	hr = DirectInput8Create(hInstance,  DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create DI8", hr);
		return false;
	}

	DIDeviceInstanceVector vDevices;

	hr = m_pDirectInput->EnumDevices(DI8DEVCLASS_ALL, DirectInputDeviceCallback, (void *)&vDevices, DIEDFL_ALLDEVICES);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enum DI devices", hr);
		return false;
	}

	CInputDeviceFactory deviceFactory;
	deviceFactory.AlsoCreateType(INTYPE_JOYSTICK);

	for(DIDeviceInstanceVector::iterator i = vDevices.begin(); i != vDevices.end(); i++)
	{
		DIDEVICEINSTANCE& deviceInstance = *i;
		CInputDevice *pDevice = NULL;

		if(!deviceFactory.CreateDevice(m_pDirectInput, deviceInstance, &pDevice))
		{
			return false;
		}

		if(pDevice == NULL)
			continue;

		m_vDevices.push_back(pDevice);
	}

	LPDIRECTINPUTDEVICE8 pkeyboardDevice = NULL;
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &pkeyboardDevice, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create keyboard", hr);
		return false;
	}
	
	if(!m_keyboard.Init(pkeyboardDevice))
	{
		return false;
	}

	for(int iSectorIndex = 0; iSectorIndex < _iKeyboardSectorCount; iSectorIndex++)
	{
		CKeyboardSector *pSector = new CKeyboardSector();
		
		if(!pSector->Init(&m_keyboard, CKeyboardSector::GetDefaultCluster(iSectorIndex)))
		{
			return false;
		}
		
		m_vDevices.push_back(pSector);
	}

	m_bInited = true;

	return true;
}

bool CInputManager::CleanUp()
{
	for(InputDeviceVector::iterator i = m_vDevices.begin(); i != m_vDevices.end(); i++)
	{
		if(!(*i)->CleanUp())
		{
			return false;
		}
	}

	if(!m_keyboard.CleanUp())
	{
		return false;
	}

	m_bInited = false;

	return true;
}

CInputManager *CInputManager::GetInstance()
{
	assert(m_pInstance->m_bInited);

	return m_pInstance;
}

const CJoystick *CInputManager::GetJoystick()
{
	return dynamic_cast<CJoystick *>(m_vDevices[0]);
}

const CKeyboard *CInputManager::GetKeyboard()
{
	return &m_keyboard;
}

int CInputManager::GetDeviceCount()
{
	return (int)m_vDevices.size();
}

const CInputDevice *CInputManager::GetDevice(int _iIndex)
{
	return m_vDevices[_iIndex];
}

const CInputDevice *CInputManager::GetFirstDeviceWithInput()
{
	InputDeviceVector::iterator i;

	for(i = m_vDevices.begin(); i != m_vDevices.end(); i++)
	{
		CInputDevice *pDevice = *i;
		if(pDevice->JustReceivedAnyInput())
		{
			return pDevice;
		}
	}

	return NULL;
}

CInputManager *CInputManager::m_pInstance = NULL;
