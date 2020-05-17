#include "AICoreHeaders.h"

AI_Manager::AI_Manager(){}
AI_Manager::~AI_Manager(){}

bool AI_Manager::startup_AI_Core( void )
{ 
	return true; 
}

bool AI_Manager::shutdown_AI_Core( void )
{
	EntityList.~LinkedList();
	return true;
}

bool AI_Manager::process( void )
{
	EntityList.Update();
	return true;
}

AI_Manager* AI_Manager::Instance() 
{
	static AI_Manager instance;
	return &instance;
}

int AI_Manager::registerEntity( BaseGameEntity* newEntity )
{
	return EntityList.PushFront( newEntity );
}

bool AI_Manager::removeEntity( int entityID )
{	
	EntityList.PopEntity( entityID );
	return true;
}

BaseGameEntity* AI_Manager::searchEntity( int entityID )
{
		return EntityList.Access( entityID );
}

