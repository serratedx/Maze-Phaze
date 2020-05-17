#ifndef AI_BOT2D_STATES_H
#define AI_BOT2D_STATES_H


#include "states.h"

class State;

class Patrol : public State
{
private:
	

public:

	Patrol(){}
	//this is a singleton
	static Patrol* Instance();

	virtual void Enter(AI_Bot2D* ai_bot2D);

	virtual void Execute(AI_Bot2D* ai_bot2D);

	virtual void Exit(AI_Bot2D* ai_bot2D);

};

/*
class Seek : public State
{
private:

	
public:
	Seek(){}

	//this is a singleton
	static Seek* Instance();
	
	virtual void Enter(AI_Bot2D* ai_bot2D);

	virtual void Execute(AI_Bot2D* ai_bot2D);

	virtual void Exit(AI_Bot2D* ai_bot2D);
};



class Persue : public State
{
private:


public:
	Persue(){}
	//this is a singleton
	static Persue* Instance();

	virtual void Enter(AI_Bot2D* ai_bot2D);

	virtual void Execute(AI_Bot2D* ai_bot2D);

	virtual void Exit(AI_Bot2D* ai_bot2D);

};
*/

class Spawn : public State
{
private:

public:
	Spawn() {}
	//this is a singleton
	static Spawn* Instance();

	virtual void Enter(AI_Bot2D* ai_bot2D);

	virtual void Execute(AI_Bot2D* ai_bot2D);

	virtual void Exit(AI_Bot2D* ai_bot2D);

};

class Death : public State
{
private:

public:
	Death() {}
	//this is a singleton
	static Death* Instance();

	virtual void Enter(AI_Bot2D* ai_bot2D);
	
	virtual void Execute(AI_Bot2D* ai_bot2D);

	virtual void Exit(AI_Bot2D* ai_bot2D);

};

#endif