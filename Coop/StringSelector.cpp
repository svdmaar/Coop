#include "StringSelector.h"

#include "GraphicsUtils.h"
#include "Logger.h"
#include "VertexTypes.h"

#include <assert.h>

using namespace std;

const float g_fOuterFrameSize = 0.5f;

bool CStringSelector::Init(SFloatRect _rPos, const StringVector *_pStrings)
{
	if(!CGuiElement::Init(_rPos))
	{
		return false;
	}

	m_pStrings = _pStrings;
	m_iStringIndex = -1;
	m_eState = SELECTOR_INACTIVE;
	
	if(!m_activeVertexBuffer.Init(_rPos, 0xff008000L))
	{
		return false;
	}

	if(!m_inactiveVertexBuffer.Init(_rPos, 0xff0000ffL))
	{
		return false;
	}

	if(!m_lockedVertexBuffer.Init(_rPos, 0xff800000L))
	{
		return false;
	}
	

	return true;
}

bool CStringSelector::Render() const
{
	if((m_eState == SELECTOR_ACTIVE) || (m_eState == SELECTOR_LOCKED))
	{
		if(m_eState == SELECTOR_ACTIVE)
		{
			if(!m_activeVertexBuffer.Render())
			{
				return false;
			}
		}
		else
		{
			if(!m_lockedVertexBuffer.Render())
			{
				return false;
			}
		}

		SFloatRect pos = GetPosition();
		float fX = 0.5f * (pos.m_fRight + pos.m_fLeft);
		float fY = 0.5f * (pos.m_fTop + pos.m_fBottom);
		float fTextheight = 0.5f * (pos.m_fTop - pos.m_fBottom);

		CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
		const CFont *pFont = pGraphicsManager->GetArial();

		string sDispString = (*m_pStrings)[m_iStringIndex];

		if(!pFont->RenderCentered(sDispString, fX, fY, fTextheight))
		{
			return false;
		}
	}
	else
	{
		if(!m_inactiveVertexBuffer.Render())
		{
			return false;
		}
	}

	return true;
}

bool CStringSelector::CleanUp()
{
	if(!m_activeVertexBuffer.CleanUp())
	{
		return false;
	}

	if(!m_inactiveVertexBuffer.CleanUp())
	{
		return false;
	}

	if(!m_lockedVertexBuffer.CleanUp())
	{
		return false;
	}

	if(!CGuiElement::CleanUp())
	{
		return false;
	}

	return true;
}

std::string CStringSelector::GetSelectedString()
{
	assert(m_iStringIndex >= 0);
	assert(m_iStringIndex < (int)m_pStrings->size());

	return m_pStrings->at(m_iStringIndex);
}

ESSELECTOR_STATE CStringSelector::GetState()
{
	return m_eState;
}

void CStringSelector::OnDPadInput(int _iUpDown)
{
	if(_iUpDown == 0)
	{
		return;
	}

	if(m_eState == SELECTOR_INACTIVE)
	{
		m_eState = SELECTOR_ACTIVE;
		m_iStringIndex = 0;
		return;
	}

	if(m_eState == SELECTOR_ACTIVE)
	{
		m_iStringIndex += _iUpDown;
		m_iStringIndex += (int)m_pStrings->size();
		m_iStringIndex = m_iStringIndex % (int)m_pStrings->size();
	}
}

bool CStringSelector::OnButtonPressed(int _iIndex)
{
	if((_iIndex < 0) || (_iIndex > 1))
	{
		return true;
	}

	if(_iIndex == 0)
	{
		// OK.
		if(m_eState == SELECTOR_INACTIVE)
		{
			m_eState = SELECTOR_ACTIVE;
			m_iStringIndex = 0;
			return true;
		}

		if(m_eState == SELECTOR_ACTIVE)
		{
			m_eState = SELECTOR_LOCKED;
			return true;
		}
	}

	if(_iIndex == 1)
	{
		// Cancel.
		if(m_eState == SELECTOR_ACTIVE)
		{
			m_eState = SELECTOR_INACTIVE;
			return true;
		}

		if(m_eState == SELECTOR_LOCKED)
		{
			m_eState = SELECTOR_ACTIVE;
			return true;
		}
	}

	return true;
}
