#include <cstdio>

#include "sounddevice.h"

#ifdef MY_WIN32

//extern QMutex SoundDevice::mutex1;
//extern QMutex SoundDevice::mutex2;

static int unfreedBuffers = 0;

void CALLBACK waveOutProc(HWAVEOUT sndDevice, UINT msg, DWORD_PTR dwInstance, DWORD param1, DWORD param2)
{
	//SoundDevice::mutex1.lock();
	if(msg == WOM_DONE)
	{
//		printf("\nwaveOutProc: cleaning data block");
		MMRESULT r = waveOutUnprepareHeader(sndDevice, (WAVEHDR *)param1, sizeof(WAVEHDR));
		if(r != MMSYSERR_NOERROR)
		{
			fprintf(stderr, "\nSnd: Error unpreparing data block. Code: %d", r);
		}
		free(((WAVEHDR *)param1)->lpData);
		delete (WAVEHDR *)param1;
//		--unfreedBuffers;
//		printf("\nwaveOutProc: unfreed buffers: %d", unfreedBuffers);
	}
	//SoundDevice::mutex1.unlock();
}

SoundDevice::SoundDevice()
{
	WAVEFORMAT waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 48000;
	waveFormat.nAvgBytesPerSec = 48000 * 2 * 2;
	waveFormat.nBlockAlign = 4;
	//waveFormat.nChannels = 1;
	//waveFormat.nSamplesPerSec = 8000;
	//waveFormat.nAvgBytesPerSec = 8000 * 1 * 2;
	//waveFormat.nBlockAlign = 4;
//	waveFormat.wBitsPerSample = 16;
//	waveFormat.cbSize = 0;
	
	PCMWAVEFORMAT pcmFormat;
	pcmFormat.wf = waveFormat;
	pcmFormat.wBitsPerSample = 16;
	MMRESULT r = waveOutOpen(&sndDevice, 0, (WAVEFORMATEX *)&pcmFormat, (DWORD)waveOutProc, 0, CALLBACK_FUNCTION);
	//MMRESULT r = waveOutOpen(&sndDevice, 0, (WAVEFORMATEX *)&pcmFormat, 0, 0, 0);
	if (r != MMSYSERR_NOERROR)
	{
		fprintf(stderr, "\nSnd: Error opening audio device! Code: %d", r);
	}
	
//	WAVEOUTCAPS caps;
//	waveOutGetDevCaps((UINT)sndDevice, &caps, sizeof(WAVEOUTCAPS));
}

void SoundDevice::Render(char *frame, unsigned int size, DWORD palybackRate)
{
	//mutex2.lock();
	MMRESULT r;
//	r = waveOutSetPlaybackRate(sndDevice, palybackRate);
//	if(r != MMSYSERR_NOERROR)
//	{
//		fprintf(stderr, "\nSnd: Error setting playback rate. Code: %d", r);
//	}
	//if(size == 0) return;
//	printf("\nSnd: sndFrame size: %d, rate: %f", size, (float)palybackRate/0x10000);
	WAVEHDR *header = new WAVEHDR();
	header->dwBufferLength = size;
	header->dwFlags = 0;
	header->dwLoops = 0;
	header->lpData = frame;
	r = waveOutPrepareHeader(sndDevice, header, sizeof(WAVEHDR));
	if(r != MMSYSERR_NOERROR)
	{
		fprintf(stderr, "\nSnd: Error preparing data block. Code: %d", r);
	}
	r = waveOutWrite(sndDevice, header, sizeof(WAVEHDR));
	if(r != MMSYSERR_NOERROR)
	{
		fprintf(stderr, "\nSnd: Error writing data block. Code: %d", r);
	}
	++unfreedBuffers;
	//mutex2.unlock();
//	free(frame);
}

#endif
