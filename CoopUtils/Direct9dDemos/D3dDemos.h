#pragma once

#include "EnvSetup.h"

#include <string>

struct SD3dDemo
{
	std::string m_sName;
	bool (* m_pInit)(void);
	bool (* m_pRender)(void);
	bool (* m_pCleanUp)(void);
};

bool RunDemo(SD3dDemo _demo);

SD3dDemo GetSimpleDemo();
SD3dDemo GetVertexBufferDemo();
SD3dDemo GetTextureDemo();
SD3dDemo GetAlphaBlendingDemo();
SD3dDemo GetPixelPerfectTextureDemo();
SD3dDemo GetFirstFontDemo();
SD3dDemo GetFrameRateDemo();
