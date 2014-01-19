#pragma once

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"
#include "../IniFileReader/IniFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <minmax.h>
#include <locale>
#include <sstream>
#include <fstream>

using namespace std;

const int g_iMinChar = 33;
const int g_iMaxChar = 127; // exclusive
const int g_nCharCount = g_iMaxChar - g_iMinChar;

class CFontRipper
{
	struct SCharDesc
	{
		CBitmap m_bitmap;
		POINT m_pUpperLeft;
		int m_iBufferLeft;
		int m_iBufferRight;
		POINT m_pInBitmap;
	};



public:
	CFontRipper();



};
