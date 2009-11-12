#include "StdAfx.h"
#include ".\cnitextureeffect.h"

CNiTextureEffect::CNiTextureEffect(void)
{
	mpc_name = NULL;
}

CNiTextureEffect::~CNiTextureEffect(void)
{
	SAFE_DELETE(mpc_name);
}


bool	CNiTextureEffect::Load(CNifStream& stream)
{
	mpc_name = stream.readstring();


	unsigned a;
	a = stream.readUInt(); assert(a==0 || a == 0xffffffff);// id?
	a = stream.readUInt(); // id?
	a = stream.readWord(); //assert(a==4); <- in v3.1
	stream.readSkip(16*4); // floats (matrix? but why?)
	
	if (stream.Version() < 0x04000000)
	{
		a = stream.readUInt(); assert(a==0);
		a = stream.readUInt(); assert(a==0);
		unsigned nof = stream.readUInt();
		stream.readSkip(nof*4); // id's
	}
	else
	{
		a = stream.readUChar();	assert(a==0);
		stream.readFloat();
	}

	stream.readSkip(12*4); // floats


	a = stream.readUInt(); assert(a==2 || a==1);
	a = stream.readUInt(); assert(a==0);
	a = stream.readUInt(); assert(a==2 || a==1);
	a = stream.readUInt(); assert(a==2 || a==1);

	mc_data.read(stream,1);

	a = stream.readUChar();  assert(a==0);
	stream.readSkip(5*4); // floats

	if (stream.Version() < 0x04020100)
	{	a = stream.readWord(); assert(a==0); }



	return true;
}



void	CNiTextureEffect::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_data.Resolve(ppc_nodes, nof);		
}


#ifdef ANALYSE_NIF
void	CNiTextureEffect::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*,char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256];
	sprintf(temp,"TextureEffect: %s",mpc_name);
	newpar = addfkt(par,this,temp,TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_data,"Data",TIC_TEXTURE,newpar,addfkt);
}
#endif


