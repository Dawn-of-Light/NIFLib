#pragma once
#include "cnode.h"

class CNiBezierTriangle4 :	public CNode
{
	public:
		CNiBezierTriangle4(void);
		~CNiBezierTriangle4(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiBezierTriangle4,"NiBezierTriangle4");
};
