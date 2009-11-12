#pragma once
#include "cnode.h"



class CNi3dsAnimationNode : public CNode
{
	public:
		CNi3dsAnimationNode(void);
		~CNi3dsAnimationNode(void);

		bool	Load(CNifStream&);
		void	Create();
		void	CreateHierachy(CNode**ppc_nodes, int nof);

		DECLARE_CHUNK(Ni3dsAnimationNode,"Ni3dsAnimationNode");

	private:
		CChilds		mc_childs;

};
