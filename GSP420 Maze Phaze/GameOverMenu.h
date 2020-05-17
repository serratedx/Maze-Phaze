#pragma once

#include <windows.h>
#include <d3d9.h>

class Surface;
class RawInputHandler;
class SoundCore;


class GameOverMenu{
public:

	GameOverMenu();
	~GameOverMenu();

	bool init(RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device);
	void update();
	void render(LPDIRECT3DDEVICE9 device);
	int getMessage();
	//Message Enumerations
	enum { RESTART = 1, MAIN_MENU = 2 };

private:
	RawInputHandler* myInputC;
	SoundCore*	mySounds;
	Surface* background;
	Surface* restartGame;
	Surface* restartGameHL;
	Surface* mainMenu;
	Surface* mainMenuHL;
	
	int menuItemSelected;
	int message;
	int go_menuSwitch;
	int go_menuSelect;
	int go_menuMusic;


};
