#include "Joystick.h"

#include "Logger.h"
#include "GraphicsManager.h"
#include "ErrorParser.h"

#include <assert.h>
#include <vector>
#include <sstream>

using namespace std;

const LONG g_lMaxAxis = 10000;
typedef vector<DIEFFECTINFO> EffectInfoVector;

// TODO: get button layout (make button 1 (A) the most important button).
CJoystick::CJoystick() : CInputDevice()
{
	m_pDevice = NULL;
	ZeroMemory(&m_state, sizeof(m_state));
}

CJoystick::~CJoystick()
{
	assert(m_pDevice == NULL);
}

static BOOL __stdcall DirectInputObjectCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	vector<DIDEVICEOBJECTINSTANCE> *pObjectVector = (vector<DIDEVICEOBJECTINSTANCE> *)pvRef;
	pObjectVector->push_back(*lpddoi);

	return TRUE;
}

static BOOL FAR PASCAL DirectInputEffectCallback(LPCDIEFFECTINFOW pdei, LPVOID pvRef)
{
	EffectInfoVector *pEffectVector = (EffectInfoVector *)pvRef;
	pEffectVector->push_back(*pdei);

	return TRUE;
}

bool CJoystick::_FailedAccess(HRESULT& _hr, bool& _bUseClearState)
{
	_bUseClearState = false;

	if(!FAILED(_hr))
	{
		return false;
	}

	bool bDeviceNotAcquired = _hr == DIERR_NOTACQUIRED;
	bool bInputLost = _hr == DIERR_INPUTLOST;
	bool bOtherAppHasPrio = _hr == DIERR_OTHERAPPHASPRIO;

	if(!bDeviceNotAcquired && !bInputLost && !bOtherAppHasPrio)
	{
		LogErrorHr("Failed to get access while not input lost", _hr);
		return true;
	}

	_hr = m_pDevice->Acquire();
	bool bAcquireFailed = FAILED(_hr);
	bInputLost = _hr == DIERR_INPUTLOST;
	bOtherAppHasPrio = _hr == DIERR_OTHERAPPHASPRIO;

	if(!bAcquireFailed)
	{
		CLogger::WriteMessage("Had to reacquire joystick");
		return false;
	}

	if(bInputLost || bOtherAppHasPrio)
	{
		_bUseClearState = true;
		return false;
	}

	LogErrorHr("Made it to the end", _hr);
	return true;
}

static string GetEffectParams(DIEFFECTINFO info)
{
	stringstream out;

	if(info.dwStaticParams & DIEP_AXES) out << "DIEP_AXES ";
	if(info.dwStaticParams & DIEP_DIRECTION) out << "DIEP_DIRECTION ";
	if(info.dwStaticParams & DIEP_DURATION) out << "DIEP_DURATION ";
	if(info.dwStaticParams & DIEP_ENVELOPE) out << "DIEP_ENVELOPE ";
	if(info.dwStaticParams & DIEP_GAIN) out << "DIEP_GAIN ";
	if(info.dwStaticParams & DIEP_SAMPLEPERIOD) out << "DIEP_SAMPLEPERIOD ";
	if(info.dwStaticParams & DIEP_STARTDELAY) out << "DIEP_STARTDELAY ";
	if(info.dwStaticParams & DIEP_TRIGGERBUTTON) out << "DIEP_TRIGGERBUTTON ";
	if(info.dwStaticParams & DIEP_TRIGGERREPEATINTERVAL) out << "DIEP_TRIGGERREPEATINTERVAL ";
	if(info.dwStaticParams & DIEP_TYPESPECIFICPARAMS) out << "DIEP_TYPESPECIFICPARAMS ";

	return out.str();
}

bool CJoystick::Init(LPDIRECTINPUTDEVICE8 _pDevice)
{
	HRESULT hr;

	if(!CInputDevice::Init())
	{
		return false;
	}

	m_pDevice = _pDevice;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	hr = m_pDevice->SetCooperativeLevel(pGraphicsManager->GetWindowHandle(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set cooperative level", hr);
		return false;
	}

	hr = m_pDevice->SetDataFormat(&c_dfDIJoystick);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set data format", hr);
		return false;
	}

	vector<DIDEVICEOBJECTINSTANCE> vObjects;
	hr = m_pDevice->EnumObjects(DirectInputObjectCallback, (void *)&vObjects, DIDFT_ALL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enum objects", hr);
		return false;
	}

	for(vector<DIDEVICEOBJECTINSTANCE>::iterator i = vObjects.begin(); i != vObjects.end(); i++)
	{
		DIDEVICEOBJECTINSTANCE& object = *i;

		if((object.dwType & DIDFT_AXIS) == 0)
			continue;

		DIPROPRANGE range;
		ZeroMemory(&range, sizeof(range));

		range.diph.dwSize = sizeof(range);
		range.diph.dwHeaderSize = sizeof(range.diph);
		range.diph.dwObj = object.dwType;
		range.diph.dwHow = DIPH_BYID;
	    range.lMin = 0;
		range.lMax = g_lMaxAxis;

		hr = m_pDevice->SetProperty(DIPROP_RANGE, &range.diph);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to set range property", hr);
			return false;
		}
	}

	hr = m_pDevice->Acquire();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to acquire device", hr);
		return false;
	}


	return true;
}

bool CJoystick::CleanUp()
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

E_INPUTTYPE CJoystick::GetType() const
{
	return INTYPE_JOYSTICK;
}

bool CJoystick::HasJoystick1() const
{
	return true;
}

bool CJoystick::HasJoystick2() const
{
	return true;
}

bool CJoystick::HasDPad() const
{
	return true;
}

int CJoystick::GetButtonCount() const
{
	return 12;
}

bool CJoystick::_ButtonIsDown(int _iButtonIndex) const
{
	return m_state.rgbButtons[_iButtonIndex] != 0;
}

static inline float CalcFloatPos(LONG _lPos)
{
	const float fMaxAxis = (float)g_lMaxAxis;
	float fDoublePos = (float)(2 * _lPos);
	float fOutput = (fDoublePos - fMaxAxis) / fMaxAxis;
	if(fabs(fOutput) < 0.05) fOutput = 0.0f;
	return fOutput;
}

float CJoystick::GetXAxis1() const
{
	return CalcFloatPos(m_state.lX);
}

float CJoystick::GetYAxis1() const
{
	return CalcFloatPos(m_state.lY);
}

float CJoystick::GetXAxis2() const
{
	LONG lZ = m_state.lZ;
	float fPos = CalcFloatPos(lZ);
	return fPos;
}

float CJoystick::GetYAxis2() const
{
	return CalcFloatPos(m_state.lRz);
}

float CJoystick::GetDPadXAxis() const
{
	if(m_state.rgdwPOV[0] == -1)
	{
		return 0.0f;
	}

	float fDegrees = ((float)m_state.rgdwPOV[0]) / 100.0f;
	float fRad = fDegrees * D3DX_PI / 180.0f;
	return sinf(fRad);
}

float CJoystick::GetDPadYAxis() const
{
	if(m_state.rgdwPOV[0] == -1)
	{
		return 0.0f;
	}

	float fDegrees = ((float)m_state.rgdwPOV[0]) / 100.0f;
	float fRad = fDegrees * D3DX_PI / 180.0f;
	return cosf(fRad);
}

bool CJoystick::_OnStateUpdateRequest()
{
	HRESULT hr;
	bool bClearStateRequested = false;

	hr = m_pDevice->Poll();
	if(_FailedAccess(hr, bClearStateRequested))
	{
		LogErrorHr("Failed to poll device", hr);
		return false;
	}

	if(bClearStateRequested)
	{
		_ClearState();
		return true;
	}

	hr = m_pDevice->GetDeviceState(sizeof(m_state), &m_state);
	if(_FailedAccess(hr, bClearStateRequested))
	{
		LogErrorHr("Failed to get device state", hr);
		return false;
	}

	if(bClearStateRequested)
	{
		_ClearState();
		return true;
	}

	return true;
}

const DIJOYSTATE *CJoystick::GetJoyState() const
{
	return &m_state;
}

void CJoystick::_ClearState()
{
	ZeroMemory(&m_state, sizeof(m_state));

	long lCenter = g_lMaxAxis / 2;

	m_state.lX = lCenter;
    m_state.lY = lCenter;
    m_state.lZ = lCenter;
    m_state.lRx = lCenter;
    m_state.lRy = lCenter;
    m_state.lRz = lCenter;

	m_state.rgdwPOV[0] = -1;
}

bool CJoystick::HasForceFeedback() const
{
	return true;
}

bool CJoystick::DoForceFeedback(float _fTime, float _fForce) const
{
	HRESULT hr;

	DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof( DIPROPDWORD );
    dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = FALSE;

	hr = m_pDevice->Unacquire();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unacquire device", hr);
		return false;
	}

    hr = m_pDevice->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set autocenter property", hr);
		return false;
	}

	EffectInfoVector vEffects;
	hr = m_pDevice->EnumEffects(DirectInputEffectCallback, &vEffects, DIEFT_ALL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enum effects", hr);
		return false;
	}

	DIEFFECTINFO effectInfo = vEffects[0];
	LPDIRECTINPUTEFFECT pEffect = NULL;
	
	// DIEP_AXES DIEP_DIRECTION DIEP_DURATION DIEP_ENVELOPE DIEP_GAIN DIEP_STARTDELAY DIEP_TRIGGERBUTTON DIEP_TYPESPECIFICPARAMS

	DWORD rgdwAxes[2] = { DIJOFS_X, DIJOFS_Y };
    LONG rglDirection[2] = { 0, 1 };
    DICONSTANTFORCE cf = { 0 };

    DIEFFECT eff;
    ZeroMemory( &eff, sizeof( eff ) );
    eff.dwSize = sizeof( DIEFFECT );
    eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.dwDuration = INFINITE;
    eff.dwSamplePeriod = 0;
    eff.dwGain = DI_FFNOMINALMAX;
    eff.dwTriggerButton = DIEB_NOTRIGGER;
    eff.dwTriggerRepeatInterval = 0;
    eff.cAxes = 1;
    eff.rgdwAxes = rgdwAxes;
    eff.rglDirection = rglDirection;
    eff.lpEnvelope = 0;
    eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
    eff.lpvTypeSpecificParams = &cf;
    eff.dwStartDelay = 0;

	hr = m_pDevice->CreateEffect(GUID_ConstantForce, &eff, &pEffect, NULL);
	if(FAILED(hr))
	{
		string s = CErrorParser::ParseError(hr);
		LogErrorHr("Failed to create effect", hr);
		return false;
	}

    // If two force feedback axis, then apply magnitude from both directions 
    rglDirection[0] = 0;
    rglDirection[1] = 0;
    cf.lMagnitude = 0;

    ZeroMemory( &eff, sizeof( eff ) );
    eff.dwSize = sizeof( DIEFFECT );
    eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.cAxes = 2;
    eff.rglDirection = rglDirection;
    eff.lpEnvelope = 0;
    eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
    eff.lpvTypeSpecificParams = &cf;
    eff.dwStartDelay = 0;

    // Now set the new parameters and start the effect immediately.
    hr = pEffect->SetParameters( &eff, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START );
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set direction and type specific params for effect", hr);
		return false;
	}

	hr = m_pDevice->Acquire();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to acquire device", hr);
		return false;
	}

	hr = pEffect->Start(1, 0);
	if(FAILED(hr))
	{
		string s = CErrorParser::ParseError(hr);
		LogErrorHr("Failed to start effect", hr);
		return false;
	}

	return true;
}

/*
Logitech F510:

X-axis 1: lX
Y-axis 1: lY
X-axis 2: lZ
Y-axis 2: lRz

D-pad: (int)joyState.rgdwPOV[0]
center: -1
up: 0
right: 9000
down: 18000
left: 27000

rgbButtons:
[0] : X
[1] : A
[2] : B
[3] : Y
[4] : LB
[5] : RB
[6] : LT
[7] : RT
[8] : back
[9] : start
[10] : button under left joystick
[11] : button under right joystick
*/
