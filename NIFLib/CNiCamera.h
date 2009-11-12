#pragma once
#include "cnode.h"

class CNiCamera :	public CNode
{
	public:
		CNiCamera(void);
		~CNiCamera(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiCamera,"NiCamera")

	private:

		char* mp_cameraname;
};
