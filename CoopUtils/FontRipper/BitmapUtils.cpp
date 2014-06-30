#include "BitmapUtils.h"

using namespace std;

void GetNonBlackData(const CBitmap& bmFull, POINT& pos, CBitmap& bmOut)
{
	const DWORD dwBackground = 0x0l;
	RECT rChar;

	rChar.top = bmFull.GetHeight();
	rChar.left = bmFull.GetWidth();
	rChar.bottom = 0;
	rChar.right = 0;

	for(int iRow = 0; iRow < bmFull.GetHeight(); iRow++)
	{
		for(int iColumn = 0; iColumn < bmFull.GetWidth(); iColumn++)
		{
			DWORD dwPixel = bmFull.GetPixel(iRow, iColumn);

			if (dwPixel != dwBackground)
			{
				rChar.top = min(rChar.top, iRow);
				rChar.left = min(rChar.left, iColumn);
				rChar.bottom = max(rChar.bottom, iRow);
				rChar.right = max(rChar.right, iColumn);
			}
		}
	}

	rChar.right++;
	rChar.bottom++;

	int iCharWidth = rChar.right - rChar.left;
	int iCharHeight = rChar.bottom - rChar.top;

	bmOut = CBitmap(iCharWidth, iCharHeight, 0);

	for(int iRow = rChar.top; iRow < rChar.bottom; iRow++)
	{
		for(int iColumn = rChar.left; iColumn < rChar.right; iColumn++)
		{
			int iTargetRow = iRow - rChar.top;
			int iTargetColumn = iColumn - rChar.left;

			DWORD dwPixel = bmFull.GetPixel(iRow, iColumn);
			bmOut.SetPixel(iTargetRow, iTargetColumn, dwPixel);
		}
	}

	pos.x = rChar.left;
	pos.y = rChar.top;
}

std::wstring ConvertStringToWide(const std::string& _sText)
{
	int nLength = _sText.length();
	wchar_t * pwcBuffer = new wchar_t[nLength + 1];

	ZeroMemory(pwcBuffer, sizeof(wchar_t) * (nLength + 1));
	char * pcBuffer = (char *)pwcBuffer;

	for(int i = 0; i < nLength; i++)
	{
		int iTargetIndex = i * 2;
		pcBuffer[iTargetIndex] = _sText[i];
	}

	wstring wsOut(pwcBuffer);
	delete [] pwcBuffer;

	return wsOut;
}
