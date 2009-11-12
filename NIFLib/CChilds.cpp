#include "StdAfx.h"
#include ".\cchilds.h"

CChilds::CChilds(void)
{
	mb_referenzed = true;
}


CChilds::~CChilds(void)
{
	Clear();
}


void	CChilds::Clear()
{
	if (mb_referenzed)
	{
		for (int t=0;t<map_childs.Count();t++)
			if (map_childs[t])
			{
				map_childs[t]->mn_referenzes--;
				if (map_childs[t]->mn_referenzes == 0)
					delete (map_childs[t]);
			}
	}
	map_childs.RemoveAll();
}


void	CChilds::read(CNifStream& stream, int len /*= -1*/)
{
	if (len == -1)	len = stream.readUInt();
	if (len == 0)	return;

	// too much childs
	assert(len<0x3ff);


	// in old files: 0 = empty  ('cause the id's were pointers)
	// in new files: -1 = empty ('cause the id's are counters)
	DWORD empty_child = 0;
	if (stream.Version()>=0x04000000) empty_child = 0xffffffff;

	unsigned id;
	for (int i=0;i<len;i++)
	{
		id = stream.readUInt();
		if (id != empty_child)
		{
			ma_child_ids.AddLast(id);
			map_childs.AddLast(NULL);
		}
	}

}


int		CChilds::GetCount()
{
	return map_childs.Count();
}


void	CChilds::AddNode(CNode* pc_node)
{
	ma_child_ids.AddLast(-1);
	map_childs.AddLast(pc_node);
	pc_node->mn_referenzes++;
}


void	CChilds::Resolve(CNode**ppc_nodes, int nof)
{
	for (int t=0;t<ma_child_ids.Count();t++)
	{
		// directly added
		if (ma_child_ids[t] == -1) continue;

		int nodenr;
		for (nodenr=0;nodenr<nof && ppc_nodes[nodenr]->GetID() != ma_child_ids[t]; nodenr++);
		assert(nodenr<nof); // Note ID !!NOT FOUND !!
		if (nodenr>=nof) continue;

		map_childs[t] = ppc_nodes[nodenr];
		map_childs[t]->mn_referenzes++;
	}

	// clear memory
	ma_child_ids.SetSize(0);
	ma_child_ids.CleanUp();
}



void	CChilds::ResolveWithoutReferenz(CNode**ppc_nodes, int nof)
{
	mb_referenzed = false;

	for (int t=0;t<ma_child_ids.Count();t++)
	{
		// directly added
		if (ma_child_ids[t] == -1) continue;

		int nodenr;
		for (nodenr=0;nodenr<nof && ppc_nodes[nodenr]->GetID() != ma_child_ids[t]; nodenr++);
		assert(nodenr<nof); // Note ID !!NOT FOUND !!
		if (nodenr>=nof) continue;

		map_childs[t] = ppc_nodes[nodenr];
	}

	// clear memory
	ma_child_ids.SetSize(0);
	ma_child_ids.CleanUp();
}

