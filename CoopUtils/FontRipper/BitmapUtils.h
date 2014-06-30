#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Coop/Bitmap.h"

void GetNonBlackData(const CBitmap& bmFull, POINT& pos, CBitmap& bmOut);
std::wstring ConvertStringToWide(const std::string& _sText);
