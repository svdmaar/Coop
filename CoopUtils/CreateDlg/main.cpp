//#include "DlgWithButton.h"
#include "FontTestDlg.h"
#include "D3d9DlgVerySimple.h"

HINSTANCE g_hInstance = 0;

int main()
{
   g_hInstance = GetModuleHandle(NULL);

   RunDlgFont();

   return 0;
}
