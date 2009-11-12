#pragma once
#include "cnode.h"

class CNiAlphaController : public CNode
{
	public:
		CNiAlphaController(void);
		~CNiAlphaController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiAlphaController,"NiAlphaController");

	private:

		CChilds		mc_controller;
		CChilds		mc_data;
	
};
