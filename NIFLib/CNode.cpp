#include "StdAfx.h"
#include ".\cnode.h"




CNode::CNode(void)
{
	mdw_id = 0;
	mn_referenzes = 0;
	mdw_render_flags = 0;
}

CNode::~CNode(void)
{
	assert(mn_referenzes==0);
}


bool	CNode::IsClass(const char*name)
{
	return strcmp(name,GetClassName())==0;
}


// ---------- Chunk id's ------------
DWORD	CNode::GetID()
{
	return mdw_id;
}


bool	CNode::Load(CNifStream& stream, DWORD id)
{
	mdw_id = id;

	// in v<4, id's are in file
	if (stream.Version() < 0x04000000)
		mdw_id = stream.readUInt();

	return Load(stream);
}




#ifdef ANALYSE_NIF
void	CNode::CreateDbgTreeChilds(CChilds& childs, char*name,  int image, void* par, void*	(*addfkt)(void*, CNode* node, char* text, int image))
{
	if (childs.GetCount()==0) return;

	void* newpar = par;;
	if (childs.GetCount()>1)
		newpar = addfkt(par,this,name, image);

	FOREACH(pc,childs,	pc->CreateDbgTree(newpar,addfkt); )
}


void	CNode::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode* node, char* text, int image))
{
	char temp[256];
	sprintf(temp,"%s (%x)",GetClassName(),GetID());

	addfkt(par,this,temp,TIC_CLASS | TICS_FINISHED);
}


#endif















