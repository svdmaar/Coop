#include "VideoDevice.h"

#include "../../Coop/Bitmap.h"
#include "Texture.h"
#include "TextureImpl.h"

#include <string>
#include <Windows.h>
#include <sstream>

using namespace std;

CBitmap * g_pbmScreenBitmap = NULL;
CVideoDevice * g_pVideoDevice = NULL;

// Just for rendering to bitmap:
CBitmap * g_pbmSelectedTexture = NULL;
CDataMap<float> * g_pmSelectedAlpha = NULL;

const int g_iWidth = 1920;
const int g_iHeight = 1200;

static string BuildFilenameBase();

struct CVideoDevice::SVideoDeviceImpl
{
	int m_iFrameIndex;
	string m_sFilenameBase;
	CTexture * m_pSelectedTexture;

	SVideoDeviceImpl()
	{
		m_iFrameIndex = 0;
	}
};

CVideoDevice::CVideoDevice()
{
	m_pImpl = new SVideoDeviceImpl();
}

CVideoDevice::~CVideoDevice()
{
	delete m_pImpl;
}

bool CVideoDevice::StartScene()
{
	*g_pbmScreenBitmap = CBitmap(g_iWidth, g_iHeight, 0);

	return true;
}

bool CVideoDevice::EndScene()
{
	const int nFramnumberChars = 5;
	stringstream ssFilename;
	ssFilename << m_pImpl->m_sFilenameBase << "_";

	ssFilename.fill('0');
	ssFilename.width(nFramnumberChars);

	ssFilename << m_pImpl->m_iFrameIndex;
	ssFilename <<  ".bmp";
	m_pImpl->m_iFrameIndex++;

	string sFilename = ssFilename.str();

	g_pbmScreenBitmap->Save(sFilename);
	*g_pbmScreenBitmap = CBitmap(g_iWidth, g_iHeight, 0);

	return true;
}

bool CVideoDevice::Init()
{
	g_pVideoDevice = new CVideoDevice();
	g_pbmScreenBitmap = new CBitmap(g_iWidth, g_iHeight, 0);

	g_pVideoDevice->m_pImpl->m_sFilenameBase = BuildFilenameBase();	

	return true;
}

bool CVideoDevice::CleanUp()
{
	delete g_pbmScreenBitmap;
	g_pbmScreenBitmap = NULL;

	delete g_pVideoDevice;
	g_pVideoDevice = NULL;

	return true;
}

static string BuildFilenameBase()
{
	stringstream ssFilenameBase;

	ssFilenameBase << "renders/";

	SYSTEMTIME time;
	GetSystemTime(&time);

	ssFilenameBase << time.wYear;

	ssFilenameBase.fill('0');

	ssFilenameBase.width(2);
	ssFilenameBase << time.wMonth ;

	ssFilenameBase.width(2);
	ssFilenameBase << time.wDay;

	ssFilenameBase.width(2);
	ssFilenameBase << time.wHour; 

	ssFilenameBase.width(2);
	ssFilenameBase << time.wMinute;

	ssFilenameBase.width(2);
	ssFilenameBase << time.wSecond;

	ssFilenameBase.width(3);
	ssFilenameBase << time.wMilliseconds;

	return ssFilenameBase.str();
}

bool CVideoDevice::SelectTexture(CTexture * _pTexture)
{
	m_pImpl->m_pSelectedTexture = _pTexture;
	
	g_pbmSelectedTexture = &(_pTexture->m_pImpl->m_bmColor);
	g_pmSelectedAlpha = &(_pTexture->m_pImpl->m_mAlpha);

	return true;
}
