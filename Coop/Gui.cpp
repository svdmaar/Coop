#include "Gui.h"

#include "GraphicsManager.h"
#include "AppState.h"
#include "InputManager.h"
#include "GraphicsUtils.h"
#include "Logger.h"

#include <assert.h>
#include <algorithm>
#include <assert.h>

using namespace std;

CGui::CGui()
{
	m_bInited = false;
	m_pSelected = NULL;
}

CGui::~CGui()
{
	assert(!m_bInited);
}

bool CGui::Init()
{
	m_bInited = true;

	return true;
}

bool CGui::CleanUp()
{
	m_bInited = false;

	return true;
}

bool CGui::Render()
{
	assert(m_bInited);

	for(GuiElementVector::iterator i = m_vElements.begin(); i != m_vElements.end(); i++)
	{
		if(!(*i)->Render())
		{
			return false;
		}
	}

	return true;
}

bool CGui::AddElement(CGuiElement *_pElement)
{
	_pElement->SetMessageSink(this);

	m_vElements.push_back(_pElement);

	return true;
}

void CGui::OnEvent(CGuiEvent _event)
{
	m_vEvents.push_back(_event);
}

bool CGui::GetEvent(CGuiEvent& _event)
{
	if(m_vEvents.empty())
	{
		return false;
	}

	_event = m_vEvents.front();

	m_vEvents.erase(m_vEvents.begin());

	return true;
}

SFloatRect CGui::GetScreenRect()
{
	SFloatRect output;
	ZeroMemory(&output, sizeof(output));

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	float fAspectRatio = pGraphicsManager->GetAspectRatio();

	const float fHalfScreenHeight = 50.0f;

	output.m_fLeft = -fAspectRatio * fHalfScreenHeight;
	output.m_fTop = fHalfScreenHeight;
	output.m_fRight = fAspectRatio * fHalfScreenHeight;
	output.m_fBottom = -fHalfScreenHeight;

	return output;
}

bool CGui::SetSelected(CGuiElement *_pElement)
{
	if(m_pSelected != NULL)
	{
		m_pSelected->SetIsSelected(false);
	}

	assert(find(m_vElements.begin(), m_vElements.end(), _pElement) != m_vElements.end());

	m_pSelected = _pElement;
	m_pSelected->SetIsSelected(true);

	return true;
}

bool CGui::ProcessInput()
{
	CAppState *pAppState = CAppState::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	const CInputDevice *pMainInputDevice = pAppState->GetMainInputDevice();

	if(pMainInputDevice == NULL)
	{
		pMainInputDevice = pInputManager->GetFirstDeviceWithInput();
		
		if(pMainInputDevice != NULL)
		{
			pAppState->SetMainInputDevice(pMainInputDevice);
		}
	}

	if(pMainInputDevice == NULL)
	{
		return true;
	}

	float fX = 0.0f, fY = 0.0f;

	_GetDeviceInput(pMainInputDevice, fX, fY);

	float fMag = fabsf(fX) + fabsf(fY);

	if(fMag > 0.0f)
	{
		_FindNextSelectableElement(fX, fY);
		return true;
	}

	if(pMainInputDevice->ButtonIsJustPressed(0))
	{
		if(m_pSelected != NULL)
		{
			m_pSelected->OnButtonPressed(0);
		}
	}

	return true;
}

bool CGui::_GetDeviceInput(const CInputDevice *_pDevice, float& _fX, float& _fY)
{
	_fX = _fY = 0.0f;

	if(_pDevice->DPadIsJustPressed(DIR_LEFT))
	{
		_fX = -1.0f;
	}
	else if(_pDevice->DPadIsJustPressed(DIR_RIGHT))
	{
		_fX = 1.0f;
	}

	if(_pDevice->DPadIsJustPressed(DIR_TOP))
	{
		_fY = 1.0f;
	}
	else if(_pDevice->DPadIsJustPressed(DIR_BOTTOM))
	{
		_fY = -1.0f;
	}

	return true;
}

CGuiElement *CGui::_FindNextSelectableElement(float _fXDir, float _fYDir)
{
	assert(m_pSelected != NULL);

	GuiElementVector::iterator i;
	SFloatRect currentPos = m_pSelected->GetPosition();
	float fCurrentCenterX = 0.0f, fCurrentCenterY = 0.0f;
	float fBestDistance = 9999.9f;
	CGuiElement *pNextSelected = NULL;

	CGraphicsUtils::CalcRectCenter(currentPos, fCurrentCenterX, fCurrentCenterY);

	for(i = m_vElements.begin(); i != m_vElements.end(); i++)
	{
		CGuiElement *pElement = *i;

		if(pElement == m_pSelected)
		{
			continue;
		}

		if(!pElement->CanBeSelected())
		{
			continue;
		}

		SFloatRect pos = pElement->GetPosition();

		float fCenterX = 0.0f, fCenterY = 0.0f;
		CGraphicsUtils::CalcRectCenter(pos, fCenterX, fCenterY);

		float fDiffX = fCenterX - fCurrentCenterX;
		float fDiffY = fCenterY - fCurrentCenterY;

		if(_FloatPositive(fDiffX) != _FloatPositive(_fXDir))
		{
			continue;
		}

		if(_FloatPositive(fDiffY) != _FloatPositive(_fYDir))
		{
			continue;
		}

		if((_fXDir > _fYDir) && (fDiffX < fDiffY))
		{
			continue;
		}

		float fDistance = fabsf(fDiffX) + fabsf(fDiffY);

		if(fDistance < fBestDistance)
		{
			fBestDistance = fDistance;
			pNextSelected = pElement;
		}
	}

	if(pNextSelected != NULL)
	{
		SetSelected(pNextSelected);
	}

	return NULL;
}

bool CGui::_FloatPositive(float _fValue)
{
	return _fValue > 0.0f;
}
