#pragma once

#include <dsound.h>
#include <string>
#include "dsutil.h"
#include <vector>

class SoundCore
{

public:

	SoundCore();
	~SoundCore();

	bool init(HWND wndHandle);
	int loadFile(std::string filename);
	void popBuffers(int count);
	void playSound(int bufferNumber, int volume);
	void loopSound(int bufferNumber, int volume);
	void stopSound(int bufferNumber);
	void resetSound(int bufferNumber);
	bool IsSoundPlaying(int bufferNumber);

private:
	LPDIRECTSOUND8	directSound;		// creates the DirectSound Device
	CSoundManager*	directSoundManager;

	std::vector	<CSound*> sounds;  // hold sound buffers
};