#pragma once
#include "cnode.h"

class CNiTriShape :	public CNode
{
	public:
		CNiTriShape(void);
		~CNiTriShape(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);
		void	Create();

		
		void	Render();

		bool	GetBoundingbox(CVector& min, CVector& max);
		void	SetRenderFlags(DWORD flags, bool recursive);


		void	RemoveTexture();

		// --------- manual creation ------
		void	CreateManual(const char* name, int nof_points, int nof_idx, int nof_texures, bool normals);
		void	SetPoint(int idx, const CVector& points);
		void	SetPointNormal(int idx, const CVector& points);
		void	SetPointTexture(int idx, float u, float v, int textchannel);
		void	SetTriangle(int idx, int p1, int p2, int p3);

		void	AddProperty(CNode*);

		void	Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result);


		DECLARE_CHUNK(NiTriShape,"NiTriShape");

	private:

		char*		mpc_object_name;
		CMatrix		mc_matrix;

		CChilds		mc_extradata;
		CChilds		mc_morphcontroller;
		CChilds		mc_propertys;
		CChilds		mc_mesh;
		CChilds		mc_skinning;

		unsigned short mn_unknown_flag;

};
