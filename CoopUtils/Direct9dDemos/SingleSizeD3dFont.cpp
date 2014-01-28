#include "SingleSizeD3dFont.h"

#include "EnvSetup.h"
#include "VertexFormats.h"

#include <iostream>

using namespace std;

const int g_nVerticesPerRect = 6;

CSingleSizeD3dFont::CSingleSizeD3dFont()
{
}

CSingleSizeD3dFont::~CSingleSizeD3dFont()
{

}

bool CSingleSizeD3dFont::_CalcUpperLeftCoor(const POINT & _pPixel, SFloatPoint & _pCoor) const
{
	_pCoor.m_fX = m_pUpperLeft.m_fX + m_pPixelSize.m_fX * (float)_pPixel.x;
	_pCoor.m_fY = m_pUpperLeft.m_fY - m_pPixelSize.m_fY * (float)_pPixel.y;

	return true;
}

bool CSingleSizeD3dFont::_CalcCoorsForPixels(const RECT & _rPixels, SFloatRect & _rCoors) const
{
	POINT point;
	
	point.x = _rPixels.left;
	point.y = _rPixels.top;
	_CalcUpperLeftCoor(point, _rCoors.m_pUpperLeft);

	point.x = _rPixels.right + 1;
	point.y = _rPixels.bottom + 1;
	_CalcUpperLeftCoor(point, _rCoors.m_pLowerRight);

	return true;
}

bool CSingleSizeD3dFont::_InitUpperLeftCoor()
{
	m_pPixelSize.m_fX = 2.0f / (float)m_pScreenSize.x;
	m_pPixelSize.m_fY = 2.0f / (float)m_pScreenSize.y;

	m_pUpperLeft.m_fX = -1.0f - 0.5f * m_pPixelSize.m_fX;
	m_pUpperLeft.m_fY = 1.0f + 0.5f * m_pPixelSize.m_fY;

	return true;
}

bool CSingleSizeD3dFont::_CalcTextureCoors(const RECT & _rTexels, SFloatRect & _rTexCoors) const
{
	SFloatPoint pPixelSize;
	pPixelSize.m_fX = 1.0f / (float)m_bmBigBitmap.GetWidth();
	pPixelSize.m_fY = 1.0f / (float)m_bmBigBitmap.GetHeight();

	_rTexCoors.m_pUpperLeft.m_fX = pPixelSize.m_fX * (float)_rTexels.left;
	_rTexCoors.m_pUpperLeft.m_fY = pPixelSize.m_fY * (float)_rTexels.top;

	_rTexCoors.m_pLowerRight.m_fX = pPixelSize.m_fX * (float)_rTexels.right;
	_rTexCoors.m_pLowerRight.m_fY = pPixelSize.m_fY * (float)_rTexels.bottom;

	return true;
}

bool CSingleSizeD3dFont::_CalcRelCoorsForChar(const SCharDesc & _charDesc, SFloatRect & _rCoors) const
{
	int iCharWidth = _charDesc.m_bitmap.GetWidth();
	int iCharHeight = _charDesc.m_bitmap.GetHeight();

	_rCoors.m_pUpperLeft.m_fX = 0.0f;
	_rCoors.m_pUpperLeft.m_fY = 0.0f;
	_rCoors.m_pLowerRight.m_fX = m_pPixelSize.m_fX * (float)iCharWidth;
	_rCoors.m_pLowerRight.m_fY = -m_pPixelSize.m_fY * (float)iCharHeight;

	return true;
}

bool CSingleSizeD3dFont::_FillVertexBuffer(STexturedVertex * _pVertices) const
{
	const float fZ = 0.5f;

	for(int iCharIndex = 0; iCharIndex < GetCharCount(); iCharIndex++)
	{
		const SCharDesc & charDesc = m_vCharDescs[iCharIndex];

		int iCharWidth = charDesc.m_bitmap.GetWidth();
		int iCharHeight = charDesc.m_bitmap.GetHeight();

		RECT rTexelCoors;
		rTexelCoors.left = charDesc.m_pInBitmap.x;
		rTexelCoors.top = charDesc.m_pInBitmap.y;
		rTexelCoors.right = rTexelCoors.left + iCharWidth;
		rTexelCoors.bottom = rTexelCoors.top + iCharHeight;

		SFloatRect rCoors;
		_CalcCoorsForPixels(rTexelCoors, rCoors);

		SFloatRect rTextureCoors;
		_CalcTextureCoors(rTexelCoors, rTextureCoors);

		SFloatRect rRelCoors;
		_CalcRelCoorsForChar(charDesc, rRelCoors);

		STexturedVertex upperLeft;
		upperLeft.m_fX = rRelCoors.m_pUpperLeft.m_fX;
		upperLeft.m_fY = rRelCoors.m_pUpperLeft.m_fY;
		upperLeft.m_fZ = fZ;
		upperLeft.m_fU = rTextureCoors.m_pUpperLeft.m_fX;
		upperLeft.m_fV = rTextureCoors.m_pUpperLeft.m_fY;

		STexturedVertex lowerLeft;
		lowerLeft.m_fX = rRelCoors.m_pUpperLeft.m_fX;
		lowerLeft.m_fY = rRelCoors.m_pLowerRight.m_fY;
		lowerLeft.m_fZ = fZ;
		lowerLeft.m_fU = rTextureCoors.m_pUpperLeft.m_fX;
		lowerLeft.m_fV = rTextureCoors.m_pLowerRight.m_fY;

		STexturedVertex lowerRight;
		lowerRight.m_fX = rRelCoors.m_pLowerRight.m_fX;
		lowerRight.m_fY = rRelCoors.m_pLowerRight.m_fY;
		lowerRight.m_fZ = fZ;
		lowerRight.m_fU = rTextureCoors.m_pLowerRight.m_fX;
		lowerRight.m_fV = rTextureCoors.m_pLowerRight.m_fY;

		STexturedVertex upperRight;
		upperRight.m_fX = rRelCoors.m_pLowerRight.m_fX;
		upperRight.m_fY = rRelCoors.m_pUpperLeft.m_fY;
		upperRight.m_fZ = fZ;
		upperRight.m_fU = rTextureCoors.m_pLowerRight.m_fX;
		upperRight.m_fV = rTextureCoors.m_pUpperLeft.m_fY;

		STexturedVertex * pTargetBuffer = _pVertices + g_nVerticesPerRect * iCharIndex;
		pTargetBuffer[0] = upperLeft;
		pTargetBuffer[1] = lowerLeft;
		pTargetBuffer[2] = lowerRight;

		pTargetBuffer[3] = upperLeft;
		pTargetBuffer[4] = lowerRight;
		pTargetBuffer[5] = upperRight;
	}

	return true;
}

bool CSingleSizeD3dFont::Init(const std::string & _sFilenameBase, const POINT & _pScreenSize, DWORD _dwColor)
{
	m_dwColor = _dwColor;

	if(!Load(_sFilenameBase))
	{
		return false;
	}

	m_pScreenSize = _pScreenSize;
	_InitUpperLeftCoor();

	int nVertices = GetCharCount() * 6;
	UINT uBufferLength = sizeof(STexturedVertex) * nVertices;
	HRESULT hr = g_pD3dDevice->CreateVertexBuffer(uBufferLength, 0, STexturedVertex::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr)) cout << "create vertex buffer: " << hr << endl;

	STexturedVertex * pBuffer = NULL;
	hr = m_pVertexBuffer->Lock(0, 0, (void **)&pBuffer, 0);
	if(FAILED(hr)) cout << "lock vertex buffer: " << hr << endl;

	if(!_FillVertexBuffer(pBuffer))
	{
		return false;
	}

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr)) cout << "unlock vertex buffer: " << hr << endl;

	int iTextureWidth = m_bmBigBitmap.GetWidth();
	int iTextureHeight = m_bmBigBitmap.GetHeight();

	IDirect3DTexture9 * pSystemTexture = NULL;
	hr = g_pD3dDevice->CreateTexture(iTextureWidth, iTextureHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pSystemTexture, NULL); // TODO: alpha blending?
	if(FAILED(hr)) cout << "create system texture: " << hr << endl;

	D3DLOCKED_RECT lockedRect;
	ZeroMemory(&lockedRect, sizeof(lockedRect));

	const UINT uLevel = 0;
	hr = pSystemTexture->LockRect(uLevel, &lockedRect, NULL, 0);
	if(FAILED(hr)) cout << "lock texture rect: " << hr << endl;

	BYTE * pbBits = (BYTE *)lockedRect.pBits;
	for(int iRow = 0; iRow < iTextureHeight; iRow++)
	{
		for(int iColumn = 0; iColumn < iTextureWidth; iColumn++)
		{
			DWORD dwPixel = m_bmBigBitmap.GetPixel(iRow, iColumn);
			dwPixel = _ColorPixel(dwPixel);

			int iTargetIndex = iRow * lockedRect.Pitch + iColumn * sizeof(DWORD);
			DWORD * pTarget = (DWORD *)(pbBits + iTargetIndex);

			*pTarget = dwPixel;
		}
	}

	hr = pSystemTexture->UnlockRect(uLevel);
	if(FAILED(hr)) cout << "unlock texture rect: " << hr << endl;

	hr = g_pD3dDevice->CreateTexture(iTextureWidth, iTextureHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pVideoTexture, NULL);
	if(FAILED(hr)) cout << "create video texture: " << hr << endl;

	hr = g_pD3dDevice->UpdateTexture(pSystemTexture, m_pVideoTexture);
	if(FAILED(hr)) cout << "update texture: " << hr << endl;

	hr = pSystemTexture->Release();
	if(FAILED(hr)) cout << "release system texture: " << hr << endl;

	return true;
}

bool CSingleSizeD3dFont::RenderString(const std::string & _sText, const POINT & _pPixel)
{
	HRESULT hr;
	vector<SCharPosition> vCharPos;

	if(!_CalcCharPositions(_sText, vCharPos))
	{
		return false;
	}

	hr = g_pD3dDevice->SetFVF(STexturedVertex::FVF);
	if(FAILED(hr)) cout << "set fvf: " << hr << endl;

	hr = g_pD3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(STexturedVertex));
	if(FAILED(hr)) cout << "set stream source: " << hr << endl;

	hr = g_pD3dDevice->SetTexture(0, m_pVideoTexture);
	if(FAILED(hr)) cout << "set texture: " << hr << endl;

	for(int iCharIndex = 0; iCharIndex < (int)vCharPos.size(); iCharIndex++)
	{
		const SCharPosition & charPos = vCharPos[iCharIndex];

		SFloatPoint pCoors;
		pCoors.m_fX = m_pUpperLeft.m_fX + m_pPixelSize.m_fX * (float)charPos.m_pPos.x;
		pCoors.m_fY = m_pUpperLeft.m_fY - m_pPixelSize.m_fY * (float)charPos.m_pPos.y;

		D3DXMATRIX worldMatrix;
		D3DXMatrixTranslation(&worldMatrix, pCoors.m_fX, pCoors.m_fY, 0.0f);

		hr = g_pD3dDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
		if(FAILED(hr)) cout << "Set world transform: " << hr << endl;

		UINT iStartVertex = (UINT)(g_nVerticesPerRect * charPos.m_iIndex);
		hr = g_pD3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, iStartVertex, 2);
		if(FAILED(hr)) cout << "draw primitive: " << hr << endl;
	}

	return true;
}

DWORD CSingleSizeD3dFont::_ColorPixel(DWORD _dwPixel) const
{
	int iRedColor;
	int iGreenColor;
	int iBlueColor;
	CBitmap::DecodeColor(m_dwColor, iRedColor, iGreenColor, iBlueColor);

	int iRedPixel;
	int iGreenPixel;
	int iBluePixel;
	CBitmap::DecodeColor(_dwPixel, iRedPixel, iGreenPixel, iBluePixel);

	int iRedOutput = iRedColor * iRedPixel / 255;
	int iGreenOutput = iGreenColor * iGreenPixel / 255;
	int iBlueOutput = iBlueColor * iBluePixel / 255;
	DWORD dwNewPixel = CBitmap::EncodeColor(iRedOutput, iGreenOutput, iBlueOutput);

	return dwNewPixel;
}
