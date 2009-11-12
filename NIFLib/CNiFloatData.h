#pragma once
#include "cnode.h"


/**
*	some extra float data
*	in v3.0		2D
*	in v3.1+	4D
**/
class CNiFloatData : public CNode
{
	public:
		CNiFloatData(void);
		~CNiFloatData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiFloatData,"NiFloatData");
};
