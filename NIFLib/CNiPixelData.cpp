#include "StdAfx.h"
#include "cnipixeldata.h"
#include "CTextureManager.h"
#include "CNiPalette.h"


CNiPixelData::CNiPixelData(void)
{
	mpc_pixel_data = NULL;
	mn_texture_id = -1;
}

CNiPixelData::~CNiPixelData(void)
{
	if (mn_texture_id != -1)	gpc_texturemanager->ReleaseTexture(mn_texture_id);
	SAFE_DELETE(mpc_pixel_data);
}



bool	CNiPixelData::Load(CNifStream& stream)
{
	unsigned a;

    a = stream.readUInt();	// flag if active?
	a = stream.readUInt();	// 0, 0xff
	a = stream.readUInt();	// 0, 0xff00
	a = stream.readUInt();	// 0, 0xff0000
	a = stream.readUInt();	// 0, 0

    a = stream.readUInt();	
	assert(a==0x08 || a==0x18 || a==0x20);


	if (a == 0x08)  // 256 colors image pallette
	{
		a = stream.readUInt();		assert(a==0x22);
		a = stream.readUInt();		assert(a==0);
		mc_palette.read(stream,1);
		a = stream.readUInt();		assert(a==1);

		mn_bpp = stream.readUInt();	assert(mn_bpp==1); 
		mn_size_x = stream.readUInt(); 
		mn_size_y = stream.readUInt(); 

		a = stream.readUInt();	assert(a==0);
		a = stream.readUInt();	assert(a==mn_size_x*mn_size_y);

		mpc_pixel_data = stream.readBlock(a);
	}
	else
	{
		unsigned size = 0;
		a = stream.readUInt();//	assert(a==0x820860);
		a = stream.readUInt();//	assert(a==0x4100);
		a = stream.readUInt();		assert(a==0xffffffff);

		unsigned nof_mipmaps = stream.readUInt();

		mn_bpp = stream.readUInt();
		mn_size_x = stream.readUInt();
		mn_size_y = stream.readUInt();

		unsigned pos = stream.readUInt();
		size = mn_size_x*mn_size_y*mn_bpp;

		// read all mipmaps
		for (unsigned n=1;n<nof_mipmaps;n++)
		{
			unsigned x = stream.readUInt();
			unsigned y = stream.readUInt();
			unsigned pos = stream.readUInt();
			size += x*y*mn_bpp;
		}

		a = stream.readUInt();		assert(a==size);

		stream.readSkip(pos);
		mpc_pixel_data = stream.readBlock(mn_size_x*mn_size_y*mn_bpp);

		// only needed first mip level
		stream.readSkip(size-pos-mn_size_x*mn_size_y*mn_bpp);
	}


	return true;
}



void	CNiPixelData::Create()
{
	// already created?
	if (mn_texture_id!= -1) return;
	ASSERT(mpc_pixel_data);


	// Maik Jurkait  01.06.2004 18:28:22
	// ------------------------------------------
	//	24 or 32 bit texture
	// ------------------------------------------
	if (mn_bpp>2)
	{
		mn_texture_id = gpc_texturemanager->CreateTexture(mn_size_x,mn_size_y,mpc_pixel_data,mn_bpp);
		mpc_pixel_data = NULL;
	}

	// Maik Jurkait  01.06.2004 18:28:44
	// ------------------------------------------
	//	8bit texture
	//  simply convert it to 32 bit
	//  \todo here we could create a 8bit texture
	// ------------------------------------------
	else
	{
		ASSERT(mn_bpp==1);
		ASSERT(mc_palette.GetCount()==1);
		ASSERT(mc_palette[0]->IsClass("NiPalette"));
		CNiPalette* pal = (CNiPalette*)(mc_palette[0]);

		// Maik Jurkait  01.06.2004 18:29:33
		// ------------------------------------------
		//	Convert it
		// ------------------------------------------
		unsigned char* pic = new unsigned char [mn_size_x*mn_size_y*4];
		unsigned char* run = pic;
		for (unsigned i =0;i<mn_size_x*mn_size_y;i++)
		{
			unsigned char *col = pal->GetColor(mpc_pixel_data[i]);
			*run++ = col[0];
			*run++ = col[1];
			*run++ = col[2];
			*run++ = col[3];
		}

		// Maik Jurkait  01.06.2004 18:33:10
		// ------------------------------------------
		//	Clear unneeded memory
		// ------------------------------------------
		delete [] mpc_pixel_data;
		mpc_pixel_data = NULL;
		mc_palette.Clear();


		mn_texture_id = gpc_texturemanager->CreateTexture(mn_size_x,mn_size_y,pic,4);
	}
}




GLuint	CNiPixelData::GetTextureID()
{
	return mn_texture_id;
}


void	CNiPixelData::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_palette.Resolve(ppc_nodes, nof);		
}




#ifdef ANALYSE_NIF
void	CNiPixelData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "PixelData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
