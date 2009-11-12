#pragma once




/**
*	Debug function
**/
#ifdef ANALYSE_NIF
	#define DBG_TREE_CREATE				void	CreateDbgTree(void* , void*(*)(void*, CNode*, char* text, int image));
#else
	#define DBG_TREE_CREATE		
#endif



/**
*	dynamic create a class
*
*   All chunk classes must be named:
*	 "Cmychunk"
*	where "mychunk" is the exact name how it is called in a Nif-File
*   
*	2 Steps are needed to be done:
*	a) add a 
*		DECLARE_CHUNK(my_class)
*	   in class definition
*	b) add a 
*		REGISTER_NODE(my_class)
*	   in InitNif() (NifLib.cpp)
**/
#define DECLARE_CHUNK(n,m)	public: static CNode*	_CreateClass()	{return new C##n();} \
									const char*		GetClassName()			{return m;} \
									DBG_TREE_CREATE
	

/**
*	ASSERT_CLASS(pc_node, name)
*	in debug version: assert if chunk is not needed type
**/
#ifdef _DEBUG
	#define ASSERT_CLASS(pc_node, name)	assert(strcmp(pc_node->GetClassName(),name)==0)
#else
	#define ASSERT_CLASS(pc_node, name) 
#endif




/**
*	Render Flags
**/
#define RF_SHOW_WIREFRAME	(1<<0)
#define RF_SHOW_BOUNDINGBOX	(1<<1)
#define RF_SHOW_SELECTED	(1<<2)  // hilight it if possible






/**
*	CNode
*
*	Base node for all
**/
class CNode
{
	// this needs direct access - but only for debugging
	// (refernz counting)
	friend class CChilds;
	friend NIF::object CreateObjectFromMemory(void* p_memory, size_t n_size);


	public:
		// ---------- Class stuff -----------
		CNode();
		virtual ~CNode();

		bool				IsClass(const char*);
		virtual	const char*	GetClassName()		{return "abstract";}

		// ---------- Debug stuff ------------
		#ifdef ANALYSE_NIF
			virtual	void		CreateDbgTree(void* , void* (*)(void* ,CNode* node,  char* text, int image));
			void				CreateDbgTreeChilds(CChilds&, char*name, int image, void* par, void* 	(*)(void* , CNode* node, char* text, int image));
		#endif


		// ---------- Chunk id ------------
		DWORD			GetID();


		// ---------- Loading -------------
		/**
		*	Base loading function
		*	Read's just the chunk head and calls Load(CNifStream&)
		**/
		bool				Load(CNifStream&, DWORD id);

		/**
		*	Abstract function 
		*	Must be overloaded and read all chunk stuff
		**/
		virtual bool		Load(CNifStream&) = 0;

		/**
		*	Called by framework to build the scene-tree
		*	Usally it consist of:
		*	-	mc_childs.Resolve(ppc, nof);
		*			which will translate the ID's to pointers
		*	-	mc_parent.ResolveWithoutReferenz(ppc, nof);
		*			the same but with out refernzing it
		**/
		virtual void		CreateHierachy(CNode**, int nof)	{};

		/**
		*	Overload to create data
		*	(textures, ...)
		**/
		virtual void		Create()			{};


		/**
		*	Here we can create meshlists.
		*
		*	This should also be called if child have changed.
		*	e.g. if removing/hiding/adding something
		**/
		virtual void		UpdateStatic()		{};


		// ---------- Rendering -----------
		virtual void		Render()			{};
		virtual void		AfterRender()		{};

		// ---------- Animation ----------
		virtual bool		IsStatic()			{ return true;};
		virtual void		Update(float time)	{};
		virtual float		MaxAnimationTime()	{ return 0;};


		// ---------- Data access ---------
		virtual bool		GetBoundingbox(CVector& min, CVector& max)			{ return false;};
		virtual void		SetRenderFlags(DWORD flags, bool recursive)			{ mdw_render_flags = flags;};
				DWORD		GetRenderFlags()									{ return mdw_render_flags;};
		virtual CNode*		FindNode(const char*, CNode** startat,bool exact)	{ return NULL;};

		virtual void		Collision(const CMatrix& world, const CVector& point, const CVector& dir, TArray<CVector> &result) {}; 

		virtual void		RemoveTexture()								{};


	protected:
		/**
		* flag field for rendering
		* just for debug reasons
		**/
		DWORD			mdw_render_flags;

	private:

		/**
		* used in hierachy generation
		* mainly for checking
		**/
		int		mn_referenzes;
		
	
		/**
		*	Chunk-ID
		**/
		DWORD	mdw_id;



};
