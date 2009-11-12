#pragma once
#include "cnode.h"

class CNiImage :	public CNode
{
	public:
		CNiImage(void);
		~CNiImage(void);

		bool	Load(CNifStream&);
		void	Create();
		void	CreateHierachy(CNode**, int nof);


		void	Render();


		DECLARE_CHUNK(NiImage,"NiImage")

	private:

		GLuint	mn_texture_id;

		unsigned char	mb_external;
		char* 			mp_texturename;
		CChilds			mc_texture;
		DWORD			mdw_unknown1;
		DWORD			mdw_unknown2;
};
