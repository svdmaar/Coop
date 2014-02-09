#pragma once

struct CTexture::STextureImpl
{
	CBitmap m_bmColor;
	CDataMap<float> m_mAlpha;
};
