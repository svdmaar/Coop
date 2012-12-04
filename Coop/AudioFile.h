#pragma once

#include <Windows.h>
#include <string>
#include <XAudio2.h>

class CAudioFile
{
	WAVEFORMATEX m_format;
	void * m_pData;
	DWORD m_dwBufferSize;

	static DWORD _MakeBigEndian(DWORD _dwValue);
	static bool _ReadChunkHeader(FILE *_pInFile, DWORD& _dwId,  DWORD& _dwSize, char *_pIdFourCc);
	static bool _ReadFourCc(FILE *_pInFile, DWORD& _dwFourCc, char *_pFourCc);

public:
	CAudioFile();
	~CAudioFile();

	bool Load(std::string _fileName);

	WAVEFORMATEX GetFormat() const;
	const void * GetData() const;
	DWORD GetBufferSize() const;
};
