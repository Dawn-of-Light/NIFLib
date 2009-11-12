#pragma once
#include "cnode.h"



/**
*	v3.1 only
**/
class CNiMorpherController :	public CNode
{
	public:
		CNiMorpherController(void);
		~CNiMorpherController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**ppc_nodes, int nof);


		DECLARE_CHUNK(NiMorpherController,"NiMorpherController");

	private:

		CChilds		mc_data;
};

