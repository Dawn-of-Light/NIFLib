#include "StdAfx.h"
#include ".\cniskinpartition.h"

CNiSkinPartition::CNiSkinPartition(void)
{
}

CNiSkinPartition::~CNiSkinPartition(void)
{
}


bool	CNiSkinPartition::Load(CNifStream& stream)
{
	unsigned base = stream.readUInt();

	mas_unknown.SetSize(base);

	for (unsigned n=0;n<base;n++)
	{
		mas_unknown[n].nof1 = stream.readWord();	
		mas_unknown[n].nof2 = stream.readWord();	
		mas_unknown[n].nof3 = stream.readWord();	
		mas_unknown[n].nof4 = stream.readWord();
		mas_unknown[n].nof5 = stream.readWord();

		stream.readSkip(2*	mas_unknown[n].nof5); 

		stream.readSkip(2*	mas_unknown[n].nof1); // word index?
		stream.readSkip(16*	mas_unknown[n].nof1);


		if (mas_unknown[n].nof4 !=0)
		{
			if (stream.Version() > 0x04020100)
			{
				mas_unknown[n].nof4= stream.readWord();	
				stream.readSkip(2*	mas_unknown[n].nof4);
			}
			else
			{
				stream.readSkip(2*	mas_unknown[n].nof4);
				stream.readSkip(2*(mas_unknown[n].nof2+2*mas_unknown[n].nof4));
			}
		}
		else
			stream.readSkip(6*mas_unknown[n].nof2); // word index?


		unsigned a = stream.readUChar();
		assert(a==0);

	}

	return true;
}



#ifdef ANALYSE_NIF
void	CNiSkinPartition::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "SkinPartition",TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	for (int t=0;t<mas_unknown.Count();t++)
	{
		char temp[256];

		sprintf(temp,"n = %02i,%02i,%02i,%02i,%02i",
			mas_unknown[t].nof1,mas_unknown[t].nof2,mas_unknown[t].nof3,mas_unknown[t].nof4,mas_unknown[t].nof5);
		addfkt(newsub,NULL,temp,TIC_DATA);
	}

}
#endif