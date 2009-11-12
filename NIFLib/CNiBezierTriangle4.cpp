#include "StdAfx.h"
#include ".\cnibeziertriangle4.h"

CNiBezierTriangle4::CNiBezierTriangle4(void)
{
}

CNiBezierTriangle4::~CNiBezierTriangle4(void)
{
}



bool	CNiBezierTriangle4::Load(CNifStream& stream)
{
	ASSERT(false);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiBezierTriangle4::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNode::CreateDbgTree(par,addfkt);
}
#endif