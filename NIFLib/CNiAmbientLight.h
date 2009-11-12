#pragma once
#include "cnode.h"

class CNiAmbientLight : public CNode
{
	public:
		CNiAmbientLight(void);
		~CNiAmbientLight(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiAmbientLight,"NiAmbientLight");

	private:
		char*		mpc_light_name;
};
