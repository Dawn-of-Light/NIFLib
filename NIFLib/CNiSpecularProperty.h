#pragma once
#include "cnode.h"

class CNiSpecularProperty : public CNode
{
	public:
		CNiSpecularProperty(void);
		~CNiSpecularProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiSpecularProperty,"NiSpecularProperty");
};
