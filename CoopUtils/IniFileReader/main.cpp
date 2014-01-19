#include "IniFile.h"

#include <iostream>

using namespace std;

int main()
{
	//CIniFile::TestTrim();

	CIniFile iniFile;
	iniFile.Load("font.ini");

	iniFile.PrintValues();

	cout << "value from file: \"" << iniFile.GetValueString("info", "name").c_str() << "\"" << endl;

	return 0;
}
