#include "StdAfx.h"
#include ".\cnistringextradata.h"

CNiStringExtraData::CNiStringExtraData(void)
{
	mpc_string = NULL;
}

CNiStringExtraData::~CNiStringExtraData(void)
{
	SAFE_DELETE(mpc_string);
}


bool	CNiStringExtraData::LoadV2_3embedded(CNifStream& stream)
{
	// Maik Jurkait  09.06.2004 14:25:39
	// ------------------------------------------
	//	in v2.3 files this can be embedded in other chunkds
	// ------------------------------------------
	char* block = stream.readstring();
	assert(strcmp(block,"NiStringExtraData")==0);
	if (strcmp(block,"NiStringExtraData")!=0)
	{
		delete (block);
		return false;
	}
	delete (block);

	stream.readUInt(); 
			
	mpc_string = stream.readstring();

	return true;
}



bool	CNiStringExtraData::Load(CNifStream& stream)
{
	mc_next_string.read(stream,1);
	stream.readUInt();
	mpc_string = stream.readstring();

	return true;
}


void	CNiStringExtraData::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_next_string.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiStringExtraData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,mpc_string,TIC_DATA);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	FOREACH(pc,mc_next_string, pc->CreateDbgTree(par, addfkt););
}
#endif