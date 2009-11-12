#pragma once
#include "cnode.h"

class CNiGeomMorpherController : public CNode
{
	public:
		CNiGeomMorpherController(void);
		~CNiGeomMorpherController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiGeomMorpherController,"NiGeomMorpherController");

	private:

		CChilds		mc_data1;
		CChilds		mc_data2;
};
