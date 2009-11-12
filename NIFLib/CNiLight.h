#pragma once
#include "cnode.h"


class CNiLight :	public CNode
{
	public:
		CNiLight(void);
		~CNiLight(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiLight,"NiLight")


};
