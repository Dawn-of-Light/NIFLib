#pragma once
#include "cnode.h"

class CNiWireframeProperty : public CNode
{
	public:
		CNiWireframeProperty(void);
		~CNiWireframeProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiWireframeProperty,"NiWireframeProperty");
};
