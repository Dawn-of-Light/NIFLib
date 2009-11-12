#pragma once
#include "CNiNode.h"

class CNiAutoNormalParticles : public CNiNode
{
	public:
		CNiAutoNormalParticles(void);
		~CNiAutoNormalParticles(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiAutoNormalParticles,"NiAutoNormalParticles");

};
