#include "2dSurface.h"
#include "InputCore.h"
#include "SoundCore.h"
#include "MazeMenu.h"
#include "DebugHeader.h"

MazeMenu::MazeMenu(){
	//null all the objects we will be useing
	myInputC = NULL;
	mySounds = NULL;
	background = NULL;
	newGame = NULL;
	newGameHighlighted = NULL;
	exit = NULL;
	exitHighlighted = NULL;

}

MazeMenu::~MazeMenu(){
	//Delete everything we created for the menu;
	if (myInputC != NULL) { myInputC = NULL;}
	if (mySounds != NULL) { mySounds->popBuffers(3); mySounds = NULL;}
	if (background != NULL){delete background; background = NULL;}
	if (newGame != NULL){delete newGame; newGame = NULL;}
	if (newGameHighlighted != NULL){delete newGameHighlighted; newGameHighlighted = NULL;}
	if (exit != NULL){ delete exit; exit = NULL;}
	if (exitHighlighted != NULL){ delete exitHighlighted; exitHighlighted = NULL;}

	
}

//
//   FUNCTION: init(InputManager* input, LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Loads all of the Needed images for the menu, and defaults the values of variables
//
bool MazeMenu::init(RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device){
	myInputC = input;

	mySounds = sounds;

	//0 is new game, 1 is exit;
	menuItemSelected = 0;
	message = 0;
	//load menu Images
	//load background menubackground.jpg
	background = new Surface();
	background->loadSurface(device, "menuBackground.png");
	
	//load new game image
	newGame = new Surface();
	newGame->loadSurface(device, "newGame.jpg");
	newGame->setPosition(315,360);
	newGameHighlighted = new Surface();
	newGameHighlighted->loadSurface(device, "newGameHighlightedv2.jpg");
	newGameHighlighted->setPosition(315,360);
	//load exit image
	exit = new Surface();
	exit->loadSurface(device, "exit.jpg");
	exit->setPosition(315,440);
	exitHighlighted = new Surface();
	exitHighlighted->loadSurface(device, "exitHighlighted.jpg");
	exitHighlighted->setPosition(315,440);

	menuSwitch = mySounds->loadFile("s_menuSwitch.wav");
	menuSelect = mySounds->loadFile("s_menuSelect.wav");
	menuMusic = mySounds->loadFile("sm_menuMusic.wav");
	
	mySounds->loopSound(menuMusic, 0);

	return true;
}
//
//   FUNCTION: update()
//
//   PURPOSE: Gets input from the keyboard to update the menu.
//
void MazeMenu::update(){
	
	bool soundPlaying = false;
	//get menu movement input
	if (myInputC->GetRawInputDownEvent(VK_DOWN) || myInputC->GetRawInputDownEvent(VK_UP))
	{
		mySounds->resetSound(menuSwitch);
		mySounds->playSound(menuSwitch, 0);
		if (menuItemSelected == 0){
			menuItemSelected = 1;
		} else {
			menuItemSelected = 0;
		}
	}

	//get menu pressed input
	if (myInputC->GetRawInputDownEvent(VK_RETURN))
	{
		mySounds->playSound(menuSelect, 0);
		
		do
		{
			soundPlaying = mySounds->IsSoundPlaying(menuSelect);  
		}while (soundPlaying != false);
		
		switch (menuItemSelected){
			case 0: { message = NEW_GAME; mySounds->stopSound(menuMusic); break; }
			case 1: { message = EXIT; break; }
		}	

	}
}
//
//   FUNCTION: render(LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Renders the menu, and its active items
//
void MazeMenu::render(LPDIRECT3DDEVICE9 device){
	//render menu
	//render background
	background->render(device);
	
	//render menu items based on which is highlighted
	if (menuItemSelected == 0){
		newGameHighlighted->render(device);
		exit->render(device);
	} else {
		newGame->render(device);
		exitHighlighted->render(device);
	}
	
}
//
//   FUNCTION: getMessage()
//
//   PURPOSE: returns the message set by the update function.
//	
int MazeMenu::getMessage(){
	return message;
}