#include "AICoreHeaders.h"

#ifndef AI_CORE_INTERFACE_H
#define AI_CORE_INTERFACE_H

/////////////////////////////////////////////////////////////////////////////////
//		AI_Core_Interface:
//		This is the main interface for the AI_Manager.  The ctor handles the startup and
//		shutdown of the AI_Manager through the constructor and destructor.  The handle to the 
//		class is through "al", from which it is possible to create or destroy entities.
//		Entitie types are enumerated, which means that when the game is developed for, they 
//		will need to be changed if any new bots are made ( the only working one at this time
//		is GENERIC_BOT.  Access to any entity is handled through "accessEntity", which gives
//		direct access to an agent, from which anything can be modified - provided that the bot
//		supports a desired action.
/////////////////////////////////////////////////////////////////////////////////


class AI_Core_Interface
{
	private:
		AI_Core_Interface();											// Singleton
		AI_Core_Interface (const AI_Manager &);							// prevent copy-construction
		AI_Core_Interface& operator=( const AI_Manager & );				// prevent assignment
		~AI_Core_Interface();	

	public:	

		static AI_Core_Interface* Instance( void );						// handle to class

		int  createEntity		( int eType );							// create entity and return it's ID: negative number indicates failure
		bool destroyEntity		( int ID );								// destroy entity searching for it's ID
		bool update				( void );

		BaseGameEntity* accessEntity ( int eID );						// access entity by ID

};
#define al AI_Core_Interface::Instance()		// handle to interface

// to make createEntity a little more intuitive
#define GENERIC_BOT 0							
#endif