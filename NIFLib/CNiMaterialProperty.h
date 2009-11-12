#pragma once
#include "cnode.h"

class CNiMaterialProperty : public CNode
{
	public:
		CNiMaterialProperty(void);
		~CNiMaterialProperty(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiMaterialProperty,"NiMaterialProperty");

	private:

		char*		mpc_material_name;
		CChilds		mc_controller;

		float		mf_amb_r, mf_amb_g, mf_amb_b;
		float		mf_dif_r, mf_dif_g, mf_dif_b;
		float		mf_spe_r, mf_spe_g, mf_spe_b;

		float		mf_selfilu;
		float		mf_opacity;

		float		mf_unknown1;
		float		mf_unknown2;
		float		mf_unknown3;
};
