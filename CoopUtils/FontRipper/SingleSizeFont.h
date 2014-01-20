#pragma once

#include "CharDesc.h"
#include "CharPosition.h"

#include "../../Coop/Bitmap.h"
#include "../IniFileReader/IniFile.h"

#include <vector>
#include <string>

class CSingleSizeFont
{

protected:
	CBitmap m_bmBigBitmap;
	CIniFile m_iniFile;
	
	std::vector<SCharDesc> m_vCharDescs;
	std::string m_sFontName;
	int m_iMinChar;
	int m_iMaxChar;
	int m_iWindowsFontHeight;
	int m_iLineDistance;
	int m_iAverageWidth;
	int m_iAverageHeight;
	int m_iSpaceSize;
	int m_iFontHeight;
	int m_iInitialRowOffset;

	bool _ReadFontFromBmpIni(const std::string & _sFilenameBase);
	bool _CalcCharPositions(const std::string & _sText, std::vector<SCharPosition> & _vCharPos) const;
	
	static std::string _FormatCharIndex(int _iChar);

public:
	CSingleSizeFont();

	bool Load(const std::string & _sFilenameBase);

	int GetCharCount() const;
	bool DrawStringToBitmap(const std::string & _sText, CBitmap & _bmOut) const;
};
