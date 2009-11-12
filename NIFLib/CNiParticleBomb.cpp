#include "StdAfx.h"
#include ".\cniparticlebomb.h"

CNiParticleBomb::CNiParticleBomb(void)
{
}

CNiParticleBomb::~CNiParticleBomb(void)
{
}


bool	CNiParticleBomb::Load(CNifStream& stream)
{
	if (stream.Version()<0x03000000 || stream.Version()>=0x04000000)
	{
		mc_child.read(stream,1);
		stream.readSkip(4); // parent
	}

	stream.readSkip(6*8);
	return true;
}


void	CNiParticleBomb::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_child.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiParticleBomb::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"ParticleBomb",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_child,"Child",TIC_CHILDS,newpar,addfkt);
}
#endif