#pragma once

#include <d3dx9.h>
#include <string>

class CMatrixUtils
{
	static void _WriteMatrixToFile(std::string _prefix, std::string _matrixName, const D3DMATRIX & _matrix,
		std::ofstream & _outFile);

public:
	static D3DXMATRIX FillIsoMatrix();
	bool WriteMatricesToFile(std::string _fileName, std::string _prefix);
};
