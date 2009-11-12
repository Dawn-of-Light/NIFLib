#include "StdAfx.h"
#include ".\cnitexturemodeproperty.h"

CNiTextureModeProperty::CNiTextureModeProperty(void)
{
}

CNiTextureModeProperty::~CNiTextureModeProperty(void)
{
}


bool	CNiTextureModeProperty::Load(CNifStream& stream)
{
	if (stream.Version() == 0x02030000)	stream.readSkip(21);
	else
	if (stream.Version() == 0x03010000)	stream.readSkip(18);
	else
	if (stream.Version() < 0x04000000)	stream.readSkip(14);
	else								stream.readSkip(21);

	return true;
}




#ifdef ANALYSE_NIF
void	CNiTextureModeProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "TextureModeProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif