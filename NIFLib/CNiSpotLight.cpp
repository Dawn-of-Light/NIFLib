#include "StdAfx.h"
#include ".\cnispotlight.h"

CNiSpotLight::CNiSpotLight(void)
{
	mpc_light_name = NULL;
}

CNiSpotLight::~CNiSpotLight(void)
{
	SAFE_DELETE(mpc_light_name);
}


bool	CNiSpotLight::Load(CNifStream& stream)
{
	mpc_light_name = stream.readstring();

	stream.readSkip(139);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiSpotLight::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[1024];	sprintf(temp, "SpotLight: %s", mpc_light_name);
	newpar = addfkt(par,this, temp,TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif