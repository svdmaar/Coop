#include "Font.h"

#include "Logger.h"
#include "VertexTypes.h"
#include "GraphicsManager.h"

#include <assert.h>
#include <fstream>

const DWORD g_baseLinePixel = 0x241ced;

using namespace std;

CFont::CFont()
{
	ZeroMemory(m_iCharWidth, sizeof(m_iCharWidth));
	ZeroMemory(m_iCharColumn, sizeof(m_iCharColumn));

	m_iHeight = 0;
	m_iNormalDistance = 0;
	m_iSpaceDistance = 0;
	m_bInited = false;
}

CFont::~CFont()
{
	assert(!m_bInited);
}

// TODO: make space a character
// TODO: steal font from Windows.
bool CFont::Init(const string& _name, const string& _fileName, IDirect3DDevice9 *_pDev)
{
	HRESULT hr;

	assert(!m_bInited);

	if(!m_bmp.Load(_fileName))
	{
		LogError("Failed to open bmp for font");
		return false;
	}

	if(_name == "arial")
	{
		_FillForArial();
	}
	else
	{
		LogError("Unexpected font name");
		return false;
	}

	hr = _pDev->CreateTexture(m_bmp.GetWidth(), m_bmp.GetHeight(), 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &m_pSystemTexture, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create font system texture", hr);
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	memset(&lockedRect, 0, sizeof(lockedRect));

	hr = m_pSystemTexture->LockRect(0, &lockedRect, NULL, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock system texture rect", hr);
		return false;
	}

	m_bmp.Save("copied_to_text.bmp");

	DWORD *pTexLocation = (DWORD *)lockedRect.pBits;
	for(int iRowIndex = 0; iRowIndex < m_bmp.GetHeight(); iRowIndex++)
	{
		for(int iColumnIndex = 0; iColumnIndex < m_bmp.GetHeight(); iColumnIndex++)
		{
			DWORD dwPixel = m_bmp.GetPixel(iRowIndex, iColumnIndex);
			
			if(dwPixel != 0)
			{
				// set alpha
				dwPixel |= 0xff000000l;
			}

			pTexLocation[iColumnIndex] = dwPixel;
		}


		BYTE *pTextureByteLocation = (BYTE *)pTexLocation;
		pTextureByteLocation += lockedRect.Pitch;
		
		pTexLocation = (DWORD *)pTextureByteLocation;
	}

	hr = m_pSystemTexture->UnlockRect(0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock system texture rect", hr);
		return false;
	}

	hr = _pDev->CreateTexture(m_bmp.GetWidth(), m_bmp.GetHeight(), 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pVideoTexture, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create font video texture", hr);
		return false;
	}

	hr = _pDev->UpdateTexture(m_pSystemTexture, m_pVideoTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to update font video texture", hr);
		return false;
	}

	UINT bufferSize = sizeof(SFaceVertex) * 6 * g_iCharCount;
	hr = _pDev->CreateVertexBuffer(bufferSize, 0, SFaceVertex::_fvf, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create font vertex buffer", hr);
		return false;
	}

	SFaceVertex *pVertices = NULL;
	hr = m_pVertexBuffer->Lock(0, bufferSize, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock font vertex buffer", hr);
		return false;
	}

	// Fill vertex buffer.
	for(int iCharIndex = 0; iCharIndex < g_iCharCount; iCharIndex++)
	{
		int iTextureWidth = m_bmp.GetWidth();
		int iTextureHeight = m_bmp.GetHeight();
		float fTextureWidth = (float)iTextureWidth;
		float fTextureHeight = (float)iTextureHeight;

		int iCharRow = m_iCharRow[iCharIndex];
		int iCharColumn = m_iCharColumn[iCharIndex];
		int iCharUpperRow = iCharRow * m_iHeight;
		int iCharLeftColumn = iCharColumn;
		int iCharLowerRow = iCharUpperRow + m_iHeight;
		int iCharRightColumn = iCharLeftColumn + m_iCharWidth[iCharIndex];

		int iStartBufferIndex = iCharIndex * 6;
		float fLeftU = ((float)iCharLeftColumn) / fTextureWidth;
		float fRightU = ((float)iCharRightColumn) / fTextureWidth;
		float fTopV = ((float)iCharUpperRow) / fTextureHeight;
		float fLowerV = ((float)iCharLowerRow) / fTextureHeight;

		float fWidth = ((float)m_iCharWidth[iCharIndex]) / ((float)m_iMaxWidth);

		SFaceVertex baseCorners[4];
		memset(baseCorners, 0, sizeof(baseCorners));

		// lower left
		baseCorners[0].tu = fLeftU;
		baseCorners[0].tv = fLowerV;

		// upper left
		baseCorners[1].y = 1.0f;
		baseCorners[1].tu = fLeftU;
		baseCorners[1].tv = fTopV;

		// upper right
		baseCorners[2].x = fWidth;
		baseCorners[2].y = 1.0f;
		baseCorners[2].tu = fRightU;
		baseCorners[2].tv = fTopV;

		// lower right
		baseCorners[3].x = fWidth;
		baseCorners[3].tu = fRightU;
		baseCorners[3].tv = fLowerV;

		// Copy four corners to two triangles.
		pVertices[iStartBufferIndex + 0] = baseCorners[0];
		pVertices[iStartBufferIndex + 1] = baseCorners[1];
		pVertices[iStartBufferIndex + 2] = baseCorners[2];
		pVertices[iStartBufferIndex + 3] = baseCorners[2];
		pVertices[iStartBufferIndex + 4] = baseCorners[3];
		pVertices[iStartBufferIndex + 5] = baseCorners[0];
	}

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock font vertex buffer", hr);
		return false;
	}

	m_bInited = true;

	return true;
}

bool CFont::RenderString(const std::string& _text, float _fHeight) const
{
	// TODO: _fHeight should be unnecessary, and be set up using world matrix.

	HRESULT hr;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	D3DXMATRIX orgWorldTransform, newWorldTransform, translateMatrix;
	float fTotalDistance = 0.0f;

	hr = pDevice->GetTransform(D3DTS_WORLD, &orgWorldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get world transform", hr);
		return false;
	}

	for(int iIndex = 0; iIndex < (int)_text.length(); iIndex++)
	{
		int iCharIndex = (int)('?' - '!');
		char c = _text[iIndex];

		if((c >= '!') && (c <= '~'))
		{
			iCharIndex = (int)(c - '!');
		}

		if(c != ' ')
		{
			RenderChar(c);
			fTotalDistance += ((float)m_iCharWidth[iCharIndex]) / ((float)m_iMaxWidth);
		}
		else
		{
			fTotalDistance += ((float)m_iSpaceDistance) / ((float)m_iMaxWidth);
		}

		fTotalDistance += ((float)m_iNormalDistance) / ((float)m_iMaxWidth);
		
		D3DXMatrixTranslation(&translateMatrix, fTotalDistance * _fHeight, 0.0f, 0.0f);
		D3DXMatrixMultiply(&newWorldTransform, &orgWorldTransform, &translateMatrix);

		hr = pDevice->SetTransform(D3DTS_WORLD, &newWorldTransform);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to set new world transform", hr);
			return false;
		}
	}

	hr = pDevice->SetTransform(D3DTS_WORLD, &orgWorldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to restore world transform", hr);
		return false;
	}

	return true;
}

bool CFont::RenderChar(char _c) const
{
	const int iCornerCount = 6;
	HRESULT hr;

	int iCharIndex = (int)('?' - '!');

	if((_c >= '!') && (_c <= '~'))
	{
		iCharIndex = (int)(_c - '!');
	}

	int iStartVertexIndex = iCornerCount * iCharIndex;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDev = pGraphicsManager->GetDevice();

	hr = pDev->SetTexture(0, m_pVideoTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set texture for font", hr);
		return false;
	}

	hr = pDev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SFaceVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set font stream source", hr);
		return false;
	}

	hr = pDev->SetFVF(SFaceVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for font", hr);
		return false;
	}

	hr = pDev->DrawPrimitive(D3DPT_TRIANGLELIST, iStartVertexIndex, 2);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw primitive for font", hr);
		return false;
	}

	hr = pDev->SetTexture(0, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set texture for font", hr);
		return false;
	}

	return true;
}

float CFont::CalcStringWidth(const string& _text, float _fHeight) const
{
	float fTotalWidth = 0.0f;

	for(int iIndex = 0; iIndex < (int)_text.length(); iIndex++)
	{
		char c = _text[iIndex];

		if(c == ' ')
		{
			fTotalWidth += m_iSpaceDistance;
			continue;
		}

		int iCharIndex = (int)('?' - '!');

		if((c >= '!') && (c <= '~'))
		{
			iCharIndex = (int)(c - '!');
		}

		fTotalWidth += m_iCharWidth[iCharIndex];
	}

	if(!_text.empty())
	{
		fTotalWidth += (float)((_text.length() - 1) * m_iNormalDistance);
	}

	float fFontHeight = (float)m_iHeight;
	return fTotalWidth * _fHeight / fFontHeight;
}

bool CFont::RenderCentered(const string& _text, float _fX, float _fY, float _fHeight) const
{
	HRESULT hr;

	float fWidth = CalcStringWidth(_text, _fHeight);

	float fLeft = _fX - 0.5f * fWidth;
	float fBottom = _fY - 0.5f * _fHeight;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	D3DXMATRIX orgWorldTransform, newWorldTransform, scalingMatrix, translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, fLeft, fBottom, 0.0f);
	D3DXMatrixScaling(&scalingMatrix, _fHeight, _fHeight, 0.0f);

	D3DXMatrixMultiply(&newWorldTransform, &scalingMatrix, &translationMatrix);
	//D3DXMatrixMultiply(&newWorldTransform, &translationMatrix, &scalingMatrix);

	hr = pDevice->GetTransform(D3DTS_WORLD, &orgWorldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get new world transform", hr);
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_WORLD, &newWorldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set new world transform", hr);
		return false;
	}

	if(!RenderString(_text, _fHeight))
	{
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_WORLD, &orgWorldTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set new world transform", hr);
		return false;
	}

	return true;
}

bool CFont::CleanUp()
{
	m_bInited = false;

	return true;
}

CBitmap CFont::_GetImportFontChar(const CBitmap& _inBmp, int _iCharRowIndex, int _iCharColumnIndex)
{
	int iSubRectSize = _inBmp.GetWidth() / g_iCharPerRowCount;
	RECT subRect;

	subRect.left = _iCharColumnIndex * iSubRectSize;
	subRect.top = _iCharRowIndex * iSubRectSize;
	subRect.right = subRect.left + iSubRectSize - 1;
	subRect.bottom = subRect.top + iSubRectSize - 1;

	return _inBmp.GetSubBitmap(subRect);
}

int CFont::_GetImportCharBaseRow(const CBitmap& _inBmp, int _iCharRowIndex)
{
	CBitmap firstBitmap = _GetImportFontChar(_inBmp, _iCharRowIndex, 0);

	int iRowIndex = 0, iColumnIndex = 0;
	bool bFoundRedPixel = firstBitmap.GetFirstPixel(g_baseLinePixel, iRowIndex, iColumnIndex);

	assert(bFoundRedPixel);

	return iRowIndex;
}

CBitmap CFont::_GetImportFontChar(const CBitmap& _inBmp, int _iCharIndex)
{
	int iCharRowIndex = _iCharIndex / g_iCharPerRowCount;
	int iCharColumnIndex = _iCharIndex % g_iCharPerRowCount;

	return _GetImportFontChar(_inBmp, iCharRowIndex, iCharColumnIndex);
}

void CFont::_CalcTextLayout(int _iCharHeight, const int *_pCharWidths, int *_pCharRow, int *_pColumn, int& _iTextSize)
{
	int iTotalWidth = 0;
	int iIndex = 0;

	for(iIndex = 0; iIndex < g_iCharCount; iIndex++)
	{
		iTotalWidth += _pCharWidths[iIndex];
	}
	
	int iTotalArea = iTotalWidth * _iCharHeight;

	// find power of two large enough to hold chars (in theory)
	_iTextSize = 1;

	while(true)
	{
		int iArea = _iTextSize * _iTextSize;
		if(iArea >= iTotalArea)
			break;

		_iTextSize *= 2;
	}

	int iColumnIndex = 0;
	int iCharRowIndex = 0;

	// Try to fit all chars on texture, double size if too small.
	while(true)
	{
		for(iIndex = 0; iIndex < g_iCharCount; iIndex++)
		{
			int iPossNextColumn = iColumnIndex + _pCharWidths[iIndex];

			if(iPossNextColumn > _iTextSize)
			{
				iCharRowIndex++;
				iColumnIndex = 0;
			}

			_pCharRow[iIndex] = iCharRowIndex;
			_pColumn[iIndex] = iColumnIndex;
			iColumnIndex += _pCharWidths[iIndex];
		}

		if(((iCharRowIndex + 1) * _iCharHeight) <= _iTextSize)
		{
			break;
		}

		_iTextSize *= 2;
	}
}

void CFont::ImportFont(const string _inBmpFile, const string _outBmpFile, const string _outTextFile)
{
	CBitmap inBmp(_inBmpFile);
	int iSubRectSize = inBmp.GetWidth() / g_iCharPerRowCount;
	int iMaxUpOffset = 0, iMaxDownOffset = 0, iMaxWidth = 0;
	int iBaseRow = 0;
	int iCharWidth[g_iCharCount];
	int iTargetCharRow[g_iCharCount];
	int iTargetColumnIndex[g_iCharCount];

	// Pass 1: get max up and down offsets, and width.
	for(int iCharIndex = 0; iCharIndex < g_iCharCount; iCharIndex++)
	{
		if((iCharIndex % g_iCharPerRowCount) == 0)
		{
			iBaseRow = _GetImportCharBaseRow(inBmp, iCharIndex / g_iCharPerRowCount);
		}

		CBitmap charBitmap = _GetImportFontChar(inBmp, iCharIndex);

		charBitmap.Replace(g_baseLinePixel, 0);

		RECT trimRect = charBitmap.GetTrimRect(0);

		int iUpOffset = iBaseRow - trimRect.top;
		int iDownOffset = trimRect.bottom - iBaseRow;
		int iWidth = trimRect.right - trimRect.left;

		iMaxUpOffset = max(iMaxUpOffset, iUpOffset);
		iMaxDownOffset = max(iMaxDownOffset, iDownOffset);
		iMaxWidth = max(iMaxWidth, iWidth);

		iCharWidth[iCharIndex] = iWidth;
	}

	int iTotalCharHeight = iMaxUpOffset + iMaxDownOffset + 1;

	int iTextSize = 0;
	_CalcTextLayout(iTotalCharHeight, iCharWidth, iTargetCharRow, iTargetColumnIndex, iTextSize);

	CBitmap outBmp(iTextSize, iTextSize, 0);
	int iCharRowIndex = -1;

	ofstream textOut(_outTextFile);

	textOut << "\tm_iHeight = " << iTotalCharHeight << ";" << endl;
	textOut << endl;

	// Pass 2: copy chars to new bitmap and write data to text file.
	for(int iCharIndex = 0; iCharIndex < g_iCharCount; iCharIndex++)
	{
		if((iCharIndex % g_iCharPerRowCount) == 0)
		{
			iBaseRow = _GetImportCharBaseRow(inBmp, iCharIndex / g_iCharPerRowCount);
		}

		CBitmap charBitmap = _GetImportFontChar(inBmp, iCharIndex);

		charBitmap.Replace(g_baseLinePixel, 0);

		RECT trimRect = charBitmap.GetTrimRect(0);

		int iUpOffset = iBaseRow - trimRect.top;

		charBitmap = charBitmap.GetSubBitmap(trimRect);
		charBitmap = charBitmap.Normalize();
		
		int iTargetRowIndex = iTotalCharHeight * iTargetCharRow[iCharIndex];
		iTargetRowIndex += iMaxUpOffset - iUpOffset;

		int iColumnIndex = iTargetColumnIndex[iCharIndex];
		outBmp.InsertBitmap(iTargetRowIndex, iColumnIndex, charBitmap);
		
		textOut << "\tm_iCharColumn[" << iCharIndex << "] = " << iColumnIndex << ";" << endl;
		textOut << "\tm_iCharWidth[" << iCharIndex << "] = " << charBitmap.GetWidth() << ";" << endl;
		textOut << "\tm_iCharRow[" << iCharIndex << "] = " << iTargetCharRow[iCharIndex] << ";" << endl;

		iColumnIndex += trimRect.right - trimRect.left;
	}

	outBmp.Save(_outBmpFile);

	textOut.close();
}

int CFont::CalcStringWidth(const string& _text)
{
	int iTotalWidth = 0;

	for(int i = 0; i < (int)_text.length(); i++)
	{
		if(i != 0)
		{
			iTotalWidth += m_iNormalDistance;
		}

		char c = _text[i];

		if(c == ' ')
		{
			iTotalWidth += m_iSpaceDistance;
		}
		else if(('!' <= c) &&(c <= '~'))
		{
			int iArrayIndex = (int)(c - '!');
			iTotalWidth += m_iCharWidth[iArrayIndex];
		}
		else
		{
			// if inrecognized char, print '?'
			int iArrayIndex = (int)('?' - '!');
			iTotalWidth += m_iCharWidth[iArrayIndex];
		}
	}

	return iTotalWidth;
}

CBitmap CFont::PrintStringToBitmap(const string& _text)
{
	int iWidth = CalcStringWidth(_text);

	CBitmap out(iWidth, m_iHeight, 0);

	int iColumnIndex = 0;

	for(int i = 0; i < (int)_text.length(); i++)
	{
		if(i != 0)
		{
			iColumnIndex += m_iNormalDistance;
		}

		char c = _text[i];

		if(c == ' ')
		{
			iColumnIndex += m_iSpaceDistance;
		}
		else
		{
			int iArrayIndex = (int)('?' - '!');
				
			if(('!' <= c) &&(c <= '~'))
			{
				iArrayIndex = (int)(c - '!');
			}

			RECT rect;
			rect.left = m_iCharColumn[iArrayIndex];
			rect.top = m_iCharRow[iArrayIndex] * m_iHeight;
			rect.right = rect.left + m_iCharWidth[iArrayIndex];
			rect.bottom = rect.top + m_iHeight;

			CBitmap charBitmap = m_bmp.GetSubBitmap(rect);

			out.InsertBitmap(0, iColumnIndex, charBitmap);

			iColumnIndex += m_iCharWidth[iArrayIndex];
		}
	}

	return out;
}
