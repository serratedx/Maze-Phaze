#include "ZombieEntity.h"
#include "MazeGenerator.h"

ZombieEntity::ZombieEntity()
{
	pInput = NULL;
	surface = NULL;

	sprite = NULL;
	frame = 0;
	spriteStartTime = 0;

	m_xPos = 1;
	m_yPos = 1;
	m_xPosSpawn = 0;
	m_yPosSpawn = 0;
	m_xPosSpawn = 0;
	m_yPosSpawn = 0;
	m_xPosWorld = m_xPos;
	m_yPosWorld = m_yPos;
	moveOffset = 0;
	direction = 2;
	inMotion = false;
	alive = false;
	moveStartTime = 0;
	moveSpeed = 250;
	theMaze = NULL;
}

ZombieEntity::~ZombieEntity()
{
	if (surface != NULL)
	{
		delete surface;
		surface = NULL;
	}

	if (sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
}

bool ZombieEntity::init(RawInputHandler *input, LPDIRECT3DDEVICE9 device, MazeGenerator* maze)
{
	pInput = input;

	sprite = new Sprite();
	sprite->spriteInit(device);
	texture = sprite->LoadTexture("assets/med/64zWalkStrip.bmp", D3DCOLOR_XRGB(255, 255, 255), device);

	theMaze = maze;
	return true;
}

void ZombieEntity::render(LPDIRECT3DDEVICE9 device, int vLeft, int vTop)
{
	if (alive)
	{
		if (directionChanged)
		{
			switch(direction)
			{
			case UP:
				SetSprite(device, "assets/med/64ZStrip_Walkup.bmp");
				break;
			case RIGHT:
				SetSprite(device, "assets/med/64zWalkStrip.bmp");
				break;
			case DOWN:
				SetSprite(device, "assets/med/64Zstrip_Walkdown.bmp");
				break;
			case LEFT:
				SetSprite(device, "assets/med/64zWalkStrip_left.bmp");
				break;
			}
		}
		int renderX = (this->m_xPosWorld - vLeft) * 2.2727;
		int renderY = (this->m_yPosWorld - vTop) * 2.0833;

		sprite->getSpriteObj()->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Sprite_Animate(frame, 0, 3, 1, spriteStartTime, 90);
		// this m.f. line should be changed to reflect the position relative to the player
		sprite->Sprite_Draw_Frame(texture, renderX, renderY, frame, 64, 64, 4);
		sprite->getSpriteObj()->End();
	}
}

void ZombieEntity::SetSprite(LPDIRECT3DDEVICE9 device, std::string filename)
{
	if (texture != NULL)
	{
		texture->Release();
		texture = NULL;
	}
	texture = sprite->LoadTexture(filename.c_str(), D3DCOLOR_XRGB(255,255,255), device);
}

int ZombieEntity::GetPosX()
{
	return m_xPos;
}

int ZombieEntity::GetPosY()
{
	return m_yPos;
}

void ZombieEntity::Spawn(MazeGenerator* maze, char spawnCode)
{
	if (maze== NULL)
		return;
	theMaze = maze;
	bool spawnFound = false;
	bool destFound = false;
	for (int row = 0; row < theMaze->GetMazeHeightNodes() && (!spawnFound || !destFound); row++)
	{
		for (int col = 0; col < theMaze->GetMazeWidthNodes() && (!spawnFound || !destFound); col++)
		{
			if (theMaze->GetPoint(col, row) == spawnCode)
			{
				if (!spawnFound)
				{
					m_xPosSpawn = col;
					m_yPosSpawn = row;
					spawnFound = true;
				}
				else if (!destFound)
				{
					m_xPosDest = col;
					m_yPosDest = row;
					destFound = true;
				}
			}
		}
	}

	if (!spawnFound)
		SetPos(1,1);
	else
	{
		SetPos(m_xPosSpawn, m_yPosSpawn);
		alive = true;
	}
}

void ZombieEntity::SetPos(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_xPosWorld = m_xPos*theMaze->GetTileSize();
	m_yPosWorld = m_yPos*theMaze->GetTileSize();
}

bool ZombieEntity::InterpMove(int dir)
{
	if (inMotion == false)
	{
		directionChanged = false;
		if (direction != dir) directionChanged = true;
		direction = dir;
		switch (direction)
		{
		case UP:
			if (theMaze->GetPoint(m_xPos, m_yPos-1) != 'W')
			{
				m_yPos -= 1;
			}
			else return false;
			break;

		case DOWN:
			if (theMaze->GetPoint(m_xPos, m_yPos+1) != 'W')
			{
				m_yPos += 1;
			}
			else return false;
			break;

		case LEFT:
			if (theMaze->GetPoint(m_xPos-1, m_yPos) != 'W')
			{
				m_xPos -= 1;
			}
			else return false;
			break;

		case RIGHT:
			if (theMaze->GetPoint(m_xPos+1, m_yPos) != 'W')
			{
				m_xPos += 1;
			}
			else return false;
			break;
		default:
			this->direction = DOWN;
			return false;
		}
		inMotion = true;
		moveStartTime = GetTickCount();
	}
	return true;
}

void ZombieEntity::Update()
{
	if (theMaze != NULL && alive)
	{
		int curTime = GetTickCount();
		if (inMotion == true)
		{
			moveOffset = curTime - moveStartTime;
			if (moveOffset >= moveSpeed)
			{
				inMotion = false;
				moveOffset = moveSpeed;
			}
			switch (direction)
			{
			case UP:
				m_yPosWorld = (m_yPos+1)*theMaze->GetTileSize() - (moveOffset*theMaze->GetTileSize())/moveSpeed;
				break;
			case DOWN:
				m_yPosWorld = (m_yPos-1)*theMaze->GetTileSize() + (moveOffset*theMaze->GetTileSize())/moveSpeed;
				break;
			case LEFT:
				m_xPosWorld = (m_xPos+1)*theMaze->GetTileSize() - (moveOffset*theMaze->GetTileSize())/moveSpeed;
				break;
			case RIGHT:
				m_xPosWorld = (m_xPos-1)*theMaze->GetTileSize() + (moveOffset*theMaze->GetTileSize())/moveSpeed;
				break;
			}
		}
		else if (m_xPosDest > 0 && m_xPosDest < 9999 &&
				 m_yPosDest > 0 && m_yPosDest < 9999 )
		{
			static bool moveToDest = true;
			if (m_xPos == m_xPosSpawn && m_yPos == m_yPosSpawn)
			{
				moveToDest = true;
			}
			else if (m_xPos == m_xPosDest && m_yPos == m_yPosDest)
			{
				moveToDest = false;
			}

			if (moveToDest)
			{
				if (m_xPos > m_xPosDest)
					InterpMove(LEFT);
				else if (m_xPos < m_xPosDest)
					InterpMove(RIGHT);

				if (m_yPos > m_yPosDest)
					InterpMove(UP);
				else if (m_yPos < m_yPosDest)
					InterpMove(DOWN);
			}
			else
			{
				if (m_xPos > m_xPosSpawn)
					InterpMove(LEFT);
				else if (m_xPos < m_xPosSpawn)
					InterpMove(RIGHT);

				if (m_yPos > m_yPosSpawn)
					InterpMove(UP);
				else if (m_yPos < m_yPosSpawn)
					InterpMove(DOWN);
			}
		}
	}
}

void ZombieEntity::AdjustMoveStartTime(int deltaPause)
{
	moveStartTime += deltaPause;
	return;
}