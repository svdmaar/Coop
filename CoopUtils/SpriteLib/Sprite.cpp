#include "Sprite.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <sstream>
#include <assert.h>

using namespace std;

struct SSpriteImpl
{
	IDirect3DTexture9 * m_pTexture;
	IDirect3DVertexBuffer9 * m_pVertexBuffer;

	// Debug
	SSpriteDesc m_spriteDesc;

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

CSprite * CSprite::Create(const SSpriteDesc & _spriteDesc)
{
	CSprite * pSprite = new CSprite();
	pSprite->m_pImpl->m_spriteDesc = _spriteDesc;

	return pSprite;
}

bool CSprite::Render()
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
