#include "StdAfx.h"
#include ".\cnicolordata.h"

CNiColorData::CNiColorData(void)
{
}

CNiColorData::~CNiColorData(void)
{
}


bool	CNiColorData::Load(CNifStream& stream)
{
	stream.readSkip(68);
	return true;
}



#ifdef ANALYSE_NIF
void	CNiColorData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "ColorData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif