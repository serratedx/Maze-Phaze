#pragma once

class Surface;
class RawInputHandler;
class SoundCore;
class PlayerEntity;
class MazeGenerator;
class dxText;
class ZombieEntity;

class MazePhaze
{
public:

	MazePhaze();
	~MazePhaze();

	bool init(int gameLevel, RawInputHandler* input, SoundCore* sound, PlayerEntity* player, LPDIRECT3DDEVICE9 device);
	void updateFrames(int numberOfFrames);
	void render(LPDIRECT3DDEVICE9 device);
	void ChangeLevel(int level, LPDIRECT3DDEVICE9 device);
	int getMessage();
	int getSeconds();
	int getMinutes();
	enum {NO_MESSAGE = 0, GAME_OVER = 1, QUIT = 2, WIN = 3};
	

private:
	Surface* background;

	// pause menu stuff
	Surface* pauseBackground;
	Surface* pauseResume;
	Surface* pauseResumeHL;
	Surface* pauseExit;
	Surface* pauseExitHL;
	Surface* instructionMenu;

	// other cores
	RawInputHandler* myInputC;
	SoundCore*	mySounds;
	PlayerEntity* playerOne;
	ZombieEntity* theZombies[10];

	// screen text stuff
	dxText* text;
	dxText* timeElapsed;
	dxText* instruction;

	int message;
	int gameMusic;
	int gameSound;
	int gameRunning;

	int menuItemSelected;
	
	// need sound buffers for pause menu selection and switch
	int pauseSwitch;
	int pauseSelect;

	int currentLevel;

	bool playerMoving;
	bool paused;
	bool isFirstLevel;

	// elapsed time stuff
	int count;
	int seconds;
	int minutes;

	// maze level related stuffzors
	MazeGenerator *theMazeLevel;
	IDirect3DSurface9* mapBuffer; 
	Surface* tile_floor;
	Surface* tile_wall;
	Surface* tile_start;
	Surface* tile_finish;
	
	void BuildMaze(LPDIRECT3DDEVICE9 device);
	void DrawTile(int x, int y, LPDIRECT3DDEVICE9 device, Surface* source);
};