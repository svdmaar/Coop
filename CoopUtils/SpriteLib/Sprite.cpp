#include "Sprite.h"

#include "VideoDevice.h"
#include "../../Coop/DataMap.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <sstream>
#include <assert.h>

using namespace std;

extern CBitmap * g_pbmScreenBitmap;

// Just for rendering to bitmap:
extern CBitmap * g_pbmSelectedTexture;
extern CDataMap<float> * g_pmSelectedAlpha;

struct SSpriteImpl
{
	IDirect3DTexture9 * m_pTexture;
	IDirect3DVertexBuffer9 * m_pVertexBuffer;

	// Debug
	SSpriteDesc m_spriteDesc;

	// For bitmap.
	RECT m_rPixels;

	SSpriteImpl()
	{
		m_pTexture = NULL;
		m_pVertexBuffer = NULL;
	}
};

CSprite::CSprite()
{
	m_pImpl = new SSpriteImpl;

	m_fDir = 0.0f;
	m_iIndex = 0;
}

bool CSprite::SetDirection(float _fDir)
{
	m_fDir = _fDir;

	_CalculatePreRender();

	return true;
}

bool CSprite::SetFrameIndex(int _iIndex)
{
	m_iIndex = _iIndex;

	_CalculatePreRender();

	return true;
}

CSprite * CSprite::CreateOld(const SSpriteDesc & _spriteDesc)
{
	CSprite * pSprite = new CSprite();
	pSprite->m_pImpl->m_spriteDesc = _spriteDesc;

	return pSprite;
}

CSprite * CSprite::Create(CTexture * _pTexture, const SSpriteDesc & _spriteDesc)
{
	CSprite * pOut = new CSprite();
	pOut->m_pTexture = _pTexture;
	pOut->m_pImpl->m_spriteDesc = _spriteDesc;

	return pOut;
}

bool CSprite::Render(const SSpriteRenderArgs & _args)
{
	g_pVideoDevice->SelectTexture(m_pTexture);

	RECT rSubRect = m_pImpl->m_spriteDesc.m_spriteSets[_args.m_iDirection].m_rSubRects[_args.m_iFrameIndex];
	CBitmap bmSubBitmap = g_pbmSelectedTexture->GetSubBitmap(rSubRect);

	POINT pPos;
	pPos.x = (int)_args.m_pPosition.m_fX;
	pPos.y = (int)_args.m_pPosition.m_fY;

	int iSubRectWidth = rSubRect.right - rSubRect.left;
	int iSubRectHeight = rSubRect.bottom - rSubRect.top;
	for(int iRow = 0; iRow < iSubRectHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < iSubRectWidth; iColumn++)
		{
			int iSourceRow = iRow + rSubRect.top;
			int iSourceColumn = iColumn + rSubRect.left;
			int iTargetRow = iRow + pPos.y;
			int iTargetColumn = iColumn + pPos.x;

			if((iTargetRow < 0) || (iTargetColumn < 0))
			{
				continue;
			}

			if((iTargetRow >= g_pbmScreenBitmap->GetHeight()) || (iTargetColumn >= g_pbmScreenBitmap->GetWidth()))
			{
				continue;
			}

			float fAlpha = g_pmSelectedAlpha->GetValue(iSourceRow, iSourceColumn);
			if(fAlpha < 0.5f)
			{
				continue;
			}

			DWORD dwPixel = g_pbmSelectedTexture->GetPixel(iSourceRow, iSourceColumn);
			g_pbmScreenBitmap->SetPixel(iTargetRow, iTargetColumn, dwPixel);
		}
	}
	
	return true;
}

bool CSprite::RenderOld()
{
	// debug, to file.
	stringstream ssFilename;

	ssFilename << "renders/";

	SYSTEMTIME time;
	GetSystemTime(&time);

	ssFilename << time.wYear;

	ssFilename.fill('0');

	ssFilename.width(2);
	ssFilename << time.wMonth ;

	ssFilename.width(2);
	ssFilename << time.wDay;

	ssFilename.width(2);
	ssFilename << time.wHour; 

	ssFilename.width(2);
	ssFilename << time.wMinute;

	ssFilename.width(2);
	ssFilename << time.wSecond;

	ssFilename.width(3);
	ssFilename << time.wMilliseconds;

	ssFilename << ".bmp";

	string sFilename = ssFilename.str();

	m_pImpl->m_spriteDesc.m_spriteSets[m_iRenderedDir].m_frames[m_iRenderedIndex].Save(sFilename);

	return true;
}

bool CSprite::_CalculatePreRender()
{
	// Rotation.

	float fMinDistance = FLT_MAX;

	float fComparedAngle = fmodf(m_fDir, 2.0f * D3DX_PI);
	fComparedAngle += 2.0f * D3DX_PI;
	fComparedAngle = fmodf(fComparedAngle, 2.0f * D3DX_PI);

	const vector<SDirectionSpriteSet> & dirSpriteSets = m_pImpl->m_spriteDesc.m_spriteSets;
	for(int iDirIndex = 0; iDirIndex < (int)dirSpriteSets.size(); iDirIndex++)
	{
		float fDirAngle = dirSpriteSets[iDirIndex].m_fDir;
		float fDistance = fabsf(fDirAngle - fComparedAngle);

		if(fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			m_iRenderedDir = iDirIndex;
		}
	}

	// TODO: more complicated calculation of rendered rotation.
	m_fRenderedRotation = 0.0f;

	// Index.
	int nFrames = (int)dirSpriteSets[m_iRenderedDir].m_frames.size();
	if((m_iIndex >= 0) && (m_iIndex < nFrames))
	{
		m_iRenderedIndex = m_iIndex;
		return true;
	}

	E_SPRITEWRAPMODE eWrapMode = m_pImpl->m_spriteDesc.m_eWrapMode;
	assert(eWrapMode != SPRITEWRAP_ERROR);

	switch(eWrapMode)
	{
	case SPRITEWRAP_MODULO:
		m_iRenderedIndex = m_iIndex % nFrames;
		break;
	case SPRITEWRAP_CLAMP:
		m_iRenderedIndex = max(m_iIndex, 0);
		m_iRenderedIndex = min(m_iRenderedIndex, nFrames - 1);
		break;
	case SPRITEWRAP_ZIGZAG:
		m_iRenderedIndex = m_iIndex % (2 * nFrames - 2);
		if(m_iRenderedIndex >= nFrames)
		{
			m_iRenderedIndex = (2 * nFrames - 2) - m_iRenderedIndex;
		}
		break;
	default:
		assert(false);
	}

	return true;
}

E_SPRITEWRAPMODE CSprite::GetWrapMode() const
{
	return m_pImpl->m_spriteDesc.m_eWrapMode;
}

int CSprite::GetFrameCount() const
{
	return m_pImpl->m_spriteDesc.m_spriteSets[0].m_frames.size();
}

vector<float> CSprite::GetDirections() const
{
	vector<float> vOut;

	const vector<SDirectionSpriteSet> & vSpriteSets = m_pImpl->m_spriteDesc.m_spriteSets;
	vector<SDirectionSpriteSet>::const_iterator i;

	for(i = vSpriteSets.begin(); i != vSpriteSets.end(); i++)
	{
		float fAngle = (*i).m_fDir;
		vOut.push_back(fAngle);
	}

	return vOut;
}
