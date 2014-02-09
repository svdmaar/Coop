#pragma once

class CTexture;

class CVideoDevice
{
	struct SVideoDeviceImpl;
	SVideoDeviceImpl * m_pImpl;

	CVideoDevice();
	~CVideoDevice();

public:
	bool StartScene();
	bool EndScene();

	bool SelectTexture(CTexture * _pTexture);
	
	static bool Init();
	static bool CleanUp();
};

extern CVideoDevice * g_pVideoDevice;
