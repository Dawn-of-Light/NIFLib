#include "StdAfx.h"
#include "cnitrishape.h"
#include "CNiTriShapeData.h"


CNiTriShape::CNiTriShape(void)
{
	mpc_object_name = NULL;
}

CNiTriShape::~CNiTriShape(void)
{
	SAFE_DELETE(mpc_object_name);
}



bool	CNiTriShape::Load(CNifStream& stream)
{
	mpc_object_name = stream.readstring();

	if (stream.Version() >= 0x03000000)
		mc_extradata.read(stream,1);

	// morph controller
	mc_morphcontroller.read(stream,1);


	if (stream.Version() >= 0x03000000)
		mn_unknown_flag = stream.readWord(); 
	else
		mn_unknown_flag = stream.readUChar();
	

	stream.readMatrix(mc_matrix);

	// childs
	mc_propertys.read(stream);



	// in version 2.3 here the mesh data follows
	if (stream.Version() < 0x03000000)
	{
		CNiTriShapeData * pc_node = new CNiTriShapeData();
		mc_mesh.AddNode(pc_node);
		bool res = pc_node->LoadV2_3(stream);
		return res;
	}


  
	// flag??
	bool b = stream.readBool();
	assert(!b);// NiTriShape list2 not empty?
	    
	// Mesh data node
	mc_mesh.read(stream,1);


	if (stream.Version() >= 0x04000000)
	{
		mc_skinning.read(stream,1);
	}

	return true;
}



void	CNiTriShape::CreateHierachy(CNode** ppc, int nof)
{
	mc_extradata.Resolve(ppc, nof);
	mc_morphcontroller.Resolve(ppc, nof);
	mc_propertys.Resolve(ppc, nof);
	mc_mesh.Resolve(ppc, nof);
	mc_skinning.Resolve(ppc, nof);
}


void	CNiTriShape::Create()
{
	FOREACH(pc,mc_extradata,		pc->Create(); )
	FOREACH(pc,mc_morphcontroller,  pc->Create(); )
	FOREACH(pc,mc_propertys,		pc->Create(); )
	FOREACH(pc,mc_mesh,				pc->Create(); )
	FOREACH(pc,mc_skinning,			pc->Create(); )
}


void	CNiTriShape::RemoveTexture()
{
	FOREACH(pc,mc_propertys,	pc->RemoveTexture(); )
}


void	CNiTriShape::Render()
{
	FOREACH(pc,mc_propertys,	pc->Render(); )


	glPushMatrix();
	glMultMatrixf(mc_matrix.Get());

	FOREACH(pc,mc_mesh,			pc->Render(); )

	glPopMatrix();

	FOREACH(pc,mc_propertys,	pc->AfterRender(); )
}




void	CNiTriShape::Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result)
{
	CMatrix mat = world;
	mat *= mc_matrix;

	FOREACH(pc,mc_mesh,		pc->Collision(mat,point,dir,result););
}



bool	CNiTriShape::GetBoundingbox(CVector& min, CVector& max)
{

	// childs
	CVector t_min, t_max;

	bool b_empty = true;

	FOREACH(pc,mc_mesh,  

		bool res = pc->GetBoundingbox(t_min,t_max);
		if (!res) continue;
		t_min = mc_matrix.Transform(t_min);
		t_max = mc_matrix.Transform(t_max);

		if (b_empty)
		{
			min = t_min;
			max = t_max;
			b_empty = false;
		}
		else
		{
			if (min.GetX()>t_min.GetX())	min.SetX(t_min.GetX());
			if (min.GetY()>t_min.GetY())	min.SetY(t_min.GetY());
			if (min.GetZ()>t_min.GetZ())	min.SetZ(t_min.GetZ());
			if (max.GetX()<t_max.GetX())	max.SetX(t_max.GetX());
			if (max.GetY()<t_max.GetY())	max.SetY(t_max.GetY());
			if (max.GetZ()<t_max.GetZ())	max.SetZ(t_max.GetZ());
		}
	)


	/// \todo animation 
//	FOREACH(pc,mc_morphcontroller,  pc->Render(); )


	return !b_empty;
}





void	CNiTriShape::SetRenderFlags(DWORD flags, bool recursive)
{
	CNode::SetRenderFlags(flags,recursive);

	// mesh
	FOREACH(pc,mc_mesh,	pc->SetRenderFlags(flags,recursive); )

	// anim
	FOREACH(pc,mc_morphcontroller,	pc->SetRenderFlags(flags,recursive); )

	// childs 
	FOREACH(pc,mc_propertys,	pc->SetRenderFlags(flags,recursive); )

	// skin
	FOREACH(pc,mc_skinning,		pc->SetRenderFlags(flags,recursive); )

}


void	CNiTriShape::AddProperty(CNode* pc_node)
{
	mc_propertys.AddNode(pc_node);
}



void	CNiTriShape::CreateManual(const char* name, int nof_points, int nof_idx, int nof_texures, bool normals)
{
	SAFE_DELETE(mpc_object_name);
	mpc_object_name = new char[strlen(name)+1];
	strcpy(mpc_object_name,name);

	if (mc_mesh.GetCount()==0) mc_mesh.AddNode(new CNiTriShapeData());
	((CNiTriShapeData*)mc_mesh[0])->CreateManual( nof_points,  nof_idx,  nof_texures,  normals);
}

void	CNiTriShape::SetPoint(int idx, const CVector& point)
{
	assert(mc_mesh.GetCount()>0);
	((CNiTriShapeData*)mc_mesh[0])->SetPoint(idx, point);
}

void	CNiTriShape::SetPointNormal(int idx, const CVector& point)
{
	assert(mc_mesh.GetCount()>0);
	((CNiTriShapeData*)mc_mesh[0])->SetPointNormal(idx,point);
}

void	CNiTriShape::SetPointTexture(int idx, float u, float v, int textchannel)
{
	assert(mc_mesh.GetCount()>0);
	((CNiTriShapeData*)mc_mesh[0])->SetPointTexture(idx,u, v, textchannel);
}

void	CNiTriShape::SetTriangle(int idx, int p1, int p2, int p3)
{
	assert(mc_mesh.GetCount()>0);
	((CNiTriShapeData*)mc_mesh[0])->SetTriangle(idx, p1,p2,p3);
}





#ifdef ANALYSE_NIF
void	CNiTriShape::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,mpc_object_name,TIC_MESH | TICS_UNKNOWNDATA_NOT_CRITICAL);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA | TICS_FINISHED);
	CNode::CreateDbgTree(newsub,addfkt);
    CreateDbgTreeChilds(mc_extradata,"ExtraData",TIC_DATA | TICS_FINISHED,newsub,addfkt);

	char temp[256]; sprintf(temp,"Flag = %i",mn_unknown_flag);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);

	// mc_matrix;

	CreateDbgTreeChilds(mc_morphcontroller,"Morph",TIC_CONTROLLER | TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_propertys,"Properties",TIC_PROPERTY | TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_mesh,"Mesh",TIC_MESH| TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_skinning,"Skinning",TIC_TEXTURE| TICS_FINISHED,newpar,addfkt);
}
#endif

