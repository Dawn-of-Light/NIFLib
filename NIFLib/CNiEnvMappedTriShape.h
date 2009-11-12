#pragma once
#include "cnode.h"



/**
*	old 
*	Version 3.1 only
**/
class CNiEnvMappedTriShape :	public CNode
{
	public:
		CNiEnvMappedTriShape(void);
		~CNiEnvMappedTriShape(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**ppc_nodes, int nof);

		DECLARE_CHUNK(NiEnvMappedTriShape,"NiEnvMappedTriShape");

	private:
		char*		mpc_name;
		CChilds		mc_parent;
		CMatrix		mc_matrix;
		CChilds		mc_childs;
		CChilds		mc_childs2;
		CChilds		mc_childs3;
};
