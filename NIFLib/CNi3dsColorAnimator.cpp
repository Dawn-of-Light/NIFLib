#include "StdAfx.h"
#include ".\cni3dscoloranimator.h"

CNi3dsColorAnimator::CNi3dsColorAnimator(void)
{
}

CNi3dsColorAnimator::~CNi3dsColorAnimator(void)
{
}


bool	CNi3dsColorAnimator::Load(CNifStream& stream)
{
	stream.readSkip(184);
	return true;
}

#ifdef ANALYSE_NIF
void	CNi3dsColorAnimator::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "3dsColorAnimator",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif