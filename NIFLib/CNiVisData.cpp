#include "StdAfx.h"
#include ".\cnivisdata.h"

CNiVisData::CNiVisData(void)
{
}

CNiVisData::~CNiVisData(void)
{
}


bool	CNiVisData::Load(CNifStream& stream)
{
	unsigned len = stream.readUInt();
	while (len>0)
	{
		struct_key key;
		key.time = stream.readFloat();
		key.vis  = stream.readUChar()==0;
		ma_keys.AddLast(key);
		len--;
	}

	return true;
}



#ifdef ANALYSE_NIF
void	CNiVisData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "VisData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif