#pragma once

#include "GraphicsManager.h"
#include "FloatRect.h"
#include "FloatPoint.h"
#include "Bitmap.h"

enum Object2DType
{
	otNone = 0,
	otInner = 1,
	otFrame = 2,
	otAll = 3,
};

struct SObject2DDesc
{
	Object2DType m_eType;
	DWORD m_dwInnerColor;
	DWORD m_dwOuterColor;
};

class CObject2D
{
	bool m_bInited;
	SObject2DDesc m_desc;
	SFloatRect m_pos;

protected:
	CObject2D();
	~CObject2D();

	bool _RenderVertexBuffer(IDirect3DVertexBuffer9 *_pVertexBuffer, size_t _vertexSize, DWORD _fvf, D3DPRIMITIVETYPE _type, UINT _iCount) const;
	bool _Init(const SObject2DDesc& _desc, const SFloatRect _pos);
	bool _CleanUp();
	Object2DType _GetType() const;
	const SObject2DDesc& _GetDesc() const;
	const SFloatRect& _GetPos() const;

public:
	bool IsInited() const;
};

class CRectObject : public CObject2D
{
	IDirect3DVertexBuffer9 *m_pInnerBuffer; // Filled rectangle.
	IDirect3DVertexBuffer9 *m_pFrameBuffer; // Just the frame.

	void _Clear();
	bool _CreateInnerBuffer();
	bool _CreateOuterBuffer();

public:
	CRectObject();
	~CRectObject();

	bool Init(const SObject2DDesc& _desc, const SFloatRect& _pos);
	bool Render() const;
	bool CleanUp();
};

class CImageObject : public CObject2D
{
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DTexture9 *m_pTexture;

	bool _CreateBuffer();
	bool _CreateTexture(const CBitmap& _bitmap);

public:
	CImageObject();
	~CImageObject();

	bool Init(const SFloatRect& _pos, const CBitmap& _bitmap);
	bool Render() const;
	bool Render(const SFloatPoint & _offset) const;
	bool CleanUp();
};
