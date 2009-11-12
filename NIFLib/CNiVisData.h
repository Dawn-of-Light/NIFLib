#pragma once
#include "cnode.h"

class CNiVisData :	public CNode
{
	public:
		CNiVisData(void);
		~CNiVisData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiVisData,"NiVisData");

	private:
		struct struct_key
		{
			float time;
			bool  vis;
		};

		TArray	<struct_key> ma_keys;	
};
