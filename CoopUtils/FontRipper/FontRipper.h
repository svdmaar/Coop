#pragma once

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"
#include "../IniFileReader/IniFile.h"
#include "FontRenderWindow.h"

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
	//HDC m_hDc;
	ofstream m_ofOut;
   CFontRenderWindow m_renderWindow;

	void _ExportDcToBitmap(CBitmap & _bmOut);
	void _SelectFontAndSize(const std::string & _sFontName, int _iSize);
	void _CreateStringBitmapTrimmed(const std::string & _sText, CBitmap & _bmOut, POINT & _pos);
	int _CalcD(char _c1, char _c2);
	void _CalcSpaceSize();
	void _FillCharDescs_BitmapUpperLeft();
	void _FillCharDescs_LeftRight();
	int _CalcTotalWidth(int _iStartChar, int _iMaxChar);
	int _CalcMaxRowWidth(int _nCharPerRow);
	int _CalcMaxHeight(int _iStartChar, int _iMaxChar);
	int _CalcTotalRowsHeight(int _nCharPerRow);
	void _CreateBigBitmap();
	void _WriteInfoToIniFile();
	void _CalcFontLineDistance();
	void _GetHeighestLowestChar(int & _iHeighest, int & _iLowest);
	void _CalcFontHeight();
	void _MoveFontUp();
	void _WriteToBmpIni(const string & _sOutName);
	string _SanitizeFileNameBase(const string & _sFileBase);

	// TODO: Get nonblack data.

	static std::wstring _ConvertStringToWide(const std::string & _sText);

public:
	CFontRipper();

	bool RipFont(const std::string & _sFontName, int _iFontSize);

};
