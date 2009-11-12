#pragma once
#include "cnode.h"

class CNiVertexColorProperty : public CNode
{
	public:
		CNiVertexColorProperty(void);
		~CNiVertexColorProperty(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiVertexColorProperty,"NiVertexColorProperty");


	private:
		unsigned mn_unknown1;
		unsigned mn_unknown2;
		unsigned mn_unknown3;
		unsigned mn_unknown4_word;
		unsigned mn_unknown5;
		unsigned mn_unknown6;
};
