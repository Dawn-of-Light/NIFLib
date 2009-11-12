#include "StdAfx.h"
#include ".\cnibillboardnode.h"

CNiBillboardNode::CNiBillboardNode(void)
{
}

CNiBillboardNode::~CNiBillboardNode(void)
{
}


bool	CNiBillboardNode::Load(CNifStream& stream)
{
	CNiNode::Load(stream);

	if (stream.Version() < 0x03000000)
		stream.readUInt();

	return true;
}


void	CNiBillboardNode::Render()
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

		// clear rotation
		float m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		memset(m,0,sizeof(float)*11);
		m[0] = 1.0f;
		m[5] = 1.0f;
		m[10] = 1.0f;
		m[12] += mc_matrix[13];
		m[13] += mc_matrix[14];
		m[14] += mc_matrix[15];
		glLoadMatrixf(m);

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


#ifdef ANALYSE_NIF
void	CNiBillboardNode::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	CNiNode::CreateDbgTree(par,addfkt);
}
#endif