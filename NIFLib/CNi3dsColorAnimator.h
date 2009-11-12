#pragma once
#include "cnode.h"

class CNi3dsColorAnimator : public CNode
{
	public:
		CNi3dsColorAnimator(void);
		~CNi3dsColorAnimator(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(Ni3dsColorAnimator,"Ni3dsColorAnimator");
};
