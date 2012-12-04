#pragma once

#include "InputDevice.h"

class CAppState
{
	const CInputDevice *m_pMainInputDevice;

	static CAppState *m_pInstance;

public:
	CAppState();
	~CAppState();

	const CInputDevice *GetMainInputDevice();
	void SetMainInputDevice(const CInputDevice *_pMainInputDevice);
	bool Init();
	bool CleanUp();

	static CAppState *GetInstance();
};
