#include "StdAfx.h"
#include ".\cni3dsparticlesystem.h"

CNi3dsParticleSystem::CNi3dsParticleSystem(void)
{
}

CNi3dsParticleSystem::~CNi3dsParticleSystem(void)
{
}


bool	CNi3dsParticleSystem::Load(CNifStream& stream)
{
	stream.readSkip(14);
	return true;
}


#ifdef ANALYSE_NIF
void	CNi3dsParticleSystem::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "3dsParticleSystem",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif