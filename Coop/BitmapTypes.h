#pragma once

#ifdef WIN32
#include <Windows.h>
#else

#define DWORD unsigned int
#define BYTE unsigned char
#define LONG int
typedef unsigned short WORD;

struct RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

#pragma pack(1)
struct BITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
};

struct BITMAPINFOHEADER
{
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};
#pragma pack()

#endif // ifndef WIN32

#pragma pack(1)
struct SBitmapHeader
{
	WORD m_bfType;
	DWORD m_bfSize;
	WORD m_bfReserved1;
	WORD m_bfReserved2;
	DWORD m_bfOffBits;
	DWORD m_biSize;
	LONG m_biWidth;
	LONG m_biHeight;
	WORD m_biPlanes;
	WORD m_biBitCount;
	DWORD m_biCompression;
	DWORD m_biSizeImage;
	LONG m_biXPelsPerMeter;
	LONG m_biYPelsPerMeter;
	DWORD m_biClrUsed;
	DWORD m_biClrImportant;
};
#pragma pack()

