#include "StdAfx.h"
#include ".\cnigeommorphercontroller.h"

CNiGeomMorpherController::CNiGeomMorpherController(void)
{
}

CNiGeomMorpherController::~CNiGeomMorpherController(void)
{
}

bool	CNiGeomMorpherController::Load(CNifStream& stream)
{
	mc_data1.read(stream,1);
	stream.readSkip(22);

	mc_data2.read(stream,1);

	stream.readSkip(1);
	return true;
}

void	CNiGeomMorpherController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_data1.Resolve(ppc_nodes, nof);		
	mc_data2.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiGeomMorpherController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this, "GeomMorpherController",TIC_CONTROLLER);

	newsub = addfkt(newpar, NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_data1,"data1",TIC_ABSTRACT,newpar,addfkt);
	CreateDbgTreeChilds(mc_data2,"data2",TIC_ABSTRACT,newpar,addfkt);
}
#endif