#include "StdAfx.h"
#include ".\cnimorphdata.h"

CNiMorphData::CNiMorphData(void)
{
}

CNiMorphData::~CNiMorphData(void)
{
}


bool	CNiMorphData::Load(CNifStream& stream)
{
	mn_frames = stream.readUInt();
	mn_points = stream.readUInt(); 
	unsigned char a = stream.readUChar(); assert(a==0 || a==1); /// \test

	for (unsigned mt = 0; mt<mn_frames;mt++)
	{
		unsigned morph_keys = stream.readUInt();
		unsigned ctrltype = stream.readUInt();

		switch(ctrltype)
		{
			case 1:	stream.readSkip(2*morph_keys*4);	break;
			case 2: stream.readSkip(4*morph_keys*4);	break;
			case 3: stream.readSkip(5*morph_keys*4);	break;
			default:
				assert(false);
		}

		for (unsigned k=0;k<mn_points;k++)
		{
			stream.readFloat(); // x
			stream.readFloat(); // y
			stream.readFloat(); // z
		}
	}

	return true;
}



#ifdef ANALYSE_NIF
void	CNiMorphData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "MorphData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	char temp[256];
	sprintf(temp,"Frames: %i",mn_frames);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"Points: %i",mn_points);
	addfkt(newsub,NULL,temp,TIC_DATA);

}
#endif