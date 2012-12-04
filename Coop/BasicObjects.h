#pragma once

#include "VertexTypes.h"

// TODO: Replace basic objects with esh objects.

class CBasicObjects
{
	bool m_bInited;
	IDirect3DVertexBuffer9 *m_pBoxVB;
	IDirect3DVertexBuffer9 *m_pGridVB;
	IDirect3DVertexBuffer9 *m_pPolesVB;
	IDirect3DVertexBuffer9 *m_pRectVB;
	IDirect3DVertexBuffer9 *m_pNormalBox;

	bool _FillBoxVB();
	bool _FillGridVB();
	bool _FillPolesVB();
	bool _FillRectVB();
	bool _FillBoxVBNormal();

	static void _SetTextureCoors(SFaceVertex *_pVertex, float _fTu, float _fTv);
	static void _SetPrevFaceNormals(SNormalVertex *_pTooLate, float _nx, float _ny, float _nz);

	static CBasicObjects *m_pInstance;

public:
	CBasicObjects();
	~CBasicObjects();

	bool Init();
	bool CleanUp();
	bool IsInited();

	bool RenderBox();
	bool RenderGrid();
	bool RenderPoles();
	bool RenderRect();
	bool RenderBoxNormal();

	static CBasicObjects *GetInstance();
};
