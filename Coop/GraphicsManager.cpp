#include "GraphicsManager.h"

#include "Application.h"
#include "Logger.h"
#include "Bitmap.h"

#include <assert.h>

#if 1
const bool g_bStartFullscreen = false;
const int g_iStartWindowWidth = 1920 / 2;
const int g_iStartWindowHeight = (1080 + 1200) / 2 / 2;
#else
const bool g_bStartFullscreen = true;
const int g_iStartWindowWidth = 1920;
const int g_iStartWindowHeight = 1200;
#endif

const LPCWSTR g_className = L"Coop class";
const LPCWSTR g_windowName = L"Coop";

CGraphicsManager::CGraphicsManager()
{
	m_bInited = false;
	m_hWnd = 0;
	m_bFullScreen = false;
	m_iWindowWidth = 0;
	m_iWindowHeight = 0;
	m_pD3Device = NULL;
	m_pDirect3D9 = NULL;
	m_bShouldQuit = false;

	m_pInstance = this;
}

CGraphicsManager::~CGraphicsManager()
{
	assert(!m_bInited);
}

bool CGraphicsManager::Init()
{
	HRESULT hr;
	HINSTANCE hInstance = CApplication::GetInstance()->GetHInstance();
	
	assert(!m_bInited);

	m_bFullScreen = g_bStartFullscreen;

	WNDCLASSEX winClass;
	memset(&winClass, 0, sizeof(winClass));
	winClass.cbSize = sizeof(winClass);
	winClass.style = CS_CLASSDC;
	winClass.lpfnWndProc = _WindowProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInstance;
	winClass.hIcon = 0;
	winClass.hCursor = 0;
	winClass.hbrBackground = 0;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = g_className;
	winClass.cbWndExtra = NULL;

	if(RegisterClassEx(&winClass) == 0)
	{
		LogError("Failed to register class");
		return false;
	}

	DWORD dwWindowStyle = m_bFullScreen ? WS_MAXIMIZE : WS_OVERLAPPEDWINDOW;

	m_hWnd = CreateWindow(g_className, g_windowName, dwWindowStyle, 100, 100, g_iStartWindowWidth, g_iStartWindowHeight, 0, 0, hInstance, NULL);
	if(m_hWnd == NULL)
	{
		LogError("Failed to create window");
		return false;
	}

	m_iWindowWidth = g_iStartWindowWidth;
	m_iWindowHeight = g_iStartWindowHeight;

	ShowWindow(m_hWnd, SW_SHOW);

	m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pDirect3D9 == NULL)
	{
		LogError("Failed to create direct3d");
		return false;
	}

	D3DDISPLAYMODE dispMode;
	memset(&dispMode, 0, sizeof(dispMode));
	hr = m_pDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispMode);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get adapter display mode", hr);
		return false;
	}
	
	D3DPRESENT_PARAMETERS presentParams;
	memset(&presentParams, 0, sizeof(presentParams));
	presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParams.Windowed = m_bFullScreen ? FALSE : TRUE;
	presentParams.EnableAutoDepthStencil = TRUE;
	presentParams.AutoDepthStencilFormat = D3DFMT_D16;

	if(m_bFullScreen)
	{
		presentParams.BackBufferWidth = dispMode.Width;
		presentParams.BackBufferHeight = dispMode.Height;
		presentParams.BackBufferFormat = dispMode.Format;
		presentParams.FullScreen_RefreshRateInHz = dispMode.RefreshRate;

		m_iWindowWidth = dispMode.Width;
		m_iWindowHeight = dispMode.Height;
	}

	hr = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParams, &m_pD3Device);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create device", hr);
		return false;
	}

	if(m_bFullScreen)
	{
		m_pD3Device->ShowCursor(FALSE);
	}

	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	if(!m_arial.Init("arial", "arial.bmp", m_pD3Device))
	{
		return false;
	}

	m_bInited = true;

	return true;
}

bool CGraphicsManager::CleanUp()
{
	HRESULT hr;
	HINSTANCE hInstance = CApplication::GetInstance()->GetHInstance();

	if(!m_arial.CleanUp())
	{
		return false;
	}

	if(UnregisterClass(g_className, hInstance))
	{
		LogError("Failed to unregister class");
		return false;
	}

	hr = m_pD3Device->Release();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to release device", hr);
		return false;
	}

	hr = m_pDirect3D9->Release();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to release direct 3d", hr);
		return false;
	}

	m_bInited = false;

	return true;
}

bool CGraphicsManager::IsInited()
{
	return m_bInited;
}

HWND CGraphicsManager::GetWindowHandle()
{
	return m_hWnd;
}

int CGraphicsManager::GetWindowWidth()
{
	return m_iWindowWidth;
}

int CGraphicsManager::GetWindowHeight()
{
	return m_iWindowHeight;
}

bool CGraphicsManager::IsFullscreen()
{
	return m_bFullScreen;
}

bool CGraphicsManager::ShouldQuit()
{
	return m_bShouldQuit;
}

IDirect3DDevice9 *CGraphicsManager::GetDevice()
{
	return m_pD3Device;
}

IDirect3D9 *CGraphicsManager::GetDirect3D()
{
	return m_pDirect3D9;
}

const CFont *CGraphicsManager::GetArial()
{
	return &m_arial;
}

CGraphicsManager *CGraphicsManager::GetInstance()
{
	assert(m_pInstance->m_bInited);

	return m_pInstance;
}

bool CGraphicsManager::ProcessWindowMessages()
{
	MSG message;

	if(PeekMessage(&message, m_hWnd, 0, 0, PM_NOREMOVE))
	{
		BOOL bNonQuit = GetMessage(&message, m_hWnd, 0, 0);
		if(bNonQuit == 0)
		{
			m_bShouldQuit = true;
		}

		DispatchMessage(&message);
	}

	return true;
}

bool CGraphicsManager::BeginScene()
{
	HRESULT hr;

	hr = m_pD3Device->BeginScene();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to begin scene", hr);
		return false;
	}

	hr = m_pD3Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to clear", hr);
		return false;
	}

	m_frameTimer.OnNewFrame();

	return true;
}

bool CGraphicsManager::EndScene()
{
	// TODO: deal with fullscreen alt-tab

	HRESULT hr;

	hr = m_pD3Device->EndScene();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to end scene", hr);
		return false;
	}

	hr = m_pD3Device->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to present scene", hr);
		return false;
	}

	return true;
}

bool CGraphicsManager::MakeScreenshot()
{
	HRESULT hr;

	IDirect3DSurface9 *pRenderTarget = NULL;
	hr = m_pD3Device->GetRenderTarget(0, &pRenderTarget);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get render target", hr);
		return false;
	}

	D3DSURFACE_DESC desc;
	hr = pRenderTarget->GetDesc(&desc);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get render target format", hr);
		return false;
	}

	IDirect3DSurface9 *pOffscreenSurface = NULL;
	hr = m_pD3Device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pOffscreenSurface, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create offscreen surface", hr);
		return false;
	}

	hr = m_pD3Device->GetRenderTargetData(pRenderTarget, pOffscreenSurface);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get render target data", hr);
		return false;
	}

	D3DLOCKED_RECT lockedRect;

	hr = pOffscreenSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock offscreen surface", hr);
		return false;
	}

	CBitmap outBitmap;
	if(!outBitmap.CreateFromData((DWORD *)lockedRect.pBits, desc.Width, desc.Height, lockedRect.Pitch))
	{
		LogError("Failed to create bitmap from data");
		return false;
	}

	if(!outBitmap.Save("screenshot.bmp"))
	{
		LogError("Failed to save screenshot bitmap");
		return false;
	}

	hr = pOffscreenSurface->UnlockRect();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock offscreen surface", hr);
		return false;
	}
		
	pOffscreenSurface->Release();
	pRenderTarget->Release();

	return true;
}

bool CGraphicsManager::CheckDeviceLostStatus()
{
	HRESULT hr;
	hr = m_pD3Device->TestCooperativeLevel();
	if(FAILED(hr))
	{
		LogErrorHr("Failed to get cooperative level", hr);
		return false;
	}

	return true;
}

bool CGraphicsManager::CreateTexture(const CBitmap& _bitmap, IDirect3DTexture9 ** _ppTexture)
{
	HRESULT hr;

	int iWidth = _bitmap.GetWidth();
	int iHeight = _bitmap.GetHeight();

	IDirect3DTexture9 *pSystemTexture = NULL;

	hr = m_pD3Device->CreateTexture((UINT)iWidth, (UINT)iHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pSystemTexture, NULL);
	if(FAILED(hr))
	{
		LogError("Failed to create system texture");
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	memset(&lockedRect, 0, sizeof(lockedRect));

	hr = pSystemTexture->LockRect(0, &lockedRect, NULL, 0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to lock system texture rect", hr);
		return false;
	}

	DWORD *pTexLocation = (DWORD *)lockedRect.pBits;
	for(int iRowIndex = 0; iRowIndex < iHeight; iRowIndex++)
	{
		memcpy(pTexLocation, _bitmap.GetRow(iRowIndex), sizeof(DWORD) * iWidth);

		BYTE *pTextureByteLocation = (BYTE *)pTexLocation;
		pTextureByteLocation += lockedRect.Pitch;
		
		pTexLocation = (DWORD *)pTextureByteLocation;
	}

	hr = pSystemTexture->UnlockRect(0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to unlock system texture rect", hr);
		return false;
	}

	hr = m_pD3Device->CreateTexture(iWidth, iHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, _ppTexture, NULL);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create video texture", hr);
		return false;
	}

	hr = m_pD3Device->UpdateTexture(pSystemTexture, *_ppTexture);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to update video texture", hr);
		return false;
	}

	pSystemTexture->Release();

	return true;
}

float CGraphicsManager::GetAspectRatio()
{
	float fWidth = (float)m_iWindowWidth;
	float fHeight = (float)m_iWindowHeight;

	return fWidth / fHeight;
}

void CGraphicsManager::SetShouldQuit()
{
	m_bShouldQuit = true;
}

CGraphicsManager *CGraphicsManager::m_pInstance = NULL;

LRESULT WINAPI CGraphicsManager::_WindowProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_Msg)
	{
	case WM_CREATE:
		{
			return 0;
		}
	case WM_CLOSE:
		{
			m_pInstance->m_bShouldQuit = true;
			return 0;
		}
	case WM_KEYDOWN:
		{
			if(_wParam == VK_ESCAPE)
			{
				m_pInstance->m_bShouldQuit = true;
				return 0;
			}
		}
	case WM_SIZE:
		{
			return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
		}
	case WM_SETCURSOR:
		{
			if(m_pInstance->m_bFullScreen)
			{
				SetCursor(NULL);
				return TRUE;
			}
			else
			{
				SetCursor(LoadCursor(CApplication::GetInstance()->GetHInstance(), IDC_ARROW));
				return true;
			}
		}
	case WM_ACTIVATE:
		{
			return 0;
		}
	case WM_ACTIVATEAPP:
		{
			if(_wParam == TRUE && m_pInstance->m_bInited)
			{
				CGraphicsManager *pGraphicsManager = CGraphicsManager::GetInstance();
				pGraphicsManager->CheckDeviceLostStatus();
			}
			return 0;
		}
	default:
		{
			return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
		}
	}
}
