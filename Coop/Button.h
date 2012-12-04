#pragma once

#include "GuiElement.h"
#include "Objects2D.h"

#include <string>
#include <d3d9.h>

class CButton : public CGuiElement
{
	std::string m_sCaption;
	//IDirect3DVertexBuffer9 *m_pUnselectedVB;
	//IDirect3DVertexBuffer9 *m_pSelectedVB;

	CRectObject m_unselectedRect;
	CRectObject m_selectedRect;

public:
	CButton();
	virtual ~CButton();

	virtual bool Init(SFloatRect _rPos, std::string _sCaption);
	virtual bool CleanUp();

	virtual bool CanBeSelected() const;
	virtual bool OnButtonPressed(int _iIndex);

	virtual bool Render() const;
};
