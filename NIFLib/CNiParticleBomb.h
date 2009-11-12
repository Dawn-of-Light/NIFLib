#pragma once
#include "cnode.h"

class CNiParticleBomb : public CNode
{
	public:
		CNiParticleBomb(void);
		~CNiParticleBomb(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);



		DECLARE_CHUNK(NiParticleBomb,"NiParticleBomb");

	private:

		CChilds		mc_child;
};
