#ifndef AI_BOT2D_H
#define AI_BOT2D_H

#include <cassert>
#include "BaseGameEntity.h"

class State;

class AI_Bot2D : public BaseGameEntity
{
private:
//pointer to the bot's current state 
	State*                m_pCurrentState;	
	
public:

	AI_Bot2D();

	int m_ihealth;
	int m_ixPos;  //current X position
	int m_iyPos;  //current Y position

	int m_ixDest;  //X position of target point
	int m_iyDest;  //Y position of target point

	void changeState(State* new_state);
	//void updateAI_Bot2D();	

	void update();
 
	void move(int& xPos, int& yPos, int xDest, int yDest);		//[references)
	void getDestPoint(int xPos, int yPos, int& xDest, int& yDest); //[references
	
	//void getID();  //function to get the object's ID
};

#endif