#include "TextureManager.h"

#include "Logger.h"

#include <assert.h>

using namespace std;

CTextureManager::CTextureManager()
{
	assert(m_pInstance == NULL);
}

string CTextureManager::_GetBaseFileName(const string & _sPath)
{
	int iSlashIndex = _sPath.find('/');
	int iBackslashIndex = _sPath.find('\\');
	int iIndex = max(iSlashIndex, iBackslashIndex);

	string sBaseFileName = _sPath;
	if(iIndex != -1)
	{
		sBaseFileName = _sPath.substr(iIndex + 1);
	}

	return sBaseFileName;
}

bool CTextureManager::Init()
{
	return true;
}

bool CTextureManager::CleanUp()
{
	HRESULT hr;

	map<string, IDirect3DTexture9 *>::iterator i;
	for(i = m_mTextures.begin(); i != m_mTextures.end(); i++)
	{
		IDirect3DTexture9 * pTexture = (*i).second;

		hr = pTexture->Release();
		if(FAILED(hr))
		{
			LogErrorHr("Failed to release texture", hr);
			return false;
		}
	}

	return true;
}

IDirect3DTexture9 * CTextureManager::GetTexture(const std::string & _sFileName)
{
	map<string, IDirect3DTexture9 *>::iterator i;
	string sBaseFileName = _GetBaseFileName(_sFileName);

	IDirect3DTexture9 * pTexture = NULL;
	i = m_mTextures.find(sBaseFileName);

	if(i == m_mTextures.end())
	{
		CGraphicsManager * pGraphicsManager = CGraphicsManager::GetInstance();
		CBitmap bmTexture(_sFileName);

		if(!pGraphicsManager->CreateTexture(bmTexture, &pTexture))
		{
			return false;
		}

		m_mTextures[sBaseFileName] = pTexture;
	}
	else
	{
		pTexture = (*i).second;
	}

	return pTexture;
}

CTextureManager * CTextureManager::GetInstance()
{
	assert(m_pInstance != NULL);

	return m_pInstance;
}

CTextureManager * CTextureManager::m_pInstance = NULL;
