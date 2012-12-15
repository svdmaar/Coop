#pragma once

#include "VertexTypes.h"
#include "Bitmap.h"
#include "FloatRect.h"

#include <d3dx9.h>

class CVertexBuffer
{
	bool m_bInited;
	void *m_pSystemMemBuffer;
	size_t m_vertexSize;
	unsigned int m_iVertexCount;
	unsigned int m_iPrimitiveCount;
	IDirect3DVertexBuffer9 *m_pVideoMemBuffer;
	IDirect3DTexture9 *m_pTexture;
	D3DPRIMITIVETYPE m_ePrimitiveType;
	DWORD m_dwFvf;

	void _CalcVertexSize();
	void _SetPosition(int _iVertexIndex, const D3DXVECTOR3& _vPos);
	void _SetTexCoor(int _iVertexIndex, const D3DXVECTOR2& _vTexCoor);
	void _SetColor(int _iVertexIndex, DWORD _dwColor);

protected:
	CVertexBuffer();

	bool _Init(unsigned int _iVertexCount, DWORD _dwFvf, D3DPRIMITIVETYPE _ePrimitiveType);
	bool _SetTexture(const CBitmap& _bitmap);
	void *_GetSystemMemBuffer();
	bool _UploadBuffer();
	bool _SetRectangle(const SFloatRect& _pos, DWORD _dwColor = 0);

public:
	~CVertexBuffer();

	bool Render() const;
	bool CleanUp();
};

class CColoredVertexBuffer : public CVertexBuffer
{


public:
	CColoredVertexBuffer();
	~CColoredVertexBuffer();

	bool Init(const SFloatRect& _pos, DWORD _dwColor);
};

class CTexturedVertexBuffer : public CVertexBuffer
{


public:
	CTexturedVertexBuffer();
	~CTexturedVertexBuffer();

	bool Init(const CBitmap& _bitmap, const SFloatRect& _pos);
};

class CTexturedRectangle : public CTexturedVertexBuffer
{
};
