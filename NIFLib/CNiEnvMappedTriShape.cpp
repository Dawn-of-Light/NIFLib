#include "StdAfx.h"
#include ".\cnienvmappedtrishape.h"

CNiEnvMappedTriShape::CNiEnvMappedTriShape(void)
{
	mpc_name = NULL;
}

CNiEnvMappedTriShape::~CNiEnvMappedTriShape(void)
{
	SAFE_DELETE(mpc_name);
}



bool	CNiEnvMappedTriShape::Load(CNifStream& stream)
{
	ASSERT(stream.Version() == 0x03010000);

	mpc_name =  stream.readstring();

	unsigned a;
	a = stream.readUInt();	assert(a==0);// ?

	mc_parent.read(stream,1);

	a = stream.readWord();	assert(a==4 || a==5);// ?

	stream.readMatrix(mc_matrix);

	mc_childs.read(stream);

	mc_childs2.read(stream,1);
	mc_childs3.read(stream,1);


	return true;
}



void	CNiEnvMappedTriShape::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_parent.Resolve(ppc_nodes, nof);		
	mc_childs.Resolve(ppc_nodes, nof);		
	mc_childs2.Resolve(ppc_nodes, nof);		
	mc_childs3.Resolve(ppc_nodes, nof);
}


#ifdef ANALYSE_NIF
void	CNiEnvMappedTriShape::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "EnvMappedTriShape",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif