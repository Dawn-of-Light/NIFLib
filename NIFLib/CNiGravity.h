#pragma once
#include "cnode.h"

class CNiGravity :	public CNode
{
	public:
		CNiGravity(void);
		~CNiGravity(void);

		
		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);



		DECLARE_CHUNK(NiGravity,"NiGravity")

	private:
		CChilds		mc_child;
};
