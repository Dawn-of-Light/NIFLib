#pragma once
#include "cnode.h"

class CNiTextureModeProperty : public CNode
{
	public:
		CNiTextureModeProperty(void);
		~CNiTextureModeProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiTextureModeProperty,"NiTextureModeProperty");
};
