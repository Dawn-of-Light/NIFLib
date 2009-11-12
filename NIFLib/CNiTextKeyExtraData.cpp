#include "StdAfx.h"
#include ".\cnitextkeyextradata.h"

CNiTextKeyExtraData::CNiTextKeyExtraData(void)
{
}

CNiTextKeyExtraData::~CNiTextKeyExtraData(void)
{
}



bool	CNiTextKeyExtraData::Load(CNifStream& stream)
{
	ASSERT(stream.Version() <= 0x03030000);

	unsigned a;
	a = stream.readUInt(); ASSERT(a==0);
	a = stream.readUInt(); ASSERT(a==0);

	unsigned nof = 	stream.readUInt(); 
	for (unsigned t=0;t<nof;t++)
	{
		float time = stream.readFloat();
		char* str = stream.readstring();
		delete(str);
	}

	return true;
}




#ifdef ANALYSE_NIF
void	CNiTextKeyExtraData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNode::CreateDbgTree(par,addfkt);
}
#endif