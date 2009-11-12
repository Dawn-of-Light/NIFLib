#pragma once
#include "cnode.h"


/**
*	Here lies the true mesh-data
*	\todo could be speed up by loading points/index/.. as one block - (was done this way for test-reason)
**/
class CNiTriShapeData : public CNode
{
	public:
		CNiTriShapeData(void);
		~CNiTriShapeData(void);

		bool	Load(CNifStream&);
		// special for v2.3 TriShapes ('cause they have the mesh data inside CNiTriShape)
		bool	LoadV2_3(CNifStream&);
		void	Create();

		void	Render();

		bool	GetBoundingbox(CVector& min, CVector& max);


		// ----- manuall creation
		void	CreateManual(int nof_points, int nof_idx, int nof_texures, bool normals);
		void	SetPoint(int idx, const CVector& points);
		void	SetPointNormal(int idx, const CVector& points);
		void	SetPointTexture(int idx, float u, float v, int textchannel);
		void	SetTriangle(int idx, int p1, int p2, int p3);

		void	Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result);


		DECLARE_CHUNK(NiTriShapeData,"NiTriShapeData");

	private:


		unsigned short	mn_nof_textures;	/** how much textures */
		int				mn_nof_points;		/** how much points */
		CVector*		mac_points;			/** points coords */
		CVector*		mac_points_normals;	/** point normals */
		float*			maf_ptexture;		/** texture-coords [pointindex*(1+textureNr)*2], [pointindex*(1+textureNr)*2+1] */

		int				mn_nof_polyindex;	/** nof poly indezes ( = polys*3, if b_use_strip = polys-2) */
		unsigned*		man_poly;			/** poly index */


		// ------ todo
		float			mn_unknown1;
		float			mn_unknown2;
		float			mn_unknown3;
		float			mn_unknown4;

		struct	s_unknown_list
		{
			unsigned short	nof;
			unsigned short* data;
		};
		unsigned		mn_nof;
		s_unknown_list*	mas_unkownlist;

};
