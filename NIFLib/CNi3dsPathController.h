#pragma once
#include "cnode.h"

class CNi3dsPathController : public CNode
{
	public:
		CNi3dsPathController(void);
		~CNi3dsPathController(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(Ni3dsPathController,"Ni3dsPathController");
};
