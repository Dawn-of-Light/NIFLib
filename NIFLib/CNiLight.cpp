#include "StdAfx.h"
#include ".\cnilight.h"

CNiLight::CNiLight(void)
{
}

CNiLight::~CNiLight(void)
{
}


bool	CNiLight::Load(CNifStream& stream)
{
	stream.readSkip(179);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiLight::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "Light",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif