#pragma once
#include "cnode.h"

class CNiTextKeyExtraData :	public CNode
{
	public:
		CNiTextKeyExtraData(void);
		~CNiTextKeyExtraData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiTextKeyExtraData,"NiTextKeyExtraData");
};
