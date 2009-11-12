#pragma once
#include "cnode.h"

class CNiColorData : public CNode
{
	public:
		CNiColorData(void);
		~CNiColorData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiColorData,"NiColorData");
};
