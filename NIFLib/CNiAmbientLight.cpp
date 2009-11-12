#include "StdAfx.h"
#include ".\cniambientlight.h"

CNiAmbientLight::CNiAmbientLight(void)
{
	mpc_light_name = NULL;
}

CNiAmbientLight::~CNiAmbientLight(void)
{
	SAFE_DELETE(mpc_light_name);
}


bool	CNiAmbientLight::Load(CNifStream& stream)
{
	mpc_light_name = stream.readstring();

	if (stream.Version() < 0x04010000)	stream.readSkip(130);
	else								stream.readSkip(119);

	return true;
}



#ifdef ANALYSE_NIF
void	CNiAmbientLight::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[1024];	sprintf(temp, "AmbLight: %s", mpc_light_name);
	newpar = addfkt(par,this, temp,TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif