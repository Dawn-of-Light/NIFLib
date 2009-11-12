#pragma once
#include "cnode.h"


/**
*	CNiUVData
*	Animation track-data for NiUVController
*
**/
class CNiUVData :	public CNode
{
	public:
		CNiUVData(void);
		~CNiUVData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiUVData,"NiUVData")
};
