#pragma once
#include "cnode.h"


class CNiTexturingProperty : public CNode
{
	public:
		CNiTexturingProperty(void);
		~CNiTexturingProperty(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);
		void	Create();

		void	Render();

		void	RemoveTexture();

		// --------- manuall create
		void	AddTexture(CNode*);


		DECLARE_CHUNK(NiTexturingProperty,"NiTexturingProperty");

	private:
		CChilds		mc_textures;
};
