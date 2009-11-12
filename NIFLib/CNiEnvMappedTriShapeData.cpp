#include "StdAfx.h"
#include ".\cnienvmappedtrishapedata.h"

CNiEnvMappedTriShapeData::CNiEnvMappedTriShapeData(void)
{
}

CNiEnvMappedTriShapeData::~CNiEnvMappedTriShapeData(void)
{
}

bool	CNiEnvMappedTriShapeData::Load(CNifStream& stream)
{
	ASSERT(stream.Version() == 0x03010000);

	unsigned a;



	// point count
	int g_nof_points = stream.readWord();

	a = stream.readUInt(); assert(a!=0); // bool => data follows

	// point data
	stream.readSkip(12*g_nof_points);	// could check if true FLOATs



    // Point-Normals
	a = stream.readInt();// if false, no normals
	if (a)
	{
		stream.readSkip(12*g_nof_points);	// could check if true FLOATs
	}


	// unkown floats
	int a1 = stream.readInt();
	int a2 = stream.readInt();
	a1 = stream.readInt();
	a2 = stream.readInt();



	// extra data
	a = stream.readInt();
	if (a)
	{
		stream.readSkip(4 * g_nof_points * 4);
	}



	// texture coord
	int nof_textures = stream.readWord();
	ASSERT(nof_textures <= 8);//"too much textures"

	a = stream.readInt();
	if (nof_textures >0)
	{
		ASSERT(a);
		stream.readSkip(nof_textures*g_nof_points*2*4);	// could check if true FLOATs
	}

		

	// nof polys
	unsigned short polys = stream.readWord();
	// nof edges
	unsigned short edges = stream.readWord();
	// nof index
	unsigned short nof_idx = stream.readWord();

	int g_nof_polyindex = polys*3;;
	ASSERT(edges == polys*3);//,			"unused value"); // thats not in TriStrips
	ASSERT(nof_idx == 0);//, "check values"); // thats only in TriStrips

//	TRACE("NOF-Poly = %i\n",g_nof_polyindex);
	for (int i=0;i<g_nof_polyindex;i++)
	{
		unsigned short pi = stream.readWord();
		ASSERT(pi<g_nof_points);
	}

	
	unsigned short points = stream.readWord();
	if (points != 0)
	{
		for (int t=0;t<points;t++)
		{
			unsigned short s = stream.readWord();	// number of point probs
			stream.readSkip(s*2);
		}
	}



	return true;
}


#ifdef ANALYSE_NIF
void	CNiEnvMappedTriShapeData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "EnvMappedTriShapeData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif