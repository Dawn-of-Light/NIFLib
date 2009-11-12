#pragma once
#include "cnode.h"

class CNiPixelData : public CNode
{
	public:
		CNiPixelData(void);
		~CNiPixelData(void);

		bool	Load(CNifStream&);

		void	Create();

		void	CreateHierachy(CNode**ppc_nodes, int nof);

		GLuint	GetTextureID();



		DECLARE_CHUNK(NiPixelData,"NiPixelData");

	private:

		CChilds		mc_palette; // NiPalette

		unsigned	mn_size_x;
		unsigned	mn_size_y;
		unsigned	mn_bpp;

		unsigned char *mpc_pixel_data;

		GLuint		mn_texture_id;
};
