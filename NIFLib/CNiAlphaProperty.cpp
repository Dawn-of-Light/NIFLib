#include "StdAfx.h"
#include ".\cnialphaproperty.h"

CNiAlphaProperty::CNiAlphaProperty(void)
{
}

CNiAlphaProperty::~CNiAlphaProperty(void)
{
}


bool	CNiAlphaProperty::Load(CNifStream& stream)
{
	if (stream.Version() < 0x03000000)	stream.readSkip(18);
	else								stream.readSkip(15);

	return true;
}



#ifdef ANALYSE_NIF
void	CNiAlphaProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "AlphaProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif