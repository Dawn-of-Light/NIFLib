#include "StdAfx.h"
#include ".\cni3dspathcontroller.h"

CNi3dsPathController::CNi3dsPathController(void)
{
}

CNi3dsPathController::~CNi3dsPathController(void)
{
}

bool	CNi3dsPathController::Load(CNifStream& stream)
{
	stream.readSkip(0x14);
	return true;
}



#ifdef ANALYSE_NIF
void	CNi3dsPathController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "3dsPathController",TIC_CONTROLLER);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif