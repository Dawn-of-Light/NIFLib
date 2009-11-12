#include "StdAfx.h"
#include ".\cnilodnode.h"
#include "math.h"



#define MAX_DISTANCE 100000.0f



CNiLODNode::CNiLODNode(void)
{
	mpc_node_name = NULL;
}

CNiLODNode::~CNiLODNode(void)
{
	SAFE_DELETE(mpc_node_name);
}

bool	CNiLODNode::Load(CNifStream& stream)
{

	mpc_node_name = stream.readstring();

	if (stream.Version() < 0x03000000)	stream.readSkip(9);
	else								stream.readSkip(10);


	// Maik Jurkait  03.06.2004 10:30:46
	// ------------------------------------------
	//	Transpose Matrix
	// ------------------------------------------
	stream.readMatrix(mc_matrix);


	if (stream.Version() < 0x03000000)	stream.readSkip(17);
	else
	if (stream.Version() < 0x04000000)	stream.readSkip(8);
	else								stream.readSkip(5);

	// Maik Jurkait  03.06.2004 10:30:55
	// ------------------------------------------
	//	Childs Object
	// ------------------------------------------
	mc_childs.read(stream);

	if (stream.Version() < 0x03000000)	stream.readSkip(9);
	else
	if (stream.Version() < 0x04000000)	stream.readSkip(8);
	else								stream.readSkip(20);

		

	// Maik Jurkait  03.06.2004 10:31:07
	// ------------------------------------------
	//	Distanze data
	// (in some (atm excatly 2) case's this list has more entries then are here Childs)
	// ------------------------------------------
	unsigned len = stream.readUInt();
	for (unsigned i=0;i<len;i++)
	{
		s_lod_data	data;	
		data.dist_near= stream.readFloat();
		data.dist_far= stream.readFloat();

		if (stream.Version() < 0x04000000)
		{
			data.unknown1 = stream.readFloat();
			data.unknown2 = stream.readFloat();
			data.unknown3 = stream.readFloat();
		}

		ma_lod_data.AddLast(data);
	}

	if (stream.Version() < 0x03000000)	stream.readSkip(1);

	return true;
}




void	CNiLODNode::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_childs.Resolve(ppc_nodes, nof);		
}




void	CNiLODNode::Create()
{
	FOREACH(pc,mc_childs,  pc->Create(); )


	// Maik Jurkait  03.06.2004 10:34:33
	// ------------------------------------------
	//	Check for illegal Distances
	// ------------------------------------------
	for (int t=0;t<ma_lod_data.Count();t++)
	{
		// illgal distance value
		if (ma_lod_data[t].dist_near<0)				ma_lod_data[t].dist_near = 0;
		if (ma_lod_data[t].dist_far>MAX_DISTANCE)	ma_lod_data[t].dist_far = MAX_DISTANCE;

		// remove entries without a child
		if (mc_childs.GetCount()<=t)
		{
			ma_lod_data.Delete(t);
			t--;
		}
	}
}



void	CNiLODNode::UpdateStatic()
{
	FOREACH(pc,mc_childs,  pc->UpdateStatic(); )
}



void	CNiLODNode::Render()
{

	// Maik Jurkait  03.06.2004 14:11:16
	// ------------------------------------------
	//	transpose
	// ------------------------------------------
	glPushMatrix();
	glMultMatrixf(mc_matrix.Get());


	// Maik Jurkait  03.06.2004 14:11:33
	// ------------------------------------------
	//	get camera-Distance
	// ------------------------------------------
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,m);
	float distance = sqrt(m[12]*m[12]+m[13]*m[13]+m[14]*m[14]);

	// Maik Jurkait  03.06.2004 14:11:39
	// ------------------------------------------
	//	choose model
	// ------------------------------------------
	int model = 0;
	for (;model<ma_lod_data.Count();model++)
		if (ma_lod_data[model].dist_near<=distance && ma_lod_data[model].dist_far >distance) break;

	// if nothing found -> take highpoly (nearest)
	if (model>=ma_lod_data.Count())
	{
		model = 0;
		for (int i = 1;i<ma_lod_data.Count();i++)
			if (ma_lod_data[i].dist_near<ma_lod_data[model].dist_near) model = i;
	}


	// Maik Jurkait  03.06.2004 14:11:46
	// ------------------------------------------
	//	render
	// ------------------------------------------
	mc_childs[model]->Render();



	// Maik Jurkait  03.06.2004 14:11:26
	// ------------------------------------------
	//	reset matrix
	// ------------------------------------------
	glPopMatrix();

}



void	CNiLODNode::Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result)
{
	CMatrix mat = world;
	mat *= mc_matrix;

	int model = 0;
	for (int i = 1;i<ma_lod_data.Count();i++)
		if (ma_lod_data[i].dist_near<ma_lod_data[model].dist_near) model = i;

	mc_childs[model]->Collision(mat, point,dir,result);
}



bool	CNiLODNode::GetBoundingbox(CVector& min, CVector& max)
{

	// childs
	CVector t_min, t_max;

	bool b_empty = true;

	FOREACH(pc,mc_childs,  

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


	return !b_empty;
}




void	CNiLODNode::SetRenderFlags(DWORD flags, bool recursive)
{
	CNode::SetRenderFlags(flags,recursive);
	if (!recursive) return;

	// childs
	FOREACH(pc,mc_childs,		pc->SetRenderFlags(flags,recursive); )
}



CNode*	CNiLODNode::FindNode(const char*name, CNode** startat, bool exact)
{
	assert(startat);
	if (*startat==NULL)
	{
		if (exact)
		{	if (stricmp(name,mpc_node_name)==0) return this;}
		else
		{	if (strnicmp(name,mpc_node_name,strlen(name))==0) return this;}
	}
	if (this == *startat)
	{
		*startat = NULL;
	}


	// childs
	FOREACH(pc,mc_childs,	
		CNode* result =	pc->FindNode(name,startat, exact);
		if (result!=NULL) return result;
	)

	return NULL;
}

void	CNiLODNode::RemoveTexture()
{
	// childs
	FOREACH(pc,mc_childs, pc->RemoveTexture();)
}



#ifdef ANALYSE_NIF
void	CNiLODNode::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256]; sprintf(temp,"LOD-Node: %s",mpc_node_name);
	newpar = addfkt(par,this, temp,TIC_TRANSPOSE);

	newsub = addfkt(newpar, NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	for (int t=0;t<ma_lod_data.Count();t++)
	{
		sprintf(temp,"%f - %f",ma_lod_data[t].dist_near,ma_lod_data[t].dist_far);
		newsub = addfkt(newpar,NULL, temp,TIC_CHILDS);
		mc_childs[t]->CreateDbgTree(newsub,addfkt);
	}
}
#endif


