#include "StdAfx.h"
#include ".\cnikeyframecontroller.h"

CNiKeyFrameController::CNiKeyFrameController(void)
{
}

CNiKeyFrameController::~CNiKeyFrameController(void)
{
}


bool	CNiKeyFrameController::Load(CNifStream& stream)
{
	mc_path.read(stream,1);
	stream.readWord();
	stream.readFloat();
	stream.readFloat();
	stream.readFloat();
	mf_max_time = stream.readFloat(); 
	stream.readSkip(4);
	mc_data.read(stream,1);

	return true;
}



void	CNiKeyFrameController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_path.Resolve(ppc_nodes, nof);		
	mc_data.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiKeyFrameController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"KeyFrameController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_path,"Path",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_data,"Data",TIC_CONTROLLER,newpar,addfkt);
}
#endif
