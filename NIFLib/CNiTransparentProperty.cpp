#include "StdAfx.h"
#include ".\cnitransparentproperty.h"

CNiTransparentProperty::CNiTransparentProperty(void)
{
}

CNiTransparentProperty::~CNiTransparentProperty(void)
{
}


bool	CNiTransparentProperty::Load(CNifStream& stream)
{
	ASSERT(stream.Version() == 0x03010000);
	stream.readSkip(18);

	return true;
}



#ifdef ANALYSE_NIF
void	CNiTransparentProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "TransparentProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif