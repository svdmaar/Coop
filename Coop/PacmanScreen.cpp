#include "PacmanScreen.h"

#include "GraphicsManager.h"
#include "Logger.h"
#include "Gui.h"

#include "MatrixInspector.h"
#include "VertexTypes.h"
#include "ErrorParser.h"

#include <string>

using namespace std;

CPacmanScreen::CPacmanScreen()
{
}

CPacmanScreen::~CPacmanScreen()
{
}

bool CPacmanScreen::Init()
{
	if(!CScreen::Init())
	{
		return false;
	}

	if(!m_gameState.Init())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::InitAfterSwap()
{
	if(!CScreen::InitAfterSwap())
	{
		return false;
	}

	if(!SetDefaultGraphicsSettings2D())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::CleanUp()
{
	if(!m_gameState.CleanUp())
	{
		return false;
	}

	if(!CScreen::CleanUp())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::ProcessInput()
{
	return CScreen::ProcessInput();
}

bool CPacmanScreen::Render()
{
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();

	if(!m_gameState.Update())
	{
		return false;
	}

	if(!pGraphicsManager->BeginScene())
	{
		return false;
	}

	if(!m_gameState.Render())
	{
		return false;
	}

	if(!_RenderDebugLine())
	{
		return false;
	}

	if(!pGraphicsManager->EndScene())
	{
		return false;
	}

	return true;
}

bool CPacmanScreen::_RenderDebugLine()
{
	string sDebugLine = CGui::GetDebugLine();

	CGraphicsManager * pGraphicsManager = CGraphicsManager::GetInstance();
	const CFont * pArial = pGraphicsManager->GetArial();

	SFloatRect rScreen = CGui::GetScreenRect();
	pArial->RenderCentered(sDebugLine, 0.0f, rScreen.m_fBottom + 1.0f, 2.0f);

	return true;	
}
