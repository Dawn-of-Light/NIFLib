#include "StdAfx.h"
#include ".\cnimorphcontroller.h"

CNiMorphController::CNiMorphController(void)
{
}

CNiMorphController::~CNiMorphController(void)
{
}



bool	CNiMorphController::Load(CNifStream& stream)
{
	assert(stream.Version()<=0x03030000);

	return true;
}




#ifdef ANALYSE_NIF
void	CNiMorphController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "MorphController",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
}
#endif