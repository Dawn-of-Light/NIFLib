#pragma once
#include "cnode.h"

class CNi3dsAlphaAnimator : public CNode
{
	public:
		CNi3dsAlphaAnimator(void);
		~CNi3dsAlphaAnimator(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(Ni3dsAlphaAnimator,"Ni3dsAlphaAnimator");

	private:

		CChilds		mc_parent;
};
