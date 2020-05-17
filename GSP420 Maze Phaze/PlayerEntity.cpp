#include "PlayerEntity.h"
#include "MazeGenerator.h"

PlayerEntity::PlayerEntity()
{
	pInput = NULL;
	playerSurface = NULL;

	playerSprite = NULL;
	playerFrame = 0;
	spriteStartTime = 0;

	m_xPos = 1;
	m_yPos = 1;
	m_xPosWorld = m_xPos;
	m_yPosWorld = m_yPos;
	moveOffset = 0;
	direction = 2;
	inMotion = false;
	moveStartTime = 0;
	moveSpeed = 200;
	theMaze = NULL;
}


PlayerEntity::~PlayerEntity()
{
	if (playerSurface != NULL)
	{
		delete playerSurface;
		playerSurface = NULL;
	}

	if (playerSprite != NULL)
	{
		delete playerSprite;
		playerSprite = NULL;
	}

}


bool PlayerEntity::init(RawInputHandler *input, LPDIRECT3DDEVICE9 device, MazeGenerator* maze)
{
	pInput = input;

	playerSprite = new Sprite();
	playerSprite->spriteInit(device);
	playerTexture = playerSprite->LoadTexture("assets/med/64walkstrip.bmp", D3DCOLOR_XRGB(255,255,255), device);

	//playerSurface = new Surface();
	//playerSurface->loadSurface(device, "SprPlayer.png");
	
	theMaze = maze;
	return true;
}


void PlayerEntity::SetPlayerPos(int xPos, int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
}

void PlayerEntity::render(LPDIRECT3DDEVICE9 device)
{
	playerSprite->getSpriteObj()->Begin(D3DXSPRITE_ALPHABLEND);
	playerSprite->Sprite_Animate(playerFrame, 0,3,1,spriteStartTime,90);
	playerSprite->Sprite_Draw_Frame(playerTexture, 364, 267, playerFrame, 64,64,4);
	playerSprite->getSpriteObj()->End();

	//playerSurface->render(device);
}

void PlayerEntity::SetSprite(LPDIRECT3DDEVICE9 devise, std::string filename)
{
	if (playerTexture != NULL)
	{
		playerTexture->Release();
		playerTexture = NULL;
	}
	playerTexture = playerSprite->LoadTexture(filename.c_str(), D3DCOLOR_XRGB(255,255,255), devise);

	//playerSurface->loadSurface( devise, filename.c_str() );

}

int PlayerEntity::GetPlayerPosX()
{
	return m_xPos;

}

int PlayerEntity::GetPlayerPosY()
{
	return m_yPos;
}

// new functions
void PlayerEntity::Spawn(MazeGenerator* maze)
{
	if (maze == NULL)
		return;
	theMaze = maze;
	bool startFound = false;
	for (int row = 0; row < theMaze->GetMazeHeightNodes() && !startFound; row++)
	{
		for (int col = 0; col < theMaze->GetMazeWidthNodes() && !startFound; col++)
		{
			if (theMaze->GetPoint(col, row) == 'S')
			{
				SetPos(col,row);
				startFound = true;;
			}
		}
	}

	if (!startFound)
		SetPos(1,1);
}

void PlayerEntity::SetPos(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_xPosWorld = m_xPos*theMaze->GetTileSize();
	m_yPosWorld = m_yPos*theMaze->GetTileSize();
}

bool PlayerEntity::InterpMove(int dir)
{
	if (inMotion == false)
	{
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

void PlayerEntity::Update()
{
	if (theMaze != NULL)
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
			//GridMove(direction);
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
	}
}

void PlayerEntity::AdjustMoveStartTime(int deltaPause)
{
	moveStartTime += deltaPause;
	return;
}