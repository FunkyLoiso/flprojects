#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#define MY_WIN32

#ifdef MY_WIN32
#include <windows.h>
#include <QMutex>

#endif

#include "../commonclasses/common.h"

class SoundDevice : public ISoundRenderer
{
public:
	SoundDevice();
	void Render(char *samples, unsigned int size, DWORD playbackRate);
	void Reset(){waveOutReset(sndDevice);};
	
	static QMutex mutex1;
	static QMutex mutex2;
	
private:
#ifdef MY_WIN32
	HWAVEOUT sndDevice;
#endif
};

#endif /*SOUNDDEVICE_H*/
