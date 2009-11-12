#pragma once
#include "cnode.h"

class CNiMorphData : public CNode
{
	public:
		CNiMorphData(void);
		~CNiMorphData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiMorphData,"NiMorphData");

	private:
		unsigned mn_frames;
		unsigned mn_points;

};
