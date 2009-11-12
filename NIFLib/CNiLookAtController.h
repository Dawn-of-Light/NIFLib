#pragma once
#include "cnode.h"

class CNiLookAtController : public CNode
{
	public:
		CNiLookAtController(void);
		~CNiLookAtController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiLookAtController,"NiLookAtController");

	private:
		CChilds		mc_controller;
};
