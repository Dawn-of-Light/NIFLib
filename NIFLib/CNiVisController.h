#pragma once
#include "cnode.h"

class CNiVisController : public CNode
{
	public:
		CNiVisController(void);
		~CNiVisController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiVisController,"NiVisController");

	private:
		CChilds		mc_keyframe;
		CChilds		mc_parent;
		CChilds		mc_data;
};
