#include "AudioFile.h"

#include "Logger.h"

using namespace std;

CAudioFile::CAudioFile()
{
	m_pData = NULL;
	m_dwBufferSize = 0;
	ZeroMemory(&m_format, sizeof(m_format));
}

CAudioFile::~CAudioFile()
{
	if(m_pData != NULL)
	{
		delete [] m_pData;
	}
}

bool CAudioFile::Load(string _fileName)
{
	FILE *pInFile = NULL;
	DWORD dwChunkID, dwChunkSize, dwFormat;
	size_t readCount;
	char fourCcBuffer[5];

	if(m_pData != NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	fopen_s(&pInFile, _fileName.c_str(), "rb");
	if(pInFile == NULL)
	{
		LogError("Failed to open audio file");
		return false;
	}

	if(!_ReadChunkHeader(pInFile, dwChunkID, dwChunkSize, fourCcBuffer))
	{
		return false;
	}

	if(strcmp(fourCcBuffer, "RIFF") != 0)
	{
		LogError("Audio file has wrong ID");
		return false;
	}

	if(!_ReadFourCc(pInFile, dwFormat, fourCcBuffer))
	{
		return false;
	}

	if(strcmp(fourCcBuffer, "WAVE") != 0)
	{
		LogError("Audio file has wrong format");
		return false;
	}

	if(!_ReadChunkHeader(pInFile, dwChunkID, dwChunkSize, fourCcBuffer))
	{
		return false;
	}

	if(strcmp(fourCcBuffer, "fmt ") != 0)
	{
		LogError("Audio file doesn\'t contain a format chunk");
		return false;
	}

	if(dwChunkSize != 16)
	{
		LogError("Audio file format chunk has wrong size");
		return false;
	}

	readCount = fread(&m_format, sizeof(BYTE), 16, pInFile);
	if(readCount != 16)
	{
		LogError("Failed to read audio format");
		return false;
	}

	m_format.cbSize = 0;

	if(!_ReadChunkHeader(pInFile, dwChunkID, m_dwBufferSize, fourCcBuffer))
	{
		return false;
	}

	if(strcmp(fourCcBuffer, "data") != 0)
	{
		LogError("Audio file doesn\'t contain a data chunk");
		return false;
	}

	m_pData = (void *)new BYTE[m_dwBufferSize];
	if(m_pData == NULL)
	{
		LogError("Unable to allocate data buffer for audio file");
		return false;
	}

	readCount = fread(m_pData, sizeof(BYTE), m_dwBufferSize, pInFile);
	if(readCount != m_dwBufferSize)
	{
		LogError("Failed to read audio data");
		delete [] m_pData;
		m_pData = NULL;
		return false;
	}

	return true;
}

WAVEFORMATEX CAudioFile::GetFormat() const
{
	return m_format;
}

const void * CAudioFile::GetData() const
{
	return m_pData;
}

DWORD CAudioFile::_MakeBigEndian(DWORD _dwValue)
{
	DWORD dwOutput;

	BYTE *pbInValue = (BYTE *)&_dwValue;
	BYTE *pbOutValue = (BYTE *)&dwOutput;

	for(int i = 0; i < 4; i++)
	{
		pbOutValue[i] = pbInValue[3 - i];
	}
	
	return dwOutput;
}

bool CAudioFile::_ReadChunkHeader(FILE *_pInFile, DWORD& _dwId, DWORD& _dwSize, char *_pIdFourCc)
{
	size_t readCount = 0;

	if(!_ReadFourCc(_pInFile, _dwId, _pIdFourCc))
	{
		LogError("Failed to read chunk ID and FourCC");
		return false;
	}

	readCount = fread(&_dwSize, sizeof(DWORD), 1, _pInFile);
	if(readCount != 1)
	{
		LogError("Failed to read chunk size");
		return false;
	}

	return true;
}

bool CAudioFile::_ReadFourCc(FILE *_pInFile, DWORD& _dwFourCc, char *_pFourCc)
{
	size_t readCount = 0;

	readCount = fread(&_dwFourCc, sizeof(DWORD), 1, _pInFile);
	if(readCount != 1)
	{
		LogError("Failed to read FourCC");
		return false;
	}

	memcpy(_pFourCc, &_dwFourCc, sizeof(char) * 4);
	_pFourCc[4] = '\0';

	return true;
}

DWORD CAudioFile::GetBufferSize() const
{
	return m_dwBufferSize;
}
