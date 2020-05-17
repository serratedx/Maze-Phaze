#ifndef STATE_H
#define STATE_H

//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//------------------------------------------------------------------------

class AI_Bot2D;

class State
{
public:

  virtual ~State(){}

  //this will execute when the state is entered
  virtual void Enter(AI_Bot2D*)=0;

  //this is the state's normal update function
  virtual void Execute(AI_Bot2D*)=0;

  //this will execute when the state is exited. (My word, isn't
  //life full of surprises... ;o))
  virtual void Exit(AI_Bot2D*)=0;

};

#endif