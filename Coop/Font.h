#pragma once

#include "Bitmap.h"

#include <string>
#include <d3d9.h>

const int g_iCharCount = 94;
const int g_iCharPerRowCount = 10;

class CFont
{
	bool m_bInited;
	CBitmap m_bmp;
	int m_iCharWidth[g_iCharCount];
	int m_iCharColumn[g_iCharCount];
	int m_iCharRow[g_iCharCount];
	int m_iHeight;
	int m_iMaxWidth;
	int m_iNormalDistance;
	int m_iSpaceDistance;
	IDirect3DTexture9 *m_pSystemTexture;
	IDirect3DTexture9 *m_pVideoTexture;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;

	void _FillForArial();
	void _FillForCourier();

	static CBitmap _GetImportFontChar(const CBitmap& _inBmp, int _iCharRowIndex, int _iCharColumnIndex);
	static int _GetImportCharBaseRow(const CBitmap& _inBmp, int _iCharRowIndex);
	static CBitmap _GetImportFontChar(const CBitmap& _inBmp, int _iCharIndex);
	static void _CalcTextLayout(int _iCharHeight, const int *_pCharWidths, int *_pCharRow, int *_pColumn, int& _iTextSize);

public:
	CFont();
	~CFont();

	bool Init(const std::string& _name, const std::string& _fileName, IDirect3DDevice9 *_pDev);
	bool CleanUp();

	bool RenderChar(char _c) const;
	bool RenderString(const std::string& _text, float _fHeight) const;
	float CalcStringWidth(const std::string& _text, float _fHeight) const;
	bool RenderCentered(const std::string& _text, float _fX, float _fY, float _fHeight) const;

	int CalcStringWidth(const std::string& _text);
	CBitmap PrintStringToBitmap(const std::string& _text);

	static void ImportFont(const std::string _inBmpFile, const std::string _outBmpFile, const std::string _outTextFile);
};
