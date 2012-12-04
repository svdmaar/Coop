#include "Application.h"

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE, LPSTR _lpCmdLine, int)
{
	CApplication app(_hInstance);
	return app.Main(_lpCmdLine);
}
