#pragma once

#include "Screen.h"
#include "Gui.h"
#include "PictureBackground.h"
#include "Button.h"
#include "VertexBuffer.h"

class CMainScreen : public CScreen
{
	CGui m_gui;
	CPictureBackground m_background;
	CButton m_btnOne;
	CButton m_btnTwo;
	CButton m_btnThree;
	CButton m_btnFour;
	CButton m_btnFive;
	
	CTexturedVertexBuffer m_texturedVertexBuffer;
	CColoredVertexBuffer m_coloredVertexBuffer;

	static CMainScreen *m_pInstance;

	SFloatRect _CalcButtonPos(int _iButtonIndex);

public:
	CMainScreen();
	virtual ~CMainScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();

	virtual bool ProcessInput();
	virtual bool Render();
	
	static CMainScreen *GetInstance();
};
