#include "GuiEvent.h"

CGuiEvent::CGuiEvent(const CGuiElement *_pElement, E_GUIEVENT _eEvent)
{
	m_pElement = _pElement;
	m_eEvent = _eEvent;
}

CGuiEvent::CGuiEvent()
{
	m_pElement = NULL;
	m_eEvent = EVENT_NONE;
}

const CGuiElement *CGuiEvent::GetElement() const
{
	return m_pElement;
}

E_GUIEVENT CGuiEvent::GetEvent() const
{
	return m_eEvent;
}

