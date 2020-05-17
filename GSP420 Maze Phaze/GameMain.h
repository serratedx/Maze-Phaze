#pragma once

#include <windows.h>
class dxManager;
class Surface;
class MazeMenu;
class WinningMenu;
class MazePhaze;
class FrameTimer;
class RawInputHandler;
class SoundCore;
class PlayerEntity;
class GameOverMenu;
class dxText;



class GameMain
{
public:
	GameMain(void);
	~GameMain(void);

	bool init(HWND wndHandle, HINSTANCE hInst);
	void update();
	bool CheckWndMsg(UINT uMsg,WPARAM wParam,LPARAM lParam);

	enum { NOTHING = 0, MENU = 1, GAME = 2, WIN = 3, GAME_OVER = 4 };
	
	


private:

	FrameTimer* timer;
	dxManager* dxMgr;
	MazeMenu* menu;
	WinningMenu* winMenu;
	MazePhaze* mazePhaze;	
	RawInputHandler* inputC;
	SoundCore*	sound;
	PlayerEntity* player;
	GameOverMenu* gameOver;

	int active;
	int level;

	// hold values for the timeElapsed from MazePhaze
	int seconds;
	int minutes;
};