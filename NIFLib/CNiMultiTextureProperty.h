#pragma once
#include "cnode.h"

class CNiMultiTextureProperty : public CNode
{
	public:
		CNiMultiTextureProperty(void);
		~CNiMultiTextureProperty(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);

		void	Render();




		DECLARE_CHUNK(NiMultiTextureProperty,"NiMultiTextureProperty");

	private:	
		CChilds		mc_texture;

};
