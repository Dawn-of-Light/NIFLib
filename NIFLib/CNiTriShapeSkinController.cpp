#include "StdAfx.h"
#include ".\cnitrishapeskincontroller.h"

CNiTriShapeSkinController::CNiTriShapeSkinController(void)
{
}

CNiTriShapeSkinController::~CNiTriShapeSkinController(void)
{
}


bool	CNiTriShapeSkinController::Load(CNifStream& stream)
{
	unsigned a;
	a = stream.readUInt();	assert(a==0);// ?

	unsigned nof = stream.readWord();
	stream.readFloat();
	stream.readFloat();
	stream.readUInt();
	stream.readUInt();
	stream.readUInt();		// id

	unsigned nof2 = stream.readUInt();
	int sum = 0;
	for (unsigned t=0;t<nof2;t++)
	{
		sum += stream.readUInt();
	}

	stream.readSkip(nof2*4); //id's


	for (int t=0;t<sum;t++)
	{
		stream.readFloat();
		unsigned a = stream.readWord();
		stream.readFloat();
		stream.readFloat();
		stream.readFloat();
	}

	return true;
}


#ifdef ANALYSE_NIF
void	CNiTriShapeSkinController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "TriShapeSkinController",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);
}
#endif