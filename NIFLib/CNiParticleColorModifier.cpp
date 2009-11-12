#include "StdAfx.h"
#include ".\cniparticlecolormodifier.h"

CNiParticleColorModifier::CNiParticleColorModifier(void)
{
}

CNiParticleColorModifier::~CNiParticleColorModifier(void)
{
}


bool	CNiParticleColorModifier::Load(CNifStream& stream)
{
	mc_fader.read(stream,1);		// fader
	stream.readSkip(4);				// parent
	mc_colordata.read(stream,1);	// colordata

	return true;
}


void	CNiParticleColorModifier::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_fader.Resolve(ppc_nodes, nof);		
	mc_colordata.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiParticleColorModifier::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"ParticleColorModifier",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_fader,"Fader",TIC_PARTICLE,newpar,addfkt);
	CreateDbgTreeChilds(mc_colordata,"ColorData",TIC_PARTICLE,newpar,addfkt);
}
#endif