/**
* NIFLib - DLL
* 
* DLL interface
**/

#ifdef NIFLIB_DLL_EXPORTS
#define NIFLIB_API __declspec(dllexport)
#else
#define NIFLIB_API __declspec(dllimport)
#endif


#ifdef _DEBUG
	/**
	*	if ANALYSE_NIF is defined
	*	all NIF checking stuff will be generated
	**/
	#define ANALYSE_NIF

	/**
	*	if NO_OPTIMATION is defined
	*	no optimation is done
	*	THIS IS NEEDED TO SHOW ALL BOUNDINGBOXES and SELECTIONS
	**/
	//#define NO_OPTIMATION

#endif



/**
*	Helper function
**/
#define SAFE_OBJ_RELEASE(x)		if (x) {NIF::ReleaseObject(x);x=NULL;}
#define SAFE_DELETE(x)			if (x) {delete(x);x=NULL;}


namespace NIF
{
	
	// --------------------------------- DATA -------------------------

	/** \brief object handle
	*
	*	The object handle.
	*	Used by many interface functions.
	*	\internal a wrapper for a CNode pointer.
	**/
	NIFLIB_API typedef void* object;






	// --------------------------------- BASIC FUNCTIONS -------------------------

	/** \brief Initialise DLL
	*
	*	Create must be called on startup.
	*   Does:
	*	- initialise internal vars.
	*	- Initialise OpenGL
	*	
	*	After that call you can use OpenGL functions to manipulate the display.
	*
	*	\param HWND Window handle which should be used for Rendering
	**/
	NIFLIB_API bool	Create(HWND wnd);



	/** \brief Release DLL
	*
	*	Release all stuff.
	*
	*	Memory cleanup and releasing OpenGL.
	**/
	NIFLIB_API void	Release();



	/** \brief call on resize
	*
	*	Should be called on OpenGL Window-Resize
	*	It'll recalc the perspective of the view
	**/
	NIFLIB_API void	OnWindowResize();




	// --------------------------------- BASIC FUNCTIONS -------------------------


	/** \brief Loads a NIF Object
	*
	* Loads a NIF Object
	* It'll generate a object which can be handle by the other function here.
	* Make sure OpenGL is alread initialized.
	*
	* !!!! RESET SKIN if it's not used (or setup skin) BEFORE !!!!
	*
	* \param  filename
	*			should be a ".nif" or ".npk" (".kfa" not supported right now)
	*
	* \param  path
	*			should be the full path name
	*			In case of a packed file this should be the filename of the mpk.
	*
	* \return   Handle to the created Nif-Object
	*			NULL on error
	*
	* Examples:
	*	object*pc = CreateObject("c:\\camelot\\nifs","bag.nif");
	*
	*	object*pc = CreateObject("c:\\camelot\\nifs","b-rndtw2.npk");
	*		same as:
	*	object*pc = CreateObject("c:\\camelot\\nifs\\","b-rndtw2.nif");
	*		same as:
	*	object*pc = CreateObject("c:\\camelot\\nifs\\b-rndtw2.npk","b-rndtw2.nif");
	*
	**/
	NIFLIB_API object	CreateObject(const char* path, const char* filename);



	/** \brief Release a object.
	*
	* Release a object.
	* Note: there is no internal code for managing the objects, so it's upto you to release these.
	*
	* @param object  Object to release
	**/
	NIFLIB_API void	ReleaseObject(object);



	/** \brief Render given object.
	*
	* Render given object.
	* It's really just renders the object.
	* !! You've to clear and setup the Matrix for the OpenGL window. !!
	* So you've many controlls how (exspecialy where) the object will be placed.
	* 
	* \param object Object to render
	*
	*
	*  So typical will have a Render loop like this one:
	*
	*	 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	*		glLoadIdentity();									// Reset View
	*		
	*		// here you could add transforms to move the object
	*		// maybe:
	*		// glTranslatef(0.0f,0.0f,-50.0f);
	*		// glRotatef(x_rot++,1.0f,0.0f,0.0f);
	*
	*		NIF::Render(m_nif_obj);								// Render the NIF::object
	*
	*		// here you could render more objects if wanted
	*
	*		SwapBuffers(m_hdc);
	**/
	NIFLIB_API void	Render(object);








	// --------------------------------- DAoC - functions -------------------------

	/** \brief Stores DAoC-Directoy
	*	
	*	The camelot-Dircetory is needed to find the CSV-Files
	*	and additional textures.
	**/
	NIFLIB_API void	SetRootDirectory(const char* path);


	/**
	*	Load a object from ID
	**/
	NIFLIB_API object	DAoC_LoadMonster(int ID);
	NIFLIB_API object	DAoC_LoadItem(int ID);
	NIFLIB_API object	DAoC_LoadZone(int ID);


	/**
	*	Get possible ID's
	*	\param int* pc_buffer
	*			buffer which will recieve all ID's
	*			if NULL the function just return how much IDs are possible
	*
	*	\return	number of filled IDs
	*			if buffer hasn't enough space more could be availabe
	*			if pc_buffer was NULL this function returns how many IDs are possible
	*
	*	\example
	*		int nof = DAoC_GetMonsterIDs(NULL,0);
	*		int* buffer = new int[nof];
	*		nof = DAoC_GetMonsterIDs(buffer,nof);
	**/
	NIFLIB_API int		DAoC_GetMonsterIDs(int* pc_buffer, int buffer_size);
	NIFLIB_API int		DAoC_GetItemIDs(int*, int size);
	NIFLIB_API int		DAoC_GetZoneIDs(int*, int size);

	/**
	*	Get name of object
	**/
	NIFLIB_API bool		DAoC_GetMonsterName(int ID, char*, int size);
	NIFLIB_API bool		DAoC_GetItemName(int ID, char*, int size);
	NIFLIB_API bool		DAoC_GetZoneName(int ID, char*, int size);

	/**
	*	Get extra information for that monster/item/zone
	**/
	NIFLIB_API bool		DAoC_GetMonsterExtraInfo(int ID, char*, int size);
	NIFLIB_API bool		DAoC_GetItemExtraInfo(int ID, char*, int size);
	NIFLIB_API bool		DAoC_GetZoneExtraInfo(int ID, char*, int size);


	// --------------------------------- Debug / Table generation ---------------
	NIFLIB_API void		DAoC_LoadAllZones();



	// --------------------------------- Manipulation / Information ---------------
	/**
	*	
	**/
	NIFLIB_API bool	GetBoundingBox(object, float& x1, float& y1, float& z1, float& x2, float& y2, float& z2);



	/**
	*	\todo functions for reading zones
	**/


	/**
	*	\todo functions for manipulation will be added later
	**/
}

