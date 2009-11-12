#pragma once
#include "cnode.h"


/**
*	daoc v1.60
*	used in v3.1 files
**/
class CNiBone : public CNode
{
	public:
		CNiBone(void);
		~CNiBone(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiBone,"NiBone");

	private:

		CChilds		mc_controller;
		CChilds		mc_childs;

		char*		mpc_name;
};
