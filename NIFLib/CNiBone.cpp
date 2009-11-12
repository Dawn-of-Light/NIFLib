#include "StdAfx.h"
#include ".\cnibone.h"

CNiBone::CNiBone(void)
{
	mpc_name = NULL;
}

CNiBone::~CNiBone(void)
{
	SAFE_DELETE(mpc_name);
}



bool	CNiBone::Load(CNifStream& stream)
{
	mpc_name = stream.readstring();

	unsigned a;
	a = stream.readUInt();	assert(a==0);// ?
	mc_controller.read(stream,1);

	unsigned nof = stream.readWord(); assert(nof==0x0c ||nof==4);
	stream.readSkip(18*4);	// floats

	mc_childs.read(stream);

	a = stream.readUInt();	assert(a==0);// ?

	return true;
}



void	CNiBone::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller.Resolve(ppc_nodes, nof);		
	mc_childs.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiBone::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256]; sprintf(temp,"Bone: %s",mpc_name);
	newpar = addfkt(par,this, temp,TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER, newpar,addfkt);
	CreateDbgTreeChilds(mc_childs,"Childs",TIC_CHILDS ,newpar,addfkt);

}
#endif