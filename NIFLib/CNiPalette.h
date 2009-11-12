#pragma once
#include "cnode.h"

class CNiPalette :	public CNode
{
	public:
		CNiPalette(void);
		~CNiPalette(void);

		bool	Load(CNifStream&);

		unsigned char* GetColor(unsigned char index);


		DECLARE_CHUNK(NiPalette,"NiPalette");

	private:
		unsigned char* mpc_palette;
};
