#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"
#include "../IniFileReader/IniFile.h"
#include "FontRipper.h"
#include "CharDesc.h"
#include "CharPosition.h"
#include "SingleSizeFont.h"
#include "FontRenderWindow.h"

#include <iostream>
#include <string>
#include <vector>
#include <minmax.h>
#include <locale>
#include <sstream>
#include <fstream>

using namespace std;

void CheckFontBmps()
{
	RECT rSub;
	rSub.left = 8;
	rSub.top = 30;
	rSub.right = rSub.left + (1920 / 2);
	rSub.bottom = rSub.top + (1200 / 2);

	const char * d3dFilename = "D:\\Temp\\font results\\00\\d3d.bmp";
	const char * gdiFilename = "D:\\Temp\\font results\\00\\gdi.bmp";

	CBitmap bmD3dComplete(d3dFilename);
	CBitmap bmGdiComplete(gdiFilename);

	const char * d3dSubFilename = "D:\\Temp\\font results\\00\\d3d_sub.bmp";
	const char * gdiSubFilename = "D:\\Temp\\font results\\00\\gdi_sub.bmp";

	CBitmap bmD3dSub = bmD3dComplete.GetSubBitmap(rSub);
	CBitmap bmGdiSub = bmGdiComplete.GetSubBitmap(rSub);

	bmD3dSub.Save(d3dSubFilename);
	bmGdiSub.Save(gdiSubFilename);

	CBitmap bmDiff(bmD3dSub.GetWidth(), bmD3dSub.GetHeight(), 0);

	for(int iRow = 0; iRow < bmD3dSub.GetHeight(); iRow++)
	{
		for(int iColumn = 0; iColumn < bmD3dSub.GetWidth(); iColumn++)
		{
			DWORD dwPixel1 = bmD3dSub.GetPixel(iRow, iColumn);
			DWORD dwPixel2 = bmGdiSub.GetPixel(iRow, iColumn);

			if(dwPixel1 != dwPixel2)
			{
				if(dwPixel1 > dwPixel2)
				{
					bmDiff.SetPixel(iRow, iColumn, 0xff);
				}
				else
				{
					bmDiff.SetPixel(iRow, iColumn, 0xff00);
				}
			}
		}
	}

	const char * diffFilename = "D:\\Temp\\font results\\00\\diff.bmp";
	bmDiff.Save(diffFilename);
}

int main()
{
   /*
   CFontRenderWindow window;
   window.Create();

   for (char c = 'A'; c <= 'C'; c++) {
      cout << c << "..." << endl;

      char buffer[2];

      buffer[0] = c;
      buffer[1] = '\0';

      string sText = buffer;
      string sFileName = "windowed/" + sText + "_window.bmp";

      //CBitmap bmText = window.RenderString(sText);
      POINT pos;
      CBitmap bmText = window.RenderGreyString(sText, pos);
      bmText.Save(sFileName);
   }
   */

   //CBitmap bmText = window.RenderString("S");
   //bmText.Save("windowed/S_window.bmp");

   //Sleep(5000);

   //return 0;


	//CheckFontBmps();

	//return 0;

	CFontRipper fontRipper;
	fontRipper.RipFont("Arial Black", 200);

	return 0;

}
