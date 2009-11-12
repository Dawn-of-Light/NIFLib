#include "StdAfx.h"
#include ".\cni3dsmorphshape.h"

CNi3dsMorphShape::CNi3dsMorphShape(void)
{
}

CNi3dsMorphShape::~CNi3dsMorphShape(void)
{
}


bool	CNi3dsMorphShape::Load(CNifStream& stream)
{
	stream.readSkip(14);
	return true;
}


#ifdef ANALYSE_NIF
void	CNi3dsMorphShape::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "3dsMorphShape",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif