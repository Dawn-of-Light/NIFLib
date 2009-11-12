#pragma once
#include "cnode.h"




/**
*	\todo unknown
**/
class CNiPosData :	public CNode
{
	public:
		CNiPosData(void);
		~CNiPosData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiPosData,"NiPosData")

};
