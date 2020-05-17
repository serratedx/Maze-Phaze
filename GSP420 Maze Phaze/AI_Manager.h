#ifndef AI_MANAGER_H
#define AI_MANAGER_H

#include "AICoreHeaders.h"
#define BGE BaseGameEntity

class AI_Manager
{
	private:
		
		AI_Manager();											// Singleton
		AI_Manager (const AI_Manager &);						// prevent copy-construction
		AI_Manager& operator=( const AI_Manager & );			// prevent assignment
		~AI_Manager();		

		LinkedList EntityList;									// entity list

	public:

		static AI_Manager* Instance( void );					// handle to class			
		
		bool startup_AI_Core	( void );						// core startup procedures
		bool shutdown_AI_Core	( void );						// shutdown core procedures
		bool process			( void );						// process the entityList

		int  registerEntity		( BGE* newEntity );				// register entity and return EntityList INDEX
		bool removeEntity		( int entityID );				// remove entity from entityList: true if successful
		BGE* searchEntity		( int entityID );				// search for entity and return access: NULL pointer is failure	
};
#define AI_Core AI_Manager::Instance()							// handle to AI_Manager
#endif
