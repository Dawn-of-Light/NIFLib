#include "StdAfx.h"
#include ".\cniimage.h"
#include "CTextureManager.h"
#include "CNiRawImageData.h"


CNiImage::CNiImage(void)
{
	mp_texturename = NULL;
	mn_texture_id = -1;
}

CNiImage::~CNiImage(void)
{
	if (mn_texture_id!=-1)	gpc_texturemanager->ReleaseTexture(mn_texture_id);
	SAFE_DELETE(mp_texturename);
}



bool	CNiImage::Load(CNifStream& stream)
{
	if (stream.Version() < 0x03000000)	stream.readSkip(8);

	mb_external = (stream.readUChar()!=0);

	if (mb_external !=0)
        mp_texturename = stream.readstring();
	else
		mc_texture.read(stream,1);

	mdw_unknown1 = stream.readUInt();
	if (stream.Version() == 0x03010000)	mdw_unknown2 = stream.readUInt();

	return true;
}



void	CNiImage::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_texture.Resolve(ppc_nodes, nof);		
}


void	CNiImage::Create()
{
	// already created
	if (mn_texture_id != -1) return;


	// Maik Jurkait  07.06.2004 19:31:30
	// ------------------------------------------
	//	Extern texture
	// ------------------------------------------
	if (mp_texturename && mp_texturename[0] != 0)
		mn_texture_id = gpc_texturemanager->GetTexture(mp_texturename);
	else

	// Maik Jurkait  07.06.2004 19:31:42
	// ------------------------------------------
	//	Internal texture
	// ------------------------------------------
	{
		// only one texture supported
		ASSERT(mc_texture.GetCount()==1);

		// only "NiPixelData" should be here
		ASSERT(mc_texture[0]->IsClass("NiRawImageData"));

		// create it
		mc_texture[0]->Create();
		mn_texture_id = ((CNiRawImageData*)mc_texture[0])->GetTextureID();
		gpc_texturemanager->GetTexture(mn_texture_id);

		// release it
		mc_texture.Clear();
	}
}



void	CNiImage::Render()
{
	gpc_texturemanager->ActivateTexture(mn_texture_id);
}




#ifdef ANALYSE_NIF
void	CNiImage::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256];
	if (mb_external && mp_texturename && mp_texturename[0] != 0)
						sprintf(temp,"Texture: %s",mp_texturename);
	else				sprintf(temp,"Internal Texture");

	newpar = addfkt(par,this, temp,TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
}
#endif





