#include "StdAfx.h"
#include ".\cnitextureproperty.h"

CNiTextureProperty::CNiTextureProperty(void)
{
}

CNiTextureProperty::~CNiTextureProperty(void)
{
}


bool	CNiTextureProperty::Load(CNifStream& stream)
{
	// this chunk is only used in old files
	ASSERT(stream.Version() < 0x04000000);

	if (stream.Version() >= 0x03000000)
	{
		stream.readSkip(14);

		mc_difusemap.read(stream,1);

		if (stream.Version() != 0x03010000)
			stream.readSkip(8);
	}
	else
	{
		stream.readSkip(13);
		unsigned i = stream.readUInt();	assert(i==1);
		mc_difusemap.read(stream,1);
	}

	return true;
}


void	CNiTextureProperty::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_difusemap.Resolve(ppc_nodes, nof);		

	#ifdef _DEBUG
	for (int t=0;t<mc_difusemap.GetCount();t++)
	{
		ASSERT(stricmp("NiImage",mc_difusemap[t]->GetClassName())==0);
	}
	#endif
}


void	CNiTextureProperty::RemoveTexture()
{
	mc_difusemap.Clear();
}



void	CNiTextureProperty::Render()
{
	FOREACH(pc,mc_difusemap,	pc->Render(); )
}

void	CNiTextureProperty::Create()
{
	FOREACH(pc,mc_difusemap,	pc->Create(); )
}





#ifdef ANALYSE_NIF
void	CNiTextureProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"TextureProp",TIC_TEXTURE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
	// mc_matrix;

	CreateDbgTreeChilds(mc_difusemap,"Difusemap",TIC_TEXTURE,newpar,addfkt);
}
#endif 

