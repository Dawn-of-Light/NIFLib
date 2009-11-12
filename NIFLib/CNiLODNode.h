#pragma once
#include "cnode.h"

class CNiLODNode :	public CNode
{
	protected:
		struct s_lod_data
		{
			float dist_near;
			float dist_far;

			float unknown1;
			float unknown2;
			float unknown3;
		};


	public:

		CNiLODNode(void);
		~CNiLODNode(void);

		bool	Load(CNifStream&);
		void	Create();
		void	CreateHierachy(CNode**, int nof);

		void	SetRenderFlags(DWORD flags, bool recursive);
		CNode*	FindNode(const char*, CNode**startat,bool exact);
		void	RemoveTexture();

		bool	GetBoundingbox(CVector& min, CVector& max);

		void	UpdateStatic();

		/// \todo could be opimizied by one list for each child
		bool	IsStatic()			{ return false;};

		void	Render();


		void	Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result);



		DECLARE_CHUNK(NiLODNode,"NiLODNode");


	private:

		char*					mpc_node_name;
		CMatrix					mc_matrix;
		CChilds					mc_childs;
		TArray	<s_lod_data>	ma_lod_data;
};
