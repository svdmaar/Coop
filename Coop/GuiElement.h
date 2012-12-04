#pragma once

#include "GuiEventListener.h"
#include "FloatRect.h"

#include <Windows.h>
#include <vector>

class CGuiElement
{
	IGuiEventListener * m_pSink;
	bool m_bInited;
	SFloatRect m_rPos;
	bool m_bIsSelected;

protected:
	void SendEvent(E_GUIEVENT _eEvent);

public:
	CGuiElement();
	virtual ~CGuiElement();

	virtual bool Init(SFloatRect _rPos);
	virtual bool CleanUp();

	SFloatRect GetPosition() const;
	void SetMessageSink(IGuiEventListener *_pSink);
	virtual bool CanBeSelected() const;
	virtual bool OnButtonPressed(int _iIndex);

	bool IsSelected() const;
	bool SetIsSelected(bool _bIsSelected);

	virtual bool Render() const = 0;
};

typedef std::vector<CGuiElement *> GuiElementVector;
