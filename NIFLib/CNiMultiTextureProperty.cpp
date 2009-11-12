#include "StdAfx.h"
#include ".\cnimultitextureproperty.h"

CNiMultiTextureProperty::CNiMultiTextureProperty(void)
{
}

CNiMultiTextureProperty::~CNiMultiTextureProperty(void)
{
}


bool	CNiMultiTextureProperty::Load(CNifStream& stream)
{

	assert(stream.Version()>=0x03000000&& stream.Version() <=0x03010000);

	unsigned a;
	a = stream.readUInt(); assert(a==0);
	a = stream.readUInt(); assert(a==0);
	a = stream.readUInt(); assert(a==0);
	a = stream.readWord(); assert(a==0);

	unsigned nof_textures = stream.readUInt();
	for (unsigned i=0;i<nof_textures;i++)
	{
		unsigned id = stream.readUInt();
		if (id==0) continue;
		mc_texture.read(stream,1); // texture

		a = stream.readUInt(); assert(a==0 || a==2 || a==3);
		a = stream.readUInt(); assert(a==1 || a==2);
		a = stream.readUInt(); assert(a==0 || a==1 || a==2 || a==0xffffffff);

		if (stream.Version() > 0x03000000)
		{
			a = stream.readWord(); assert(a==0);
			a = stream.readWord(); assert(a==0xffb5);
			a = stream.readWord(); assert(a==0);
		}
	}

	return true;
}


void	CNiMultiTextureProperty::Render()
{
	FOREACH(pc,mc_texture, pc->Render(););
}

void	CNiMultiTextureProperty::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_texture.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiMultiTextureProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this,"MultiTextureProperty",TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_texture,"Texture",TIC_TEXTURE,newpar,addfkt);
}
#endif
