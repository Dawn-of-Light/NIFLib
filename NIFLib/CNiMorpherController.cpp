#include "StdAfx.h"
#include ".\cnimorphercontroller.h"

CNiMorpherController::CNiMorpherController(void)
{
}

CNiMorpherController::~CNiMorpherController(void)
{
}



bool	CNiMorpherController::Load(CNifStream& stream)
{
	assert(stream.Version()<=0x03010000);
	unsigned a = stream.readUInt();	assert(a==0);// ?
	stream.readSkip(18);
	stream.readUInt();	// ?

	mc_data.read(stream,1);

	return true;
}


void	CNiMorpherController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_data.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiMorpherController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "MorpherController",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	// could be possible that "mc_data" must be filled, but I'm not sure
	assert(mc_data.GetCount()==0 || mc_data.GetCount() == 1);
	if (mc_data.GetCount()==1)
	{
		mc_data[0]->CreateDbgTree(newpar, addfkt);
	}
}
#endif