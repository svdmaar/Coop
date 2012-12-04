#pragma once

#include <Windows.h>

class CGuiElement;

enum E_GUIEVENT
{
	EVENT_NONE,
	EVENT_BUTTONPRESSED,
};

class CGuiEvent
{
	const CGuiElement *m_pElement;
	E_GUIEVENT m_eEvent;

public:
	CGuiEvent(const CGuiElement *_pElement, E_GUIEVENT _eEvent);
	CGuiEvent();

	const CGuiElement *GetElement() const;
	E_GUIEVENT GetEvent() const;
};
