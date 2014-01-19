#include "D3dDemos.h"

bool CheckMessages(bool & _bQuit)
{
	_bQuit = false;

	MSG message;
	while(PeekMessage(&message, g_hWindow, 0, 0, PM_REMOVE))
	{
		if(message.message == WM_QUIT)
		{
			_bQuit = true;
		}
		else
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return true;
}

bool RunDemo(SD3dDemo _demo)
{
	SetupWindow();
	_demo.m_pInit();

	bool bQuit = false;
	while(!bQuit)
	{
		CheckMessages(bQuit);

		if(!bQuit)
		{
			_demo.m_pRender();
		}
	}

	_demo.m_pCleanUp();
	CleanUpWindow();

	return true;
}
