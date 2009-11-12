#pragma once
#include "cnode.h"

class CNiParticleRotation : public CNode
{
	public:
		CNiParticleRotation(void);
		~CNiParticleRotation(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);
		

		DECLARE_CHUNK(NiParticleRotation,"NiParticleRotation");

	private:

		CChilds		mc_data;
};
