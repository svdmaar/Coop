#include "SingleSizeD3dFont.h"

#include "EnvSetup.h"
#include "VertexFormats.h"

#include <iostream>

using namespace std;

CSingleSizeD3dFont::CSingleSizeD3dFont()
{
}

CSingleSizeD3dFont::~CSingleSizeD3dFont()
{

}

bool CSingleSizeD3dFont::_CalcUpperLeftCoor(const POINT & _pPixel, SFloatPoint & _pCoor) const
{


	return true;
}

bool CSingleSizeD3dFont::_CalcCoorstForPixels(const RECT & _rPixels, SFloatRect & _rCoors) const
{



	return true;
}

bool CSingleSizeD3dFont::Init(const std::string & _sFilenameBase, const POINT & _pScreenSize)
{
	if(!Load(_sFilenameBase))
		return false;

	m_pScreenSize = _pScreenSize;

	int nVertices = GetCharCount() * 6;
	UINT uBufferLength = sizeof(STexturedVertex) * nVertices;
	HRESULT hr = g_pD3dDevice->CreateVertexBuffer(uBufferLength, 0, STexturedVertex::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr)) cout << "create vertex buffer: " << hr << endl;

	STexturedVertex * pBuffer = NULL;
	hr = m_pVertexBuffer->Lock(0, 0, (void **)&pBuffer, 0);
	if(FAILED(hr)) cout << "lock vertex buffer: " << hr << endl;





	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr)) cout << "unlock vertex buffer: " << hr << endl;



	return true;
}

bool CSingleSizeD3dFont::RenderString(const std::string & _sText, const POINT & _pPixel)
{




	return true;
}
