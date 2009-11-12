#pragma once
#include "cnode.h"

class CNi3dsParticleSystem : public CNode
{
	public:
		CNi3dsParticleSystem(void);
		~CNi3dsParticleSystem(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(Ni3dsParticleSystem,"Ni3dsParticleSystem");
};
