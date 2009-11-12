#pragma once
#include "cnode.h"

class CNiSpotLight : public CNode
{
	public:
		CNiSpotLight(void);
		~CNiSpotLight(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiSpotLight,"NiSpotLight");


	private:
		char*		mpc_light_name;
};
