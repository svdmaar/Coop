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

	POINT pTest = iniFile.GetValuePoint("testblock", "testPoint");
	cout << "(" << pTest.x << ", " << pTest.y << ")" << endl;

   iniFile.SetValueInt("extraBlock1", "key1", 12);

   iniFile.SetValueInt("extraBlock2", "key1", 23);
   iniFile.SetValueInt("extraBlock2", "key1", 24);
   iniFile.Save("testout.ini");

	return 0;
}
