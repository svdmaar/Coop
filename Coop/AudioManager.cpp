#include "AudioManager.h"

#include "Logger.h"

#include <assert.h>

CAudioManager::CAudioManager()
{
	m_bInited = false;
	m_pXAudio2 = NULL;
	m_pDevice = NULL;
	m_iTrackIndex = 0;

	m_pInstance = this;
}

CAudioManager::~CAudioManager()
{
	assert(!m_bInited);
}

bool CAudioManager::Init()
{
	HRESULT hr;
	
	hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create XAudio2", hr);
		return false;
	}

	hr = m_pXAudio2->CreateMasteringVoice(&m_pDevice);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to create mastering voice", hr);
		return false;
	}

	for(int iTrackIndex = 0; iTrackIndex < g_iAudioTrackCount; iTrackIndex++)
	{
		if(!m_tracks[iTrackIndex].Init(m_pXAudio2, m_pDevice))
		{
			return false;
		}
	}

	m_bInited = true;

	return true;
}

bool CAudioManager::CleanUp()
{
	for(int iTrackIndex = 0; iTrackIndex < g_iAudioTrackCount; iTrackIndex++)
	{
		if(!m_tracks[iTrackIndex].CleanUp())
		{
			return false;
		}
	}

	m_pXAudio2->Release();

	m_bInited = false;

	return true;
}

bool CAudioManager::PlayAudioFile(const CAudioFile& _audioFile)
{
	if(!m_tracks[m_iTrackIndex].PlayAudioFile(_audioFile))
	{
		return false;
	}

	m_iTrackIndex = (m_iTrackIndex + 1) % g_iAudioTrackCount;

	return true;
}

CAudioManager *CAudioManager::GetInstance()
{
	return m_pInstance;
}

CAudioManager *CAudioManager::m_pInstance = NULL;
