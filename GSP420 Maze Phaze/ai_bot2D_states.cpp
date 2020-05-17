#include "ai_bot2D.h"
#include "ai_bot2D_states.h"
#include "states.h"

Patrol* Patrol::Instance()
{
	static Patrol instance;
	return &instance;
}

void Patrol::Enter(AI_Bot2D* pAi_bot2d)
{
	//AI_Bot2D::getDestPoint(pAI_bot2d->m_ixPos, m_iyPos, &m_ixDest, &m_iyDest);

	pAi_bot2d->getDestPoint( pAi_bot2d->m_ixPos, pAi_bot2d->m_iyPos, 
		pAi_bot2d->m_ixDest, pAi_bot2d->m_iyDest );	

}

void Patrol::Execute(AI_Bot2D* pAi_bot2d)
{
	pAi_bot2d->move( pAi_bot2d->m_ixPos, pAi_bot2d->m_iyPos, 
		pAi_bot2d->m_ixDest, pAi_bot2d->m_iyDest );
	//if player is near, enter persuit state
	//if player is far, enter the seek state

}

void Patrol::Exit(AI_Bot2D* pAi_bot2d)
{
}

//methods for Spawn State
Spawn* Spawn::Instance()
{
	static Spawn instance;
	return &instance;
}

void Spawn::Enter(AI_Bot2D* Ai_bot2D)
{
}

void Spawn::Execute(AI_Bot2D* Ai_bot2D)
{
	Ai_bot2D->m_ixPos = 0;
	Ai_bot2D->m_iyPos = 0;	

	Ai_bot2D->changeState(Patrol::Instance());
}

void Spawn::Exit(AI_Bot2D* Ai_bot2D)
{
}

//methods for Death State
Death* Death::Instance()
{
	static Death instance;
	return &instance;
}

void Death::Enter(AI_Bot2D *ai_bot2D)
{
}

void Death::Execute(AI_Bot2D *ai_bot2D)
{
}

void Death::Exit(AI_Bot2D *ai_bot2D)
{
}