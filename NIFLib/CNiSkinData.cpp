#include "StdAfx.h"
#include ".\cniskindata.h"

CNiSkinData::CNiSkinData(void)
{
}

CNiSkinData::~CNiSkinData(void)
{
}


bool	CNiSkinData::Load(CNifStream& stream)
{

	stream.readSkip(13*4); // floats

	unsigned nof = stream.readUInt();
	mc_partion.read(stream,1);

	if (stream.Version() >= 0x04020000)
	{
		int mn_unknown1 = stream.readUChar();
		assert(mn_unknown1 == 1);
	}

	for (unsigned t=0;t<nof;t++)
	{
		stream.readSkip(17*4); // floats

		unsigned nof2 = stream.readWord();	
		stream.readSkip(nof2*6); // pair of word-float- maybe bone->weigth
	}

	return true;
}


void	CNiSkinData::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_partion.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiSkinData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"SkinData",TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	assert(mc_partion.GetCount()==0 || mc_partion.GetCount() == 1);
	if (mc_partion.GetCount()==1)
	{
		mc_partion[0]->CreateDbgTree(newpar, addfkt);
	}
}
#endif


