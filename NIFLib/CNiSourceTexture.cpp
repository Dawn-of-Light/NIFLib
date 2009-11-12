#include "StdAfx.h"
#include "cnisourcetexture.h"
#include "CNiPixelData.h"
#include "CTextureManager.h"


CNiSourceTexture::CNiSourceTexture(void)
{
	mpc_texture_name = NULL;
	mn_texture_id = -1;
}

CNiSourceTexture::~CNiSourceTexture(void)
{
	if (mn_texture_id != -1)
		gpc_texturemanager->ReleaseTexture(mn_texture_id);

	SAFE_DELETE(mpc_texture_name);
}



bool	CNiSourceTexture::Load(CNifStream& stream)
{
	unsigned a;
	a = stream.readUInt();	assert(a==0);			// unexspected value
	a = stream.readUInt();	assert(a==0xffffffff);	// unexspected value
	a = stream.readUInt();	assert(a==0xffffffff);	// unexspected value


	int externtex = stream.readUChar();
	if (externtex)
	{
		mpc_texture_name = stream.readstring();
	}
	else
	{
		int interntex = stream.readUChar();
		assert(interntex);
		mc_internaltexture.read(stream,1);
	}


	mn_unknown1 = stream.readUInt();	//assert(a==0 || a==4 || a==5);
	mn_unknown2 = stream.readUInt();	//assert(a>=0 && a<=3);
	mn_unknown3 = stream.readUInt();	//assert(a==1 || a==3);
	a = stream.readUChar();	assert(a==1);

	return true;
}


void	CNiSourceTexture::SetTexture(const char* name)
{
	// Maik Jurkait  12.06.2004 11:03:33
	// ------------------------------------------
	//	clear old data
	// ------------------------------------------
	if (mn_texture_id != -1)
		gpc_texturemanager->ReleaseTexture(mn_texture_id);

	mc_internaltexture.Clear();

	SAFE_DELETE(mpc_texture_name);
	mpc_texture_name = new char[strlen(name)+1];
	strcpy(mpc_texture_name,name);
}



void	CNiSourceTexture::SetTexture(GLuint new_texture_id)
{
	// Maik Jurkait  12.06.2004 11:03:33
	// ------------------------------------------
	//	clear old data
	// ------------------------------------------
	if (mn_texture_id != -1)
		gpc_texturemanager->ReleaseTexture(mn_texture_id);

	mc_internaltexture.Clear();


	// Maik Jurkait  12.06.2004 11:03:36
	// ------------------------------------------
	//	
	// ------------------------------------------
	mn_texture_id = new_texture_id;
	gpc_texturemanager->GetTexture(mn_texture_id);
}



void	CNiSourceTexture::Create()
{
	if (mn_texture_id != -1) return; // already created


	// Maik Jurkait  01.06.2004 18:23:53
	// ------------------------------------------
	//	std: load texture
	// ------------------------------------------
	if (mc_internaltexture.GetCount()==0)
	{
		mn_texture_id = gpc_texturemanager->GetTexture(mpc_texture_name);
		return;
	}



	// Maik Jurkait  01.06.2004 18:16:44
	// ------------------------------------------
	//	if this is an internal texture -> create it
	// ------------------------------------------
	if (mc_internaltexture.GetCount()>0)
	{
		// only one texture supported
		ASSERT(mc_internaltexture.GetCount()==1);

		// only "NiPixelData" should be here
		ASSERT(mc_internaltexture[0]->IsClass("NiPixelData"));

		// create it
		mc_internaltexture[0]->Create();
		mn_texture_id = ((CNiPixelData*)mc_internaltexture[0])->GetTextureID();
		gpc_texturemanager->GetTexture(mn_texture_id);

		// release it
		mc_internaltexture.Clear();
		return;
	}


}



void	CNiSourceTexture::Render()
{
	gpc_texturemanager->ActivateTexture(mn_texture_id);
}




void	CNiSourceTexture::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_internaltexture.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiSourceTexture::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256];
	if (mpc_texture_name)	sprintf(temp,"Texture: %s",mpc_texture_name);
	else					sprintf(temp,"Internal Texture");
	newpar = addfkt(par,this,temp,TIC_TEXTURE | TICS_UNKNOWNDATA_LISTED);


	newsub = addfkt(newpar,NULL,"Data",TIC_DATA| TICS_FINISHED);
	CNode::CreateDbgTree(newsub,addfkt);

	newsub = addfkt(newpar,NULL,"Unknown",TIC_ABSTRACT);
	sprintf(temp,"u1 (0-5)= %i",mn_unknown1);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"u2 (0-3)= %i",mn_unknown2);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"u3 (1-3)= %i",mn_unknown3);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);

}
#endif


