#include "EshFile.h"

#include "Logger.h"
#include "GraphicsManager.h"
#include "VertexTypes.h"

#include <fstream>
#include <assert.h>
#include <sstream>

using namespace std;

const DWORD g_dwMagicNumber = 451982;

// TODO: load 3ds files.
struct CEshFile::STriangle
{
	D3DXVECTOR3 m_vCoors[3];
	D3DXVECTOR3 m_vNormal;
	D3DCOLORVALUE m_colors[3];
	D3DXVECTOR3 m_vTexCoors[3];
	int m_iBitmapIndex;
};

struct CEshFile::SObject
{
	std::string m_sName;
	int m_iFirstTriangleIndex;
	int m_iTriangleCount;
	bool m_bHasTextures;
};

CEshFile::CEshFile()
{
	m_pVertexBuffer = NULL;
	m_pTriangles = NULL;
}

CEshFile::~CEshFile()
{
	assert(m_pTriangles == NULL);
	assert(m_pVertexBuffer == NULL);
}

static bool ReadTextVector3(ifstream& _inFile, D3DXVECTOR3& _vec)
{
	float fValue = 0.0f;

	_inFile >> fValue;
	_vec.x = fValue;

	_inFile >> fValue;
	_vec.y = fValue;

	_inFile >> fValue;
	_vec.z = fValue;

	return true;
}

static bool ReadTextVector2(ifstream& _inFile, D3DXVECTOR3& _vec)
{
	float fValue = 0.0f;

	_inFile >> fValue;
	_vec.x = fValue;

	_inFile >> fValue;
	_vec.y = fValue;

	_vec.z = 0.0f;

	return true;
}

static bool ReadTextColor(ifstream& _inFile, D3DCOLORVALUE& _vec)
{
	float fValue = 0.0f;

	_inFile >> fValue;
	_vec.r = fValue / 255.0f;
	_vec.r = min(_vec.r, 1.0f);
	_vec.r = max(_vec.r, 0.0f);

	_inFile >> fValue;
	_vec.g = fValue / 255.0f;
	_vec.g = min(_vec.g, 1.0f);
	_vec.g = max(_vec.g, 0.0f);

	_inFile >> fValue;
	_vec.b = fValue / 255.0f;
	_vec.b = min(_vec.b, 1.0f);
	_vec.b = max(_vec.b, 0.0f);

	_vec.a = 1.0f;

	return true;
}

bool CEshFile::_LoadTextFile(std::string _fileName)
{
	char buffer[128];
	ZeroMemory(buffer, sizeof(buffer));

	ifstream inFile(_fileName.c_str());
	
	inFile >> buffer;
	
	int iVersionNumber = 0;
	inFile >> iVersionNumber;

	int iObjectCount = 0;
	inFile >> iObjectCount;

	int iTriangleCount = 0;
	inFile >> iTriangleCount;


	m_pTriangles = new STriangle[iTriangleCount];
	if(m_pTriangles == NULL)
	{
		LogError("Failed to allocate triangles when loading esh file");
		return false;
	}

	ZeroMemory(m_pTriangles, sizeof(STriangle) * iTriangleCount);

	int iCurrentTriangleIndex = 0;

	for(int iObjectIndex = 0; iObjectIndex < iObjectCount; iObjectIndex++)
	{
		SObject object;

		inFile >> buffer;
		object.m_sName = buffer;

		int iObjectTriangleCount = 0;
		inFile >> iObjectTriangleCount;

		int iObjectHasTextures = 0;
		inFile >> iObjectHasTextures;

		object.m_iFirstTriangleIndex = iCurrentTriangleIndex;
		object.m_iTriangleCount = iObjectTriangleCount;
		object.m_bHasTextures = iObjectHasTextures == 1;

		for(int iTriangleIndex = 0; iTriangleIndex < iObjectTriangleCount; iTriangleIndex++)
		{
			STriangle& triangleInfo = m_pTriangles[iCurrentTriangleIndex];

			// Read coordinates.
			ReadTextVector3(inFile, triangleInfo.m_vCoors[0]);
			ReadTextVector3(inFile, triangleInfo.m_vCoors[1]);
			ReadTextVector3(inFile, triangleInfo.m_vCoors[2]);

			// Read normal.
			ReadTextVector3(inFile, triangleInfo.m_vNormal);

			// Read colors.
			ReadTextColor(inFile, triangleInfo.m_colors[0]);
			ReadTextColor(inFile, triangleInfo.m_colors[1]);
			ReadTextColor(inFile, triangleInfo.m_colors[2]);

			// Read texture coordinates.
			ReadTextVector2(inFile, triangleInfo.m_vTexCoors[0]);
			ReadTextVector2(inFile, triangleInfo.m_vTexCoors[1]);
			ReadTextVector2(inFile, triangleInfo.m_vTexCoors[2]);

			int iBitmapIndex = 0;
			inFile >> iBitmapIndex;

			iCurrentTriangleIndex++;
		}

		m_vObjects.push_back(object);
	}

	return true;
}

bool CEshFile::_LoadBinFile(std::string _fileName)
{
	FILE *pFile = NULL;
	fopen_s(&pFile, _fileName.c_str(), "rb");

	DWORD dwHeader[2];
	
	size_t readElementCount = 0;

	readElementCount = fread(dwHeader, sizeof(DWORD), 2, pFile);
	if(readElementCount != 2)
	{
		LogError("Failed to read first two DWORDs of Esh file");
		return false;
	}

	int iObjectCount, iTriangleCount;
	readElementCount = fread(&iObjectCount, sizeof(int), 1, pFile);
	if(readElementCount != 1)
	{
		LogError("Failed to read object count");
		return false;
	}

	readElementCount = fread(&iTriangleCount, sizeof(int), 1, pFile);
	if(readElementCount != 1)
	{
		LogError("Failed to read triangle count");
		return false;
	}

	int iCurrentTriangleIndex = 0;

	for(int iObjectIndex = 0; iObjectIndex < iObjectCount; iObjectIndex++)
	{
		SObject addedObject;

		int iNameLength;

		readElementCount = fread(&iNameLength, sizeof(int), 1, pFile);
		if(readElementCount != 1)
		{
			LogError("Failed to read object name length");
			return false;
		}

		char *pNameBuffer = new char[iNameLength + 1];

		readElementCount = fread(pNameBuffer, sizeof(char), iNameLength, pFile);
		if(readElementCount != iNameLength)
		{
			LogError("Failed to read object name");
			return false;
		}

		pNameBuffer[iNameLength] = '\0';

		addedObject.m_sName = string(pNameBuffer);
		delete [] pNameBuffer;

		readElementCount = fread(&addedObject.m_iTriangleCount, sizeof(int), 1, pFile);
		if(readElementCount != 1)
		{
			LogError("Failed to read object triangle count");
			return false;
		}

		addedObject.m_iFirstTriangleIndex = iCurrentTriangleIndex;
		iCurrentTriangleIndex++;

		int iObjectHasTextures;
		readElementCount = fread(&iObjectHasTextures, sizeof(int), 1, pFile);
		if(readElementCount != 1)
		{
			LogError("Failed to read if object has textures");
			return false;
		}

		addedObject.m_bHasTextures = iObjectHasTextures != 0;

		m_vObjects.push_back(addedObject);
	}

	m_pTriangles = new STriangle[iTriangleCount];
	readElementCount = fread(m_pTriangles, sizeof(STriangle), iTriangleCount, pFile);
	if(readElementCount != iTriangleCount)
	{
		LogError("Failed to read triangle data");
		return false;
	}

	fclose(pFile);

	return true;
}
	
bool CEshFile::Load(string _fileName)
{
	FILE *pFile = NULL;
	fopen_s(&pFile, _fileName.c_str(), "rb");

	if(pFile == NULL)
	{
		LogError("Failed to open esh file.");
		return false;
	}

	m_vBitmaps.clear();
	m_vObjects.clear();

	if(m_pTriangles != NULL)
	{
		delete [] m_pTriangles;
	}

	char buffer[128];
	const DWORD * pdwBuffer = (const DWORD *)buffer;
	ZeroMemory(buffer, sizeof(buffer));

	int iReadByteCount = fread(buffer, sizeof(char), 8, pFile);
	if(iReadByteCount != 8)
	{
		LogError("Esh file is too small");
		return false;
	}

	fclose(pFile);

	bool bLoadedFile = false;

	if(strcmp("eshfile ", buffer) == 0)
	{
		if(!_LoadTextFile(_fileName))
		{
			LogError("Esh file seemed a text file, but unable to read it");
			return false;
		}

		bLoadedFile = true;
	} else if(pdwBuffer[0] == g_dwMagicNumber && pdwBuffer[1] == g_dwMagicNumber)
	{
		if(!_LoadBinFile(_fileName))
		{
			LogError("Esh file seemed a binary file, but unable to read it");
			return false;
		}

		bLoadedFile = true;
	}

	return bLoadedFile;
}

bool CEshFile::Save(string _fileName) const
{
	FILE *pOutFile = NULL;
	size_t writtenElementCount;
	vector<SObject>::const_iterator objectIterator;

	fopen_s(&pOutFile, _fileName.c_str(), "wb");
	if(pOutFile == NULL)
	{
		LogError("Failed to create output esh file");
		return false;
	}

	DWORD dwHeader[2] = {g_dwMagicNumber, g_dwMagicNumber};
	writtenElementCount = fwrite(dwHeader, sizeof(DWORD), 2, pOutFile);
	if(writtenElementCount != 2)
	{
		LogError("Failed to write magic numbers");
		return false;
	}

	int iObjectCount = (int)m_vObjects.size();
	writtenElementCount = fwrite(&iObjectCount, sizeof(int), 1, pOutFile);
	if(writtenElementCount != 1)
	{
		LogError("Failed to write object count");
		return false;
	}

	int iTriangleCount = 0;

	if(!m_vObjects.empty())
	{
		const SObject& lastObject = m_vObjects.back();
		iTriangleCount = lastObject.m_iFirstTriangleIndex + lastObject.m_iTriangleCount;
	}

	writtenElementCount = fwrite(&iTriangleCount, sizeof(int), 1, pOutFile);
	if(writtenElementCount != 1)
	{
		LogError("Failed to write triangle count");
		return false;
	}

	// Write objects.
	for(objectIterator = m_vObjects.begin(); objectIterator != m_vObjects.end(); objectIterator++)
	{
		const SObject& object = *objectIterator;

		int iNameLength = object.m_sName.length();
		writtenElementCount = fwrite(&iNameLength, sizeof(int), 1, pOutFile);
		if(writtenElementCount != 1)
		{
			LogError("Failed to write object name length");
			return false;
		}

		writtenElementCount = fwrite(object.m_sName.c_str(), sizeof(char), iNameLength, pOutFile);	
		if(writtenElementCount != iNameLength)
		{
			LogError("Failed to write object name string");
			return false;
		}
		
		writtenElementCount = fwrite(&object.m_iTriangleCount, sizeof(int), 1, pOutFile);	
		if(writtenElementCount != 1)
		{
			LogError("Failed to write object triangle count");
			return false;
		}
		
		int iHasTextures = object.m_bHasTextures ? 1 : 0;
		writtenElementCount = fwrite(&iHasTextures, sizeof(int), 1, pOutFile);	
		if(writtenElementCount != 1)
		{
			LogError("Failed to write if object has textures");
			return false;
		}
	}

	// Write triangles.
	writtenElementCount = fwrite(m_pTriangles, sizeof(STriangle), iTriangleCount, pOutFile);	
	if(writtenElementCount != iTriangleCount)
	{
		LogError("Failed to write triangles to binary esh file");
		return false;
	}

	fclose(pOutFile);

	return true;
}

bool CEshFile::CreateVertexBuffer()
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pGraphicsManager->GetDevice();

	SObject& object = m_vObjects[0];
	UINT iVertexCount = object.m_iTriangleCount * 3;

	hr = pDevice->CreateVertexBuffer(iVertexCount * sizeof(SNormalVertex), 0, SNormalVertex::_fvf, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create vertex buffer for esh object", hr);
		return false;
	}

	SNormalVertex *pLockedMem = NULL;
	hr = m_pVertexBuffer->Lock(0, sizeof(SNormalVertex) * iVertexCount, (void **)&pLockedMem, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock esh vertex buffer", hr);
		return false;
	}

	ZeroMemory(pLockedMem, sizeof(SNormalVertex) * iVertexCount);

	for(int iTriangleIndex = 0; iTriangleIndex < object.m_iTriangleCount; iTriangleIndex++)
	{
		for(int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
		{
			int iTargetVertexIndex = iTriangleIndex * 3 + iVertexIndex;
			int iSourceTriangleIndex = object.m_iFirstTriangleIndex + iTriangleIndex;

			pLockedMem[iTargetVertexIndex].x = m_pTriangles[iSourceTriangleIndex].m_vCoors[iVertexIndex].x;
			pLockedMem[iTargetVertexIndex].y = m_pTriangles[iSourceTriangleIndex].m_vCoors[iVertexIndex].y;
			pLockedMem[iTargetVertexIndex].z = m_pTriangles[iSourceTriangleIndex].m_vCoors[iVertexIndex].z;

			pLockedMem[iTargetVertexIndex].nx = m_pTriangles[iSourceTriangleIndex].m_vNormal.x;
			pLockedMem[iTargetVertexIndex].ny = m_pTriangles[iSourceTriangleIndex].m_vNormal.y;
			pLockedMem[iTargetVertexIndex].nz = m_pTriangles[iSourceTriangleIndex].m_vNormal.z;
		}
	}

	hr = m_pVertexBuffer->Unlock();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock esh vertex buffer", hr);
		return false;
	}

	return true;
}

bool CEshFile::Render()
{
	HRESULT hr;
	IDirect3DDevice9 * pDevice = CGraphicsManager::GetInstance()->GetDevice();

	hr = pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SNormalVertex));
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set stream source to esh", hr);
		return false;
	}

	hr = pDevice->SetFVF(SNormalVertex::_fvf);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set FVF for esh", hr);
		return false;
	}

	SObject& object = m_vObjects[0];
	UINT iVertexCount = object.m_iTriangleCount * 3;

	hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, object.m_iTriangleCount);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to draw esh primitive", hr);
		return false;
	}

	return true;
}

bool CEshFile::CleanUp()
{
	if(m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	if(m_pTriangles != NULL)
	{
		delete [] m_pTriangles;
		m_pTriangles = NULL;
	}

	return true;
}
