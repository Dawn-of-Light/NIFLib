#include "StdAfx.h"
#include ".\cnivertexcolorproperty.h"

CNiVertexColorProperty::CNiVertexColorProperty(void)
{
}

CNiVertexColorProperty::~CNiVertexColorProperty(void)
{
}


bool	CNiVertexColorProperty::Load(CNifStream& stream)
{
	if (stream.Version() == 0x02030000)
	{
		mn_unknown3 = 0;
		mn_unknown5 = 0;

		mn_unknown1 = stream.readUInt();		//assert(a==0);
		mn_unknown2 = stream.readUInt();		//assert(a==0);
		mn_unknown4_word = stream.readUChar();	//assert(a==0); 
		mn_unknown5 = stream.readUInt();		//assert(a==4);
	}
	else
	{
		mn_unknown1 = stream.readUInt();		// assert(a==0);
		mn_unknown2= stream.readUInt();			//assert(a==0 || a==0xffffffff); // ID?
		mn_unknown3= stream.readUInt();			//assert(a==0 || a==0xffffffff); // ID?
		mn_unknown4_word = stream.readWord();	//assert(a==0);
		mn_unknown5 = stream.readUInt();		//assert(a>=0 && a<=2);
		mn_unknown6 = stream.readUInt();		//assert(a==0 || a==1);
	}

	return true;
}



#ifdef ANALYSE_NIF
void	CNiVertexColorProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "VertexColorProperty",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif