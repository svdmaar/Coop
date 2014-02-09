#include "Texture.h"

#include "TextureImpl.h"

CTexture::CTexture()
{
	m_pImpl = new STextureImpl;
}

CTexture * CTexture::Create(const CBitmap & _bmColor, const CDataMap<float> & _mAlpha)
{
	CTexture * pOut = new CTexture;

	pOut->m_pImpl->m_bmColor = _bmColor;
	pOut->m_pImpl->m_mAlpha = _mAlpha;

	return pOut;
}
