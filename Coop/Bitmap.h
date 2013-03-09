#pragma once

#include "BitmapTypes.h"

//#include <Windows.h>
#include <string>

class CBitmap
{
	int m_iWidth;
	int m_iHeight;
	DWORD * m_pData;

	void _CopyFromOtherBmp(const CBitmap& _bmp);

public:
	CBitmap();
	CBitmap(RECT _size);
	CBitmap(std::string _fileName);
	CBitmap(int _iWidth, int _iHeight, DWORD _dwColor);
	CBitmap(const CBitmap& _bmp);
	virtual ~CBitmap();

	CBitmap& operator=(const CBitmap& _bmp);

	DWORD GetPixel(int _iRow, int _iColumn) const;
	void SetPixel(int _iRow, int _iColumn, DWORD _dwColor);
	bool GetFirstPixel(DWORD _dwColor, int& _iRow, int& _iColumn) const;
	RECT GetTrimRect(DWORD _dwIgnoredColor) const;
	void Replace(DWORD _dwOldColor, DWORD _dwNewColor);
	void InsertBitmap(int _iRow, int _iColumn, const CBitmap& _bmp);
	CBitmap Normalize();
	CBitmap MakeGrayValue();

	bool Load(std::string _fileName);
	bool Save(std::string _fileName) const;
	bool CreateFromData(const DWORD *_pData, int _iWidth, int _iHeight, int _iByteStride);

	int GetWidth() const;
	int GetHeight() const;
	const DWORD *GetData() const;
	const DWORD *GetRow(int _iRowIndex) const;

	CBitmap GetSubBitmap(RECT _rect) const;

	static DWORD EncodeColor(int _iRed, int _iGreen, int _iBlue);
	static void DecodeColor(DWORD _dwColor, int& _iRed, int& _iGreen, int& _iBlue);

	static void Test();
};
