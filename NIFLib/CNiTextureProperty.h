#pragma once
#include "cnode.h"

class CNiTextureProperty : public CNode
{
	public:
		CNiTextureProperty(void);
		~CNiTextureProperty(void);

		bool	Load(CNifStream&);
		void	Create();
		void	CreateHierachy(CNode**, int nof);

		void	Render();


		void	RemoveTexture();


		DECLARE_CHUNK(NiTextureProperty,"NiTextureProperty");

	private:

		CChilds		mc_difusemap;	// NiImage
};
