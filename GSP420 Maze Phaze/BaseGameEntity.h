#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

class BaseGameEntity
{
	private:

		int entity_ID;						
		void setID ( int val ){ entity_ID = val; };		

	public:

		BaseGameEntity()	{ setID( nextValidID() );};
		int nextValidID()	{static int NextID = 1; return NextID++;}
		virtual void update () = 0;
		int ID()const{ return entity_ID; }
};
#endif