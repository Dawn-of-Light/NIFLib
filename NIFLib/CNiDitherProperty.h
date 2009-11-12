#pragma once
#include "cnode.h"

class CNiDitherProperty : public CNode
{
	public:
		CNiDitherProperty(void);
		~CNiDitherProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiDitherProperty,"NiDitherProperty");
};
