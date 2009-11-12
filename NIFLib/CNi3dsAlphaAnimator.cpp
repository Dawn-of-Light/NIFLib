#include "StdAfx.h"
#include ".\cni3dsalphaanimator.h"

CNi3dsAlphaAnimator::CNi3dsAlphaAnimator(void)
{
}

CNi3dsAlphaAnimator::~CNi3dsAlphaAnimator(void)
{
}


bool	CNi3dsAlphaAnimator::Load(CNifStream& stream)
{
	stream.readSkip(40);
	mc_parent.read(stream,1);

	unsigned x1 = stream.readUInt(); 
	unsigned x2 = stream.readUInt(); 
	stream.readSkip(x1*x2*8);

	return true;
}



void	CNi3dsAlphaAnimator::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_parent.ResolveWithoutReferenz(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNi3dsAlphaAnimator::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this, "3dsAlphaAnimator",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
}
#endif 



