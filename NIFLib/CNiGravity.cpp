#include "StdAfx.h"
#include ".\cnigravity.h"

CNiGravity::CNiGravity(void)
{
}

CNiGravity::~CNiGravity(void)
{
}


bool	CNiGravity::Load(CNifStream& stream)
{
	mc_child.read(stream,1);
	stream.readUInt(); // parent

	stream.readSkip(28);
	if (stream.Version() >= 0x04010000)		stream.readSkip(8);


/*
 Result = FileHandle.ReadLong(m_Unknown1);
  if (Result) Result = FileHandle.ReadLong(m_Unknown2);
  if (Result) Result = FileHandle.ReadLong(m_Unknown3);
  if (Result) Result = FileHandle.Read((char *)&m_Floats[0], sizeof(float)*8);
*/

	return true;
}


void	CNiGravity::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_child.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiGravity::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this, "Gravity",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_child,"Childs",TIC_PARTICLE,newpar,addfkt);
}
#endif

