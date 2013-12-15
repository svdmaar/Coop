#pragma once

#include "3dsFile2.h"

#include "GraphicsManager.h"

class C3dsFileRenderer
{
	C3dsFile2 m_file;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	C3dsFileRenderer();

	bool Init();
	bool CreateVertexBuffer();
	bool Render();
	bool CleanUp();
};
