#pragma once

#include "VertexTypes.h"

class CVertexBuffer
{
	void *m_pSystemMemBuffer;
	size_t m_vertexSize;
	unsigned int m_iCount;
	IDirect3DVertexBuffer9 *m_pVideoMemBuffer;
	IDirect3DTexture9 *m_pTexture;

protected:
	bool _Initalize(size_t _vertexSize, unsigned int _iCount, D3DPRIMITIVETYPE _ePrimitiveType);
};

class CColoredVertexBuffer : public CVertexBuffer
{


};

class CTexturedVertexBuffer : public CVertexBuffer
{
};
