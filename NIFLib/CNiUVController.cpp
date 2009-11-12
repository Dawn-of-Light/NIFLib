#include "StdAfx.h"
#include ".\cniuvcontroller.h"

CNiUVController::CNiUVController(void)
{
}

CNiUVController::~CNiUVController(void)
{
}


bool	CNiUVController::Load(CNifStream& stream)
{
	mc_child.read(stream,1);
	mc_parent.read(stream,1);
	stream.readSkip(20);
	mc_data.read(stream,1);
	return true;
}


void	CNiUVController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_child.Resolve(ppc_nodes, nof);		
	mc_data.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiUVController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"UVController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_child,"Childs",TIC_ABSTRACT,newpar,addfkt);
	CreateDbgTreeChilds(mc_data,"Data",TIC_ABSTRACT,newpar,addfkt);
}
#endif
