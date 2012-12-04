#include "GuiElement.h"

#include <assert.h>

CGuiElement::CGuiElement()
{
	m_bInited = false;
	m_pSink = NULL;
	m_bIsSelected = false;

	ZeroMemory(&m_rPos, sizeof(m_rPos));
}

CGuiElement::~CGuiElement()
{
	assert(!m_bInited);
}

bool CGuiElement::Init(SFloatRect _rPos)
{
	m_rPos = _rPos;

	m_bInited = true;
	
	return true;
}

bool CGuiElement::CleanUp()
{
	ZeroMemory(&m_rPos, sizeof(m_rPos));

	m_bInited = false;

	return true;
}

SFloatRect CGuiElement::GetPosition() const
{
	return m_rPos;
}

void CGuiElement::SendEvent(E_GUIEVENT _eEvent)
{
	assert(m_pSink != NULL);

	m_pSink->OnEvent(CGuiEvent(this, _eEvent));
}

void CGuiElement::SetMessageSink(IGuiEventListener *_pSink)
{
	assert(m_pSink == NULL);

	m_pSink = _pSink;
}

bool CGuiElement::CanBeSelected() const
{
	return false;
}

bool CGuiElement::OnButtonPressed(int _iIndex)
{
	return false;
}

bool CGuiElement::IsSelected() const
{
	return m_bIsSelected;
}

bool CGuiElement::SetIsSelected(bool _bIsSelected)
{
	assert(CanBeSelected());

	m_bIsSelected = _bIsSelected;

	return true;
}
