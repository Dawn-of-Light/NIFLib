#pragma once
#include "cnode.h"

class CNiTriStripsData : public CNode
{
	public:
		CNiTriStripsData(void);
		~CNiTriStripsData(void);

		bool	Load(CNifStream&);

		void	Render();

		bool	GetBoundingbox(CVector& min, CVector& max);


		void	Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result);


		DECLARE_CHUNK(NiTriStripsData,"NiTriStripsData");


	private:



		unsigned short	mn_nof_textures;	/** how much textures */
		int				mn_nof_points;		/** how much points */
		CVector*		mac_points;			/** points coords */
		CVector*		mac_points_normals;	/** point normals */
		float*			maf_ptexture;		/** texture-coords [pointindex*(1+textureNr)*2], [pointindex*(1+textureNr)*2+1] */

		unsigned		mn_nof_strips;		/** how much strips */
		unsigned*		man_nof_polys;		/** nof poly indezes per strip */
		unsigned*		man_poly_index;		/** poly index */

};
