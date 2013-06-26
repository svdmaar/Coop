#include "Bitmap.h"

#include <assert.h>
#include <minmax.h>

using namespace std;

CBitmap::CBitmap()
{
	m_iWidth = m_iHeight = 0;
	m_pData = NULL;
}

CBitmap::CBitmap(RECT _size)
{
	m_iWidth = _size.right - _size.left;
	m_iHeight = _size.bottom - _size.top;

	DWORD dwColor = 0;
	
	m_pData = new DWORD[m_iWidth * m_iHeight];
	memset(m_pData, dwColor, sizeof(DWORD) * m_iWidth * m_iHeight);
}

CBitmap::CBitmap(string _fileName)
{
	m_iWidth = m_iHeight = 0;
	m_pData = NULL;

	Load(_fileName);
}

CBitmap::CBitmap(int _iWidth, int _iHeight, DWORD _dwColor)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	
	int iPixelCount = m_iWidth * m_iHeight;
	m_pData = new DWORD[iPixelCount];
	//memset(m_pData, _dwColor, sizeof(DWORD) * m_iWidth * m_iHeight);

	for(int i = 0; i < iPixelCount; i++)
	{
		m_pData[i] = _dwColor;
	}
}

CBitmap::CBitmap(const CBitmap& _bmp)
{
	_CopyFromOtherBmp(_bmp);
}

CBitmap::~CBitmap()
{
	if(m_pData != NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	m_iWidth = m_iHeight = 0;
}

CBitmap& CBitmap::operator=(const CBitmap& _bmp)
{
	if(this == &_bmp)
		return *this;

	if(m_pData != NULL)
		delete [] m_pData;

	_CopyFromOtherBmp(_bmp);

	return *this;
}

DWORD CBitmap::GetPixel(int _iRow, int _iColumn) const
{
	int iIndex = _iColumn + _iRow * m_iWidth;
	return m_pData[iIndex];
}

void CBitmap::SetPixel(int _iRow, int _iColumn, DWORD _dwColor)
{
	int iIndex = _iColumn + _iRow * m_iWidth;
	m_pData[iIndex] = _dwColor;
}

bool CBitmap::Load(string _fileName)
{
	// TODO: deal with negative height
	FILE *pInFile = NULL;
	fopen_s(&pInFile, _fileName.c_str(), "rb");

	if(pInFile == NULL)
		return false;

	BITMAPFILEHEADER fileHeader;

	fread(&fileHeader, sizeof(fileHeader), 1, pInFile);

	BITMAPINFOHEADER infoHeader;

	fread(&infoHeader, sizeof(infoHeader), 1, pInFile);

	if(m_pData != NULL)
		delete [] m_pData;

	m_iWidth = infoHeader.biWidth;
	m_iHeight = infoHeader.biHeight;

	m_pData = new DWORD[m_iWidth * m_iHeight];

	int iSkippedByteCount = 0;
	
	if(infoHeader.biBitCount == 24)
	{
		if(m_iWidth % 4 != 0)
		{
			iSkippedByteCount = 4 - (3 * m_iWidth) % 4;
		}
	}

	for(int iRowIndex = (m_iHeight - 1); iRowIndex >= 0; iRowIndex--)
	{
		for(int iColumnIndex = 0; iColumnIndex < m_iWidth; iColumnIndex++)
		{
			int iIndex = iColumnIndex + iRowIndex * m_iWidth;
			DWORD dwValue = 0;

			if(infoHeader.biBitCount == 24)
			{
				BYTE buffer[3];
				fread(buffer, sizeof(buffer), 1, pInFile);
				dwValue = (((int)buffer[2]) << 16) |  (((int)buffer[1]) << 8) | (((int)buffer[0]));
			}
			else
			{
				fread(&dwValue, sizeof(dwValue), 1, pInFile);
			}


			m_pData[iIndex] = dwValue;
		}

		fseek(pInFile, iSkippedByteCount, SEEK_CUR);
	}

	fclose(pInFile);

	return true;
}

bool CBitmap::Save(string _fileName) const
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	fileHeader.bfType = 19778;
	fileHeader.bfSize = sizeof(fileHeader) + sizeof(infoHeader) + m_iWidth * m_iHeight * 4;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader);

	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = m_iWidth;
	infoHeader.biHeight	= m_iHeight;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 32;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = m_iWidth * m_iHeight * 4;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed= 0;
	infoHeader.biClrImportant = 0;
	
	FILE *pOutFile = NULL;
	fopen_s(&pOutFile, _fileName.c_str(), "wb");
	fwrite(&fileHeader, sizeof(fileHeader), 1, pOutFile);
	fwrite(&infoHeader, sizeof(infoHeader), 1, pOutFile);
	
	for(int iRowIndex = (m_iHeight - 1); iRowIndex >= 0; iRowIndex--)
	{
		fwrite(m_pData + iRowIndex * m_iWidth, sizeof(DWORD), m_iWidth, pOutFile);
	}
	fclose(pOutFile);

	return true;
}

bool CBitmap::CreateFromData(const DWORD *_pData, int _iWidth, int _iHeight, int _iByteStride)
{
	// TODO: deal with different pixel formats
	if(m_pData != NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	m_pData = new DWORD[m_iWidth * m_iHeight];

	int iBytesLeft = _iByteStride - _iWidth * (int)sizeof(DWORD);
	const DWORD *pSourceLocation = _pData;

	for(int iRowIndex = 0; iRowIndex < m_iHeight; iRowIndex++)
	{
		memcpy(m_pData + iRowIndex * m_iWidth, pSourceLocation, sizeof(DWORD) * m_iWidth);

		pSourceLocation += m_iWidth;
		BYTE *pSourceByte = (BYTE *)pSourceLocation;
		pSourceByte += iBytesLeft;
		pSourceLocation = (DWORD *)pSourceByte;
	}

	return true;
}

void CBitmap::Test()
{
	CBitmap bm("in.bmp");
	bm.Save("out.bmp");

	const int fontTexSize = 512;
	const int fontSquareSize = fontTexSize / 10;
	CBitmap bm2(fontTexSize, fontTexSize, 0);
	
	int iColumnIndex = 0, iRowIndex = 0;

	for(iColumnIndex = fontSquareSize - 1; iColumnIndex < fontTexSize; iColumnIndex += fontSquareSize)
	{
		for(iRowIndex = 0; iRowIndex < fontTexSize; iRowIndex++)
		{
			bm2.SetPixel(iRowIndex, iColumnIndex, 0xffffff);
		}
	}

	for(iRowIndex = fontSquareSize - 1; iRowIndex < fontTexSize; iRowIndex += fontSquareSize)
	{
		for(iColumnIndex = 0; iColumnIndex < fontTexSize; iColumnIndex++)
		{
			bm2.SetPixel(iRowIndex, iColumnIndex, 0xffffff);
		}
	}

	bm2.Save("for_font.bmp");
}

int CBitmap::GetWidth() const
{
	return m_iWidth;
}

int CBitmap::GetHeight() const
{
	return m_iHeight;
}

const DWORD *CBitmap::GetData() const
{
	return m_pData;
}

const DWORD *CBitmap::GetRow(int _iRowIndex) const
{
	return m_pData + _iRowIndex * GetWidth();
}

CBitmap CBitmap::GetSubBitmap(RECT _rect) const
{
	CBitmap out(_rect);

	assert(_rect.right <= m_iWidth);	
	assert(_rect.bottom <= m_iHeight);

	for(int iOutRowIndex = 0; iOutRowIndex < out.GetHeight(); iOutRowIndex++)
	{
		for(int iOutColumnIndex = 0; iOutColumnIndex < out.GetWidth(); iOutColumnIndex++)
		{
			int iInRowIndex = iOutRowIndex + _rect.top;
			int iInColumnIndex = iOutColumnIndex + _rect.left;

			DWORD dwPixel = GetPixel(iInRowIndex, iInColumnIndex);
			out.SetPixel(iOutRowIndex, iOutColumnIndex, dwPixel);
		}
	}

	return out;
}

bool CBitmap::GetFirstPixel(DWORD _dwColor, int& _iRow, int& _iColumn) const
{
	bool bFound = false;

	for(_iRow = 0; _iRow < GetHeight(); _iRow++)
	{
		for(_iColumn = 0; _iColumn < GetWidth(); _iColumn++)
		{
			if(GetPixel(_iRow, _iColumn) == _dwColor)
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
		{
			break;
		}
	}

	if(!bFound)
	{
		_iRow = _iColumn = -1;
	}
	
	return bFound;
}

RECT CBitmap::GetTrimRect(DWORD _dwIgnoredColor) const
{
	RECT out = {GetWidth(), GetHeight(), 0, 0};
	bool bFound = false;

	for(int iRowIndex = 0; iRowIndex < GetHeight(); iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < GetWidth(); iColumnIndex++)
		{
			DWORD dwPixel = GetPixel(iRowIndex, iColumnIndex);
			if(dwPixel != _dwIgnoredColor)
			{
				out.left = min(out.left, iColumnIndex);
				out.top = min(out.top, iRowIndex);
				out.right = max(out.right, iColumnIndex + 1);
				out.bottom = max(out.bottom, iRowIndex + 1);

				bFound = true;
			}
		}
	}

	if(!bFound)
	{
		out.left = out.right = out.top = out.bottom = 0;
	}

	return out;
}

void CBitmap::Replace(DWORD _dwOldColor, DWORD _dwNewColor)
{
	for(int iIndex = 0; iIndex < (GetWidth() * GetHeight()); iIndex++)
	{
		if(m_pData[iIndex] == _dwOldColor)
		{
			m_pData[iIndex] = _dwNewColor;
		}
	}
}

void CBitmap::InsertBitmap(int _iRow, int _iColumn, const CBitmap& _bmp)
{
	assert((_iColumn + _bmp.GetWidth()) <= m_iWidth);
	assert((_iRow + _bmp.GetHeight()) <= m_iHeight);

	for(int iSourceRowIndex = 0; iSourceRowIndex < _bmp.GetHeight(); iSourceRowIndex++)
	{
		for(int iSourceColumnIndex = 0; iSourceColumnIndex < _bmp.GetWidth(); iSourceColumnIndex++)
		{
			int iTargetRowIndex = _iRow + iSourceRowIndex;
			int iTargetColumnIndex = _iColumn + iSourceColumnIndex;

			DWORD dwPixel = _bmp.GetPixel(iSourceRowIndex, iSourceColumnIndex);
			SetPixel(iTargetRowIndex, iTargetColumnIndex, dwPixel);
		}
	}
}

void CBitmap::_CopyFromOtherBmp(const CBitmap& _bmp)
{
	m_iWidth = _bmp.GetWidth();
	m_iHeight = _bmp.GetHeight();

	m_pData = new DWORD[m_iWidth * m_iHeight];
	memcpy(m_pData, _bmp.m_pData, sizeof(DWORD) * m_iWidth * m_iHeight);
}

CBitmap CBitmap::Normalize()
{
	CBitmap out = MakeGrayValue();

	int iMaxGray = 0;

	for(int i = 0; i < (m_iWidth * m_iHeight); i++)
	{
		int iGrayValue = out.m_pData[i] & 0xff;
		iMaxGray = max(iMaxGray, iGrayValue);
	}

	for(int i = 0; i < (m_iWidth * m_iHeight); i++)
	{
		int iGrayValue = out.m_pData[i] & 0xff;
		iGrayValue = iGrayValue * 255 / iMaxGray;
		out.m_pData[i] = EncodeColor(iGrayValue, iGrayValue, iGrayValue);
	}

	return out;
}

CBitmap CBitmap::MakeGrayValue()
{
	CBitmap out = *this;

	for(int i = 0; i < (m_iWidth * m_iHeight); i++)
	{
		int iRed, iGreen, iBlue;
		DecodeColor(m_pData[i], iRed, iGreen, iBlue);
		int iGrayValue = (iRed + iGreen + iBlue) / 3;
		out.m_pData[i] = EncodeColor(iGrayValue, iGrayValue, iGrayValue);
	}

	return out;
}

DWORD CBitmap::EncodeColor(int _iRed, int _iGreen, int _iBlue)
{
	int iClampedRed = min(255, max(0, _iRed));
	int iClampedGreen = min(255, max(0, _iGreen));
	int iClampedBlue = min(255, max(0, _iBlue));

	DWORD dwOut = (iClampedRed << 16) + (iClampedGreen << 8) + iClampedBlue;
	return dwOut;
}

void CBitmap::DecodeColor(DWORD _dwColor, int& _iRed, int& _iGreen, int& _iBlue)
{
	_iRed = (_dwColor >> 16) & 0xff;
	_iGreen = (_dwColor >> 8) & 0xff;
	_iBlue = _dwColor & 0xff;
}
