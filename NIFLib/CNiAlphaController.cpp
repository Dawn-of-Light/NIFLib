#include "StdAfx.h"
#include ".\cnialphacontroller.h"

CNiAlphaController::CNiAlphaController(void)
{
}

CNiAlphaController::~CNiAlphaController(void)
{
}


bool	CNiAlphaController::Load(CNifStream& stream)
{
	mc_controller.read(stream,1);
	stream.readSkip(22);
	mc_data.read(stream,1);
	return true;
}


void	CNiAlphaController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller.Resolve(ppc_nodes, nof);		
	mc_data.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiAlphaController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "AlphaController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_data,"Data",TIC_CONTROLLER,newpar,addfkt);
}
#endif