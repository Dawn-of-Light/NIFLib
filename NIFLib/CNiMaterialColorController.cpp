#include "StdAfx.h"
#include ".\cnimaterialcolorcontroller.h"

CNiMaterialColorController::CNiMaterialColorController(void)
{
}

CNiMaterialColorController::~CNiMaterialColorController(void)
{
}


bool	CNiMaterialColorController::Load(CNifStream& stream)
{
	mc_controller1.read(stream,1);
	stream.readSkip(18);
	stream.readSkip(4);// parent
	mc_controller2.read(stream,1);
	return true;
}



void	CNiMaterialColorController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller1.Resolve(ppc_nodes, nof);		
	mc_controller2.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiMaterialColorController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"MaterialColorController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_controller1,"Controller1",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_controller2,"Controller2",TIC_CONTROLLER,newpar,addfkt);
}
#endif