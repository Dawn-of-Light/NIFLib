#pragma once
#include "cnode.h"

class CNiZBufferProperty : public CNode
{
	public:
		CNiZBufferProperty(void);
		~CNiZBufferProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiZBufferProperty,"NiZBufferProperty");
};
