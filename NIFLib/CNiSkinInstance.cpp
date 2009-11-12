#include "StdAfx.h"
#include ".\cniskininstance.h"
#include "CNiNode.h"


CNiSkinInstance::CNiSkinInstance(void)
{
}

CNiSkinInstance::~CNiSkinInstance(void)
{
}


bool	CNiSkinInstance::Load(CNifStream& stream)
{
	unsigned a;
	mc_skindata.read(stream,1);
	a = stream.readUInt(); assert(a==0);	// unexspected value
	mc_bones.read(stream);
	return true;
}


void	CNiSkinInstance::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_skindata.Resolve(ppc_nodes, nof);		
	mc_bones.ResolveWithoutReferenz(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiSkinInstance::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"SkinInstance",TIC_TEXTURE | TICS_FINISHED_RENDERING_MISSING);
	newsub = addfkt(newpar,NULL,"Data",TIC_DATA | TICS_FINISHED);
	CNode::CreateDbgTree(newsub,addfkt);

	// could be possible that "mc_skindata" must be filled, but I'm not sure
	assert(mc_skindata.GetCount()==0 || mc_skindata.GetCount() == 1);
	if (mc_skindata.GetCount()==1)
	{
		mc_skindata[0]->CreateDbgTree(newpar, addfkt);
	}

    
	newsub = addfkt(newpar,NULL,"Bones",TIC_LINK | TICS_FINISHED_RENDERING_MISSING );
	FOREACH(pc,mc_bones,	
		char temp[256];
		if (pc->IsClass("NiNode"))
			sprintf(temp,"%s (%s,%x)",((CNiNode*)pc)->GetNodeName(), pc->GetClassName(),pc->GetID());
		else
			sprintf(temp,"%s (%x)",pc->GetClassName(),pc->GetID());

		addfkt(newsub,NULL,temp,TIC_LINK | TICS_FINISHED_RENDERING_MISSING );
	)

}
#endif