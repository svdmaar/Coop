#include "IniFile.h"

#include <iostream>

using namespace std;

int main()
{
	//CIniFile::TestTrim();

	CIniFile iniFile;
	iniFile.Load("testfile.ini");

	iniFile.PrintValues();

	cout << "value from file: \"" << iniFile.GetValueString("info", "name").c_str() << "\"" << endl;

	vector<int> vValues = iniFile.GetValueIntVector("testblock", "intarray");
	cout << vValues[1] << endl;

	return 0;
}
