#pragma once
#include "cnode.h"

class CNiSphericalCollider : public CNode
{
	public:
		CNiSphericalCollider(void);
		~CNiSphericalCollider(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiSphericalCollider,"NiSphericalCollider");
};
