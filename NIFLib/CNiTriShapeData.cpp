#include "StdAfx.h"
#include "Cnitrishapedata.h"
#include "glh/glh_extensions.h"
#include <math.h>


bool LineHitsTriangle( const CVector& LinePoint,  const CVector& LineDir,
                       const CVector& v1,  const CVector& v2,  const CVector& v3,
					   CVector& result);

CNiTriShapeData::CNiTriShapeData(void)
{
	mn_nof_textures = 0;
	mn_nof_points = 0;
	mn_nof_polyindex = 0;
	mac_points = NULL;
	mac_points_normals = NULL;
	maf_ptexture = NULL;
	man_poly = NULL;

	mas_unkownlist = NULL;
	mn_nof = 0;

	mdw_render_flags = 0;
}

CNiTriShapeData::~CNiTriShapeData(void)
{
	for (unsigned t=0;t<mn_nof;t++)
	{
		delete(mas_unkownlist[t].data);
	}
	SAFE_DELETE(mas_unkownlist);


	SAFE_DELETE(mac_points);
	SAFE_DELETE(mac_points_normals);
	SAFE_DELETE(maf_ptexture);
	SAFE_DELETE(man_poly);
}




bool	CNiTriShapeData::Load(CNifStream& stream)
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

	// unkown floats, in v4 ints?
	mn_unknown1 = stream.readFloat();
	mn_unknown2 = stream.readFloat();
	mn_unknown3 = stream.readFloat();
	mn_unknown4 = stream.readFloat();


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
	unsigned short edges = stream.readWord();
	// nof index
	unsigned short nof_idx = stream.readWord();

	mn_nof_polyindex = polys*3;
	assert(edges == polys*3);	//	check values
	assert(nof_idx == 0);		//	unused


	man_poly = new unsigned[mn_nof_polyindex];
	for (int i=0;i<mn_nof_polyindex;i++)
	{
		man_poly[i] = stream.readWord();
	}



	// Maik Jurkait  09.06.2004 10:45:27
	// ------------------------------------------
	//	UNKNOWN LIST
	// ------------------------------------------
	mn_nof = 0;
	if (stream.Version() >= 0x03010000 && (stream.Version() != 0x03030000)) // <- strange
	{
		mn_nof = stream.readWord();
        if (mn_nof != 0)
		{
			mas_unkownlist = new s_unknown_list[mn_nof];

			for (unsigned t=0;t<mn_nof;t++)
			{
				mas_unkownlist[t].nof = stream.readWord();	// number of point probs
				mas_unkownlist[t].data = new unsigned short[mas_unkownlist[t].nof];
				for (int t2=0;t2<mas_unkownlist[t].nof;t2++)
				{
					mas_unkownlist[t].data[t2] = stream.readWord();
				}
			}
		}
	}
	
	return true;
}



bool	CNiTriShapeData::LoadV2_3(CNifStream& stream)
{
	unsigned char b_data_available;

	unsigned i = stream.readUInt();	ASSERT(i==2);
	i = stream.readUInt();	ASSERT(i==0);

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

	// unkown floats, in v4 ints?
	stream.readSkip(4 * 4);



	// nof polys
	unsigned short polys = stream.readWord();



	mn_nof_textures = 0;

	// texture coord
	mn_nof_textures = stream.readWord();
	assert(mn_nof_textures <= 8);//,"too much textures");

	b_data_available = stream.readBool();

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
				stream.readFloat();
			}
		}
	}


	// ---------------- TO CHECK
	if (stream.readBool())
	{
		stream.readSkip(mn_nof_points*16);	// floats
	}

	if (stream.readBool())
	{
		stream.readSkip(polys*16);	// floats
	}
	// ----------------



	// Poly index
	mn_nof_polyindex = polys*3;

	man_poly = new unsigned[mn_nof_polyindex];
	for (int i=0;i<mn_nof_polyindex;i++)
	{
		man_poly[i] = stream.readWord();
	}



	return true;
}






void	CNiTriShapeData::Create()
{
}



void	CNiTriShapeData::Render()
{

	// Maik Jurkait  30.05.2004 13:37:32
	// ------------------------------------------
	//	Mesh
	// ------------------------------------------
	{
		if (mn_nof_textures==0)		glDisable(GL_TEXTURE_2D);	

		glBegin(GL_TRIANGLES );

		unsigned *p = man_poly;
		for (int i=0;i<mn_nof_polyindex;i++)
			{
				// Maik Jurkait  30.05.2004 13:38:12
				// ------------------------------------------
				//	Textures
				// ------------------------------------------
				// texture coords
				if (mn_nof_textures>0)
				{
					for (int texture=0;texture<mn_nof_textures;texture++)
						glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture,	
												maf_ptexture[*p*2+texture*mn_nof_points*2],
												maf_ptexture[*p*2+texture*mn_nof_points*2+1]); 
				}

				// Normal
				if (mac_points_normals)
				{
					mac_points_normals[*p].GLOutNRL();
				}

				// point
				mac_points[*p].GLOut();
				p++;
			}
		glEnd();
		if (mn_nof_textures==0)		glEnable(GL_TEXTURE_2D);	
	}


	// Maik Jurkait  30.05.2004 13:45:46
	// ------------------------------------------
	//	Wire
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_WIREFRAME)
	{
		glDepthFunc(GL_LEQUAL);

		glColor3f(1,1,0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES );
			unsigned *p = man_poly;
				for (int i=0;i<mn_nof_polyindex/3;i++)
				{
					mac_points[*p].GLOut();
					mac_points[*(p+1)].GLOut();
					mac_points[*(p+1)].GLOut();
					mac_points[*(p+2)].GLOut();
					mac_points[*(p+2)].GLOut();
					mac_points[*p].GLOut();
					p+=3;
				}
		glEnd();
		glDepthFunc(GL_LESS);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
	}



	// Maik Jurkait  30.05.2004 14:18:30
	// ------------------------------------------
	//	Boundingbox
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_BOUNDINGBOX)
	{
		glDisable(GL_TEXTURE_2D);

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
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
	}


	// Maik Jurkait  30.05.2004 13:45:46
	// ------------------------------------------
	//	Wire
	// ------------------------------------------
	if (mdw_render_flags & RF_SHOW_SELECTED)
	{
		glDepthFunc(GL_LEQUAL);

		glColor3f(1,0,0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_LINES );
			unsigned *p = man_poly;
				for (int i=0;i<mn_nof_polyindex/3;i++)
				{
					mac_points[*p].GLOut();
					mac_points[*(p+1)].GLOut();
					mac_points[*(p+1)].GLOut();
					mac_points[*(p+2)].GLOut();
					mac_points[*(p+2)].GLOut();
					mac_points[*p].GLOut();
					p+=3;
				}
		glEnd();
		glDepthFunc(GL_LESS);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
	}



	// Maik Jurkait  30.05.2004 13:45:46
	// ------------------------------------------
	//	TEST
	// ------------------------------------------
	/*
	for (int t=0;t<mn_nof;t++)
	{
		glDepthFunc(GL_LEQUAL);

		glColor3f(1,0,0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_LINES );

		for (int t2=0;t2<mas_unkownlist[t].nof;t2++)
		{
			mac_points[man_poly[t2]].GLOut();
			mac_points[man_poly[mas_unkownlist[t].data[t2]]].GLOut();
		}

		glEnd();
		glDepthFunc(GL_LESS);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
	}
	*/
}



void	CNiTriShapeData::Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result)
{

	// since we are in object-space we have to transform each vertice
	CVector* tp= new CVector[mn_nof_points];
	for (int t=0;t<mn_nof_points;t++)
		tp[t] = world.Transform(mac_points[t]);

	CVector res;
	unsigned *p = man_poly;
	for (int i=0;i<mn_nof_polyindex/3;i++)
	{
		bool rs = LineHitsTriangle(point,dir,tp[*p],tp[*(p+1)],tp[*(p+2)],res);
		if (rs)	
			result.AddLast(res);
		p+=3;
	}

	delete (tp);
}




void	CNiTriShapeData::CreateManual(int nof_points, int nof_polys, int nof_texures, bool normals)
{
	mn_nof_points = nof_points;
	SAFE_DELETE(mac_points);
	mac_points = new CVector[mn_nof_points+1];
	
	SAFE_DELETE(mac_points_normals);
	if (normals) mac_points_normals = new CVector[mn_nof_points];
	
	mn_nof_textures = nof_texures;
	SAFE_DELETE(maf_ptexture);
	if (mn_nof_textures>0)
			maf_ptexture = new float[(mn_nof_points+1)*2*mn_nof_textures];

	mn_nof_polyindex = nof_polys*3;
	SAFE_DELETE(man_poly);
	man_poly = new unsigned[mn_nof_polyindex];
}


void	CNiTriShapeData::SetPoint(int idx, const CVector& point)
{
	assert(idx < mn_nof_points);
	mac_points[idx]=point;
}


void	CNiTriShapeData::SetPointNormal(int idx, const CVector& point)
{
	assert(idx < mn_nof_points);
	mac_points_normals[idx]=point;
}

void	CNiTriShapeData::SetPointTexture(int idx, float u, float v, int textchannel)
{
	assert(idx < mn_nof_points && textchannel<mn_nof_textures);
	maf_ptexture[idx*2    + mn_nof_points*textchannel] = u;
	maf_ptexture[idx*2 +1 + mn_nof_points*textchannel] = v;
}


void	CNiTriShapeData::SetTriangle(int poly, int p1, int p2, int p3)
{
	assert(p1<mn_nof_points);
	assert(p2<mn_nof_points);
	assert(p3<mn_nof_points);

	int idx = poly*3;
	assert(idx+2 < mn_nof_polyindex);
	man_poly[idx++] = p1;
	man_poly[idx++] = p2;
	man_poly[idx++] = p3;
}







bool	CNiTriShapeData::GetBoundingbox(CVector& min, CVector& max)
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
void	CNiTriShapeData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newsub;

	char temp[256];
	newsub = addfkt(par,this, "TriShapeData",TIC_MESH | TICS_UNKNOWNDATA_LISTED);

	CNode::CreateDbgTree(newsub,addfkt);

	// mesh data
	sprintf(temp,"Textures: %i",mn_nof_textures);
	addfkt(newsub,NULL,temp,TIC_DATA | TICS_FINISHED );

	sprintf(temp,"Points: %i",mn_nof_points);
	addfkt(newsub,NULL,temp,TIC_DATA | TICS_FINISHED);

	sprintf(temp,"Indizes: %i",mn_nof_polyindex);
	addfkt(newsub,NULL,temp,TIC_DATA | TICS_FINISHED);


	// ------- unknown data
	sprintf(temp,"Unknow1: %f",mn_unknown1);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"Unknow2: %f",mn_unknown2);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"Unknow3: %f",mn_unknown3);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);
	sprintf(temp,"Unknow4: %f",mn_unknown4);
	addfkt(newsub,NULL,temp,TIC_ABSTRACT);

	sprintf(temp,"LIst: %i",mn_nof_polyindex);
	newsub = addfkt(newsub,NULL,temp,TIC_ABSTRACT);

	for (unsigned t=0;t<mn_nof;t++)
	{
		void* newsub2;
		sprintf(temp,"%i (%i entries)",t,(int)mas_unkownlist[t].nof);
		newsub2 = addfkt(newsub,NULL,temp,TIC_ABSTRACT);

		for (int t2=0;t2<mas_unkownlist[t].nof;t2++)
		{
			sprintf(temp,"%i",(int)mas_unkownlist[t].data[t2]);
			addfkt(newsub2,NULL,temp,TIC_ABSTRACT);
		}
	}
	// ------------------------------------------

}
#endif




/**
*	Line/triangle hit test
*	http://www.scherfgen-software.net/index.php?action=tutorials&topic=collision_2
**/
bool LineHitsTriangle( const CVector& LinePoint,  const CVector& LineDir,
                       const CVector& v1,  const CVector& v2,  const CVector& v3,
					   CVector& intersection)  
{

	// Maik Jurkait  12.07.2004 19:10:07
	// ------------------------------------------
	//	calc plane intersection
	// ------------------------------------------
	// plane
	CVector plane_normal;
	float   plane_distanze;

	// calc plane
	CVector temp = v3-v2;
	plane_normal = temp.CrossProdukt(v1-v2);
	plane_distanze = -(v1.GetX() * plane_normal.GetX() + v1.GetY() * plane_normal.GetY() + v1.GetZ() * plane_normal.GetZ());

	if (fabs(plane_normal.Length())<0.1f) return false;

	// Line - plane check
    const float Denominator = plane_normal.DotProduct(LineDir);

    if(fabsf(Denominator) < 0.0001f)
    {
		// so, the line is parallel 
        if(fabsf(plane_normal.DotProduct(LinePoint)+plane_distanze) < 0.0001f)
        {
            // line inside the plane
            intersection = LinePoint;
        }
        else
        {
            // no collision
            return false;
        }
    }

    // line position
    float s =   (plane_normal.DotProduct(LinePoint)+plane_distanze) / -Denominator;

	// to safe calculations we only want point from linepoint to linepoint+linedir
    if(s < 0.0f || s > 1.0f) return false;

    // Es gibt einen Schnittpunkt!
    intersection = LinePoint + LineDir*s;


	// Maik Jurkait  12.07.2004 19:30:12
	// ------------------------------------------
	//	part 2) check point is in triangle
	// ------------------------------------------
	temp = v1-v2;
	CVector plane_normal2 = temp.CrossProdukt(plane_normal);
	plane_distanze = -(v1.GetX() * plane_normal2.GetX() + v1.GetY() * plane_normal2.GetY() + v1.GetZ() * plane_normal2.GetZ());
    if(intersection.DotProduct(plane_normal2) + plane_distanze <0.0f) return false;

	temp = v2-v3;
	plane_normal2 = temp.CrossProdukt(plane_normal);
	plane_distanze = -(v2.GetX() * plane_normal2.GetX() + v2.GetY() * plane_normal2.GetY() + v2.GetZ() * plane_normal2.GetZ());
    if(intersection.DotProduct(plane_normal2) + plane_distanze <0.0f) return false;

	temp = v3-v1;
	plane_normal2 = temp.CrossProdukt(plane_normal);
	plane_distanze = -(v3.GetX() * plane_normal2.GetX() + v3.GetY() * plane_normal2.GetY() + v3.GetZ() * plane_normal2.GetZ());
    if(intersection.DotProduct(plane_normal2) + plane_distanze <0.0f) return false;

 
    return true;
}

