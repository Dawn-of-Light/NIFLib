#include "StdAfx.h"
#include ".\cnispecularproperty.h"

CNiSpecularProperty::CNiSpecularProperty(void)
{
}

CNiSpecularProperty::~CNiSpecularProperty(void)
{
}


bool	CNiSpecularProperty::Load(CNifStream& stream)
{
	stream.readSkip(14);
	return true;
}




#ifdef ANALYSE_NIF
void	CNiSpecularProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "SpecularProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif