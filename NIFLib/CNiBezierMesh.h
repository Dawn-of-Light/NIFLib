#pragma once
#include "cnode.h"

class CNiBezierMesh :	public CNode
{
	public:
		CNiBezierMesh(void);
		~CNiBezierMesh(void);

		bool	Load(CNifStream&);


		DECLARE_CHUNK(NiBezierMesh,"NiBezierMesh");
};
