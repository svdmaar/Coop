#pragma once

#include "AudioFile.h"
#include "AudioTrack.h"

#include <XAudio2.h>

const int g_iAudioTrackCount = 32;

class CAudioManager
{
	bool m_bInited;

	IXAudio2 *m_pXAudio2;
	IXAudio2MasteringVoice *m_pDevice;
	CAudioTrack m_tracks[g_iAudioTrackCount];
	int m_iTrackIndex;

	static CAudioManager *m_pInstance;

public:
	CAudioManager();
	~CAudioManager();

	bool Init();
	bool CleanUp();

	bool PlayAudioFile(const CAudioFile& _audioFile);

	static CAudioManager *GetInstance();
};
