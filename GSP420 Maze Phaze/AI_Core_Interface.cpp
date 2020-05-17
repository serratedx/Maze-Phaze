#include "AI_Core_Interface.h"

AI_Core_Interface::AI_Core_Interface()
{
	AI_Core->startup_AI_Core();
}

AI_Core_Interface::~AI_Core_Interface()
{
	AI_Core->shutdown_AI_Core();
}

AI_Core_Interface* AI_Core_Interface::Instance( void )
{
	static AI_Core_Interface instance;

	return &instance;
}

int AI_Core_Interface::createEntity( int eType )
{
	switch( eType )
	{
		case 0:
				return ( AI_Core->registerEntity( new AI_Bot2D ) );

		case 1:
				// nothing
			break;
		case 2:	
				// nothing
			break;

		default:
			return -1;
	}
	return -2;
}

bool AI_Core_Interface::destroyEntity( int entityID )
{
	AI_Core->removeEntity( entityID );
	return true;
}

bool AI_Core_Interface::update( void )
{
	AI_Core->process();
	return true;
}

BaseGameEntity* AI_Core_Interface::accessEntity( int ID )
{
	return AI_Core->searchEntity( ID );
}
