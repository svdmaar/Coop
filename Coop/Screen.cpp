#include "Screen.h"

#include "Logger.h"
#include "GraphicsManager.h"
#include "Camera.h"

CScreen::CScreen()
{
}

CScreen::~CScreen()
{
}

bool CScreen::Init()
{
	return true;
}

bool CScreen::InitAfterSwap()
{
	return true;
}

bool CScreen::ProcessInput()
{
	return true;
}

bool CScreen::CleanUp()
{
	return true;
}

bool CScreen::SetDefaultGraphicsSettings2D()
{
	HRESULT hr;
	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();

	D3DXMATRIX identityTransform, projTransform;
	D3DXMatrixIdentity(&identityTransform);
	D3DXMatrixOrthoLH(&projTransform, 100.0f * pGraphicsManager->GetAspectRatio(), 100.0f, -1.0f, 1.0f);

	hr = pDevice->SetTransform(D3DTS_WORLD, &identityTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set world transform", hr);
		return false;
	}
		
	hr = pDevice->SetTransform(D3DTS_VIEW, &identityTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set view transform", hr);
		return false;
	}

	hr = pDevice->SetTransform(D3DTS_PROJECTION, &projTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set projection transform", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set cull mode render state", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enable z", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set z write enabled", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set lighting to false", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha blending to true", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha source render state", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set alpha dest render state", hr);
		return false;
	}

	return true;
}

static void setupDirectionalLight(D3DLIGHT9 & _light, D3DCOLORVALUE _color, D3DXVECTOR3 _direction)
{
	D3DXVECTOR3 normDir;
	D3DXVec3Normalize(&normDir, &_direction);

	memset(&_light, 0, sizeof(_light));
	_light.Type = D3DLIGHT_DIRECTIONAL;
	_light.Diffuse = _color;
	_light.Direction = normDir;
	_light.Range = 1000.0f;
}

bool CScreen::SetDefaultGraphicsSettings3D()
{
	HRESULT hr;

	D3DXMATRIX identityTransform, projTransform;
	D3DXMatrixIdentity(&identityTransform);
	D3DXMatrixPerspectiveFovLH(&projTransform, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

    D3DXVECTOR3 vEyePt(-10.0f, -10.0f, 10.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 0.0f, 1.0f);
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	D3DCOLORVALUE lightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	D3DXVECTOR3 lightDir(-1.0f, 0.7f, -0.2f);

	D3DLIGHT9 light;
	setupDirectionalLight(light, lightColor, lightDir);

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse = lightColor;
	mtrl.Ambient = lightColor;
	mtrl.Ambient.g = 0.0f;

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
	IDirect3DDevice9 *pDevice = pGraphicsManager->GetDevice();
	CCamera * pCamera = CCamera::GetInstance();

	hr = pDevice->SetTransform(D3DTS_WORLD, &identityTransform);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set world transform", hr);
		return false;
	}
		
	D3DXMATRIX camViewMat = pCamera->GetViewMatrix();
	hr = pDevice->SetTransform(D3DTS_VIEW, &camViewMat);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set view transform", hr);
		return false;
	}

	D3DXMATRIX camProjMat = pCamera->GetProjectionMatrix();
	hr = pDevice->SetTransform(D3DTS_PROJECTION, &camProjMat);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set projection transform", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set cull mode render state", hr);
		return false;
	}

	hr = pDevice->SetLight(0, &light);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set light 0", hr);
		return false;
	}

	hr = pDevice->SetMaterial(&mtrl);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set material", hr);
		return false;
	}

	hr = pDevice->LightEnable(0, TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enable light", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set ambient color", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to enable z", hr);
		return false;
	}

	hr = pDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set z write enabled", hr);
		return false;
	}

	hr = pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set sampler state - MINFILTER", hr);
		return false;
	}

	hr = pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set sampler state - MAGFILTER", hr);
		return false;
	}

	hr = pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to set sampler state - MIPFILTER", hr);
		return false;
	}

	return true;
}
