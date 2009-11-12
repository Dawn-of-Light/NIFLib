#pragma once
#include "cnode.h"

class CNiPointLight : public CNode
{
	public:
		CNiPointLight(void);
		~CNiPointLight(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiPointLight,"NiPointLight");

	private:

		char*		mpc_light_name;
};
