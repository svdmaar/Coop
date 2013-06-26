#pragma once

#include "GraphicsManager.h"

#include <map>
#include <string>

class CTextureManager
{
	std::map<std::string, IDirect3DTexture9 *> m_mTextures;

	static std::string _GetBaseFileName(const std::string & _sPath);
	static CTextureManager * m_pInstance;
	
public:
	CTextureManager();

	bool Init();
	bool CleanUp();

	IDirect3DTexture9 * GetTexture(const std::string & _sFileName);

	static CTextureManager * GetInstance();
};
