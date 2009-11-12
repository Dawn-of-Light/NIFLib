#include "StdAfx.h"
#include ".\cniparticlemeshes.h"

CNiParticleMeshes::CNiParticleMeshes(void)
{
}

CNiParticleMeshes::~CNiParticleMeshes(void)
{
}


bool	CNiParticleMeshes::Load(CNifStream& stream)
{
	mpc_node_name = stream.readstring();

	unsigned char  i;
	i = stream.readUInt();

	// vis controller
	mc_controller.read(stream,1);

	stream.readSkip(2);

	// Matrix 
	stream.readMatrix(mc_matrix);

	// Properties (Texturemodeporperty, VertexColorProperty)
	mc_properties.read(stream);	
	
	i = stream.readUChar(); // flag??
	assert(i==0);// was always 0, maybe here is another list");

	// childs
	mc_childs.read(stream,1);	// 

	stream.readUInt();
	//ASSERTE(i==0, "was always 0, maybe here is another list");
	return true;	
}



#ifdef ANALYSE_NIF
void	CNiParticleMeshes::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNiNode::CreateDbgTree(par,addfkt);
}
#endif
