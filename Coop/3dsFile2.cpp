#include "3dsFile2.h"

#pragma warning(disable:4996)

#include <iostream>
#include <fstream>

using namespace std;

const long g_lChunkHeaderSize = sizeof(short) + sizeof(long);

#define EDIT3DS       0x3D3D
#define EDIT_MATERIAL 0xAFFF
#define EDIT_OBJECT   0x4000
#define OBJ_TRIMESH   0x4100
#define TRI_MAPPINGC  0x4140
#define TRI_VERTEXL   0x4110
#define TEXTURE1      0xA200
#define MATERIALNAME  0xA000
#define TEXTNAME      0xA300
#define TRI_FACEL1    0x4120

C3dsFile2::C3dsFile2()
{
	m_file = NULL;
}

C3dsFile2::~C3dsFile2()
{
}

bool C3dsFile2::Load(const std::string & _sFileName)
{
	m_vVertices.clear();
	m_vTriangles.clear();

	m_file = fopen(_sFileName.c_str(), "rb");

	short sId = 0;
	long lChunkSize = 0;
	_ReadChunckHeader(sId, lChunkSize);

	long lEdit3dsIndex = _FindSubChunk(EDIT3DS, lChunkSize);
	if(lEdit3dsIndex == -1)
	{
		return false;
	}

	fseek(m_file, lEdit3dsIndex, SEEK_SET);
	_ParseEdit3ds();

	fclose(m_file);
	m_file = NULL;

	return true;
}

long C3dsFile2::_FindSubChunk(unsigned short _sId, long _lMaxFileLocation)
{
	long lOrgFileIndex = ftell(m_file);
	long lFileIndex = lOrgFileIndex;

	while(lFileIndex < _lMaxFileLocation)
	{
		short sId = 0;
		long lSize = 0;

		if(!_ReadChunckHeader(sId, lSize))
		{
			return -1;
		}
		
		lFileIndex = ftell(m_file);
		if(sId == _sId)
		{
			lFileIndex -= sizeof(short) + sizeof(long);
			break;
		}

		long lSeekDistance = lSize - sizeof(short) - sizeof(long);
		fseek(m_file, lSeekDistance, SEEK_CUR);
	}

	if(lFileIndex >= _lMaxFileLocation)
	{
		lFileIndex = -1;
	}

	fseek(m_file, lOrgFileIndex, SEEK_SET);

	return lFileIndex;
}

bool C3dsFile2::_ReadChunckHeader(short & _sId, long & _lSize)
{
	size_t readCount = fread(&_sId, sizeof(_sId), 1, m_file);
	if(readCount != 1)
	{
		return false;
	}

	readCount = fread(&_lSize, sizeof(_lSize), 1, m_file);
	if(readCount != 1)
	{
		return false;
	}

	return true;
}

bool C3dsFile2::_ParseEdit3ds()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	long lChunkStartIndex = ftell(m_file);
	long lMaxFileLocation = ftell(m_file) + lSize;
	long lEditMaterialIndex = _FindSubChunk(EDIT_MATERIAL, lMaxFileLocation);
	if(lEditMaterialIndex != -1)
	{
		fseek(m_file, lEditMaterialIndex, SEEK_SET);
		if(!_ParseEditMaterial())
		{
			return false;
		}
	}

	fseek(m_file, lChunkStartIndex, SEEK_SET);

	long lEditObjectIndex = _FindSubChunk(EDIT_OBJECT, lMaxFileLocation);

	if(lEditObjectIndex == -1)
	{
		return false;
	}

	fseek(m_file, lEditObjectIndex, SEEK_SET);

	if(!_ParseEditObject())
	{
		return false;
	}

	return true;
}

bool C3dsFile2::_ParseEditObject()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	string name = _ReadString();

	long lMaxFileLocation = ftell(m_file) + lSize;
	long lTriMeshIndex = _FindSubChunk(OBJ_TRIMESH, lMaxFileLocation);
	if(lTriMeshIndex == -1)
	{
		return false;
	}

	fseek(m_file, lTriMeshIndex, SEEK_SET);

	if(!_ParseObjectTriMesh())
	{
		return false;
	}

	return true;
}

bool C3dsFile2::_ParseObjectTriMesh()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	long lStartIndex = ftell(m_file);

	long lMaxFileLocation = ftell(m_file) + lSize;
	long lMappingCoorsIndex = _FindSubChunk(TRI_MAPPINGC, lMaxFileLocation);
	if(lMappingCoorsIndex != -1)
	{
		fseek(m_file, lMappingCoorsIndex, SEEK_SET);

		if(!_ParseMappingCoordinates())
		{
			return false;
		}
	}

	fseek(m_file, lStartIndex, SEEK_SET);
	
	long lTriVertexIndex = _FindSubChunk(TRI_VERTEXL, lMaxFileLocation);
	if(lTriVertexIndex == -1)
	{
		return false;
	}

	fseek(m_file, lTriVertexIndex, SEEK_SET);
	if(!_ParseTriVertex())
	{
		return false;
	}

	long lTriFaceIndex = _FindSubChunk(TRI_FACEL1, lMaxFileLocation);
	if(lTriFaceIndex == -1)
	{
		return false;
	}

	fseek(m_file, lTriFaceIndex, SEEK_SET);
	if(!_ParseTriFace())
	{
		return false;
	}

	return true;
}

bool C3dsFile2::_ParseTriVertex()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	long lMaxFileLocation = ftell(m_file) + lSize;
	unsigned short sVertexCount = 0;

	fread(&sVertexCount, sizeof(sVertexCount), 1, m_file);
	
	int nVertices = (int)sVertexCount;
	for(int i = 0; i < nVertices; i++)
	{
		float fCoors[3];
		fread(fCoors, sizeof(float), 3, m_file);

		S3dsVertex2 vertex;
		vertex.m_fX = fCoors[0];
		vertex.m_fY = fCoors[1];
		vertex.m_fZ = fCoors[2];

		m_vVertices.push_back(vertex);
	}

	return true;
}

bool C3dsFile2::_ParseTriFace()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	unsigned short iTriangleCount = 0;
	fread(&iTriangleCount, sizeof(iTriangleCount), 1, m_file);

	int nTriangles = (int)iTriangleCount;
	for(int i = 0; i < nTriangles; i++)
	{
		unsigned short sVertexIndices[3];
		fread(sVertexIndices, sizeof(sVertexIndices), 1, m_file);

		S3dsTriangle2 triangle;
		triangle.m_iVertices[0] = (int)sVertexIndices[0];
		triangle.m_iVertices[1] = (int)sVertexIndices[1];
		triangle.m_iVertices[2] = (int)sVertexIndices[2];

		m_vTriangles.push_back(triangle);

		fseek(m_file, 2, SEEK_CUR);
	}

	return true;
}

bool C3dsFile2::_ParseEditMaterial()
{
	short sId = 0;
	long lSize = 0;
	string sFileName;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	long lStartFileIndex = ftell(m_file);

	long lMaxFileLocation = ftell(m_file) + lSize;
	long lTexture1Index = _FindSubChunk(TEXTURE1, lMaxFileLocation);

	fseek(m_file, lTexture1Index, SEEK_SET);

	if(!_ParseTexture1(sFileName))
	{
		return false;
	}

	fseek(m_file, lStartFileIndex, SEEK_SET);

	long lMaterialNameIndex = _FindSubChunk(MATERIALNAME, lMaxFileLocation);

	if(lMaterialNameIndex == -1)
	{
		return false;
	}

	fseek(m_file, lMaterialNameIndex + sizeof(short), SEEK_SET);
	long lMaterialStringSize = 0;

	fread(&lMaterialStringSize, sizeof(lMaterialStringSize), 1, m_file);

	char buffer[128];
	fread(buffer, sizeof(char), lMaterialStringSize, m_file);
	buffer[lMaterialStringSize] = '\0';

	S3dsMaterial2 material;
	material.m_sFileName = sFileName;
	material.m_sName = buffer;

	m_vMaterials.push_back(material);

	return true;
}

bool C3dsFile2::_ParseTexture1(string & _sFileName)
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	long lMaxFileLocation = ftell(m_file) + lSize;
	long lTextureFileName = _FindSubChunk(TEXTNAME, lMaxFileLocation);

	if(lTextureFileName == -1)
	{
		return false;
	}

	fseek(m_file, lTextureFileName + sizeof(short), SEEK_SET);
	long lTextureStringSize = 0;

	fread(&lTextureStringSize, sizeof(lTextureStringSize), 1, m_file);
	char buffer[128];
	fread(buffer, sizeof(char), lTextureStringSize, m_file);
	buffer[lTextureStringSize] = '\0';

	_sFileName = buffer;

	return true;
}

bool C3dsFile2::_ParseMappingCoordinates()
{
	short sId = 0;
	long lSize = 0;

	if(!_ReadChunckHeader(sId, lSize))
	{
		return false;
	}

	unsigned short sCoorCount = 0;
	fread(&sCoorCount, sizeof(short), 1, m_file);

	int nCoors = (int)sCoorCount;
	for(int i = 0; i < nCoors; i++)
	{
		S3dsTextureCoordinate2 textCoor;

		fread(&textCoor, sizeof(textCoor), 1, m_file);

		m_vTextureCoors.push_back(textCoor);
	}

	return true;
}

string C3dsFile2::_ReadString()
{
	const int nBufferSize = 128;
	char buffer[128];
	int iIndex = 0;

	char readChar = '\0';

	do
	{
		fread(&readChar, sizeof(char), 1, m_file);
		buffer[iIndex] = readChar;
		iIndex++;
	}
	while(readChar != '\0');

	return string(buffer);
}

void C3dsFile2::_PrintSubChunks()
{
	long lStartIndex = ftell(m_file);

	// Skip ID.
	fseek(m_file, 2, SEEK_CUR);
	
	long lChunkSize = 0;
	fread(&lChunkSize, sizeof(long), 1, m_file);

	long lMaxFileIndex = ftell(m_file) - 2 + lChunkSize;
	while(ftell(m_file) < lMaxFileIndex)
	{
		unsigned short sId = 0;
		unsigned long lSize = 0;

		fread(&sId, sizeof(sId), 1, m_file);
		fread(&lSize, sizeof(lSize), 1, m_file);

		long lNewIndex = ftell(m_file) - 6 + lSize;
		if(lNewIndex > lMaxFileIndex)
		{
			cout << "too far" << endl;
			break;
		}

		fseek(m_file, lNewIndex, SEEK_SET);
		
		char buffer[256];
		int iSize = (int)lSize;
//		sprintf(buffer, "0x%04x (%d): %s", sId, iSize, ChunkTypeToString(sId));

		cout << buffer << endl;
	}

	fseek(m_file, lStartIndex, SEEK_SET);
}

bool C3dsFile2::WriteToTextFile()
{
	const char * fileName = "3ds2out.txt";

	ofstream os(fileName);

	int nTriangles = m_vTriangles.size();

	for(int iTriangle = 0; iTriangle < nTriangles; iTriangle++)
	{
		const S3dsTriangle2 & triangle = m_vTriangles[iTriangle];

		os << "Triangle " << iTriangle << endl;
		
		for(int iVertex = 0; iVertex < 3; iVertex++)
		{
			int iRefVertex = triangle.m_iVertices[iVertex];

			S3dsVertex2 & vertex = m_vVertices[iRefVertex];
			S3dsTextureCoordinate2 & textureCoordinate = m_vTextureCoors[iRefVertex];

			os << "  Vertex " << iVertex << " (" << iRefVertex << "): (" << vertex.m_fX << ", " << vertex.m_fY << ", " << vertex.m_fZ << ") (" <<  textureCoordinate.m_fU << ", " << textureCoordinate.m_fV << ")" << endl;
		}

		os << endl;
	}

	return true;
}

/*
bool C3dsFile2::WriteToTextFile2()
{
	const char * fileName = "3ds2out2.txt";

	ofstream os(fileName);

	int nTriangles = GetTriangleCount();

	for(int iTriangle = 0; iTriangle < nTriangles; iTriangle++)
	{
		S3dsCompleteTrianlge2 triangle = GetTriangle(iTriangle);
		
		os << "Triangle " << iTriangle << endl;
		
		for(int iVertex = 0; iVertex < 3; iVertex++)
		{
			float fX = triangle.m_fPos[iVertex][0];
			float fY = triangle.m_fPos[iVertex][1];
			float fZ = triangle.m_fPos[iVertex][2];
			float fU = triangle.m_fTextCoor[iVertex][0];
			float fV = triangle.m_fTextCoor[iVertex][1];

			os << "  Vertex " << iVertex << " (" << -1 << "): (" << fX << ", " << fY << ", " << fZ << ") (" <<  fU << ", " << fV << ")" << endl;
		}

		os << endl;
	}

	return true;
}
*/

int C3dsFile2::GetTriangleCount() const
{
	return m_vTriangles.size();
}

S3dsCompleteTrianlge2 C3dsFile2::GetTriangle(int _iIndex) const
{
	const S3dsTriangle2 & internalTriangle = m_vTriangles[_iIndex];
	S3dsCompleteTrianlge2 outTriangle;

	for(int i = 0; i < 3; i++)
	{
		int iVertex = internalTriangle.m_iVertices[i];
		const S3dsVertex2 & vertex = m_vVertices[iVertex];
		const S3dsTextureCoordinate2 & textCoor = m_vTextureCoors[iVertex];

		outTriangle.m_fPos[i][0] = vertex.m_fX;
		outTriangle.m_fPos[i][1] = vertex.m_fY;
		outTriangle.m_fPos[i][2] = vertex.m_fZ;

		outTriangle.m_fTextCoor[i][0] = textCoor.m_fU;
		outTriangle.m_fTextCoor[i][1] = textCoor.m_fV;

		outTriangle.m_iTextureIndex = 0; // Always the first for now.
	}
	
	return outTriangle;
}

int C3dsFile2::GetBitmapCount() const
{
	return 1;
}

std::string C3dsFile2::GetBitmapFileName(int _iIndex) const
{
	return m_vMaterials[0].m_sFileName;
}
