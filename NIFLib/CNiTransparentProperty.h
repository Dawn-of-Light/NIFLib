#pragma once
#include "cnode.h"


/**
*	old
*	only version 3.1
**/
class CNiTransparentProperty :	public CNode
{
	public:
		CNiTransparentProperty(void);
		~CNiTransparentProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiTransparentProperty,"NiTransparentProperty");
};
