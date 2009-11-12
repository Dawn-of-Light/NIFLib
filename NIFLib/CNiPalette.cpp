#include "StdAfx.h"
#include ".\cnipalette.h"

CNiPalette::CNiPalette(void)
{
	mpc_palette = NULL;
}

CNiPalette::~CNiPalette(void)
{
	SAFE_DELETE(mpc_palette);
}


bool	CNiPalette::Load(CNifStream& stream)
{
	stream.readUChar(); // flag 
	unsigned a = stream.readUInt(); assert(a==256); // nof colors
	mpc_palette = stream.readBlock(1024);
	return true;
}


unsigned char* CNiPalette::GetColor(unsigned char index)
{
	ASSERT(mpc_palette);
	return &mpc_palette[index*4];
}


#ifdef ANALYSE_NIF
void	CNiPalette::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "Palette",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
