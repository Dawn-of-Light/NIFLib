#include "StdAfx.h"
#include ".\cnipointlight.h"

CNiPointLight::CNiPointLight(void)
{
	mpc_light_name = NULL;
}

CNiPointLight::~CNiPointLight(void)
{
	SAFE_DELETE(mpc_light_name);
}


bool	CNiPointLight::Load(CNifStream& stream)
{
	mpc_light_name = stream.readstring();
	stream.readSkip(131);
	return true;
}




#ifdef ANALYSE_NIF
void	CNiPointLight::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[1024];	sprintf(temp, "PointLight: %s", mpc_light_name);
	newpar = addfkt(par,this, temp,TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif