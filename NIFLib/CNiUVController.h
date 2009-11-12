#pragma once
#include "cnode.h"

class CNiUVController : public CNode
{
	public:
		CNiUVController(void);
		~CNiUVController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiUVController,"NiUVController");

	private:
		CChilds		mc_child;
		CChilds		mc_parent;
		CChilds		mc_data;
};
