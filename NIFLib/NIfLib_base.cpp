/**
* NIFLib
* Interface functions
**/
#include "stdafx.h"
#include "niflib_base.h"
#include "TArray.h"

#include "CTextureManager.h"
#include "glh/glh_extensions.h"

#include "CNifStream.h"
#include "CDirList.h"
#include "CReaderCSV.h"
#include "CReaderINI.h"
#include "CDebugWindow.h"

#include "CNiSourceTexture.h"
#include "CNiNode.h"





using namespace NIF;





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
	CDebugWindow* gpc_wnd_debug = NULL;
#endif

/**
*	gh_instance is needed for resource access
*	(Debug-Dialog, Texture-not-found are the current used resources)
**/
HINSTANCE	gh_instance = NULL;

/**
*	the render WND
*	if this is NULL most functions will fail
**/
HWND		gh_renderwnd  = NULL;

/**
*	Rendering Context
**/
HGLRC		gh_render_context = NULL;

/**
*	Base directory of DAoC
**/
char*		gpc_base_directory = NULL;




CReaderCSV*	gapc_cvs[NOF_CVS];
CReaderINI*	gpc_zones;




// -------------------- TEST SECTION -------------------------
/**
*	
**/
const int		m_number_of_skins = NOF_SKINPOS;
GLuint			man_skins[NOF_SKINPOS];
const char*		mstr_skin_pos[]= {"Body","Head","Arms","Gloves","lbody","Legs","Boots","Cloak","Helm"};
const char*		mstr_names[16][16] = 
{
	{	"Body","Body1","Body2","Body3","Body4","Body5","Body1b","Body2b","Body3b","Body4b","Body5b","" },
	{	"Head","Head1","Head2","Head3","Head4","Head5",""},
	{   "Arms","Arms1","Arms2","Arms3","Arms4","Arms5","Arms6",""},
	{	"Gloves1","Gloves2","Gloves3",""},
	{	"LBody1","LBody2","LBody3","Lbody4",""},
	{	"Legs","Legs1","Legs2","Legs3","Legs4",""},
	{	"Boots1","Boots2","Boots3","Boots4","Boots5",""},
	{	"Cloak1","Cloak2","Cloak3",""},
	{	"HeadA1","HeadA2","HeadA3","HeadA4","HeadA5","HeadB1","HeadB2","HeadB3","HeadB4","HeadB5",""},
	{	"Lod1a","Lod1b","Lod1c","Lod1d",""}
};













/**
*	
**/
bool		LoadSkin(int nr, const char* archive, const char* name);
void		ClearSkin();
bool		AssignSkin(NIF::object);




BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
				DLLInit((HINSTANCE)hModule);
				break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
				break;

		case DLL_PROCESS_DETACH:
				DLLClose();
				break;
	}
    return TRUE;
}




void	DLLInit(HINSTANCE hinst)
{
	// -- reset vars -- just to be sure they are 0
	INITLOG();
	gh_instance = hinst;
	gh_renderwnd = NULL;
	gh_render_context = NULL;
	gpc_base_directory = NULL;

	for (int t=0;t<NOF_CVS;t++)
		gapc_cvs[t] = 0;
	gpc_zones = NULL;

	for (int t=0;t<m_number_of_skins;t++)
		man_skins[t] = -1;

	#ifdef ANALYSE_NIF
		// -- create dialog --
		gpc_wnd_debug = new CDebugWindow();
		gpc_wnd_debug->Create();
		gpc_wnd_debug->Show();
	#endif

}



void	DLLClose()
{

	SAFE_DELETE(gpc_zones);
	for (int t=0;t<NOF_CVS;t++)
		SAFE_DELETE(gapc_cvs[t]);


	#ifdef ANALYSE_NIF
		SAFE_DELETE(gpc_wnd_debug);
	#endif

	if (gh_renderwnd)	Release();
}










/*
*	Initialise
**/
NIFLIB_API bool	NIF::Create(HWND wnd)
{
	// Maik Jurkait  02.06.2004 18:33:45
	// ------------------------------------------
	//	Register Classes
	// ------------------------------------------
	InitNif();


	// Maik Jurkait  02.06.2004 18:33:58
	// ------------------------------------------
	//	Create texture manager
	// ------------------------------------------
	ASSERT(gpc_texturemanager == NULL);
	gpc_texturemanager = new CTextureManager;


	// Maik Jurkait  02.06.2004 18:34:19
	// ------------------------------------------
	//	Create GL-View
	// ------------------------------------------
	bool res = CreateGL(wnd);
	if (!res)
	{
		SAFE_DELETE(gpc_texturemanager);
	}


	return res;
}





NIFLIB_API void	NIF::Release()
{
	// not created
	assert(gh_renderwnd);
	if (!gh_renderwnd)	return;

	SAFE_DELETE(gpc_base_directory);

	for (int t=0;t<m_number_of_skins;t++)
		if (man_skins[t] != -1)
		{
			gpc_texturemanager->ReleaseTexture(man_skins[t]);
			man_skins[t] = -1;
		};


	SAFE_DELETE(gpc_texturemanager);

	KillGLWindow();
}










bool CreateGL(HWND wnd)
{

	// Maik Jurkait  02.06.2004 19:42:29
	// ------------------------------------------
	//	Get vars
	// ------------------------------------------
    gh_renderwnd = wnd;
	HDC hDC = GetDC(gh_renderwnd);


	// Maik Jurkait  02.06.2004 19:42:36
	// ------------------------------------------
	//	PixelFormat
	// ------------------------------------------
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};



	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.","OpenGL-ERROR",MB_OK|MB_ICONEXCLAMATION);
		gh_renderwnd = NULL;
		return false;
	}

	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","OpenGL-ERROR",MB_OK|MB_ICONEXCLAMATION);
		gh_renderwnd = NULL;
		return false;
	}

	// Are We Able To Get A Rendering Context?
	if (!(gh_render_context=wglCreateContext(hDC)))
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","OpenGL-ERROR",MB_OK|MB_ICONEXCLAMATION);
		gh_renderwnd = NULL;
		return false;
	}

	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(hDC,gh_render_context))
	{
		KillGLWindow();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","OpenGL-ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}



	// Maik Jurkait  02.06.2004 19:45:55
	// ------------------------------------------
	//	Set Perspective
	// ------------------------------------------
	OnWindowResize();



	// Maik Jurkait  02.06.2004 19:46:07
	// ------------------------------------------
	//	Init extensions
	// ------------------------------------------
	if( !glh_init_extensions(	"GL_ARB_texture_compression GL_ARB_multitexture ") )
	{
		char text[2024];
		sprintf(text,"Necessary extensions were not supported: %s\n", glh_get_unsupported_extensions());
		MessageBox(NULL,text,"OpenGL Extention error",MB_OK);
		return false;
	};


	// Maik Jurkait  02.06.2004 19:46:20
	// ------------------------------------------
	//	Set basic gl-flags
	// ------------------------------------------
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable(GL_TEXTURE_2D);
 	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	return true;		
}



NIFLIB_API void	NIF::OnWindowResize()
{
	if (!gh_renderwnd)	return;


	// Maik Jurkait  02.06.2004 19:18:19
	// ------------------------------------------
	//	Get new size
	// ------------------------------------------
	RECT	r;
	GetClientRect(gh_renderwnd,&r);
	int n_width = r.right - r.left;
	int n_height = r.bottom - r.top;

	// Maik Jurkait  02.06.2004 19:18:29
	// ------------------------------------------
	//	Set Viewport and PROJECTION matrix
	// ------------------------------------------
	glViewport(0,0,n_width,n_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();			


	// Maik Jurkait  02.06.2004 19:19:05
	// ------------------------------------------
	// Set the aspect ratio of the window	
	// ------------------------------------------
//	gluPerspective(45.0f,(GLfloat)r.Width()/(GLfloat)r.Height(),20.0f,20000.0f);
	gluPerspective(45.0f,(GLfloat)n_width/(GLfloat)n_height,20.0f,50000.0f);
//	gluPerspective(45.0f,(GLfloat)r.Width()/(GLfloat)r.Height(),100.0f,50000.0f);

	// Maik Jurkait  02.06.2004 19:19:26
	// ------------------------------------------
	//	Reset MODELVIEW Matrix
	// ------------------------------------------
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

}




void KillGLWindow()						
{
	if (gh_render_context)											
	{
		glh_shutdown_extensions();

		if (!wglMakeCurrent(NULL,NULL))					
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(gh_render_context))						
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

	gh_renderwnd = NULL;
	gh_render_context = NULL;
}





/*
*  RENDER	
**/
NIFLIB_API void	NIF::Render(object pc_data)
{
	// not created
	assert(gh_renderwnd);
	if (!gh_renderwnd)	return;

	if (!pc_data)		return;
	


	// in case somebody else then the gpc_texturemanager changed a texture
	gpc_texturemanager->TextureChanged();

	((CNode*)pc_data)->Render();
}






// ----------------------------------------------------------------------
// - CHUNK-REGISTRATION -------------------------------------------------
// ----------------------------------------------------------------------

// nodes headers
#include "CNiNode.h"
#include "CNiLight.h"
#include "CNiImage.h"
#include "CNiCamera.h"
#include "CNiUVData.h"
#include "CNiGravity.h"
#include "CNiLODNode.h"
#include "CNiVisData.h"
#include "CNiPalette.h"
#include "CNiSkinData.h"
#include "CNiTriShape.h"
#include "CNiSpotLight.h"
#include "CNiTriStrips.h"
#include "CNiFloatData.h"
#include "CNiPixelData.h"
#include "CNiMorphData.h"
#include "CNiColorData.h"
#include "CNiPointLight.h"
#include "CNiKeyframeData.h"
#include "CNiTriShapeData.h"
#include "CNiRawImageData.h"
#include "CNiSkinInstance.h"
#include "CNiAmbientLight.h"
#include "CNiUVController.h"
#include "CNiParticleBomb.h"
#include "CNi3dsMorphShape.h"
#include "CNiSkinPartition.h"
#include "CNiTextureEffect.h"
#include "CNiAlphaProperty.h"
#include "CNiTriStripsData.h"
#include "CNiSourceTexture.h"
#include "CNiVisController.h"
#include "CNiBillboardNode.h"
#include "CNiPathController.h"
#include "CNiRollController.h"
#include "CNiDitherProperty.h"
#include "CNiParticleMeshes.h"
#include "CNiAlphaController.h"
#include "CNiStringExtraData.h"
#include "CNiStencilProperty.h"
#include "CNiTextureProperty.h"
#include "CNiZBufferProperty.h"
#include "CNi3dsAlphaAnimator.h"
#include "CNiSpecularProperty.h"
#include "CNiMaterialProperty.h"
#include "CNiLookAtController.h"
#include "CNiDirectionalLight.h"
#include "CNiParticleRotation.h"
#include "CNiParticleGrowFade.h"
#include "CNi3dsAnimationNode.h"
#include "CNi3dsColorAnimator.h"
#include "CNi3dsPathController.h"
#include "CNi3dsParticleSystem.h"
#include "CNiRotatingParticles.h"
#include "CNiTexturingProperty.h"
#include "CNiSphericalCollider.h"
#include "CNiWireframeProperty.h"
#include "CNiParticleMeshesData.h"
#include "CNiKeyFrameController.h"
#include "CNiKeyframeController.h"
#include "CNiAutoNormalParticles.h"
#include "CNiTextureModeProperty.h"
#include "CNiVertexColorProperty.h"
#include "CNiMultiTextureProperty.h"
#include "CNiParticleMeshModifier.h"
#include "CNiParticleColorModifier.h"
#include "CNiRotatingParticlesData.h"
#include "CNiGeomMorpherController.h"
#include "CNiMaterialColorController.h"
#include "CNiAutoNormalParticlesData.h"
#include "CNiParticleSystemController.h"
#include "CNiPosData.h"
#include "CNiBone.h"
#include "CNiTriShapeSkinController.h"
#include "CNiMorpherController.h"
#include "CNiBezierMesh.h"
#include "CNiTransparentProperty.h"
#include "CNiEnvMappedTriShape.h"
#include "CNiEnvMappedTriShapeData.h"
#include "CNiMorphController.h"
#include "CNiBezierTriangle4.h"
#include "CNiTextKeyExtraData.h"




// Chunk description
#pragma pack(1)
struct	node_constructor
{

	unsigned len;		// lenght of string
	char name[64];		// the name
	CNode*(*create)();	// a function which generates the class 
};
#pragma pack()



/**
*	array of registered chunks
**/
TArray <node_constructor> ma_chunk_description;


/**
*	register helper macro
**/
#define REGISTER_NODE(x,y)	{node_constructor n;					\
								n.len = (unsigned)strlen(x);		\
								strcpy(n.name,x);					\
								n.create = C##y::_CreateClass;		\
								ma_chunk_description.AddLast(n);};




/**
*	Register all Chunks
**/
void InitNif()
{

	// matrix nodes
	REGISTER_NODE( "NiNode", NiNode);
	REGISTER_NODE( "NiLODNode", NiLODNode);
	REGISTER_NODE( "NiBillboardNode", NiBillboardNode);

	// textures
	REGISTER_NODE( "NiImage", NiImage);
	REGISTER_NODE( "NiPalette", NiPalette);
	REGISTER_NODE( "NiSourceTexture", NiSourceTexture);
	REGISTER_NODE( "NiPixelData", NiPixelData);

	// Lights
	REGISTER_NODE( "NiLight", NiLight);
	REGISTER_NODE( "NiSpotLight", NiSpotLight);
	REGISTER_NODE( "NiAmbientLight", NiAmbientLight);
	REGISTER_NODE( "NiDirectionalLight", NiDirectionalLight);

	// Mesh
	REGISTER_NODE( "NiTriShape", NiTriShape);
	REGISTER_NODE( "NiTriStrips", NiTriStrips);
	REGISTER_NODE( "NiTriShapeData", NiTriShapeData);
	REGISTER_NODE( "NiTriStripsData", NiTriStripsData);

	// Extra data
	REGISTER_NODE( "NiFloatData", NiFloatData);


	// Animation
	REGISTER_NODE( "NiUVController", NiUVController);
	REGISTER_NODE( "NiMorphData", NiMorphData);

	// Particles
	REGISTER_NODE( "NiParticleBomb", NiParticleBomb);
	REGISTER_NODE( "NiParticleRotation", NiParticleRotation);
	REGISTER_NODE( "NiParticleGrowFade", NiParticleGrowFade);


	// unsorted
	REGISTER_NODE( "NiCamera", NiCamera);
	REGISTER_NODE( "NiUVData", NiUVData);
	REGISTER_NODE( "NiGravity", NiGravity);
	REGISTER_NODE( "NiVisData", NiVisData);
	REGISTER_NODE( "NiSkinData", NiSkinData);
	REGISTER_NODE( "NiColorData", NiColorData);
	REGISTER_NODE( "NiPointLight", NiPointLight);
	REGISTER_NODE( "NiKeyframeData", NiKeyframeData);
	REGISTER_NODE( "NiRawImageData", NiRawImageData);
	REGISTER_NODE( "NiSkinInstance", NiSkinInstance);
	REGISTER_NODE( "Ni3dsMorphShape", Ni3dsMorphShape);
	REGISTER_NODE( "NiSkinPartition", NiSkinPartition);
	REGISTER_NODE( "NiTextureEffect", NiTextureEffect);
	REGISTER_NODE( "NiAlphaProperty", NiAlphaProperty);
	REGISTER_NODE( "NiVisController", NiVisController);
	REGISTER_NODE( "NiPathController", NiPathController);
	REGISTER_NODE( "NiRollController", NiRollController);
	REGISTER_NODE( "NiDitherProperty", NiDitherProperty);
	REGISTER_NODE( "NiParticleMeshes", NiParticleMeshes);
	REGISTER_NODE( "NiAlphaController", NiAlphaController);
	REGISTER_NODE( "NiStringExtraData", NiStringExtraData);
	REGISTER_NODE( "NiStencilProperty", NiStencilProperty);
	REGISTER_NODE( "NiTextureProperty", NiTextureProperty);
	REGISTER_NODE( "NiZBufferProperty", NiZBufferProperty);
	REGISTER_NODE( "Ni3dsAlphaAnimator", Ni3dsAlphaAnimator);
	REGISTER_NODE( "NiSpecularProperty", NiSpecularProperty);
	REGISTER_NODE( "NiMaterialProperty", NiMaterialProperty);
	REGISTER_NODE( "NiLookAtController", NiLookAtController);
	REGISTER_NODE( "Ni3dsAnimationNode", Ni3dsAnimationNode);
	REGISTER_NODE( "Ni3dsColorAnimator", Ni3dsColorAnimator);
	REGISTER_NODE( "Ni3dsPathController", Ni3dsPathController);
	REGISTER_NODE( "Ni3dsParticleSystem", Ni3dsParticleSystem);
	REGISTER_NODE( "NiRotatingParticles", NiRotatingParticles);
	REGISTER_NODE( "NiTexturingProperty", NiTexturingProperty);
	REGISTER_NODE( "NiSphericalCollider", NiSphericalCollider);
	REGISTER_NODE( "NiWireframeProperty", NiWireframeProperty);
	REGISTER_NODE( "NiParticleMeshesData", NiParticleMeshesData);
	REGISTER_NODE( "NiKeyFrameController", NiKeyFrameController); //  <-writting error +gg+
	REGISTER_NODE( "NiKeyframeController", NiKeyFrameController); //
	REGISTER_NODE( "NiAutoNormalParticles", NiAutoNormalParticles);
	REGISTER_NODE( "NiTextureModeProperty", NiTextureModeProperty);
	REGISTER_NODE( "NiVertexColorProperty", NiVertexColorProperty);
	REGISTER_NODE( "NiMultiTextureProperty", NiMultiTextureProperty);
	REGISTER_NODE( "NiParticleMeshModifier", NiParticleMeshModifier);
	REGISTER_NODE( "NiParticleColorModifier", NiParticleColorModifier);
	REGISTER_NODE( "NiRotatingParticlesData", NiRotatingParticlesData);
	REGISTER_NODE( "NiGeomMorpherController", NiGeomMorpherController);
	REGISTER_NODE( "NiMaterialColorController", NiMaterialColorController);
	REGISTER_NODE( "NiAutoNormalParticlesData", NiAutoNormalParticlesData);
	REGISTER_NODE( "NiParticleSystemController", NiParticleSystemController);
	REGISTER_NODE( "NiPosData", NiPosData);
	REGISTER_NODE( "NiBone", NiBone);
	REGISTER_NODE( "NiTriShapeSkinController", NiTriShapeSkinController);
	REGISTER_NODE( "NiMorpherController", NiMorpherController);
	REGISTER_NODE( "NiBezierMesh", NiBezierMesh);
	REGISTER_NODE( "NiTransparentProperty", NiTransparentProperty);
	REGISTER_NODE( "NiEnvMappedTriShape", NiEnvMappedTriShape);
	REGISTER_NODE( "NiEnvMappedTriShapeData", NiEnvMappedTriShapeData);
	REGISTER_NODE( "NiMorphController", NiMorphController);
	REGISTER_NODE( "NiBezierTriangle4", NiBezierTriangle4);
	REGISTER_NODE( "NiTextKeyExtraData", NiTextKeyExtraData);
	
}



/**
*	Create the correspondending class
**/
CNode*	CreateNode(const char* chunkname)
{
	for (int t=0;t<ma_chunk_description.Count();t++)
	{
		if (strcmp(ma_chunk_description[t].name, chunkname)==0)
		{
			delete chunkname;
			return ma_chunk_description[t].create();
		}
	}

	return NULL;
}



/**
* Searches the next chunk in file
* This should only be used for error recovering
* or analysing.
**/
void	Recover(CNifStream& stream)
{

	// Maik Jurkait  25.05.2004 16:55:29
	// ------------------------------------------
	//	local pointer
	// ------------------------------------------
	char* pointer = stream.GetPosition();
	size_t size = stream.Size();

	do
	{

		// Maik Jurkait  25.05.2004 16:55:36
		// ------------------------------------------
		// Search next "Ni" "En" "To"
		// This	is just a "word" compare and faster then th exact search
		// ------------------------------------------
		while (	*(WORD*)(pointer+4) != 'iN' && 
				*(WORD*)(pointer+4) != 'nE' && 
				*(WORD*)(pointer+4) != 'oT' && size >6)
		{
			pointer++; 
			size--;
		}


		// Maik Jurkait  25.05.2004 16:56:36
		// ------------------------------------------
		//	Check if this is a registered chunk
		// ------------------------------------------
		for (int t=0;t<ma_chunk_description.Count();t++)
		{
			if (	size >= ma_chunk_description[t].len &&
					memcmp(&(ma_chunk_description[t].len),pointer,ma_chunk_description[t].len+4)!=0)
			{
				// FOUND a new chunk
				stream.readSkip((int)(stream.Size()-size));
				return;
			}
		}

		// search more
		pointer++; size--;
	} while (size>0);


	// nothing found
	stream.readSkip((int)(stream.Size()-size));

}






// - DAOC Monster-FUNCTION ---------------------------------------------
bool	CreateCReaderCSV(e_CSV e, const char* str_mpak_name, const char* str_filename)
{
	SAFE_DELETE(gapc_cvs[e]);

	if (!gpc_base_directory) return false;

	gapc_cvs[e] = new CReaderCSV();

	// Maik Jurkait  08.06.2004 10:18:59
	// ------------------------------------------
	//	Open "gamedata.mpk"
	// ------------------------------------------
	CDirList m_gamedata;
	char	path[MAX_PATH];
	strcpy(path,gpc_base_directory);
	strcat(path,str_mpak_name);
	m_gamedata.Open(path);

	// Maik Jurkait  08.06.2004 10:24:20
	// ------------------------------------------
	//	read "monsters.csv"
	// ------------------------------------------
	char*	mem_buffer;
	size_t	mem_size;
	if (!m_gamedata.ReadFile(str_filename,mem_buffer,mem_size))
		return false;

	// Maik Jurkait  08.06.2004 10:24:38
	// ------------------------------------------
	//	Create the parser
	// ------------------------------------------
	gapc_cvs[e]->Open(mem_buffer,mem_size);

	return true;
}



