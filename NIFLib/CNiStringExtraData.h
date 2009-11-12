#pragma once
#include "cnode.h"

class CNiStringExtraData : public CNode
{
	public:
		CNiStringExtraData(void);
		~CNiStringExtraData(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**ppc_nodes, int nof);




		// --------- unique functions -----------
		bool	LoadV2_3embedded(CNifStream&);



		DECLARE_CHUNK(NiStringExtraData,"NiStringExtraData");

	private:

		char*		mpc_string;
		CChilds		mc_next_string;

};
