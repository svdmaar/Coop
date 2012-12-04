#pragma once

#include "AudioFile.h"

#include <XAudio2.h>

class CAudioTrack
{
	bool m_bInited;

	IXAudio2 *m_pXAudio2;	
	IXAudio2MasteringVoice *m_pDevice;
	IXAudio2SourceVoice *m_pVoice;
	WAVEFORMATEX m_currentFormat;

	bool _SameAsCurrentFormat(const WAVEFORMATEX& _format);

public:
	CAudioTrack();
	~CAudioTrack();

	bool Init(IXAudio2 *_pXAudio2, IXAudio2MasteringVoice *_pAudioDevice);
	bool CleanUp();

	bool PlayAudioFile(const CAudioFile& _audioFile);
};
