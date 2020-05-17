#include "2dSurface.h"
#include "MazePhaze.h"
#include "InputCore.h"
#include "SoundCore.h"
#include "DebugHeader.h"
#include "PlayerEntity.h"
#include "MazeGenerator.h"
#include "dxText.h"
#include "ZombieEntity.h"

int maxLevel = 4;

MazePhaze::MazePhaze()
{
	background = NULL;
	pauseBackground = NULL;
	pauseResume = NULL;
	pauseResumeHL = NULL;
	pauseExit= NULL;
	pauseExitHL = NULL;
	instructionMenu = NULL;
	//currentLevel = 0;
	myInputC = NULL;
	mySounds = NULL;
	playerOne= NULL;
	for (int i = 0; i < 10; i++)
	{
		theZombies[i] = NULL;
	}

	theMazeLevel = NULL;
	tile_floor = NULL;
	tile_wall = NULL;
	mapBuffer = NULL;

	paused = false;
	menuItemSelected = 0;

	// text drawing
	text = NULL;
	timeElapsed = NULL;
	instruction = NULL;
}

MazePhaze::~MazePhaze()
{
	//if (background != NULL) { delete background; background = NULL; }

	if (pauseBackground != NULL) { delete pauseBackground; pauseBackground = NULL; }
	if (pauseResume != NULL) { delete pauseResume; pauseResume = NULL; }
	if (pauseResumeHL != NULL) { delete pauseResumeHL; pauseResumeHL = NULL; }
	if (pauseExit != NULL) { delete pauseExit; pauseExit = NULL; }
	if (pauseExitHL != NULL) { delete pauseExitHL; pauseExitHL = NULL; }
	if (instructionMenu != NULL){ delete instructionMenu; instructionMenu = NULL; }

	if (myInputC != NULL) { myInputC = NULL; }
	if (playerOne != NULL) { delete playerOne; playerOne = NULL; }
	
	for (int i = 0; i < 10; i++)
	{
		if (theZombies[i] != NULL) { delete theZombies[i]; theZombies[i] = NULL; }
	}
	
	if (mapBuffer != NULL){mapBuffer->Release(); mapBuffer = NULL;}

	if (theMazeLevel != NULL){delete theMazeLevel; theMazeLevel = NULL;}

	if (tile_wall != NULL){delete tile_wall; tile_wall = NULL;}
	if (tile_floor != NULL){delete tile_floor; tile_floor = NULL;}
	if (tile_start != NULL){delete tile_start; tile_start = NULL;}
	if (tile_finish != NULL){delete tile_finish; tile_finish = NULL;}

	// text drawing destructor calls
	if (text != NULL){delete text; text = NULL;}
	if (timeElapsed != NULL){delete timeElapsed; timeElapsed = NULL;}
	if (instruction != NULL){delete instruction; instruction = NULL;}

	if (mySounds != NULL) { mySounds->popBuffers(5); mySounds = NULL; }
}

bool MazePhaze::init(int gameLevel, RawInputHandler* input, SoundCore* sound, PlayerEntity* player, LPDIRECT3DDEVICE9 device)
{	
	currentLevel = gameLevel;

	if (currentLevel == 0)
	{
		isFirstLevel = true;
	}
	else isFirstLevel = false;

	// from InputCore.h
	myInputC = input;

	mySounds = sound;

	message = NO_MESSAGE;
	count = 0;
	seconds = 0;
	minutes = 0;

	//load the background image
	/*background = new Surface();
	background->loadSurface(device, "bg_Level1.png");*/

	// need to initilize and load pause menu surfaces
	pauseBackground = new Surface();
	pauseBackground->loadSurface(device, "bg_PauseMenu.png");

	pauseResume = new Surface();
	pauseResume->loadSurface(device, "bg_PauseMenuResume.png");
	pauseResume->setPosition(335, 300);

	pauseResumeHL = new Surface();
	pauseResumeHL->loadSurface(device, "bg_PauseMenuResumeHL.png");
	pauseResumeHL->setPosition(335, 300);

	pauseExit = new Surface();
	pauseExit->loadSurface(device, "bg_PauseMenuExit.png");
	pauseExit->setPosition(335, 350);

	pauseExitHL = new Surface();
	pauseExitHL->loadSurface(device, "bg_PauseMenuExitHL.png");
	pauseExitHL->setPosition(335, 350);

	instructionMenu = new Surface();
	instructionMenu->loadSurface(device, "bg_instructionsMenu.png");
	instructionMenu->setPosition(200, 150);

	gameMusic = mySounds->loadFile("MusicMono.wav");
	gameSound = mySounds->loadFile("Slap.wav");
	gameRunning = mySounds->loadFile("s_running.wav");
	pauseSwitch = mySounds->loadFile("s_menuSwitch.wav");
	pauseSelect = mySounds->loadFile("s_menuSelect.wav");
	mySounds->loopSound(gameMusic, -5000);

	ChangeLevel(currentLevel, device);

	// maze related stuffs
	char filename[256];
	int tLevel = currentLevel + 1;
	int result = sprintf(filename, "MazePhaze_Level_%i.txt", tLevel);
	theMazeLevel = new MazeGenerator(filename);

	// player related stuffs
	playerOne = new PlayerEntity();
	playerOne->init(input, device, theMazeLevel);
	playerOne->SetPlayerPos(200, 200);
	playerOne->Spawn(theMazeLevel);

	char zombieNumber[16];
	for (int i = 0; i < 10; i++)
	{
		sprintf(zombieNumber, "%i", i);
		theZombies[i] = new ZombieEntity();
		theZombies[i]->init(input, device, theMazeLevel);
		theZombies[i]->SetPos(200, 200);
		theZombies[i]->Spawn(theMazeLevel, zombieNumber[0]);
	}

	// tiles
	int tPercentScale = theMazeLevel->GetTileSize() * 100 / 32;
	tile_floor = new Surface();
	tile_floor->loadSurface(device, "tile_floor1.png");
	tile_floor->setSize(tPercentScale);
	tile_wall = new Surface();
	tile_wall->loadSurface(device, "tile_wall1.png");
	tile_wall->setSize(tPercentScale);
	tile_start = new Surface();
	tile_start->loadSurface(device, "tile_start1.png");
	tile_start->setSize(tPercentScale);
	tile_finish = new Surface();
	tile_finish->loadSurface(device, "tile_finish1.png");
	tile_finish->setSize(tPercentScale);

	// text stuff
	text = new dxText();
	text->init(24, device);

	timeElapsed = new dxText();
	timeElapsed->init(24, device);

	instruction = new dxText();
	instruction->init(20, device);

	// map buffer
	BuildMaze(device);

	return true;
}

void MazePhaze::updateFrames(int numberOfFrames)
{
	static int pauseTime;
	static int unPauseTime;
	
	if (isFirstLevel != true)
	{
		if (paused == false)
		{
			playerMoving = false;

			if (myInputC->GetRawInputState(VK_LEFT)) 
			{
				//playerOne->m_xPos -= 3;
				playerOne->InterpMove(LEFT);
				mySounds->loopSound(gameRunning, 0);
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_RIGHT))
			{
				//playerOne->m_xPos += 3;
				playerOne->InterpMove(RIGHT);
				mySounds->loopSound(gameRunning, 0);
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_UP))
			{	
				//playerOne->m_yPos -= 3;
				playerOne->InterpMove(UP);
				mySounds->loopSound(gameRunning, 0);
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_DOWN))
			{
				//playerOne->m_yPos += 3;
				playerOne->InterpMove(DOWN);
				mySounds->loopSound(gameRunning, 0);
				playerMoving = true;
			}

			if (myInputC->GetRawInputDownEvent(VK_ESCAPE) && !playerOne->inMotion) 
			{
				mySounds->stopSound(gameMusic);
				mySounds->stopSound(gameRunning);
				mySounds->playSound(gameSound, 0);

				paused = true;
				pauseTime = GetTickCount();
			}

			if (playerMoving == false)
			{	
				mySounds->stopSound(gameRunning);
			}

			// creates a 32x32 space that acts as a "WIN" condition
			//if (playerOne->m_xPos > 384 && playerOne->m_xPos < 416 && playerOne->m_yPos > 284 && playerOne->m_yPos < 316)
			if (myInputC->GetRawInputDownEvent(VK_F1) || 
				theMazeLevel->GetPoint(playerOne->GetPlayerPosX(), playerOne->GetPlayerPosY()) == 'F')
			{
				mySounds->stopSound(gameMusic);
				mySounds->stopSound(gameRunning);
				delete playerOne->playerSurface;
				playerOne->playerSurface = NULL;
				message = WIN;
			}

			bool playerDied = false;
			for (int i = 0; i < 10; i++)
			{
				// yeah this is the oh-so intelligent collision detection!
				if (playerOne->GetPlayerPosX() == theZombies[i]->GetPosX() && playerOne->GetPlayerPosY() == theZombies[i]->GetPosY() && !theZombies[i]->inMotion) playerDied = true;
			}
			if (myInputC->GetRawInputDownEvent(VK_F2) || playerDied)
			{
				mySounds->stopSound(gameMusic);
				mySounds->stopSound(gameRunning);
				delete playerOne->playerSurface;
				playerOne->playerSurface = NULL;
				message = GAME_OVER;
			}

			if (!paused)
			{
				playerOne->Update();
				for (int i = 0; i < 10; i++)
				{
					theZombies[i]->Update();
				}
			}
			count = ((count + 1)%60);
			if (count >= 59)
			{
				count = 0;
				seconds++;
				if (seconds >= 59)
				{
					seconds = 0;
					minutes+= 10;
				}
			}
		}
		else
		{
			bool soundPlaying = false;
			//get menu movement input
			if (myInputC->GetRawInputDownEvent(VK_DOWN) || myInputC->GetRawInputDownEvent(VK_UP))
			{
				mySounds->resetSound(pauseSwitch);
				mySounds->playSound(pauseSwitch, 0);
				if (menuItemSelected == 0){
					menuItemSelected = 1;
				} else {
					menuItemSelected = 0;
				}
			}

			//get menu pressed input
			if (myInputC->GetRawInputDownEvent(VK_RETURN))
			{
				mySounds->playSound(pauseSelect, 0);

				do
				{
					soundPlaying = mySounds->IsSoundPlaying(pauseSelect);  
				}while (soundPlaying != false);

				switch (menuItemSelected){
				case 0: { paused = false; unPauseTime = GetTickCount(); playerOne->AdjustMoveStartTime(unPauseTime - pauseTime); mySounds->loopSound(gameMusic, 0); 
					for (int i = 0; i < 10; i++)
						theZombies[i]->AdjustMoveStartTime(unPauseTime - pauseTime);
						break; }
				case 1: { message = QUIT; break; }
				}	
			}
		}
	}
	else
	{
		bool soundPlaying = false;
		//get menu pressed input
		if (myInputC->GetRawInputDownEvent(VK_ESCAPE))
		{
			mySounds->playSound(pauseSelect, 0);

			do
			{
				soundPlaying = mySounds->IsSoundPlaying(pauseSelect);  
			}while (soundPlaying != false);
			isFirstLevel = false;
		}
		// some value needs to be changed here to exit statement
		
		unPauseTime = GetTickCount(); 
		playerOne->AdjustMoveStartTime(unPauseTime - pauseTime); 
		mySounds->loopSound(gameMusic, 0);
	}
}

void MazePhaze::render(LPDIRECT3DDEVICE9 device)
{
	int level = currentLevel;

	if (isFirstLevel != true)
	{
		if (paused == false)
		{
			/*if (myInputC->GetRawInputDownEvent('32'))
			{
			if (level >= maxLevel)
			{
			level = maxLevel;
			}
			else
			level++;

			ChangeLevel(level, device);
			}
			if (myInputC->GetRawInputDownEvent('31'))
			{
			if (level <= 0)
			{
			level = 0;
			}
			else
			level--;
			ChangeLevel(level, device);
			}*/

			//render the background
			//background->render(device);
			//render any other object here

			// render the map first
			IDirect3DSurface9* backbuffer = NULL;
			device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
			//RECT r1 = {0, 0, 11*32, 9*32};
			//RECT r1 = {(playerOne->GetPlayerPosX()-5)*32, (playerOne->GetPlayerPosY()-4)*32, 
			//(playerOne->GetPlayerPosX()-5)*32 + 11*32, (playerOne->GetPlayerPosY()-4)*32 + 9*32};
			RECT r1 = {playerOne->m_xPosWorld - 5*32, playerOne->m_yPosWorld - 4*32, playerOne->m_xPosWorld + 6*32, playerOne->m_yPosWorld + 5*32};
			RECT r2 = {0, 0, 800, 600};
			HRESULT result = device->StretchRect(mapBuffer, &r1, backbuffer, &r2, D3DTEXF_NONE);

			backbuffer->Release();
			backbuffer = NULL;

			if (result != D3D_OK)
			{
				//debug_con << "Error rendering map" << "";
			}
			// end map render

			playerMoving= false;

			if (myInputC->GetRawInputState(VK_LEFT)) 
			{
				playerOne->SetSprite(device, "assets/med/64walkstrip_left.bmp");
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_RIGHT))
			{
				playerOne->SetSprite(device, "assets/med/64walkstrip.bmp");
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_UP))
			{	
				playerOne->SetSprite(device, "assets/med/64CStrip_Walkup.bmp");
				playerMoving = true;
			}

			if (myInputC->GetRawInputState(VK_DOWN))
			{
				playerOne->SetSprite(device, "assets/med/64CStrip_Walkup.bmp");
				playerMoving = true;
			}

			if (playerMoving == false)
			{
				playerOne->SetSprite(device, "assets/med/64CStrip_Walkup.bmp");
			}

			//playerOne->playerSurface->setPosition(playerOne->m_xPos, playerOne->m_yPos);
			//playerOne->playerSurface->setPosition(32*11+8, 32*8+8);
			//playerOne->playerSurface->setPosition(364, 267);

			//render player object
			playerOne->render(device);
			for (int i = 0; i < 10; i++)
			{
				theZombies[i]->render(device, r1.left, r1.top);
			}



		}
		else  // if game is paused
		{
			//background->render(device);

			// render the map first
			IDirect3DSurface9* backbuffer = NULL;
			device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
			RECT r1 = {playerOne->m_xPosWorld - 5*32, playerOne->m_yPosWorld - 4*32, playerOne->m_xPosWorld + 6*32, playerOne->m_yPosWorld + 5*32};
			RECT r2 = {0, 0, 800, 600};
			HRESULT result = device->StretchRect(mapBuffer, &r1, backbuffer, &r2, D3DTEXF_NONE);

			backbuffer->Release();
			backbuffer = NULL;

			//playerOne->playerSurface->setPosition(364, 267);
			playerOne->render(device);
			for (int i = 0; i < 10; i++)
			{
				theZombies[i]->render(device, r1.left, r1.top);
			}
			pauseBackground->setPosition(200, 150);
			pauseBackground->render(device);

			//render menu items based on which is highlighted
			if (menuItemSelected == 0){
				pauseResumeHL->render(device);
				pauseExit->render(device);
			} else {
				pauseResume->render(device);
				pauseExitHL->render(device);
			}

		}
	}
	else
	{
		// render the map first
		IDirect3DSurface9* backbuffer = NULL;
		device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		RECT r1 = {playerOne->m_xPosWorld - 5*32, playerOne->m_yPosWorld - 4*32, playerOne->m_xPosWorld + 6*32, playerOne->m_yPosWorld + 5*32};
		RECT r2 = {0, 0, 800, 600};
		HRESULT result = device->StretchRect(mapBuffer, &r1, backbuffer, &r2, D3DTEXF_NONE);

		backbuffer->Release();
		backbuffer = NULL;

		//playerOne->playerSurface->setPosition(364, 267);
		playerOne->render(device);
		
		// render instructions surface
		instructionMenu->setPosition(200, 150);
		instructionMenu->render(device);
		
		// render text inside surface
		char instructions[200] = "";
		sprintf(instructions, "UP / DOWN / LEFT / RIGHT ----> Player Movement\n\n\nESC ----> Pause\n\n\nENTER ----> Accept/Select\n\n\n\n\nPress 'ESC' to PLAY" );
		instruction->drawText(instructions, 215, 170, 150, 100, 0xFF000000);
		
								
	}

		// draw text
		char textOut[25] = "";
		sprintf(textOut, "Level: %d", level+1);
		text->drawText(textOut, 10, 5, 100, 50, 0xFFFFFFFF);

		// draw timeElapsed Text
		char textTimer[25] = "";
		sprintf(textTimer, "%02i:%02i", minutes, seconds);
		timeElapsed->drawText(textTimer, 400, 5, 100, 50, 0xFFFFFFFF);

}

void MazePhaze::ChangeLevel(int level, LPDIRECT3DDEVICE9 device)
{
	currentLevel = level;
	switch (level) {
	case 0: { /*background->loadSurface(device, "bg_Level1.png");*/ playerOne->Spawn(theMazeLevel);break; }
	case 1: { /*background->loadSurface(device, "bg_Level2.png");*/ playerOne->Spawn(theMazeLevel); break; }
	case 2: { /*background->loadSurface(device, "bg_Level3.png");*/ playerOne->Spawn(theMazeLevel); break; }
	case 3: { /*background->loadSurface(device, "bg_Level4.png");*/ playerOne->Spawn(theMazeLevel); break; }
	case 4: { /*background->loadSurface(device, "bg_Level5.png");*/ playerOne->Spawn(theMazeLevel); break; }
	}
}

int MazePhaze::getMessage()
{
	return message;
}

int MazePhaze::getSeconds()
{
	return seconds;
}

int MazePhaze::getMinutes()
{
	return minutes;
}

void MazePhaze::BuildMaze(LPDIRECT3DDEVICE9 device)
{
	int mapWidth = theMazeLevel->GetMazeWidthNodes() * theMazeLevel->GetTileSize();
	int mapHeight = theMazeLevel->GetMazeHeightNodes() * theMazeLevel->GetTileSize();

	HRESULT result = device->CreateOffscreenPlainSurface(
		mapWidth,
		mapHeight,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&mapBuffer,
		NULL);

	if (result != D3D_OK)
	{
		debug_con << "Error creating map surface" << "";
	}

	for (int y = 0; y < theMazeLevel->GetMazeHeightNodes(); y++)
	{
		for (int x = 0; x < theMazeLevel->GetMazeWidthNodes(); x++)
		{
			switch (theMazeLevel->GetPoint(x, y))
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ' ':
				//draw blank tile
				DrawTile(x, y, device, tile_floor);
				break;
			case 'S':
				// draw starting position
				DrawTile(x, y, device, tile_start);
				break;
			case 'F':
				// draw finishing position
				DrawTile(x, y, device, tile_finish);
				break;
			case 'W':
				// draw a wall
				DrawTile(x, y, device, tile_wall);
				break;
			}
		}
	}
}

void MazePhaze::DrawTile(int x, int y, LPDIRECT3DDEVICE9 device, Surface* source)
{
	int tTileSize = theMazeLevel->GetTileSize();

	RECT r1;
	r1.left = 0;
	r1.top = 0;
	r1.right = 32;
	r1.bottom = 32;

	RECT r2;
	r2.left = x*tTileSize;
	r2.top = y*tTileSize;
	r2.right = r2.left + tTileSize;
	r2.bottom = r2.top + tTileSize;

	HRESULT result = device->StretchRect(source->getSurface(), &r1, mapBuffer, &r2, D3DTEXF_NONE);

	//source->setDestRect(x*tTileSize, y*tTileSize, tTileSize, tTileSize);
	//source->render(device, background->getSurface());
	return;
}