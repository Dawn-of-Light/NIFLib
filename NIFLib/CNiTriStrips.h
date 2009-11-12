#pragma once
#include "CNiTriShape.h"


/**
*	Exactly the same as CNiTriShape
**/
class CNiTriStrips : public CNiTriShape
{
	public:
		CNiTriStrips(void);
		~CNiTriStrips(void);


		DECLARE_CHUNK(NiTriStrips,"NiTriStrips");
};
