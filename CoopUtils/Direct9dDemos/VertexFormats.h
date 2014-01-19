#pragma once

#include <d3dx9.h>

struct SColoredVertex
{
	float m_fX, m_fY, m_fZ;
	DWORD m_dwColor;

	const static DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

struct STexturedVertex
{
	float m_fX, m_fY, m_fZ;
	float m_fU, m_fV;

	const static DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};
