#pragma once
#include "cnode.h"

class CNiParticleGrowFade : public CNode
{
	public:
		CNiParticleGrowFade(void);
		~CNiParticleGrowFade(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiParticleGrowFade,"NiParticleGrowFade");
};
