#include "StdAfx.h"
#include ".\cnizbufferproperty.h"

CNiZBufferProperty::CNiZBufferProperty(void)
{
}

CNiZBufferProperty::~CNiZBufferProperty(void)
{
}


bool	CNiZBufferProperty::Load(CNifStream& stream)
{
	if (stream.Version() < 0x03000000)	stream.readSkip(11);
	else
	if (stream.Version() < 0x04000003)	stream.readSkip(14);
	else								stream.readSkip(18);
		
	return true;
}



#ifdef ANALYSE_NIF
void	CNiZBufferProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "ZBufferProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif