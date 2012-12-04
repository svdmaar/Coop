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
	
	m_rInner = _rPos;
	m_rInner.m_fLeft += g_fOuterFrameSize;
	m_rInner.m_fBottom += g_fOuterFrameSize;
	m_rInner.m_fRight -= g_fOuterFrameSize;
	m_rInner.m_fTop -= g_fOuterFrameSize;

	if(!CGraphicsUtils::CreateColoredRectangle(m_rInner, &m_pActiveVB, 0xff008000L))
	{
		LogError("Failed to create active VB for string selector");
		return false;
	}

	if(!CGraphicsUtils::CreateColoredRectangle(m_rInner, &m_pInactiveVB, 0xff0000ffL))
	{
		LogError("Failed to create inactive VB for string selector");
		return false;
	}

	if(!CGraphicsUtils::CreateColoredRectangle(_rPos, &m_pLockedVB, 0xff800000L))
	{
		LogError("Failed to create locked VB for string selector");
		return false;
	}

	m_eState = SELECTOR_INACTIVE;

	return true;
}

bool CStringSelector::Render() const
{
	if((m_eState == SELECTOR_ACTIVE) || (m_eState == SELECTOR_LOCKED))
	{
		IDirect3DVertexBuffer9 *pRenderedBuffer = m_eState == SELECTOR_ACTIVE ? m_pActiveVB : m_pLockedVB;

		if(!CGraphicsUtils::RenderVertexBuffer(pRenderedBuffer, sizeof(SColoredVertex), SColoredVertex::_fvf, D3DPT_TRIANGLELIST, 2))
		{
			return false;
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
		if(!CGraphicsUtils::RenderVertexBuffer(m_pInactiveVB, sizeof(SColoredVertex), SColoredVertex::_fvf, D3DPT_TRIANGLELIST, 2))
		{
			return false;
		}
	}

	return true;
}

bool CStringSelector::CleanUp()
{
	m_pActiveVB->Release();
	m_pInactiveVB->Release();

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
