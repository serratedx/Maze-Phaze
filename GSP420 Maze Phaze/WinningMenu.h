#pragma once

#include <windows.h>
#include <d3d9.h>

class Surface;
class RawInputHandler;
class SoundCore;
class dxText;


class WinningMenu{
public:

	WinningMenu();
	~WinningMenu();

	bool init(int sec, int min, RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device);
	void update();
	void render(LPDIRECT3DDEVICE9 device);
	int getMessage();
	//Message Enumerations
	enum { CONTINUE = 1 };

private:
	RawInputHandler* myInputC;
	SoundCore*	Sounds;
	Surface* background;
	Surface* WinMenuButton;
	Surface* WinMenuButtonSelected;
	dxText* timeText;
	
	int message;		//holds enum value to send to GameMain
	int winMenuSelect;		//holds the sound buffer id
	int winMenuMusic;		//holds the sound buffer id

	// values for displaying text
	int seconds;
	int minutes;


};
