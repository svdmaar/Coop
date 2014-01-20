#pragma once

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"
#include "../IniFileReader/IniFile.h"

#include "SingleSizeFont.h"

#include <iostream>
#include <string>
#include <vector>
#include <minmax.h>
#include <locale>
#include <sstream>
#include <fstream>

using namespace std;

class CFontRipper : public CSingleSizeFont
{
	HDC m_hDc;
	ofstream m_ofOut;

	void _ExportDcToBitmap(CBitmap & _bmOut);
	void _SelectFontAndSize(const std::string & _sFontName, int _iSize);
	void _CreateStringBitmap(const std::string & _sText, CBitmap & _bmOut, POINT & _pos);
	int _CalcD(char _c1, char _c2);
	void _CalcSpaceSize();
	void _FillCharDescs_BitmapUpperLeft();

	// TODO: Get nonblack data.

	static std::wstring _ConvertStringToWide(const std::string & _sText);

public:
	CFontRipper();

	bool RipFont(const std::string & _sFont, int _iSize);

};
