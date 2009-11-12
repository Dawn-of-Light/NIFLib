#pragma once
#include "cnode.h"

class CNiSkinInstance : public CNode
{
	public:
		CNiSkinInstance(void);
		~CNiSkinInstance(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiSkinInstance,"NiSkinInstance");

	private:
		CChilds			mc_skindata;
		CChilds			mc_bones;
};
