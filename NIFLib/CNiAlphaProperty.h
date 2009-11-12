#pragma once
#include "cnode.h"

class CNiAlphaProperty : public CNode
{
	public:
		CNiAlphaProperty(void);
		~CNiAlphaProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiAlphaProperty,"NiAlphaProperty");
};
