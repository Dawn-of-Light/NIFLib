#include "StdAfx.h"
#include ".\cnimaterialproperty.h"

CNiMaterialProperty::CNiMaterialProperty(void)
{
	mpc_material_name = NULL;
}

CNiMaterialProperty::~CNiMaterialProperty(void)
{
	SAFE_DELETE(mpc_material_name);
}


bool	CNiMaterialProperty::Load(CNifStream& stream)
{
	
	mpc_material_name = stream.readstring();
	
	if (stream.Version() >= 0x03000000)
	{
		int u1 = stream.readUInt();	assert(u1==-1 || u1 == 0);
		mc_controller.read(stream,1);
		unsigned short d = stream.readWord();	assert(d==1 || d==0);

		// ambiente
		mf_amb_r = stream.readFloat();		
		mf_amb_g = stream.readFloat();
		mf_amb_b = stream.readFloat();

		// difuse
		mf_dif_r = stream.readFloat();		
		mf_dif_g = stream.readFloat();
		mf_dif_b = stream.readFloat();

		// specular
		mf_spe_r = stream.readFloat();		
		mf_spe_g = stream.readFloat();
		mf_spe_b = stream.readFloat();

		mf_unknown1 = stream.readFloat(); // 0

		// Selfillu
		mf_selfilu = stream.readFloat();

		mf_unknown2 = stream.readFloat();	// 0
		mf_unknown3 = stream.readFloat();

		// opactity
		mf_opacity = stream.readFloat();

	}
	else
	{
		unsigned i = stream.readUInt();
		while (i>0)
		{
			char*p=stream.readstring();
			unsigned a = stream.readUInt(); assert(a==4);
			mc_controller.read(stream,1);
			delete(p);
			i--;
		}
		stream.readSkip(57);
	}

	return true;
}



void	CNiMaterialProperty::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller.Resolve(ppc_nodes, nof);		
}



#ifdef ANALYSE_NIF
void	CNiMaterialProperty::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256]; sprintf(temp,"Material: %s",mpc_material_name);
	newpar = addfkt(par,this, temp,TIC_TEXTURE);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	sprintf(temp,"Ambient: %.2f,%.2f,%.2f",mf_amb_r,mf_amb_g,mf_amb_b);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"Difuse: %.2f,%.2f,%.2f",mf_dif_r,mf_dif_g,mf_dif_b);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"Specular: %.2f,%.2f,%.2f",mf_spe_r,mf_spe_g,mf_spe_b);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"Opacity: %.2f",mf_opacity);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"Selfilu: %.2f",mf_selfilu);
	addfkt(newsub,NULL,temp,TIC_DATA);

	sprintf(temp,"unknown1: %.2f",mf_unknown1);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"unknown2: %.2f",mf_unknown2);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"unknown3: %.2f",mf_unknown3);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);


	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER,newpar,addfkt);
}
#endif



