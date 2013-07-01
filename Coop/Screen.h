#pragma once

#include <string>
#include <vector>

class CScreenManager;

class CScreen
{

public:
	CScreen();
	virtual ~CScreen();

	virtual bool Init();
	virtual bool InitAfterSwap();
	virtual bool CleanUp();

	virtual bool ProcessInput();

	virtual bool Render() = 0;

protected:
	bool SetDefaultGraphicsSettings2D();
	bool SetDefaultGraphicsSettings3D();
};

typedef std::vector<CScreen *> ScreenVector;
