#pragma once
#include "cnode.h"

class CNiMaterialColorController : public CNode
{
	public:
		CNiMaterialColorController(void);
		~CNiMaterialColorController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiMaterialColorController,"NiMaterialColorController");

	private:

		CChilds	mc_controller1;
		CChilds	mc_controller2;
};
