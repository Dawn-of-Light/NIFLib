#pragma once
#include "cnode.h"

class CNiRawImageData : public CNode
{
	public:
		CNiRawImageData(void);
		~CNiRawImageData(void);

		bool	Load(CNifStream&);

		void	Create();
		GLuint	GetTextureID();

		DECLARE_CHUNK(NiRawImageData,"NiRawImageData");

	private:
		GLuint	mn_texture_id;

		unsigned		mn_width;
		unsigned		mn_height;
		unsigned		mn_bpp;
		unsigned char*	mp_pixel_data;
};
