#pragma once
#include "cnode.h"

class CNiRollController : public CNode
{
	public:
		CNiRollController(void);
		~CNiRollController(void);

		bool	Load(CNifStream&);

		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiRollController,"NiRollController");

	private:

		CChilds			mc_transform_node;
		CChilds			mc_extradata;
};
