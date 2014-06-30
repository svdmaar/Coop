#pragma once

#include "../../Coop/Bitmap.h"
#include "BitmapUtils.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

class CFontRenderWindow
{
   HWND m_hWindow;

   static LRESULT CALLBACK FontTestCallbackFunc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
   
   // TODO: move these to Utils class.
   void _ExportDcToBitmap(HDC _hDc, CBitmap & _bmOut);
   std::wstring _ConvertStringToWide(const std::string & _sText);
   void _ClearDc(HDC _hDc);
   void _ProcessMessages();

public:
   bool Create();
   bool Destroy();
   CBitmap RenderString(const std::string& _sText);
   CBitmap RenderGreyString(const std::string& _sText, POINT& _pos);
   void SelectFontAndSize(const std::string & _sFontName, int _iSize);
};
