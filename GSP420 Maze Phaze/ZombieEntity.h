#pragma once
#include "InputCore.h"
#include "2dSurface.h"
#include "Sprites.h"
class RawInputHandler;
class MazeGenerator;

class ZombieEntity
{
private:
	
	RawInputHandler* pInput;
	int frame;
	int spriteStartTime;
	LPDIRECT3DTEXTURE9 texture;

public:

	ZombieEntity();
	~ZombieEntity();
	
	bool init(RawInputHandler* input, LPDIRECT3DDEVICE9 device, MazeGenerator* maze);
	void render(LPDIRECT3DDEVICE9 device, int vLeft, int vTop);
	void SetSprite(LPDIRECT3DDEVICE9 devise, std::string filename);
	int GetPosX();
	int GetPosY();

	// new functions
	void Spawn(MazeGenerator* maze, char spawnCode);
	void SetPos(int x, int y);
	//bool GridMove(int direction);
	bool InterpMove(int dir);
	void Update();
	void AdjustMoveStartTime(int deltaPause); // used for pausing while interpmoving
	
	Surface* surface;
	Sprite* sprite;

	int m_xPos, m_yPos;
	int m_xPosWorld, m_yPosWorld;
	int m_xPosSpawn, m_yPosSpawn;
	int m_xPosDest, m_yPosDest;
	int direction;
	int moveOffset;
	bool inMotion;
	bool alive;
	bool directionChanged;
	int moveStartTime;
	int moveSpeed;	// ms to move 1 tile
	MazeGenerator* theMaze;

};
