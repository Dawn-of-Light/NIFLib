#include "StdAfx.h"
#include ".\cniparticlemeshmodifier.h"

CNiParticleMeshModifier::CNiParticleMeshModifier(void)
{
}

CNiParticleMeshModifier::~CNiParticleMeshModifier(void)
{
}


bool	CNiParticleMeshModifier::Load(CNifStream& stream)
{
	mc_fader.read(stream,1);
	stream.readSkip(4); // parent
	mc_data.read(stream); 

	return true;
}



void	CNiParticleMeshModifier::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_fader.Resolve(ppc_nodes, nof);		
	mc_data.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiParticleMeshModifier::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"ParticleMeshModifier",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_fader,"Fader",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_data,"Data",TIC_CONTROLLER,newpar,addfkt);
}
#endif 