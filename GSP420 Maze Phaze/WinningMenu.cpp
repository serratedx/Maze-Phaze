#include "2dSurface.h"
#include "InputCore.h"
#include "SoundCore.h"
#include "WinningMenu.h"
#include "DebugHeader.h"
#include "dxText.h"

WinningMenu::WinningMenu(){
	//null all the objects we will be useing
	myInputC = NULL;
	Sounds = NULL;
	background = NULL;
	WinMenuButton = NULL;
	WinMenuButtonSelected = NULL;
	timeText = NULL;
}

WinningMenu::~WinningMenu(){
	//Delete everything we created for the menu;
	if (myInputC != NULL) { myInputC = NULL;}
	if (Sounds != NULL) { Sounds->popBuffers(2); Sounds = NULL;}
	if (background != NULL){delete background; background = NULL;}
	if (WinMenuButton != NULL){delete WinMenuButton; WinMenuButton = NULL;}
	if (WinMenuButtonSelected != NULL){delete WinMenuButtonSelected; WinMenuButtonSelected = NULL;}
	if (timeText != NULL){delete timeText; timeText = NULL;}
}
//
//   FUNCTION: init(InputManager* input, LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Loads all of the Needed images for the menu, and defaults the values of variables
//
bool WinningMenu::init(int sec, int min, RawInputHandler* input, SoundCore* sounds, LPDIRECT3DDEVICE9 device){
	myInputC = input;

	Sounds = sounds;

	seconds = sec;
	minutes = min;

	//0 is new game, 1 is exit;
	//menuItemSelected = 0;
	message = 0;
	//load menu Images
	//load background menubackground.jpg
	background = new Surface();
	background->loadSurface(device, "bg_WinMenu.png");
	
	//load new game image
	WinMenuButton = new Surface();
	WinMenuButton->loadSurface(device, "bg_WinMenuButton.png");
	WinMenuButton->setPosition(255,425);
	WinMenuButtonSelected = new Surface();
	WinMenuButtonSelected->loadSurface(device, "bg_WinMenuButton.png");
	WinMenuButtonSelected->setPosition(257,427);
	

	winMenuSelect = Sounds->loadFile("s_menuSelect.wav");
	winMenuMusic = Sounds->loadFile("s_winLevel.wav");

	timeText = new dxText();
	timeText->init(28, device);
	
	Sounds->playSound(winMenuMusic, 0);

	return true;
}
//
//   FUNCTION: update()
//
//   PURPOSE: Gets input from the keyboard to update the menu.
//
void WinningMenu::update(){
	
	bool soundPlaying = false;

	//get menu pressed input
	if (myInputC->GetRawInputDownEvent(VK_RETURN))
	{
		Sounds->playSound(winMenuSelect, 0);
		
		do
		{
			soundPlaying = Sounds->IsSoundPlaying(winMenuSelect);  
		}while (soundPlaying != false);
		
		message = CONTINUE;
	}
}
//
//   FUNCTION: render(LPDIRECT3DDEVICE9 device)
//
//   PURPOSE: Renders the menu, and its active items
//
void WinningMenu::render(LPDIRECT3DDEVICE9 device){
	//render menu
	//render background
	background->render(device);
	WinMenuButton->render(device);

	// display time spent in level
	char timeTaken[30] = "";
	sprintf(timeTaken, "Time spent in Level: %02i:%02i", minutes, seconds);
	timeText->drawText(timeTaken, 250, 340, 200, 75, 0xFF000000);
	
	
	if (myInputC->GetRawInputDownEvent(VK_RETURN))
	{
		//WinMenuButtonSelected->setPosition(255,425);
		//WinMenuButtonSelected->render(device);
		
	}
	
}
//
//   FUNCTION: getMessage()
//
//   PURPOSE: returns the message set by the update function.
//	
int WinningMenu::getMessage(){
	return message;
}