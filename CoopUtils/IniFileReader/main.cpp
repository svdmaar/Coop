#include "IniFile.h"

int main()
{
	//CIniFile::TestTrim();

	CIniFile iniFile;
	iniFile.Load("font.ini");

	iniFile.PrintValues();

	return 0;
}
