#include "StdAfx.h"
#include ".\cnibeziermesh.h"

CNiBezierMesh::CNiBezierMesh(void)
{
}

CNiBezierMesh::~CNiBezierMesh(void)
{
}


bool	CNiBezierMesh::Load(CNifStream& stream)
{
	stream.readstring();
	stream.readUInt();
	stream.readUInt();
	int nof = stream.readWord();

	stream.readSkip(4*16); // 16 floats -> matrix?


	int a = stream.readUInt(); // childs?
	stream.readSkip(a*4);

	a = stream.readUInt(); // childs?
	stream.readSkip(a*4);

	a = stream.readUInt(); // childs?
	stream.readSkip(a*4);

	stream.readUInt();

	int nof2 = stream.readWord();
	int a2 = stream.readWord();

	stream.readSkip(nof2*4*3);

	stream.readUInt();

	stream.readSkip(nof2*8);


	stream.readUInt();
	int nof3 = stream.readWord();
	stream.readSkip(nof3*4*2); // words

	return true;
}



#ifdef ANALYSE_NIF
void	CNiBezierMesh::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNode::CreateDbgTree(par,addfkt);
}
#endif