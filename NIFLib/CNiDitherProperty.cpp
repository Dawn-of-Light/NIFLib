#include "StdAfx.h"
#include ".\cniditherproperty.h"

CNiDitherProperty::CNiDitherProperty(void)
{
}

CNiDitherProperty::~CNiDitherProperty(void)
{
}


bool	CNiDitherProperty::Load(CNifStream& stream)
{
	stream.readSkip(14);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiDitherProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "DitherProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif