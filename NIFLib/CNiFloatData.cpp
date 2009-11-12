#include "StdAfx.h"
#include ".\cnifloatdata.h"

CNiFloatData::CNiFloatData(void)
{
}

CNiFloatData::~CNiFloatData(void)
{
}


bool	CNiFloatData::Load(CNifStream& stream)
{
	unsigned len = stream.readUInt();
	stream.readUInt();

	if (stream.Version() <= 0x03000000)
		stream.readSkip(len*8);
	else
		stream.readSkip(len*16);


	return true;
}



#ifdef ANALYSE_NIF
void	CNiFloatData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "FloatData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif