#include "MatrixUtils.h"

#include "GraphicsManager.h"
#include "Logger.h"

using namespace std;

D3DXMATRIX CMatrixUtils::FillIsoMatrix()
{
	D3DXMATRIX out;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	int iWindowWidth = pGraphicsManager->GetWindowWidth();
	int iWindowHeight = pGraphicsManager->GetWindowHeight();

	D3DXMatrixIdentity(&out);

	float fAspectRation = ((float)iWindowWidth) / (float)iWindowHeight;
	float fSinTheta = sinf(D3DX_PI / 6.0f);
	float fCosTheta = cosf(D3DX_PI / 6.0f);
	float fS = 0.1f;

	// x unit vector
	float Txx = fCosTheta * fS / fAspectRation;
	float Txy = -fSinTheta * fS;
	float Txz = 0.0f;

	out._11 = Txx;
	out._12 = Txy;
	out._13 = Txz;
	
	// y unit vector
	float Tyx = -fCosTheta * fS / fAspectRation;
	float Tyy = -fSinTheta * fS;
	float Tyz = 0.0f;
	
	out._21 = Tyx;
	out._22 = Tyy;
	out._23 = Tyz;

	// z unit vector
	float Tzx = 0;
	float Tzy = fS;
	float Tzz = 0.0f;

	out._31 = Tzx;
	out._32 = Tzy;
	out._33 = Tzz;
	
	// zero vector (center of screen)
	float Twx = 0;
	float Twy = 0;
	float Twz = 0;

	out._41 = Twx;
	out._42 = Twy;
	out._43 = Twz;

	return out;
}

void CMatrixUtils::_WriteMatrixToFile(string _prefix, string _matrixName, const D3DMATRIX & _matrix,
	ofstream & _outFile)
{
	_outFile << 
	_outFile << _prefix.c_str() << _matrixName.c_str() << " = " << flush;
	_outFile << "[ " << _matrix._11 << " " << _matrix._21 << " " << _matrix._31 << " " << _matrix._41
			 << "; " << _matrix._12 << " " << _matrix._22 << " " << _matrix._32 << " " << _matrix._42
			 << "; " << _matrix._13 << " " << _matrix._23 << " " << _matrix._33 << " " << _matrix._43
			 << "; " << _matrix._14 << " " << _matrix._24 << " " << _matrix._34 << " " << _matrix._44
			 << "];" << endl;
}

bool CMatrixUtils::WriteMatricesToFile(std::string _fileName, std::string _prefix)
{
	HRESULT hr;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	D3DMATRIX worldMatrix, viewMatrix, projectionMatrix;
	IDirect3DDevice9 * pDev = pGraphicsManager->GetDevice();

	hr = pDev->GetTransform(D3DTS_WORLD, &worldMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get world transform", hr);
		return false;
	}

	hr = pDev->GetTransform(D3DTS_VIEW, &viewMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get view transform", hr);
		return false;
	}

	hr = pDev->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get projection transform", hr);
		return false;
	}

	ofstream outFile(_fileName);

 	_WriteMatrixToFile(_prefix, "world", worldMatrix, outFile);
	_WriteMatrixToFile(_prefix, "view", viewMatrix, outFile);
	_WriteMatrixToFile(_prefix, "projection", projectionMatrix, outFile);

	return true;
}
