#include "TestScreen.h"

#include "InputManager.h"
#include "BasicObjects.h"
#include "GraphicsManager.h"
#include "Logger.h"
#include "Camera.h"
#include "FrameTimer.h"

#include <d3d9.h>
#include <sstream>
#include <assert.h>

using namespace std;

CTestScreen::CTestScreen()
{
	assert(m_pInstance == NULL);

	m_pInstance = this;
}

CTestScreen::~CTestScreen()
{
}

bool CTestScreen::Init()
{
	if(!m_box.Load("mesh/sphere2_bin.esh"))
	{
		return false;
	}

	if(!m_box.CreateVertexBuffer())
	{
		return false;
	}

	/*
	if(!m_sounds[0].Load("audio/chimes.wav")) return false;
	if(!m_sounds[1].Load("audio/notify.wav")) return false;
	if(!m_sounds[2].Load("audio/Speech Off.wav")) return false;
	if(!m_sounds[3].Load("audio/tada.wav")) return false;
	if(!m_sounds[4].Load("audio/Windows Battery Critical.wav")) return false;
	if(!m_sounds[5].Load("audio/Windows Battery Low.wav")) return false;
	if(!m_sounds[6].Load("audio/Windows Ding.wav")) return false;
	if(!m_sounds[7].Load("audio/Windows Exclamation.wav")) return false;
	if(!m_sounds[8].Load("audio/Windows Logoff Sound.wav")) return false;
	if(!m_sounds[9].Load("audio/Windows Notify.wav")) return false;
	if(!m_sounds[10].Load("audio/Windows Print complete.wav")) return false;
	if(!m_sounds[11].Load("audio/Windows Ringin.wav")) return false;
	*/

	return true;
}

bool CTestScreen::InitAfterSwap()
{
	return SetDefaultGraphicsSettings3D();
}

bool CTestScreen::CleanUp()
{
	if(!m_box.CleanUp())
	{
		return false;
	}

	return true;
}

static string BuildButtonString(DIJOYSTATE _joyState)
{
	bool bFirst = true;
	stringstream out;

	for(int i = 0; i < 32; i++)
	{
		if(_joyState.rgbButtons[i] != 0)
		{
			if(!bFirst)
			{
				out << " ";
			}

			out << i << ": " << (int)_joyState.rgbButtons[i];
			bFirst = false;
		}
	}

	return out.str();
}

bool CTestScreen::ProcessInput()
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CCamera * pCamera = CCamera::GetInstance();

	if(!pInputManager->OnNewFrame())
	{
		return false;
	}

	const CJoystick *pJoyStick = pInputManager->GetJoystick();

	//char buffer[256];
	//sprintf_s(buffer, "%u %u %u %u %u %u", joyState.lX, joyState.lY, joyState.lZ, joyState.lRx, joyState.lRy, joyState.lRz);
	//sprintf_s(buffer, "%u %u %u %u %u %u", joyState.rglSlider[0], joyState.rglSlider[1], joyState.rgdwPOV[0], joyState.rgdwPOV[1], joyState.rgdwPOV[2], joyState.rgdwPOV[3]);
	//sprintf_s(buffer, "%u = %i", joyState.rgdwPOV[0], (int)joyState.rgdwPOV[0]);
	//CLogger::WriteMessage(buffer);
	//CLogger::WriteMessage(BuildButtonString(joyState));

	float fRotationAngle = pCamera->GetRotationAngle();
	float fUpAngle = pCamera->GetUpAngle();
	
	const float fStepSize = 3.14f / 30.0f;
	const float fDistanceStepSize = 0.5f;

	fRotationAngle += pJoyStick->GetXAxis1() * fStepSize;

	fUpAngle -= fStepSize * pJoyStick->GetYAxis1();
	fUpAngle = min(0.49f * D3DX_PI, fUpAngle);
	fUpAngle = max(0.0f, fUpAngle);

	pCamera->SetRotationAngle(fRotationAngle);
	pCamera->SetUpAngle(fUpAngle);
	
	if(pJoyStick->GetDPadYAxis() > 0.2f)
	{
		float fDistance = pCamera->GetDistance();
		fDistance -= fDistanceStepSize;
		pCamera->SetDistance(fDistance);
	}
	else if(pJoyStick->GetDPadYAxis() < -0.2f)
	{
		float fDistance = pCamera->GetDistance();
		fDistance += fDistanceStepSize;
		pCamera->SetDistance(fDistance);
	}

	D3DXVECTOR3 vLookAt = pCamera->GetLookAt();
	const float fLookAtStepSize = 0.1f;
	
	if(pJoyStick->GetXAxis2() < -0.2f)
	{
		vLookAt.x -= fLookAtStepSize;
	}
	else if(pJoyStick->GetXAxis2() > 0.2f)
	{
		vLookAt.x += fLookAtStepSize;
	}

	if(pJoyStick->GetYAxis2() < -0.2f)
	{
		vLookAt.y -= fLookAtStepSize;
	}
	else if(pJoyStick->GetYAxis2() > 0.2f)
	{
		vLookAt.y += fLookAtStepSize;
	}

	pCamera->SetLookAt(vLookAt);

	CAudioManager *pAudioManager = CAudioManager::GetInstance();
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	for(int iButtonIndex = 0; iButtonIndex < 12; iButtonIndex++)
	{
		if(pJoyStick->ButtonIsJustPressed(iButtonIndex))
		{
			if(!pAudioManager->PlayAudioFile(m_sounds[iButtonIndex]))
			{
				return false;
			}

			pJoyStick->DoForceFeedback(1.0f, 1.0f);
		}
	}

	return true;
}

bool CTestScreen::_RenderGUI()
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	CCamera * pCamera = CCamera::GetInstance();

	D3DXMATRIX worldTransform, viewTransform, projTransform;

	const float fScreenSize = 50.0f;
	
	D3DXMatrixIdentity(&viewTransform);
	D3DXMatrixOrthoLH(&projTransform, fScreenSize, fScreenSize, -100.0f, 100.0f);

	hr = pDevice->SetTransform(D3DTS_VIEW, &viewTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set view transform", hr);
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_PROJECTION, &projTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set projection transform", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lighting to false", hr);
		return false;
	}

	const CFont *pFont = pGraphicsManager->GetArial();
	CInputManager *pInputManager = CInputManager::GetInstance();
	const CKeyboard *pKeyboard = pInputManager->GetKeyboard();

	D3DXMatrixTranslation(&worldTransform, -fScreenSize / 2.0f, fScreenSize / 2.0f - 1.0f, 0.0f);
	hr = pDevice->SetTransform(D3DTS_WORLD, &worldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set world transform", hr);
		return false;
	}

	stringstream frameRateString;
	frameRateString << CFrameTimer::GetInstance()->GetFrameRate();

	if(!pFont->RenderString(frameRateString.str(), 1.0f))
	{
		return false;
	}

	float fDeviceCount = (float)pInputManager->GetDeviceCount();
	for(int iDeviceIndex = 0; iDeviceIndex < pInputManager->GetDeviceCount(); iDeviceIndex++)
	{
		float fDeviceIndex = (float)iDeviceIndex;
		D3DXMatrixTranslation(&worldTransform, -fScreenSize / 2.0f, -fScreenSize / 2.0f + fDeviceCount - fDeviceIndex - 1.0f, 0.0f);
		hr = pDevice->SetTransform(D3DTS_WORLD, &worldTransform);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to set world transform", hr);
			return false;
		}

		if(!pFont->RenderString(pInputManager->GetDevice(iDeviceIndex)->GetStateString(), 1.0f))
		{
			return false;
		}
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lightning enabled", hr);
		return false;
	}

	D3DXMATRIX camProjMat = pCamera->GetProjectionMatrix();
	hr = pDevice->SetTransform(D3DTS_PROJECTION, &camProjMat);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set projection transform", hr);
		return false;
	}

	D3DXMatrixTranslation(&worldTransform, 0.0f, 0.0f, 0.0f);
	hr = pDevice->SetTransform(D3DTS_WORLD, &worldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set world transform", hr);
		return false;
	}

	return true;
}

bool CTestScreen::Render()
{
	HRESULT hr;

	CBasicObjects *pBasicObjects = CBasicObjects::GetInstance();
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	CCamera * pCamera = CCamera::GetInstance();

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	D3DXMATRIX camViewMat = pCamera->GetViewMatrix();
	hr = pDevice->SetTransform(D3DTS_VIEW, &camViewMat);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set view transform", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lightning render state", hr);
		return false;
	}
	
	if(!pBasicObjects->RenderGrid())
	{
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lightning render state", hr);
		return false;
	}

	if(!m_box.Render())
	{
		return false;
	}

	if(!_RenderGUI())
	{
		return false;
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}

CTestScreen *CTestScreen::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

CTestScreen *CTestScreen::m_pInstance = NULL;
