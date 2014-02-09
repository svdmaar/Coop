#include "SpriteLoader.h"
#include "VideoDevice.h"
#include "RenderedSprite.h"

#include <iostream>

using namespace std;

int main()
{

	CSpriteLoader spriteLoader;
	spriteLoader.Init("sprites/pacman");

	CSprite * pSprite = spriteLoader.LoadSprite("player");

	SSpriteRenderArgs args;
	ZeroMemory(&args, sizeof(args));

	CVideoDevice::Init();

	CRenderedSprite * pRenderedSprite = new CRenderedSprite(pSprite);

	for(int i = 0; i < 100; i++)
	{
		g_pVideoDevice->StartScene();

		/*
		args.m_pPosition.m_fX = 20.0f + (float)i;
		args.m_pPosition.m_fY = 20.0f;

		args.m_iFrameIndex = i;

		pSprite->Render(args);
		*/

		pRenderedSprite->SetDirection(0.0f);
		pRenderedSprite->SetFrameIndex(i);

		SFloatPoint pPosition;
		pPosition.m_fX = 20.0f + (float)i;
		pPosition.m_fY = 20.0f;
		pRenderedSprite->SetPosition(pPosition);

		pRenderedSprite->Render();

		g_pVideoDevice->EndScene();
	}









/*
	CSpriteLoader spriteLoader;
	spriteLoader.Init("sprites/pacman");

	CSprite * pSprite = spriteLoader.LoadSprite("player");

	pSprite->SetDirection(3.1415f / 2.0f * 3.0f);
	
	for(int i = 0; i < 32; i++)
	{
		pSprite->SetFrameIndex(i);

		Sleep(100);

		//pSprite->Render();
		pSprite->RenderOld();
	}
	*/

	return 0;
}
