#include "StdAfx.h"
#include ".\cnitristrips.h"

CNiTriStrips::CNiTriStrips(void)
{
}

CNiTriStrips::~CNiTriStrips(void)
{
}



#ifdef ANALYSE_NIF
void	CNiTriStrips::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNiTriShape::CreateDbgTree(par,addfkt);
}
#endif
