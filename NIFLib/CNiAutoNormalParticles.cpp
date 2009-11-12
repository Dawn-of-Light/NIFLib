#include "StdAfx.h"
#include ".\cniautonormalparticles.h"

CNiAutoNormalParticles::CNiAutoNormalParticles(void)
{
}

CNiAutoNormalParticles::~CNiAutoNormalParticles(void)
{
}


bool	CNiAutoNormalParticles::Load(CNifStream& stream)
{
	unsigned i;

	mpc_node_name = stream.readstring();


	i = stream.readUInt();
	assert((stream.Version() <0x04000000 && i == 0) || (stream.Version() >=0x04000000 &&  i == 0xffffffff));


	// vis controller
	mc_controller.read(stream,1);


	if (stream.Version() < 0x03000000)	stream.readSkip(1);
	else								stream.readSkip(2);



	// Matrix 
	stream.readMatrix(mc_matrix);


	if (stream.Version() < 0x03000000)	
	{
		unsigned sum = 0;
		unsigned nof = stream.readUInt();
		for (unsigned n = 0; n<nof;n++)
		{
			i = stream.readUInt();
			assert(i == 0);

			unsigned val = stream.readWord();
			sum += val;

			i = stream.readUInt(); // data
			i = stream.readUInt(); // data
			i = stream.readUInt(); // data
			i = stream.readUInt(); // data
			i = stream.readUInt(); // data

			stream.readSkip(val*12);
		}

		stream.readSkip(sum*10+8);
	}
	else						
	{	

		if (stream.Version() <0x04000000)	mc_properties.read(stream,1);	
		else								mc_properties.read(stream);	

		BOOL res = stream.readBool();
		assert(!res);//, "was always 0, maybe here is another list");

		mc_childs.read(stream,1);	


		if (stream.Version() >0x04000000)
		{
			i = stream.readUInt();
			assert(i==0xffffffff);//, "was always 0, maybe here is another list");
		}
	}


	return true;
}



#ifdef ANALYSE_NIF
void	CNiAutoNormalParticles::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNiNode::CreateDbgTree(par,addfkt);
}
#endif