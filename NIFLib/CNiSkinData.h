#pragma once
#include "cnode.h"

class CNiSkinData :	public CNode
{
	public:
		CNiSkinData(void);
		~CNiSkinData(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiSkinData,"NiSkinData");

	private:
		
		CChilds			mc_partion;
};
