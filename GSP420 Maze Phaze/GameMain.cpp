#include "GameMain.h"
#include "dxManager.h"
#include "2dSurface.h"
#include "MazeMenu.h"
#include "WinningMenu.h"
#include "MazePhaze.h"
#include "FrameTimer.h"
#include "InputCore.h"
#include "SoundCore.h"
#include "DebugHeader.h"
#include "AI_Core_Interface.h"
#include "PlayerEntity.h"
#include "GameOverMenu.h"
#include "dxText.h"

GameMain::GameMain(void)
{
	mazePhaze = NULL;
	inputC = NULL;
}


GameMain::~GameMain(void)
{
	if( dxMgr != NULL)
	{
		delete dxMgr;
		dxMgr = NULL;
	}
}

bool GameMain::init(HWND wndHandle, HINSTANCE hInst)
{
	level = 0;
	seconds = 0;
	minutes = 0;

	dxMgr = new dxManager();
	dxMgr-> init(wndHandle, 800, 600, false);
	if (!dxMgr){
		return false;
	}

	inputC = new RawInputHandler();

	sound = new SoundCore();
	sound->init(wndHandle);

	menu = new MazeMenu();
	menu->init(inputC, sound, dxMgr->getD3DDevice());
	active = MENU;

	timer = new FrameTimer();
	timer->init(60);

	mazePhaze = NULL;
	return true;
}

void GameMain::update()
{
	int framesToUpdate;
	// call update function
	framesToUpdate = timer->framesToUpdate();

	// update the menu
	if (active == MENU) 
	{
		menu->update();
		if (menu->getMessage() == MazeMenu::EXIT) 
		{
			active = NOTHING;
			delete menu;
			menu = NULL;
			PostQuitMessage(0);
		}
		else if (menu->getMessage() == MazeMenu::NEW_GAME)
		{
			level = 0;
			//if an existing game exists, delete it
			if (mazePhaze != NULL) 
			{
				delete mazePhaze;
				mazePhaze = NULL;
			}
			//start a new game
			delete menu;
			menu = NULL;
			mazePhaze = new MazePhaze();
			mazePhaze->init(level, inputC, sound, player, dxMgr->getD3DDevice());
			active = GAME;
		}
	}
	
	//game update calls here
	if (active == GAME)
	{
		mazePhaze->updateFrames(framesToUpdate);
		if (mazePhaze->getMessage() == MazePhaze::QUIT)
		{
			active = NOTHING;
			delete mazePhaze;
			mazePhaze = NULL;
			//reactivate the menu
			menu = new MazeMenu();
			menu->init(inputC, sound, dxMgr->getD3DDevice());
			active = MENU;
		}

		else if (mazePhaze->getMessage() == MazePhaze::WIN)
		{
			level++;
			if (level > 4)
			{
				level = 0;
			}
			seconds = mazePhaze->getSeconds();
			minutes = mazePhaze->getMinutes();

			active = NOTHING;
			delete mazePhaze;
			mazePhaze = NULL;
			winMenu = new WinningMenu();
			winMenu->init(seconds, minutes, inputC, sound, dxMgr->getD3DDevice());
			active = WIN;
		}

		else if (mazePhaze->getMessage() == MazePhaze::GAME_OVER)
		{
			active = NOTHING;
			delete mazePhaze;
			mazePhaze = NULL;
			gameOver = new GameOverMenu();
			gameOver->init(inputC, sound, dxMgr->getD3DDevice());
			active = GAME_OVER;
		}
	}

	if (active == WIN)
	{
		winMenu->update();
		if (winMenu->getMessage() == WinningMenu::CONTINUE)
		{
			active = NOTHING;
			delete winMenu;
			winMenu = NULL;
			mazePhaze = new MazePhaze();
			mazePhaze->init(level, inputC, sound, player, dxMgr->getD3DDevice());
			active = GAME;
		}
	}

	if (active == GAME_OVER)
	{
		gameOver->update();
		if (gameOver->getMessage() == GameOverMenu::RESTART)
		{
			active = NOTHING;
			delete gameOver;
			gameOver = NULL;

			mazePhaze = new MazePhaze();
			mazePhaze->init(level, inputC, sound, player, dxMgr->getD3DDevice());
			active = GAME;
		}
		else if (gameOver->getMessage() == GameOverMenu::MAIN_MENU)
		{
			active = NOTHING;
			delete gameOver;
			gameOver = NULL;
			
			//reactivate the menu
			menu = new MazeMenu();
			menu->init(inputC, sound, dxMgr->getD3DDevice());
			active = MENU;
		}
	}

	// begin rendering
	dxMgr->beginRender();
	// game menu render calls here
	if (active == MENU) 
	{
		menu->render(dxMgr->getD3DDevice());
	}

	// game render call here
	if (active == GAME)
	{
		mazePhaze->render(dxMgr->getD3DDevice());
	}

	if (active == WIN)
	{
		winMenu->render(dxMgr->getD3DDevice());
	}

	if (active == GAME_OVER)
	{
		gameOver->render(dxMgr->getD3DDevice());
	}

	//end render
	dxMgr->endRender();

	inputC->StorePrevInput();

}

bool GameMain::CheckWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (inputC != NULL)
		inputC->CheckInput(uMsg, wParam, lParam);
	return true;
}