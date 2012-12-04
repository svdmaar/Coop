#pragma once

#include <d3dx9.h>
#include <string>

class CMatrixUtils
{

public:
	static D3DXMATRIX FillIsoMatrix();
	bool WriteMatricesToFile(std::string _fileName, std::string _prefix);
};
