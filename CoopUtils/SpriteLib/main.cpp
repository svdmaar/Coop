#include "SpriteLoader.h"

#include <iostream>

using namespace std;

int main()
{
	CSpriteLoader spriteLoader;
	spriteLoader.Init("sprites/pacman");

	CSprite * pSprite = spriteLoader.LoadSprite("player");

	pSprite->SetDirection(3.1415f / 2.0f * 3.0f);
	
	for(int i = 0; i < 32; i++)
	{
		pSprite->SetFrameIndex(i);

		Sleep(100);

		pSprite->Render();
	}

	return 0;
}
