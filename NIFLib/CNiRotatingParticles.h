#pragma once
#include "CNiNode.h"

class CNiRotatingParticles : public CNiNode
{
	public:
		CNiRotatingParticles(void);
		~CNiRotatingParticles(void);

		bool	Load(CNifStream&);


		DECLARE_CHUNK(NiRotatingParticles,"NiRotatingParticles");

	private:
		char*	mpc_texture_name;
};
