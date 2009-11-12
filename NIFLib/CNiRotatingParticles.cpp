#include "StdAfx.h"
#include ".\cnirotatingparticles.h"

CNiRotatingParticles::CNiRotatingParticles(void)
{
	mpc_texture_name = NULL;
}

CNiRotatingParticles::~CNiRotatingParticles(void)
{
	SAFE_DELETE(mpc_texture_name);
}


bool	CNiRotatingParticles::Load(CNifStream& stream)
{
	mpc_texture_name = stream.readstring();

	unsigned i = stream.readUInt();	// data?

	if (stream.Version() == 0x02030000)
	{
		// V2.3
		i = stream.readUInt(); //ASSERT(i==1);
		i = stream.readUInt(); //ASSERT(i==0x01000001);
		i = stream.readUInt(); assert(i==2);
		float f = stream.readFloat(); assert(f==0.0f);
		f = stream.readFloat(); assert(f==1.0f);
		f = stream.readFloat(); assert(f==0.0f);
		f = stream.readFloat(); //ASSERT(f==1.6666f);
		f = stream.readFloat(); //ASSERT(f==1.13333f);

		i = stream.readUInt(); // child?

		unsigned x1 = stream.readUInt(); 
		unsigned x2 = stream.readUInt(); 

		stream.readSkip(x1*x2*8);
	}
	else
	{
		// vis controller
		mc_controller.read(stream,1);

		unsigned short f = stream.readWord();

		// Matrix 
		stream.readMatrix(mc_matrix);

		// Properties (Texturemodeporperty, VertexColorProperty)
		mc_properties.read(stream);	
		
		i = stream.readBool(); // flag??
		assert(i==0);// was always 0, maybe here is another list

		// childs
		mc_childs.read(stream,1);	// 

		// unknown
		mc_unknown.read(stream,1);
		//	ASSERTE(unknown.GetCount()==0, "was always 0, maybe here is another list");
	}

	return true;

}



#ifdef ANALYSE_NIF
void	CNiRotatingParticles::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*,char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"RotatingParticles",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	char temp[1024]; sprintf(temp,"Texture = %s",mpc_texture_name);
	addfkt(newsub,NULL, temp,TIC_TEXTURE);
	CNode::CreateDbgTree(newsub,addfkt);
	CreateDbgTreeChilds(mc_extraData,"ExtraData",TIC_DATA,newsub,addfkt);
	// mc_matrix;

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_properties,"Properties",TIC_ABSTRACT,newpar,addfkt);
	CreateDbgTreeChilds(mc_childs,"Childs",TIC_CHILDS,newpar,addfkt);
	CreateDbgTreeChilds(mc_unknown,"Unknwon",TIC_ABSTRACT,newpar,addfkt);
}
#endif


