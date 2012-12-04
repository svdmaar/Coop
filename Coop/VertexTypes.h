#pragma once 

#include <Windows.h>
#include <d3d9.h>

struct SFaceVertex
{
	float x, y, z;
	float tu, tv;

	const static DWORD _fvf = (D3DFVF_XYZ | D3DFVF_TEX1);
};

struct SLineVertex
{
	float x, y, z;
	DWORD color;

	const static DWORD _fvf = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
};

struct SNormalVertex
{
	float x, y, z;
	float nx, ny, nz;

	const static DWORD _fvf = (D3DFVF_XYZ | D3DFVF_NORMAL);
};

struct SSimpleVertex
{
	float x, y, z;
	DWORD color;

	const static DWORD m_fvf = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
};

struct SColoredVertex
{
	float x, y, z;
	DWORD color;

	const static DWORD _fvf = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
};

struct STexturedVertex
{
	float x, y, z;
	float tu, tv;

	const static DWORD _fvf = (D3DFVF_XYZ | D3DFVF_TEX1);
};
