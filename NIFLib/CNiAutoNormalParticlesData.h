#pragma once
#include "cnode.h"

class CNiAutoNormalParticlesData : public CNode
{
	public:
		CNiAutoNormalParticlesData(void);
		~CNiAutoNormalParticlesData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiAutoNormalParticlesData,"NiAutoNormalParticlesData");
};
