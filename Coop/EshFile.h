#pragma once

#include "Bitmap.h"

#include <string>
#include <vector>
#include <d3dx9.h>

class CEshFile
{
	struct STriangle;
	struct SObject;

	std::vector<SObject> m_vObjects;
	STriangle *m_pTriangles;
	std::vector<CBitmap> m_vBitmaps;

	bool _LoadTextFile(std::string _fileName);
	bool _LoadBinFile(std::string _fileName);

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

public:
	CEshFile();
	~CEshFile();

	bool Load(std::string _fileName);
	bool Save(std::string _fileName) const;

	int GetBitmapCount() const;

	bool CreateVertexBuffer();
	bool Render();
	bool CleanUp();
};
