#include "2dSurface.h"
#include "InputCore.h"
#include "SoundCore.h"
#include "GameOverMenu.h"
#include "DebugHeader.h"

GameOverMenu::GameOverMenu(){
	//null all the objects we will be useing
	myInputC = NULL;
	//mySounds = NULL;
	background = NULL;
	restartGame = NULL;
	restartGameHL = NULL;
	mainMenu = NULL;
	mainMenuHL= NULL;

}

GameOverMenu::~GameOverMenu(){
	//Delete everything we created for the menu;
	if (myInputC != NULL) { myInputC = NULL;}
	if (mySounds != NULL) { mySounds->popBuffers(3); mySounds = NULL;}
	if (background != NULL){delete background; background = NULL;}
	if (restartGame != NULL){delete restartGame; restartGame = NULL;}
	if (restartGameHL != NULL){delete restartGameHL; restartGameHL = NULL;}
	if (mainMenu != NULL){delete mainMenu; mainMenu = NULL;}
	if (mainMenuHL != NULL){delete mainMenuHL; mainMenuHL = NULL;}

}
//
//   FUNCTION: init(InputManager* input, LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Loads all of the Needed images for the menu, and defaults the values of variables
//
bool GameOverMenu::init(RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device){
	myInputC = input;

	mySounds = sounds;

	//0 is new game, 1 is exit;
	menuItemSelected = 0;
	message = 0;
	//load menu Images
	//load background menubackground.jpg
	background = new Surface();
	background->loadSurface(device, "bg_GameOver.png");
	
	//load new game image
	restartGame = new Surface();
	restartGame->loadSurface(device, "bg_GameOverRestart.png");
	restartGame->setPosition(280,320);
	restartGameHL = new Surface();
	restartGameHL->loadSurface(device, "bg_GameOverRestartHL.png");
	restartGameHL->setPosition(280,320);
	//load exit image
	mainMenu = new Surface();
	mainMenu->loadSurface(device, "bg_GameOverMainMenu.png");
	mainMenu->setPosition(280,400);
	mainMenuHL = new Surface();
	mainMenuHL->loadSurface(device, "bg_GameOverMainMenuHL.png");
	mainMenuHL->setPosition(280,400);

	go_menuSwitch = mySounds->loadFile("s_menuSwitch.wav");
	go_menuSelect = mySounds->loadFile("s_menuSelect.wav");
	go_menuMusic = mySounds->loadFile("sm_menuMusic.wav");
	
	mySounds->loopSound(go_menuMusic, 0);

	return true;
}
//
//   FUNCTION: update()
//
//   PURPOSE: Gets input from the keyboard to update the menu.
//
void GameOverMenu::update(){
	
	bool soundPlaying = false;
	//get menu movement input
	if (myInputC->GetRawInputDownEvent(VK_DOWN) || myInputC->GetRawInputDownEvent(VK_UP))
	{
		mySounds->resetSound(go_menuSwitch);
		mySounds->playSound(go_menuSwitch, 0);
		if (menuItemSelected == 0){
			menuItemSelected = 1;
		} else {
			menuItemSelected = 0;
		}
	}

	//get menu pressed input
	if (myInputC->GetRawInputDownEvent(VK_RETURN))
	{
		mySounds->playSound(go_menuSelect, 0);
		
		do
		{
			soundPlaying = mySounds->IsSoundPlaying(go_menuSelect);  
		}while (soundPlaying != false);
		
		switch (menuItemSelected){
			case 0: { message = RESTART; mySounds->stopSound(go_menuMusic); break; }
			case 1: { message = MAIN_MENU; mySounds->stopSound(go_menuMusic); break; }
		}	

	}
}
//
//   FUNCTION: render(LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Renders the menu, and its active items
//
void GameOverMenu::render(LPDIRECT3DDEVICE9 device){
	//render menu
	//render background
	background->render(device);
	
	//render menu items based on which is highlighted
	if (menuItemSelected == 0){
		restartGameHL->render(device);
		mainMenu->render(device);
	} else {
		restartGame->render(device);
		mainMenuHL->render(device);
	}
	
}
//
//   FUNCTION: getMessage()
//
//   PURPOSE: returns the message set by the update function.
//	
int GameOverMenu::getMessage(){
	return message;
}