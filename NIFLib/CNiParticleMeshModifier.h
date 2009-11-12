#pragma once
#include "cnode.h"

class CNiParticleMeshModifier : public CNode
{
	public:
		CNiParticleMeshModifier(void);
		~CNiParticleMeshModifier(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiParticleMeshModifier,"NiParticleMeshModifier");

	private:
		CChilds		mc_fader;
		CChilds		mc_data;

};
