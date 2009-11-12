#pragma once
#include "cnode.h"


/**
*	CDebugNodeUnconnected
*	is only needed if unconnected chunks are found. 
*	this node will store all unconnected chunks as childs
**/
class CDebugNodeUnconnected :	public CNode
{
	public:
		CDebugNodeUnconnected(void);
		~CDebugNodeUnconnected(void);

		bool	Load(CNifStream&) { ASSERT(false);return false;};

		void	AddNode(CNode*);

		void	CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image));

	private:
		CChilds	m_unconnected;
};
