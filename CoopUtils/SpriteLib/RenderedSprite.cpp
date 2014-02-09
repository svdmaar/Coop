#include "RenderedSprite.h"

#include <vector>
#include <assert.h>
#include <minmax.h>

using namespace std;

const float g_fPi = 3.141592654f;

CRenderedSprite::CRenderedSprite(CSprite * _pSprite)
{
	m_pSprite = _pSprite;
}

void CRenderedSprite::SetFrameIndex(int _iIndex)
{
	m_iFrameIndex = _iIndex;

	_CalculatePreRender();
}

int CRenderedSprite::GetFrameIndex() const
{
	return m_iFrameIndex;
}

void CRenderedSprite::SetDirection(float _fDirection)
{
	m_fDirection = _fDirection;

	_CalculatePreRender();
}

float CRenderedSprite::GetDirection() const
{
	return m_fDirection;
}

bool CRenderedSprite::_CalculatePreRender()
{
	// Rotation.

	float fMinDistance = FLT_MAX;

	float fComparedAngle = fmodf(m_fDirection, 2.0f * g_fPi);
	fComparedAngle += 2.0f * g_fPi;
	fComparedAngle = fmodf(fComparedAngle, 2.0f * g_fPi);

	vector<float> vDirections = m_pSprite->GetDirections();
	for(int iDirIndex = 0; iDirIndex < (int)vDirections.size(); iDirIndex++)
	{
		float fDirAngle = vDirections[iDirIndex];
		float fDistance = fabsf(fDirAngle - fComparedAngle);

		if(fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			m_renderArgs.m_iDirection = iDirIndex;
		}
	}

	// TODO: more complicated calculation of rendered rotation.
	m_renderArgs.m_fRotation = 0.0f;

	// Index.
	int nFrames = m_pSprite->GetFrameCount();
	if((m_iFrameIndex >= 0) && (m_iFrameIndex < nFrames))
	{
		m_renderArgs.m_iFrameIndex = m_iFrameIndex;
		return true;
	}

	E_SPRITEWRAPMODE eWrapMode = m_pSprite->GetWrapMode();
	assert(eWrapMode != SPRITEWRAP_ERROR);

	switch(eWrapMode)
	{
	case SPRITEWRAP_MODULO:
		m_renderArgs.m_iFrameIndex = m_iFrameIndex % nFrames;
		break;
	case SPRITEWRAP_CLAMP:
		m_renderArgs.m_iFrameIndex = max(m_iFrameIndex, 0);
		m_renderArgs.m_iFrameIndex = min(m_renderArgs.m_iFrameIndex, nFrames - 1);
		break;
	case SPRITEWRAP_ZIGZAG:
		m_renderArgs.m_iFrameIndex = m_iFrameIndex % (2 * nFrames - 2);
		if(m_renderArgs.m_iFrameIndex >= nFrames)
		{
			m_renderArgs.m_iFrameIndex = (2 * nFrames - 2) - m_renderArgs.m_iFrameIndex;
		}
		break;
	default:
		assert(false);
	}

	return true;
}

void CRenderedSprite::SetPosition(const SFloatPoint & _pPosition)
{
	m_pPosition = _pPosition;

	m_renderArgs.m_pPosition = m_pPosition;
}

SFloatPoint CRenderedSprite::GetPosition() const
{
	return m_pPosition;
}

bool CRenderedSprite::Render()
{
	return m_pSprite->Render(m_renderArgs);
}
