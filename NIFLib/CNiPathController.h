#pragma once
#include "cnode.h"

class CNiPathController : public CNode
{
	public:
		CNiPathController(void);
		~CNiPathController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

	
		DECLARE_CHUNK(NiPathController,"NiPathController");

	private:

		CChilds		mc_path;
		CChilds		mc_floatdata;
};
