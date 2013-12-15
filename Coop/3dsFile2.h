#pragma once

#include <string>
#include <vector>
#include <cstdio>

struct S3dsVertex2
{
	float m_fX;
	float m_fY;
	float m_fZ;
};

struct S3dsTextureCoordinate2
{
	float m_fU;
	float m_fV;
};

struct S3dsTriangle2
{
	int m_iVertices[3];
};

struct S3dsChunkDesc2
{
	long m_lIndex;
	short m_sId;
	long m_lSize;
};

struct S3dsMaterial2
{
	std::string m_sName;
	std::string m_sFileName;
};

struct S3dsCompleteTrianlge2
{
	float m_fPos[3][3];
	float m_fTextCoor[3][2];
	int m_iTextureIndex;
};

class C3dsFile2
{
	std::vector<S3dsVertex2> m_vVertices;
	std::vector<S3dsTriangle2> m_vTriangles;
	std::vector<S3dsMaterial2> m_vMaterials;
	std::vector<S3dsTextureCoordinate2> m_vTextureCoors;
	FILE * m_file;

	long _FindSubChunk(unsigned short _sId, long _lMaxFileLocation);
	bool _ReadChunckHeader(short & _sId, long & _lSize);

	bool _ParseEditMaterial();
	bool _ParseTexture1(std::string & _sFileName);
	bool _ParseMappingCoordinates();

	bool _ParseEdit3ds();
	bool _ParseEditObject();
	bool _ParseObjectTriMesh();
	bool _ParseTriVertex();
	bool _ParseTriFace();

	std::string _ReadString();

	void _PrintSubChunks();

public:
	C3dsFile2();
	~C3dsFile2();

	bool Load(const std::string & _sFileName);

	int GetTriangleCount() const;
	S3dsCompleteTrianlge2 GetTriangle(int _iIndex) const;
	int GetBitmapCount() const;
	std::string GetBitmapFileName(int _iIndex) const;

	// debug
	bool WriteToTextFile();
};
