#include "StdAfx.h"
#include "cnirawimagedata.h"
#include "CTextureManager.h"

CNiRawImageData::CNiRawImageData(void)
{
	mn_texture_id = -1;
	mp_pixel_data = NULL;
}

CNiRawImageData::~CNiRawImageData(void)
{
	if (mn_texture_id != -1)	gpc_texturemanager->ReleaseTexture(mn_texture_id);
	SAFE_DELETE(mp_pixel_data);
}


bool	CNiRawImageData::Load(CNifStream& stream)
{
	if (stream.Version() <0x03000000) stream.readSkip(8);


	mn_width = stream.readUInt();
	mn_height = stream.readUInt();
	unsigned format = stream.readUInt();
	assert(format==1 || format==2);//"check value"

	mn_bpp=3;
	if (format == 2) mn_bpp=4;

	mp_pixel_data = stream.readBlock(mn_width*mn_height*mn_bpp);

	return true;
}


void	CNiRawImageData::Create()
{
	// already created?
	if (mn_texture_id!= -1) return;

	ASSERT(mp_pixel_data);
	mn_texture_id = gpc_texturemanager->CreateTexture(mn_width,mn_height,mp_pixel_data,mn_bpp);
	mp_pixel_data = NULL;
}


GLuint	CNiRawImageData::GetTextureID()
{
	return mn_texture_id;
}


#ifdef ANALYSE_NIF
void	CNiRawImageData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "RawImageData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
