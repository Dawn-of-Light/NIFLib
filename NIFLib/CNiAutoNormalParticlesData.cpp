#include "StdAfx.h"
#include ".\cniautonormalparticlesdata.h"

CNiAutoNormalParticlesData::CNiAutoNormalParticlesData(void)
{
}

CNiAutoNormalParticlesData::~CNiAutoNormalParticlesData(void)
{
}


bool	CNiAutoNormalParticlesData::Load(CNifStream& stream)
{

		unsigned short len = stream.readWord(); 
		BOOL data = stream.readBool();
		if (data)	stream.readSkip(len*12);

		data = stream.readBool();
		stream.readSkip(len*12);

		stream.readSkip(16);
		data = stream.readBool();
		if (data)	stream.readSkip(len*16);

		stream.readSkip(8);
		if (stream.Version() < 0x04000000)	stream.readSkip(6);

		data = stream.readBool();
		if (data)	stream.readSkip(len*4);

		return true;
}



#ifdef ANALYSE_NIF
void	CNiAutoNormalParticlesData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "AutoNormalParticlesData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif