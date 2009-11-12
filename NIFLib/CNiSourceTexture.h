#pragma once
#include "cnode.h"

class CNiSourceTexture : public CNode
{
	public:
		CNiSourceTexture(void);
		~CNiSourceTexture(void);

		bool	Load(CNifStream&);

		void	Create();
		void	CreateHierachy(CNode**, int nof);


		void	Render();


		//    ----------- class unique -------------
		// manual construction
		void	SetTexture(GLuint);
		void	SetTexture(const char*);


		DECLARE_CHUNK(NiSourceTexture,"NiSourceTexture");


	private:

		char*		mpc_texture_name;
		CChilds		mc_internaltexture;

		// -- unknown --
		unsigned mn_unknown1;
		unsigned mn_unknown2;
		unsigned mn_unknown3;

		// internal
		GLuint		mn_texture_id;
};
