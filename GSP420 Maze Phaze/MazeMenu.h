#pragma once

#include <windows.h>
#include <d3d9.h>

class Surface;
class RawInputHandler;
class SoundCore;
class dxText;


class MazeMenu{
public:

	MazeMenu();
	~MazeMenu();

	bool init(RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device);
	void update();
	void render(LPDIRECT3DDEVICE9 device);
	int getMessage();
	//Message Enumerations
	enum { NEW_GAME = 1, EXIT = 2 };

private:
	RawInputHandler* myInputC;
	SoundCore*	mySounds;
	Surface* background;
	Surface* newGame;
	Surface* newGameHighlighted;
	Surface* exit;
	Surface* exitHighlighted;

	// text pointers
	dxText* instructions;
	
	int menuItemSelected;
	int message;
	int menuSwitch;
	int menuSelect;
	int menuMusic;


};
