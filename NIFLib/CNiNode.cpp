#include "StdAfx.h"
#include "CNiNode.h"
#include "CNiStringExtraData.h"


CNiNode::CNiNode(void)
{
	mb_invisible = false;
	mpc_node_name = NULL;
	mc_matrix.Identity();
	mn_meshlist = -1;
}

CNiNode::~CNiNode(void)
{
	SAFE_DELETE(mpc_node_name);
}



bool	CNiNode::Load(CNifStream& stream)
{
	// Name
	mpc_node_name = stream.readstring();
	if (!mpc_node_name) return false;


	if (stream.Version() < 0x03000000)
	{
		// NiStringExtraData embedded
		if (stream.readBool())
		{
			CNiStringExtraData* pc_newnode = new CNiStringExtraData();
			bool res = pc_newnode->LoadV2_3embedded(stream);
			if(!res) return false;
			mc_extraData.AddNode(pc_newnode);
		}
		unsigned i = stream.readUChar(); assert(i==0);
		mn_unknown = 0;
	}
	else
	{
		// strings
		mc_extraData.read(stream,1);

		// vis controller
		mc_controller.read(stream,1);

		mn_unknown = stream.readWord();
		//ASSERT(f==12); // ??
	}


	// Matrix 
	stream.readMatrix(mc_matrix);


	// Properties (Texturemodeporperty, VertexColorProperty)
	mc_properties.read(stream);	



	if (stream.Version() < 0x03000000)
	{
		unsigned i;
		i = stream.readUInt();	assert(i==2);
		i = stream.readUInt();	assert(i==0);
		i = stream.readUInt();	assert(i==2);
		i = stream.readUInt();	assert(i==0);
		i = stream.readUChar(); assert(i==1 || i==0);
	}
	else
	{
		unsigned i = stream.readBool(); // flag??
		assert(i==0);// was always 0, maybe here is another list
	}


	// childs
	mc_childs.read(stream);

	mc_unknown.read(stream);




	/// \todo automatic hidding. This should be catched somewhere else.
	if (mpc_node_name && strnicmp(mpc_node_name,"climb",5)==0)			mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"collidee",8)==0)		mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"pick",4)==0)			mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"bounding",8)==0)		mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"shadowcaster",12)==0)	mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"footprints",10)==0)	mb_invisible = true;
	if (mpc_node_name && strnicmp(mpc_node_name,"options",7)==0)		mb_invisible = true; // housing

	if (stream.Version()>0x03010000)
		if (mpc_node_name && strnicmp(mpc_node_name,"bip",3)==0)			mb_invisible = true;

	return true;
}




void	CNiNode::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_extraData.Resolve(ppc_nodes, nof);		
	mc_properties.Resolve(ppc_nodes, nof);		
	mc_controller.Resolve(ppc_nodes, nof);		
	mc_childs.Resolve(ppc_nodes, nof);		
	mc_unknown.Resolve(ppc_nodes, nof);
}


void	CNiNode::Create()
{
	FOREACH(pc,mc_extraData,  pc->Create(); )
	FOREACH(pc,mc_properties,  pc->Create(); )
	FOREACH(pc,mc_controller,  pc->Create(); )
	FOREACH(pc,mc_childs,  pc->Create(); )
	FOREACH(pc,mc_unknown,  pc->Create(); )
}


bool	CNiNode::IsStatic()
{
	FOREACH(pc,mc_properties,	if (!pc->IsStatic()) return false; )
	FOREACH(pc,mc_controller,	if (!pc->IsStatic()) return false; )
	FOREACH(pc,mc_childs,		if (!pc->IsStatic()) return false; )
	FOREACH(pc,mc_unknown,		if (!pc->IsStatic()) return false; )
	return true;
}


void	CNiNode::UpdateStatic()
{
	#ifdef NO_OPTIMATION
		return;
	#endif


	// Maik Jurkait  30.06.2004 12:56:57
	// ------------------------------------------
	//	Delete old
	// ------------------------------------------
	if (mn_meshlist != -1)
		glDeleteLists(mn_meshlist,1);
	mn_meshlist = -1;



	// Maik Jurkait  30.06.2004 12:56:55
	// ------------------------------------------
	//	check if this is static
	// ------------------------------------------
	if (IsStatic())
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// needs temp var else render will render nothing
		GLuint newlist = glGenLists(1); 
		glNewList(newlist,GL_COMPILE);
		Render();
		glEndList();
		mn_meshlist = newlist;
	}
	else
	{
		// Maik Jurkait  30.06.2004 12:57:07
		// ------------------------------------------
		//	since we can't opimize this
		//  give childs a chance
		// ------------------------------------------
		FOREACH(pc,mc_childs,	pc->UpdateStatic(); )
	}
}


void	CNiNode::Render()
{
	// if optimized
	if (mn_meshlist != -1)
	{
		glCallList(mn_meshlist);
	}
	else
	{
		// if invisible
		if (mb_invisible) return;

		// properties
		FOREACH(pc,mc_properties,  pc->Render(); )

		glPushMatrix();
		glMultMatrixf(mc_matrix.Get());

		// anim
		FOREACH(pc,mc_controller,  pc->Render(); )

		// childs
		FOREACH(pc,mc_childs,  pc->Render(); )

		glPopMatrix();
	}



	// Maik Jurkait  30.05.2004 14:18:30
	// ------------------------------------------
	//	Boundingbox
	//	since this should be debug only we do it un-optimized
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_BOUNDINGBOX)
	{
		glDisable(GL_TEXTURE_2D);

		// Calc Boundingbox
		CVector min;
		CVector max;
		GetBoundingbox( min, max); // min&max is in worldspace

		// boundingbox
		glColor3f(0,0,1);
		glBegin(GL_LINES );
				glVertex3f( min.GetX(),  min.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( min.GetX(),  min.GetY(),  min.GetZ());

				glVertex3f( min.GetX(),  min.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  max.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  max.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  max.GetZ());
				glVertex3f( min.GetX(),  min.GetY(),  max.GetZ());

				glVertex3f( min.GetX(),  min.GetY(),  min.GetZ());
				glVertex3f( min.GetX(),  min.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  min.GetY(),  max.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( max.GetX(),  max.GetY(),  max.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  min.GetZ());
				glVertex3f( min.GetX(),  max.GetY(),  max.GetZ());
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
	}
}




bool	CNiNode::GetBoundingbox(CVector& min, CVector& max)
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


	/// \todo animation tr anim
//	FOREACH(pc,mc_controller,  pc->Render(); )


	return !b_empty;
}





void	CNiNode::SetRenderFlags(DWORD flags, bool recursive)
{
	CNode::SetRenderFlags(flags,recursive);
	if (!recursive) return;

	// properties
	FOREACH(pc,mc_properties,	pc->SetRenderFlags(flags,recursive); )

	// anim
	FOREACH(pc,mc_controller,	pc->SetRenderFlags(flags,recursive); )

	// childs
	FOREACH(pc,mc_childs,		pc->SetRenderFlags(flags,recursive); )
}



CNode*	CNiNode::FindNode(const char*name, CNode** startat, bool exact)
{
	assert(startat);
	if (*startat==NULL && mpc_node_name)
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
		CNode* result =	pc->FindNode(name,startat,exact);
		if (result!=NULL) return result;
	)

	return NULL;
}


void	CNiNode::RemoveTexture()
{
	FOREACH(pc,mc_properties,	pc->RemoveTexture(););
	FOREACH(pc,mc_childs,		pc->RemoveTexture(););
}

void	CNiNode::Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result)
{
	if (mb_invisible) return;

	CMatrix mat = world;
	mat *= mc_matrix;

	FOREACH(pc,mc_childs,		pc->Collision(mat,point,dir,result););
}




void	CNiNode::AddProperty(CNode* pc_node)
{
	mc_properties.AddNode(pc_node);
}


void	CNiNode::AddChild(CNode* pc_node)
{
	mc_childs.AddNode(pc_node);
}


void	CNiNode::SetName(const char* name)
{
	SAFE_DELETE(mpc_node_name);
	mpc_node_name = new char[strlen(name)+1];
	strcpy(mpc_node_name,name);
}






#ifdef ANALYSE_NIF
void	CNiNode::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,mpc_node_name,TIC_TRANSPOSE | TICS_UNKNOWNDATA_NOT_CRITICAL);

	// base data
	newsub = addfkt(newpar,NULL,"Data",TIC_DATA | TICS_FINISHED);
	CNode::CreateDbgTree(newsub,addfkt);
	CreateDbgTreeChilds(mc_extraData,"ExtraData",TIC_DATA | TICS_FINISHED,newsub,addfkt);
	mc_matrix.CreateDbgTree(newsub,addfkt);

	newsub = addfkt(newsub,NULL,"unkown",TIC_ABSTRACT | TICS_INVESTIGATE);
	char temp[256]; sprintf(temp,"word(flag?) = 0x%x",(int)mn_unknown);
	addfkt(newsub,NULL,temp,TIC_DATA | TICS_FINISHED);
	

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER | TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_properties,"Properties",TIC_PROPERTY | TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_childs,"Childs",TIC_CHILDS | TICS_FINISHED,newpar,addfkt);
	CreateDbgTreeChilds(mc_unknown,"Unknown",TIC_ABSTRACT,newpar,addfkt);
}
#endif 



