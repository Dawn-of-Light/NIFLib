#include "StdAfx.h"
#include "CNirollcontroller.h"
#include "CNiNode.h"

CNiRollController::CNiRollController(void)
{
}

CNiRollController::~CNiRollController(void)
{
}


bool	CNiRollController::Load(CNifStream& stream)
{
	stream.readSkip(22);

	mc_transform_node.read(stream,1);
	mc_extradata.read(stream,1);
	return true;
}

void	CNiRollController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_transform_node.Resolve(ppc_nodes, nof);		
	mc_extradata.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiRollController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "RollController",TIC_ABSTRACT);
	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
	CreateDbgTreeChilds(mc_extradata,"ExtraData",TIC_DATA | TICS_FINISHED,newsub,addfkt);

	FOREACH(pc,mc_transform_node,	
		char temp[256];
		if (pc->IsClass("NiNode"))
			sprintf(temp,"%s (%s,%x)",((CNiNode*)pc)->GetNodeName(), pc->GetClassName(),pc->GetID());
		else
			sprintf(temp,"%s (%x)",pc->GetClassName(),pc->GetID());

		addfkt(newsub,NULL,temp,TIC_LINK | TICS_FINISHED_RENDERING_MISSING );
	)	

}
#endif
