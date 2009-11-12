#pragma once
#include "cnode.h"

class CNi3dsMorphShape : public CNode
{
	public:
		CNi3dsMorphShape(void);
		~CNi3dsMorphShape(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(Ni3dsMorphShape,"Ni3dsMorphShape");
};
