#include "StdAfx.h"
#include ".\cniposdata.h"

CNiPosData::CNiPosData(void)
{
}

CNiPosData::~CNiPosData(void)
{
}

bool	CNiPosData::Load(CNifStream&stream)
{
	unsigned len1 = stream.readUInt(); // 0x79
	unsigned len2 = stream.readUInt(); // 2

	stream.readSkip(10*4*len1);
	return true;
}




#ifdef ANALYSE_NIF
void	CNiPosData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "PosData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
