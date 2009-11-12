/**
* NIFLib-Base.h
* Global vars and DLL base stuff
**/

#include "niflib.h"
class CReaderCSV;
class CReaderINI;



// -----------------------------------------------
// -- GLOBAL VARS
// -----------------------------------------------

/**
*	Maximal nodes in a file
*	\todo change it to dynamic array
**/
#define MAX_NODES 0xfff


/**
*	Object tree
**/
#ifdef ANALYSE_NIF
	class	CDebugWindow;
	extern	CDebugWindow* gpc_wnd_debug;
#endif

/**
*	gh_instance is needed for resource access
*	(Debug-Dialog, Texture-not-found are the current used resources)
**/
extern HINSTANCE	gh_instance;

/**
*	the render WND
*	if this is NULL most functions will fail
**/
extern HWND			gh_renderwnd;

/**
*	Rendering Context
**/
extern HGLRC		gh_render_context;

/**
*	Base directory of DAoC
**/
extern char*		gpc_base_directory;


/**
*	Gamedata.mpk CSV
*	Will be loaded into memory for faster access
**/
enum	e_CSV
{
	MONSTERS	= 0,
	MONNIFS		= 1,
	OBJECTS		= 2,
	ITEMS		= 3,
	SKINS		= 4,

	NOF_CVS
};

extern CReaderCSV*	gapc_cvs[NOF_CVS];
extern CReaderINI*	gpc_zones;



// -------------------- TEST SECTION -------------------------
enum	e_SkinPos
{
	BODY = 0,
	HEAD = 1,
	ARMS = 2,
	GLOVES = 3,
	LBODY = 4,
	LEGS = 5,
	BOOTS = 6,
	CLOAK = 7,
	HELM = 8,

	NOF_SKINPOS
};

extern const int		m_number_of_skins;
extern GLuint			man_skins[NOF_SKINPOS];
extern const char*		mstr_skin_pos[];
extern const char*		mstr_names[16][16]; 
// -------------------- TEST SECTION -------------------------


// DLL-Functions
NIFLIB_API void	NIF::Render(object pc_data);



// ---------------- FUNCTIONS -------------------------
void		DLLInit(HINSTANCE);
void		DLLClose();

/**
*	Register all NODE Classes
**/
void		InitNif();


/**
*	
**/
bool		CreateGL(HWND);

/**
*	
**/
void		KillGLWindow();


/**
*	
**/
bool		CreateCReaderCSV(e_CSV, const char* str_mpak_name, const char* str_filename);



/**
*	Creates the NODE-Class
* @param	chunk_name name of the chunk
* @return	the new class or NULL on error
**/
CNode*		CreateNode(const char* chunk_name );


/**
*	Error-Recovering. Searchs the beginning of the next chunk
**/
void		Recover(CNifStream&);


