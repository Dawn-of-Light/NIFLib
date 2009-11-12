#pragma once
#include "cnode.h"


/**
*	daoc v1.60
*	used in v3.0 - v3.1 files
**/
class CNiTriShapeSkinController :	public CNode
{
	public:
		CNiTriShapeSkinController(void);
		~CNiTriShapeSkinController(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiTriShapeSkinController,"NiTriShapeSkinController");

};
