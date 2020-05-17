
#include "SoundCore.h"

SoundCore::SoundCore()
{
	directSoundManager = NULL;
}


SoundCore::~SoundCore()
{
	if (directSoundManager)
	{
		delete directSoundManager;
		directSoundManager = NULL;
	}
	
	if (!sounds.empty())
	{
		for (unsigned int i=0; i < sounds.size(); i++)
		{
			if (sounds[i])
			{
				delete sounds[i];
			}
		}
	}
}

//
//   FUNCTION: init(HWND wndHandle)
//
//   PURPOSE: Creates a DirectSound object
//		
bool SoundCore::init(HWND wndHandle)
{

	// create the direct sound manager
	directSoundManager = new CSoundManager();
	HRESULT	hr;
	//initilize the sound buffer
	hr = directSoundManager->Initialize(wndHandle, DSSCL_PRIORITY);

	if FAILED(hr)
	{
		return false;
	}

	return true;
}
//
//   FUNCTION: loadFile(std::string filename)
//
//   PURPOSE: Load a wav file into a direct sound buffer using CWaveFile
//	The store this buffer in our sounds vector, return the sound number
//		
int	SoundCore::loadFile(std::string filename)
{
	CSound* tempSound = NULL;  //holds sound buffer
	HRESULT hr;  //reuslt variable

	// make the filename usable by the sound manager
	char* file = (char*)filename.c_str();

	//set buffer format
	directSoundManager->SetPrimaryBufferFormat( 2, 22050, 16);

	//create the sound buffer from the file
	directSoundManager->Create( &tempSound, file, 0, GUID_NULL );

	//save loaded buffer into the sounds vector
	sounds.push_back((CSound*) tempSound);

	//return the index number of the saved buffer
	return (int)(sounds.size() - 1);
}

void SoundCore::popBuffers(int count)
{
	for (int i = 0; i < count; i++)
	{
		if (sounds[sounds.size() - 1])
		{
			delete sounds[sounds.size() - 1];
			sounds.pop_back();
		}
	}
}

//
//   FUNCTION: playSound(int bufferNumber)
//
//   PURPOSE: starts the playback of the sound in the buffer
//	
void SoundCore::playSound(int bufferNumber, int volume)
{

	sounds[bufferNumber]->Play(0, 0, volume);
}

//
//   FUNCTION: loopSound(int bufferNumber)
//
//   PURPOSE: starts the playback of the sound in the buffer, looping until stopped
//	
void SoundCore::loopSound(int bufferNumber, int volume)
{

	sounds[bufferNumber]->Stop();
	sounds[bufferNumber]->Play(0, DSBPLAY_LOOPING, volume );
}

//
//   FUNCTION: stopSound(int bufferNumber)
//
//   PURPOSE: stops buffer playback
//	
void SoundCore::stopSound(int bufferNumber)
{

	sounds[bufferNumber]->Stop();
}

// 
//	 FUNCTION: resetSound()
//
//	 PURPOSE: reset buffer
void SoundCore::resetSound(int bufferNumber)
{

	sounds[bufferNumber]->Reset();
}

bool SoundCore::IsSoundPlaying(int bufferNumber)
{
	return sounds[bufferNumber]->IsSoundPlaying();
}