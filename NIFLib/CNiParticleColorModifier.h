#pragma once
#include "cnode.h"


/**
*	CNiParticleColorModifier
*	Connection to all color animation controllers.
*	
**/
class CNiParticleColorModifier : public CNode
{
	public:
		CNiParticleColorModifier(void);
		~CNiParticleColorModifier(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);
		
		DECLARE_CHUNK(NiParticleColorModifier,"NiParticleColorModifier");

	private:
		CChilds		mc_fader;
		CChilds		mc_colordata;

};
