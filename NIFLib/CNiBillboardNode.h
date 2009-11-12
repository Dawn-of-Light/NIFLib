#pragma once
#include "CNiNode.h"

class CNiBillboardNode : public CNiNode
{
	public:
		CNiBillboardNode(void);
		~CNiBillboardNode(void);

		bool	Load(CNifStream&);

		void	Render();

		// ---------- Animation ----------
		bool		IsStatic()	{ return false;};



		DECLARE_CHUNK(NiBillboardNode,"NiBillboardNode");
};
