#include "SpriteLoader.h"
#include "VideoDevice.h"

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

	for(int i = 0; i < 8; i++)
	{
		g_pVideoDevice->StartScene();

		args.m_pPosition.m_fX = 20.0f + (float)i;
		args.m_pPosition.m_fY = 20.0f;

		args.m_iFrameIndex = i;

		pSprite->Render(args);

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
