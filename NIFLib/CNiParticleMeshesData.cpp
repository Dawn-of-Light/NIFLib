#include "StdAfx.h"
#include ".\cniparticlemeshesdata.h"

CNiParticleMeshesData::CNiParticleMeshesData(void)
{
}

CNiParticleMeshesData::~CNiParticleMeshesData(void)
{
}


bool	CNiParticleMeshesData::Load(CNifStream& stream)
{
	unsigned len = stream.readWord();
	stream.readBool();
	stream.readSkip(len*12);
	stream.readSkip(26);
	stream.readBool();
	stream.readSkip(len*5*4);
	stream.readBool();
	mc_child.read(stream,1);

	return true;
}



void	CNiParticleMeshesData::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_child.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiParticleMeshesData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*,char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"ParticleMeshesData",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_child,"Childs",TIC_CHILDS,newpar,addfkt);
}
#endif


