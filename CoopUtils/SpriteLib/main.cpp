#include "SpriteLoader.h"

#include <iostream>

using namespace std;

int main()
{
	CSpriteLoader spriteLoader;
	spriteLoader.Init("sprites/pacman");

	CSprite * pSprite = spriteLoader.LoadSprite("player");

	pSprite->SetDirection(0.0f);
	
	for(int i = 0; i < 16; i++)
	{
		pSprite->SetFrameIndex(i);

		Sleep(100);

		pSprite->Render();
	}

	return 0;
}
