#include "StdAfx.h"
#include ".\cnirotatingparticlesdata.h"

CNiRotatingParticlesData::CNiRotatingParticlesData(void)
{
}

CNiRotatingParticlesData::~CNiRotatingParticlesData(void)
{
}


bool	CNiRotatingParticlesData::Load(CNifStream& stream)
{
	unsigned char data;
	unsigned short len = stream.readWord();

	data = stream.readUChar();
	if (data)	stream.readSkip(len*12);

	stream.readSkip(17);

	data = stream.readUChar();
	if (data)	stream.readSkip(len*16);

	stream.readSkip(8);
	data = stream.readUChar();
	if (data)	stream.readSkip(len*20);
	stream.readSkip(1);

	return true;
}



#ifdef ANALYSE_NIF
void	CNiRotatingParticlesData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "RotatingParticlesData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif
