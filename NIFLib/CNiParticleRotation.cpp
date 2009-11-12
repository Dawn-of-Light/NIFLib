#include "StdAfx.h"
#include ".\cniparticlerotation.h"

CNiParticleRotation::CNiParticleRotation(void)
{
}

CNiParticleRotation::~CNiParticleRotation(void)
{
}


bool	CNiParticleRotation::Load(CNifStream& stream)
{
	mc_data.read(stream,1);
	stream.readSkip(4); // parent

	stream.readSkip(17); 
	return true;
}

void	CNiParticleRotation::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_data.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiParticleRotation::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"ParticleRotation",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
	// mc_matrix;

	CreateDbgTreeChilds(mc_data,"data",TIC_PARTICLE,newpar,addfkt);
}
#endif
