#include "StdAfx.h"
#include ".\cniwireframeproperty.h"

CNiWireframeProperty::CNiWireframeProperty(void)
{
}

CNiWireframeProperty::~CNiWireframeProperty(void)
{
}


bool	CNiWireframeProperty::Load(CNifStream& stream)
{
	stream.readSkip(14);
	return true;
}


#ifdef ANALYSE_NIF
void	CNiWireframeProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "WireframeProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif