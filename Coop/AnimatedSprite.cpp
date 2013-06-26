#include "AnimatedSprite.h"

#include <assert.h>

using namespace std;

CAnimatedSprite::CAnimatedSprite()
{
	m_bInited = false;
}

CAnimatedSprite::~CAnimatedSprite()
{
	assert(!m_bInited);
}

bool CAnimatedSprite::Init(const string & _sFileName, const RECT & _firstFrame, int _nFrames)
{
	assert(!m_bInited);

	int iFrameWidth = _firstFrame.right - _firstFrame.left;
	int iFrameHeight = _firstFrame.bottom - _firstFrame.top;
	CBitmap bmBase(_sFileName);

	for(int iFrame = 0; iFrame < _nFrames; iFrame++)
	{
		int iRightOffset = iFrame * iFrameWidth;
		RECT rSub = _firstFrame;
		rSub.left += iRightOffset;
		rSub.right += iRightOffset;

		CBitmap bmFrame = bmBase.GetSubBitmap(rSub);
		
		CSprite * pSprite = new CSprite();
		pSprite->Init(bmFrame);

		m_vFrames.push_back(pSprite);
	}

	m_bInited = true;
	return true;
}

bool CAnimatedSprite::CleanUp()
{
	vector<CSprite *>::iterator i;

	for(i = m_vFrames.begin(); i != m_vFrames.end(); i++)
	{
		CSprite * pSprite = *i;

		if(!pSprite->CleanUp())
		{
			return false;
		}

		delete pSprite;
	}

	m_vFrames.clear();

	m_bInited = false;
	return true;
}

int CAnimatedSprite::GetFrameCount() const
{
	return m_vFrames.size();
}

bool CAnimatedSprite::Render(int iFrameIndex, const SFloatPoint & _pos)
{
	assert(iFrameIndex >= 0);
	assert(iFrameIndex < GetFrameCount());

	CSprite * pSprite = m_vFrames[iFrameIndex];

	return pSprite->Render(_pos);
}

bool CAnimatedSprite::Init(const std::string & _sName, int _iRow)
{
	//assert(_sName == "pacman.player");

	RECT rSub;
	rSub.left = 0;
	rSub.top = _iRow * 32;
	rSub.right = 32;
	rSub.bottom = rSub.top + 32;

	if(!Init("D:\\projects\\Coop\\bin\\final\\Debug\\bitmap\\pacman.bmp", rSub, 8))
	{
		return false;
	}

	return true;
}

bool CAnimatedSprite::Init(const CBitmap & _bitmap)
{
	int iFrameHeight = _bitmap.GetHeight();
	int iFrameCount = _bitmap.GetWidth() / iFrameHeight;

	//assert(iFrameCount * iFrameHeight


	return false;
}
