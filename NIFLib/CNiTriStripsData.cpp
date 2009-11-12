#include "StdAfx.h"
#include "CNitristripsdata.h"
#include "glh/glh_extensions.h"


bool LineHitsTriangle( const CVector& LinePoint,  const CVector& LineDir,
                       const CVector& v1,  const CVector& v2,  const CVector& v3,
					   CVector& result);



CNiTriStripsData::CNiTriStripsData(void)
{
	mn_nof_textures = 0;
	mn_nof_points = 0;
	mn_nof_strips = 0;

	mac_points = NULL;
	mac_points_normals = NULL;
	maf_ptexture = NULL;

	man_nof_polys = NULL;
	man_poly_index = NULL;
}

CNiTriStripsData::~CNiTriStripsData(void)
{
	SAFE_DELETE(mac_points);
	SAFE_DELETE(mac_points_normals);
	SAFE_DELETE(maf_ptexture);
	SAFE_DELETE(man_nof_polys);
	SAFE_DELETE(man_poly_index);
}



bool	CNiTriStripsData::Load(CNifStream& stream)
{
	unsigned char b_data_available;

    // point count
	mn_nof_points = stream.readWord();
	
	
	b_data_available = stream.readBool();// if false no point data
	assert(b_data_available);// always need mesh points

	// point data
	mac_points = new CVector[mn_nof_points];
	stream.readPoints(mac_points, mn_nof_points);


    // Point-Normals
	b_data_available = stream.readBool();// if false, no normals
	if (b_data_available)
	{
		mac_points_normals = new CVector[mn_nof_points];
		stream.readPoints(mac_points_normals, mn_nof_points);
	}

	// unkown floats, v4 ints?
	stream.readSkip(4 * 4);


	// extra data  (Vertex Color?)
	b_data_available = stream.readBool();
	if (b_data_available)
	{
		stream.readSkip(4 * mn_nof_points * 4);
	}




	// texture coord
	mn_nof_textures = stream.readWord();
	assert(mn_nof_textures <= 8);//,"too much textures");

	// old version got additional points stored
	b_data_available = true;
	if (stream.Version() < 0x04000003)	b_data_available = stream.readBool();

	if (mn_nof_textures >0)
	{
		assert(b_data_available);

		maf_ptexture = new float[mn_nof_points*2*mn_nof_textures];

		for (int m=0;m<mn_nof_textures;m++)
		{
			for (int i=0;i<mn_nof_points;i++)
			{
				maf_ptexture[i*2+m*mn_nof_points*2]		= stream.readFloat();
				maf_ptexture[i*2+m*mn_nof_points*2+1]	= stream.readFloat();
			}
		}
	}



	// nof polys
	unsigned short polys = stream.readWord();

	// nof edges
	mn_nof_strips = stream.readWord();


	man_nof_polys = new unsigned[mn_nof_strips];
	int counter = 0;
	for(unsigned i=0;i<mn_nof_strips;i++)
	{
		int r = stream.readWord();
		man_nof_polys[i]=r;
		counter +=r;
	}

	// poly indezes
	man_poly_index = new unsigned[counter];
	for(int i=0;i<counter;i++)
	{
		man_poly_index[i] = stream.readWord();
	}


	return true;
}


void	CNiTriStripsData::Render()
{
	if (mn_nof_textures==0)		glDisable(GL_TEXTURE_2D);	

	// Maik Jurkait  02.06.2004 17:38:56
	// ------------------------------------------
	//	Mesh out
	// ------------------------------------------
	unsigned int*p = man_poly_index;
	for (unsigned int i=0;i<mn_nof_strips;i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (unsigned int t=0;t<man_nof_polys[i];t++)
		{
			// Maik Jurkait  02.06.2004 17:39:52
			// ------------------------------------------
			//	texture coords
			// ------------------------------------------
			if (mn_nof_textures>0)
			{
				for (int texture=0;texture<mn_nof_textures;texture++)
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture,	
											maf_ptexture[*p*2+texture*mn_nof_points*2],
											maf_ptexture[*p*2+texture*mn_nof_points*2+1]); 
			}

			// Maik Jurkait  02.06.2004 17:41:04
			// ------------------------------------------
			//	Normal
			// ------------------------------------------
			if (mac_points_normals)
			{
				mac_points_normals[*p].GLOutNRL();
			}


			// Maik Jurkait  02.06.2004 17:41:09
			// ------------------------------------------
			//	Point
			// ------------------------------------------
			mac_points[*p].GLOut();
			p++;
		}

		glEnd();
	}

	if (mn_nof_textures==0)		glEnable(GL_TEXTURE_2D);	


	
	// Maik Jurkait  02.06.2004 17:50:22
	// ------------------------------------------
	//	Wire
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_WIREFRAME)
	{
		glDepthFunc(GL_LEQUAL);
		// polys
		glColor3f(1,1,0);

		unsigned int*p = man_poly_index;
		for (unsigned int i=0;i<mn_nof_strips;i++)
		{
			glBegin(GL_LINES);
			for (unsigned int t=0;t<man_nof_polys[i]-2;t++)
			{
				// point
				mac_points[*p].GLOut();		mac_points[*(p+1)].GLOut();
				mac_points[*(p+1)].GLOut();	mac_points[*(p+2)].GLOut();
				mac_points[*(p+2)].GLOut();	mac_points[*p].GLOut();
				p++;
			}
			glEnd();
			p+=2;
		}
		glDepthFunc(GL_LESS);
	}


	// Maik Jurkait  02.06.2004 17:51:21
	// ------------------------------------------
	//	Boundingbox
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_BOUNDINGBOX)
	{
		// Calc Boundingbox
		CVector min;
		CVector max;
		GetBoundingbox( min, max);

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
	}


	// Maik Jurkait  30.05.2004 13:45:46
	// ------------------------------------------
	//	Wire
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_SELECTED)
	{
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);


		// polys
		glColor3f(1,0,0);

		unsigned int*p = man_poly_index;
		for (unsigned int i=0;i<mn_nof_strips;i++)
		{
			glBegin(GL_LINES);
			for (unsigned int t=0;t<man_nof_polys[i]-2;t++)
			{
				// point
				mac_points[*p].GLOut();		mac_points[*(p+1)].GLOut();
				mac_points[*(p+1)].GLOut();	mac_points[*(p+2)].GLOut();
				mac_points[*(p+2)].GLOut();	mac_points[*p].GLOut();
				p++;
			}
			glEnd();
			p+=2;
		}
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
	}
}


void	CNiTriStripsData::Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result)
{

	// since we are in object-space we have to transform each vertice
	CVector* tp= new CVector[mn_nof_points];
	for (int t=0;t<mn_nof_points;t++)
		tp[t] = world.Transform(mac_points[t]);

	CVector res;
	unsigned int*p = man_poly_index;
	for (unsigned int i=0;i<mn_nof_strips;i++)
	for (unsigned int t=0;t<man_nof_polys[i]/3;t++)
	{
		bool rs = LineHitsTriangle(point,dir,tp[*p],tp[*(p+1)],tp[*(p+2)],res);
		if (rs)	
			result.AddLast(res);
		p+=3;
	}


	delete (tp);
}




bool	CNiTriStripsData::GetBoundingbox(CVector& min, CVector& max)
{
	/// \todo could be optimized by recalcing only if changed ... but since bounding box isn't realy needed it's okay this way

	min = mac_points[0];
	max = mac_points[0];

	for (int i=1;i<mn_nof_points;i++)
	{
		if (min.GetX()>mac_points[i].GetX())	min.SetX(mac_points[i].GetX());
		if (min.GetY()>mac_points[i].GetY())	min.SetY(mac_points[i].GetY());
		if (min.GetZ()>mac_points[i].GetZ())	min.SetZ(mac_points[i].GetZ());
		if (max.GetX()<mac_points[i].GetX())	max.SetX(mac_points[i].GetX());
		if (max.GetY()<mac_points[i].GetY())	max.SetY(mac_points[i].GetY());
		if (max.GetZ()<mac_points[i].GetZ())	max.SetZ(mac_points[i].GetZ());
	}

	return true;
}







#ifdef ANALYSE_NIF
void	CNiTriStripsData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "TriStripsData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif