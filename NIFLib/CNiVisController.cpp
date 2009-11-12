#include "StdAfx.h"
#include ".\cniviscontroller.h"

CNiVisController::CNiVisController(void)
{
}

CNiVisController::~CNiVisController(void)
{
}

bool	CNiVisController::Load(CNifStream& stream)
{
	mc_keyframe.read(stream,1);
	stream.readSkip(18);

	mc_parent.read(stream,1);
	mc_data.read(stream,1);
	return true;
}



void	CNiVisController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_keyframe.Resolve(ppc_nodes, nof);		
	mc_data.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiVisController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"VisController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_keyframe,"KeyFrames",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_data,"Data",TIC_CONTROLLER,newpar,addfkt);
}

#endif