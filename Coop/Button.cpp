#include "Button.h"

#include "VertexTypes.h"
#include "GraphicsUtils.h"
#include "Logger.h"
#include "GraphicsManager.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

bool CButton::Init(SFloatRect _rPos, std::string _sCaption)
{
	if(!CGuiElement::Init(_rPos))
	{
		return false;
	}

	m_sCaption = _sCaption;

	SObject2DDesc objDesc;
	ZeroMemory(&objDesc, sizeof(objDesc));
	objDesc.m_dwInnerColor = 0xff0000ffL;
	objDesc.m_eType = otInner;

	if(!m_unselectedRect.Init(objDesc, _rPos))
	{
		LogError("Failed to create unselected rect for button");
		return false;
	}

	objDesc.m_dwInnerColor = 0xff008000L;

	if(!m_selectedRect.Init(objDesc, _rPos))
	{
		LogError("Failed to create unselected rect for button");
		return false;
	}

	return true;
}

bool CButton::CleanUp()
{
	if(!m_selectedRect.CleanUp())
	{
		return false;
	}

	if(!m_unselectedRect.CleanUp())
	{
		return false;
	}

	if(!CGuiElement::CleanUp())
	{
		return false;
	}

	return true;
}

bool CButton::CanBeSelected() const
{
	return true;
}

bool CButton::Render() const
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	const CFont *pFont = pGraphicsManager->GetArial();

	if(IsSelected())
	{
		if(!m_selectedRect.Render())
		{
			return false;
		}
	}
	else
	{
		if(!m_unselectedRect.Render())
		{
			return false;
		}
	}

	SFloatRect pos = GetPosition();
	float fX = 0.5f * (pos.m_fRight + pos.m_fLeft);
	float fY = 0.5f * (pos.m_fTop + pos.m_fBottom);
	float fTextheight = 0.5f * (pos.m_fTop - pos.m_fBottom);

	if(!pFont->RenderCentered(m_sCaption, fX, fY, fTextheight))
	{
		return false;
	}

	return true;
}

bool CButton::OnButtonPressed(int _iIndex)
{
	if(_iIndex == 0)
	{
		SendEvent(EVENT_BUTTONPRESSED);
		return true;
	}

	return false;
}
