#include <iostream>

using namespace std;

#pragma pack(1)
struct SBitmapHeader
{
	unsigned short m_bfType;
	unsigned long m_bfSize;
	unsigned short m_bfReserved1;
	unsigned short m_bfReserved2;
	unsigned long m_bfOffBits;
	unsigned long m_biSize;
	long m_biWidth;
	long m_biHeight;
	unsigned short m_biPlanes;
	unsigned short m_biBitCount;
	unsigned long m_biCompression;
	unsigned long m_biSizeImage;
	long m_biXPelsPerMeter;
	long m_biYPelsPerMeter;
	unsigned long m_biClrUsed;
	unsigned long m_biClrImportant;
};
#pragma pack()

void PrintBitmapHeaderFields(const SBitmapHeader & _header)
{
   cout << "header info: " << endl;
   cout << "m_bfType: " << _header.m_bfType << endl;
   cout << "m_bfSize: " << _header.m_bfSize << endl;
   cout << "m_bfReserved1: " << _header.m_bfReserved1 << endl;
   cout << "m_bfReserved2: " << _header.m_bfReserved2 << endl;
   cout << "m_bfOffBits: " << _header.m_bfOffBits << endl;
   cout << "m_biSize: " << _header.m_biSize << endl;
   cout << "m_biWidth: " << _header.m_biWidth << endl;
   cout << "m_biHeight: " << _header.m_biHeight << endl;
   cout << "m_biPlanes: " << _header.m_biPlanes << endl;
   cout << "m_biBitCount: " << _header.m_biBitCount << endl;
   cout << "m_biCompression: " << _header.m_biCompression << endl;
   cout << "m_biSizeImage: " << _header.m_biSizeImage << endl;
   cout << "m_biXPelsPerMeter: " << _header.m_biXPelsPerMeter << endl;
   cout << "m_biYPelsPerMeter: " << _header.m_biYPelsPerMeter << endl;
   cout << "m_biClrUsed: " << _header.m_biClrUsed << endl;
   cout << "m_biClrImportant: " << _header.m_biClrImportant << endl;
}

void PrintTypeSizes()
{
   // Directly referenced.
   cout << "Sizeof unsigned short: " << sizeof(unsigned short) << endl;
   cout << "Sizeof unsigned long: " << sizeof(unsigned long) << endl;
   cout << "Sizeof long: " << sizeof(long) << endl;

   // Possible replacements.
   cout << "Sizeof unsigned int: " << sizeof(unsigned int) << endl;
   cout << "Sizeof int: " << sizeof(int) << endl;

   // Pointer size.
   int * pInt = NULL;
   cout << "Sizeof int pointer: " << sizeof(pInt) << endl;
}

void PrintCalibrateBmpHeader()
{
   FILE * pFile = fopen("calibrate.bmp", "rb");

   SBitmapHeader header;
   fread(&header, sizeof(header), 1, pFile);

   PrintBitmapHeaderFields(header);

   fclose(pFile);
}

int main()
{
   //PrintTypeSizes();
   PrintCalibrateBmpHeader();

   return 0;
}

/*
Sizes.

Windows 32 bits:
Sizeof unsigned short: 2
Sizeof unsigned long: 4
Sizeof long: 4
Sizeof unsigned int: 4
Sizeof int: 4
Sizeof int pointer: 4

Windows 64 bits:
Sizeof unsigned short: 2
Sizeof unsigned long: 4
Sizeof long: 4
Sizeof unsigned int: 4
Sizeof int: 4
Sizeof int pointer: 8

*/
