#pragma once
#include "cnode.h"

class CNiParticleMeshesData : public CNode
{
	public:
		CNiParticleMeshesData(void);
		~CNiParticleMeshesData(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiParticleMeshesData,"NiParticleMeshesData");

	private:

		CChilds		mc_child;
};
