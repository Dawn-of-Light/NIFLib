#include "StdAfx.h"
#include ".\cnisphericalcollider.h"

CNiSphericalCollider::CNiSphericalCollider(void)
{
}

CNiSphericalCollider::~CNiSphericalCollider(void)
{
}


bool	CNiSphericalCollider::Load(CNifStream& stream)
{
	stream.readSkip(30);
	if (stream.Version() < 0x04010000)	stream.readSkip(10);

	return true;
}



#ifdef ANALYSE_NIF
void	CNiSphericalCollider::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "SphericalCollider",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif