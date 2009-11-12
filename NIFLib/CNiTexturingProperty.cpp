#include "StdAfx.h"
#include ".\cnitexturingproperty.h"

CNiTexturingProperty::CNiTexturingProperty(void)
{
}

CNiTexturingProperty::~CNiTexturingProperty(void)
{
}


bool	CNiTexturingProperty::Load(CNifStream& stream)
{
	stream.readUInt();	// 0
	stream.readUInt();	// -1
	stream.readUInt();	// -1
	stream.readWord();	// 0
	stream.readUInt();	// 0,2, 4
	
	unsigned nof_textures = stream.readUInt();
	assert(nof_textures == 7);

	for (unsigned t=0;t<nof_textures;t++)
		if (stream.readBool())
		{
			mc_textures.read(stream,1);

			stream.readUInt();	//(a>=0 && a<=3);
			stream.readUInt();	//(a>=0 && a<=2);
			stream.readUInt();	// 0-5;
			stream.readWord();	// 0;
			stream.readWord();	// assert(a>0xff00);

			if (stream.Version() <= 0x04020000)
			{
				stream.readWord();	//assert(a==0 ||a == 0x100);
			}

			if (t == 5)
			{
				stream.readFloat();
				stream.readFloat();
				stream.readFloat();
				stream.readFloat();
				stream.readFloat();
				stream.readFloat();
			}
		}

	return true;
}


void	CNiTexturingProperty::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_textures.Resolve(ppc_nodes, nof);		

	#ifdef _DEBUG
	for (int t=0;t<mc_textures.GetCount();t++)
	{
		ASSERT(stricmp("NiSourceTexture",mc_textures[t]->GetClassName())==0);
	}
	#endif
}


void	CNiTexturingProperty::Render()
{
	/// \todo atm just rendering first texture
	if (mc_textures.GetCount()>0) mc_textures[0]->Render();

//	FOREACH(pc,mc_textures,	pc->Render(); )
}

void	CNiTexturingProperty::Create()
{
	FOREACH(pc,mc_textures,	pc->Create(); )
}


void	CNiTexturingProperty::RemoveTexture()
{
	mc_textures.Clear();
}

void	CNiTexturingProperty::AddTexture(CNode* pc_node)
{
	mc_textures.Clear();
	mc_textures.AddNode(pc_node);
}



#ifdef ANALYSE_NIF
void	CNiTexturingProperty::CreateDbgTree(void* par, void*(*addfkt)(void*,CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"TexturingProp",TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_textures,"Textures",TIC_TEXTURE,newpar,addfkt);

}
#endif 

