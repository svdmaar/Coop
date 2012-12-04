#include "AudioTrack.h"

#include "Logger.h"

CAudioTrack::CAudioTrack()
{
	m_bInited = false;
	m_pXAudio2 = NULL;
	m_pVoice = NULL;
	m_pDevice = NULL;

	ZeroMemory(&m_currentFormat, sizeof(m_currentFormat));
}

CAudioTrack::~CAudioTrack()
{

}

bool CAudioTrack::Init(IXAudio2 *_pXAudio2, IXAudio2MasteringVoice *_pAudioDevice)
{
	m_pXAudio2 = _pXAudio2;
	m_pDevice = _pAudioDevice;

	m_bInited = true;

	return true;
}

bool CAudioTrack::CleanUp()
{
	if(m_pVoice != NULL)
	{
		m_pVoice->DestroyVoice();
		m_pVoice = NULL;
	}

	m_bInited = false;

	return true;
}

bool CAudioTrack::PlayAudioFile(const CAudioFile& _audioFile)
{
	HRESULT hr;
	WAVEFORMATEX waveFormat = _audioFile.GetFormat();

	if(!_SameAsCurrentFormat(waveFormat))
	{
		if(m_pVoice != NULL)
		{
			m_pVoice->DestroyVoice();
		}

		hr = m_pXAudio2->CreateSourceVoice(&m_pVoice, &waveFormat);
		if(FAILED(hr))
		{
			LogErrorHr("Failed to create source voice", hr);
			return false;
		}

		m_currentFormat = waveFormat;
	}

	DWORD dwSampleSize = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
	DWORD dwSampleCount = _audioFile.GetBufferSize() / dwSampleSize;

	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(buffer));

    buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = _audioFile.GetBufferSize();
	buffer.pAudioData = (const BYTE *)_audioFile.GetData();
    buffer.PlayBegin = 0;
    buffer.PlayLength = dwSampleCount;
    buffer.LoopBegin = 0;
    buffer.LoopLength = dwSampleCount;
    buffer.LoopCount = 0;
    buffer.pContext = NULL;

	hr = m_pVoice->SubmitSourceBuffer(&buffer);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to submit source buffer", hr);
		return false;
	}

	hr = m_pVoice->Start(0);
	if(FAILED(hr))
	{
		LogErrorHr("Failed to start source voice", hr);
		return false;
	}

	return true;
}

bool CAudioTrack::_SameAsCurrentFormat(const WAVEFORMATEX& _format)
{
	return (memcmp(&m_currentFormat, &_format, sizeof(WAVEFORMATEX)) == 0);
}
