#pragma once
#include "cnode.h"

class CNiRotatingParticlesData : public CNode
{
	public:
		CNiRotatingParticlesData(void);
		~CNiRotatingParticlesData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiRotatingParticlesData,"NiRotatingParticlesData");
};
