#include "ai_bot2D.h"
#include "ai_bot2D_states.h"

#include <time.h>


//#include <iostream>
//using namespace std;

AI_Bot2D::AI_Bot2D():BaseGameEntity(), m_pCurrentState(Spawn::Instance())
	//						m_ihealth(0), m_xiPos(0), m_yiPos(0)
{}//constructor




void AI_Bot2D::changeState( State* pNewState )
{
 //make sure both states are both valid before attempting to 
  //call their methods
  assert (m_pCurrentState && pNewState);

  //call the exit method of the existing state
  m_pCurrentState->Exit(this);

  //change state to the new state
  m_pCurrentState = pNewState;

  //call the entry method of the new state
  m_pCurrentState->Enter(this);

}

/*------------------------------------------------------*/
//void AI_Bot2D::updateAI_Bot2D()
//{
//	  if (m_pCurrentState)
//  {
//    m_pCurrentState->Execute(this);
//  }
//}

//redefine the virtual funciton from BaseGameEntity
void AI_Bot2D::update()
{
	  if (m_pCurrentState)
  {
    m_pCurrentState->Execute(this);
  }
}

/*------------------------------------------------------*/

//function with references
void AI_Bot2D::move(int& m_ixPos, int& m_iyPos, int m_ixDest, int m_iyDest)
{

	//This function takes the AI bot's posiiton and destination point,
	//and increments the posiiton up to the dest. point

	while( m_ixPos != m_ixDest || m_iyPos != m_iyDest)
	{  
		if ( m_ixPos == m_ixDest )  //if the x's already match
		{
			m_iyPos++;  //only increment y
		}
		else if (m_iyPos == m_iyDest)  //if the y's already match
		{
			m_ixPos++;  //only increment x
		}
		else  //if nothing matches
		{
			m_ixPos++;  //increment both
			m_iyPos++;
		}

	}
	
}

/*-------------------------------------------------------------*/


//  function with references
void AI_Bot2D::getDestPoint(int m_ixPos, int m_iyPos, int& m_ixDest, int& m_iyDest)
{

	//getDestPoint returns a point on the map that the AI bot will move to

	/*
	randomly generate a destination point relative to the bot's current position
	, and return it to the AI_Bot object*/
	
	//This funciton does not test for walls yet

	//srand(time(0));

//	m_ixDest = rand ()% 30;  //number between 0-99
//	m_iyDest = rand ()% 30;

}

//void getID(){ return ID;}