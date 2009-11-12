#include "StdAfx.h"
#include ".\cni3dsanimationnode.h"

CNi3dsAnimationNode::CNi3dsAnimationNode(void)
{
}

CNi3dsAnimationNode::~CNi3dsAnimationNode(void)
{
}

bool	CNi3dsAnimationNode::Load(CNifStream& stream)
{
	assert(stream.Version() < 0x03000000);

	char * name = stream.readstring();
	delete (name);

	unsigned i = stream.readUInt();
	if (i!=1)
	{
		stream.readSkip(86); // looks like a matrix

		mc_childs.read(stream);

		stream.readSkip(48); 

		unsigned nof2 = stream.readUInt();
		stream.readSkip(5*nof2); 
	}
	
	return true;
}


void	CNi3dsAnimationNode::Create()
{
	FOREACH(pc,mc_childs,  pc->Create(); )
}


void	CNi3dsAnimationNode::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_childs.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNi3dsAnimationNode::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "3dsAnimationNode",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_childs,"Childs",TIC_ABSTRACT,newpar,addfkt);
}
#endif