#pragma once
#include "cnode.h"

class CNiStencilProperty : public CNode
{
	public:
		CNiStencilProperty(void);
		~CNiStencilProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiStencilProperty,"NiStencilProperty");
};
