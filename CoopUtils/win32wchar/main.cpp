#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

using namespace std;

int main()
{
	const wchar_t testData2[] = L"ëtest";

	//wcout << testData2 << endl;

	char testData2Char[2];
	ZeroMemory(testData2Char, 2);

	testData2Char[0] = *(char*)(void*)testData2;
	
	cout << testData2Char << endl;

	return 0;
	
	const wchar_t testData[] = L"test";


	cout << sizeof(testData) << endl;

	cout << sizeof(wchar_t) << endl;

	const char * pcTestData = (const char *)(const void *)testData;

	cout << (int)pcTestData[1] << endl;

	cout << pcTestData[0] << endl;

	const int iStartChar = 33;
	const int iMaxChar = 127;

	int nCharCount = iMaxChar - iStartChar;
	int nBufferSize = nCharCount + 1;
	wchar_t * pwcAllChars = new wchar_t[nBufferSize];

	ZeroMemory(pwcAllChars, sizeof(wchar_t) * nBufferSize);

	char * pCharBuffer = (char *)(void *)pwcAllChars;

	for(int iChar = iStartChar; iChar < iMaxChar; iChar++)
	{
		int iTargetIndex = iChar - iStartChar;
		char * pInBufferChar = pCharBuffer + iTargetIndex * 2;
		*pInBufferChar = (char)iChar;
	}

	wcout << pwcAllChars << endl;

	char outBuffer[4];
	ZeroMemory(outBuffer, sizeof(outBuffer));

	outBuffer[0] = 'F';

	wchar_t * pwcOutBuffer = (wchar_t *)(void *)outBuffer;

	wcout << pwcOutBuffer << endl;

	return 0;
}
