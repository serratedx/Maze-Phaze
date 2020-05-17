#pragma once
#include "InputCore.h"
#include "2dSurface.h"
#include "Sprites.h"
class RawInputHandler;
class MazeGenerator;

class PlayerEntity
{
private:
	
	RawInputHandler* pInput;
	int playerFrame;
	int spriteStartTime;
	LPDIRECT3DTEXTURE9 playerTexture;

public:

	PlayerEntity();
	~PlayerEntity();
	
	bool init(RawInputHandler* input, LPDIRECT3DDEVICE9 device, MazeGenerator* maze);
	void SetPlayerPos(int xPos, int yPos);
	void render(LPDIRECT3DDEVICE9 device);
	void SetSprite(LPDIRECT3DDEVICE9 devise, std::string filename);
	int GetPlayerPosX();
	int GetPlayerPosY();

	// new functions
	void Spawn(MazeGenerator* maze);
	void SetPos(int x, int y);
	//bool GridMove(int direction);
	bool InterpMove(int dir);
	void Update();
	void AdjustMoveStartTime(int deltaPause); // used for pausing while interpmoving
	
	Surface* playerSurface;
	Sprite* playerSprite;

	int m_xPos, m_yPos;
	int m_xPosWorld, m_yPosWorld;
	int direction;
	int moveOffset;
	bool inMotion;
	int moveStartTime;
	int moveSpeed;	// ms to move 1 tile
	MazeGenerator* theMaze;

};
