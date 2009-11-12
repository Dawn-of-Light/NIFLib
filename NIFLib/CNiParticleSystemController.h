#pragma once
#include "cnode.h"

class CNiParticleSystemController : public CNode
{
	public:
		CNiParticleSystemController(void);
		~CNiParticleSystemController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		DECLARE_CHUNK(NiParticleSystemController,"NiParticleSystemController");


	private:

		CChilds		mc_controller;
		CChilds		mc_collider;
		CChilds		mc_color;

};
