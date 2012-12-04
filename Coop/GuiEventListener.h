#pragma once

#include "GuiEvent.h"

#include <Windows.h>

class IGuiEventListener
{

public:
	virtual void OnEvent(CGuiEvent _event) = 0;
};
