#include "StdAfx.h"
#include ".\cnipathcontroller.h"

CNiPathController::CNiPathController(void)
{
}

CNiPathController::~CNiPathController(void)
{
}


bool	CNiPathController::Load(CNifStream& stream)
{
	stream.readSkip(40);
	mc_path.read(stream,1);
	mc_floatdata.read(stream,1);
	return true;
}


void	CNiPathController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_path.Resolve(ppc_nodes, nof);		
	mc_floatdata.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiPathController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*,char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"PathController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_path,"Path",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_floatdata,"Data",TIC_CONTROLLER,newpar,addfkt);
}
#endif