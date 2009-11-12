#pragma once
#include "CNiNode.h"

class CNiParticleMeshes : public CNiNode
{
	public:
		CNiParticleMeshes(void);
		~CNiParticleMeshes(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiParticleMeshes,"NiParticleMeshes");
};
