#pragma once



/**
*	\brief standard main Node
*
*   CNiNode
*
*   Basic node in the scene tree.
*	Mainly a transformation matrix and connections to many sub-nodes
*
*
*	\todo: unknown list, some not important values(minmal)
**/
class CNiNode : public CNode
{
	public:
		CNiNode(void);
		~CNiNode(void);

		// ---------- Loading -------------
		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);
		void	Create();
		void	UpdateStatic();


		void	Render();

		bool	GetBoundingbox(CVector& min, CVector& max);
		void	SetRenderFlags(DWORD flags, bool recursive);
		CNode*	FindNode(const char*, CNode**startat,bool exact);

		void	RemoveTexture();
		void	Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result); 


		//  -------- class unique ---------
		const char*	GetNodeName()			{ return mpc_node_name;}
		bool	IsVisible()					{ return !mb_invisible;};
		void	SetVisible(bool vis=true)	{ mb_invisible = !vis; };


		//  -------- manual construction ---------
		void		AddProperty(CNode*);
		void		AddChild(CNode*);
		void		SetName(const char*);
		CMatrix&	GetMatrix()				{ return mc_matrix;};

		// ---------- Animation ----------
		bool		IsStatic();



		DECLARE_CHUNK(NiNode,"NiNode")


	protected:


		// File data
		CChilds		mc_extraData;		
		CChilds		mc_properties;	
		CChilds		mc_controller;
		CChilds		mc_childs;	
		CChilds		mc_unknown;

		char*		mpc_node_name;
		CMatrix		mc_matrix;

		// unknown
		unsigned short mn_unknown;

		// Internal data
		bool		mb_invisible;

		// static mesh
		GLuint		mn_meshlist;

};
