#pragma once
#include "cnode.h"

class CNiDirectionalLight : public CNode
{
	public:
		CNiDirectionalLight(void);
		~CNiDirectionalLight(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiDirectionalLight,"NiDirectionalLight");

	private:

		char*	mpc_light_name;
};
