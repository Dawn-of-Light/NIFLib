#include "StdAfx.h"
#include ".\cniparticlegrowfade.h"

CNiParticleGrowFade::CNiParticleGrowFade(void)
{
}

CNiParticleGrowFade::~CNiParticleGrowFade(void)
{
}


bool	CNiParticleGrowFade::Load(CNifStream& stream)
{
	stream.readSkip(16);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiParticleGrowFade::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "ParticleGrowFade",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
