#include "StdAfx.h"
#include ".\cnilookatcontroller.h"

CNiLookAtController::CNiLookAtController(void)
{
}

CNiLookAtController::~CNiLookAtController(void)
{
}


bool	CNiLookAtController::Load(CNifStream& stream)
{
	mc_controller.read(stream,1);
	stream.readSkip(26);
	return true;
}

void	CNiLookAtController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiLookAtController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "LookAtController",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER,newpar,addfkt);
}
#endif