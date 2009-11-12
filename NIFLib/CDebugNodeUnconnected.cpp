#include "StdAfx.h"
#include ".\cdebugnodeunconnected.h"

CDebugNodeUnconnected::CDebugNodeUnconnected(void)
{
}

CDebugNodeUnconnected::~CDebugNodeUnconnected(void)
{
}


void	CDebugNodeUnconnected::AddNode(CNode* pc_node)
{
	m_unconnected.AddNode(pc_node);
}


#ifdef ANALYSE_NIF
void	CDebugNodeUnconnected::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CreateDbgTreeChilds(m_unconnected,"UNCONNECTED",TIC_NOTCONNECTED,par,addfkt);
}
#endif 


