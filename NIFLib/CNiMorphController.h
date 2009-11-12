#pragma once
#include "cnode.h"

class CNiMorphController :	public CNode
{
	public:
		CNiMorphController(void);
		~CNiMorphController(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiMorphController,"NiMorphController");

};
