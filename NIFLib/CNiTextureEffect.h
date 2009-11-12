#pragma once
#include "cnode.h"

class CNiTextureEffect : public CNode
{
	public:
		CNiTextureEffect(void);
		~CNiTextureEffect(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiTextureEffect,"NiTextureEffect");

	private:
		char*		mpc_name;
		CChilds		mc_data;
};
