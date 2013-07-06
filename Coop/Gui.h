#pragma once

#include "GuiElement.h"
#include "GuiEventListener.h"
#include "InputDevice.h"

class CGui : IGuiEventListener
{
	bool m_bInited;
	GuiElementVector m_vElements;
	std::vector<CGuiEvent> m_vEvents;
	CGuiElement *m_pSelected;

	static std::string m_sDebugLine;

	bool _GetDeviceInput(const CInputDevice *_pDevice, float& _fX, float& _fY);
	CGuiElement *_FindNextSelectableElement(float _fXDir, float _fYDir);
	bool _FloatPositive(float _fValue);

public:
	CGui();
	~CGui();

	bool Init();
	bool CleanUp();

	bool Render();
	bool AddElement(CGuiElement *_pElement);

	void OnEvent(CGuiEvent _event);
	bool GetEvent(CGuiEvent& _event);
	bool SetSelected(CGuiElement *_pElement);
	bool ProcessInput();

	static SFloatRect GetScreenRect();
	static bool SetDebugLine(const std::string & _sText);
	static std::string GetDebugLine();
};
