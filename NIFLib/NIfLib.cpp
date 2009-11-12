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
#include "CDebugNodeUnconnected.h"

#include "CNiSourceTexture.h"
#include "CNiNode.h"





using namespace NIF;




void	Show(object object, int nr, int type);



void	ShowStyle(object pc)
{
	if (pc)
	{
		Show(pc,BODY,1);
		Show(pc,HEAD,1);
		Show(pc,ARMS,1);
		Show(pc,GLOVES,0);
		Show(pc,LBODY,0);
		Show(pc,LEGS,1);
		Show(pc,BOOTS,0);
		Show(pc,CLOAK,3);
		Show(pc,HELM,0);
	}
}




bool		AssignSkin(NIF::object object)
{
	CNode*	pc_root = (CNode*)object;
	bool	b_okay = true;
	bool	b_skinset = false;

	for (int t=0;t<m_number_of_skins;t++)
		if (man_skins[t] != -1)
		{
			b_skinset = true;
			CNode* pc_node = NULL;
			pc_node = pc_root->FindNode(mstr_skin_pos[t],&pc_node,false);
			if (!pc_node)
			{
				TRACE1("NODE NOT FOUND: %s\n",mstr_skin_pos[t]);
				b_okay = false;
				continue;
			}
		
			while (pc_node)
			{
				/// \todo we should search for NiTriShape /NiTriStips and replace/add textures there
				CNiSourceTexture * pc_new_texture = new CNiSourceTexture;
				pc_new_texture->SetTexture(man_skins[t]);

				assert(pc_node->IsClass("NiNode"));
				pc_node->RemoveTexture();
				((CNiNode*)pc_node)->AddProperty(pc_new_texture);

				pc_node = pc_root->FindNode(mstr_skin_pos[t],&pc_node,false);
			};
		}


	// Maik Jurkait  12.06.2004 12:00:38
	// ------------------------------------------
	//	Hide LOD
	// ------------------------------------------
	if (b_skinset)
	{
		CNode* pc_node = NULL;
		pc_node = pc_root->FindNode("lod",&pc_node,false);
		while (pc_node)
		{
			assert(pc_node->IsClass("NiNode"));
			((CNiNode*)pc_node)->SetVisible(false);
			pc_node->RemoveTexture();

			pc_node = pc_root->FindNode("lod",&pc_node,false);
		}
	}

	return true;
}


void		Show(object object, int nr, int type)
{
	CNode*	pc_root = (CNode*)object;

	// Maik Jurkait  14.06.2004 13:18:18
	// ------------------------------------------
	//	Hide all nodes of that type
	// ------------------------------------------
	int t=0;
	CNode* pc_node = NULL;
	while (mstr_names[nr][t][0]!=0)
	{
		pc_node = NULL;
		pc_node = pc_root->FindNode(mstr_names[nr][t],&pc_node,true);
		if (pc_node)
		{
			assert(pc_node->IsClass("NiNode"));
			((CNiNode*)pc_node)->SetVisible(false);
		}
		t++;
	}

	// Maik Jurkait  14.06.2004 13:18:37
	// ------------------------------------------
	//	activate selected node
	// ------------------------------------------
	pc_node = NULL;
	pc_node = pc_root->FindNode(mstr_names[nr][type],&pc_node,true);
	if (pc_node)
	{
		assert(pc_node->IsClass("NiNode"));
		((CNiNode*)pc_node)->SetVisible(true);
	}
	else
	{
		// Maik Jurkait  14.06.2004 13:18:52
		// ------------------------------------------
		//	if not found activate atleast one
		// ------------------------------------------
		int t=0;
		do
		{
			pc_node = NULL;
			pc_node = pc_root->FindNode(mstr_names[nr][t],&pc_node,true);
			t++;
		} while (!pc_node && mstr_names[nr][t][0]!=0);

		if (pc_node)
		{
			assert(pc_node->IsClass("NiNode"));
			((CNiNode*)pc_node)->SetVisible(true);
		}
	}
}




// -------------------- TEST SECTION -------------------------




// ----------------------------------------------------------------------
// - NIF INTERFACE-FUNCTION ---------------------------------------------
// ----------------------------------------------------------------------
NIFLIB_API object	NIF::CreateObject(const char* path, const char* filename)
{
	// call Create first
	assert(gh_renderwnd);
	if (!gh_renderwnd)	return NULL;


	char str_textpath[MAX_PATH];
	char name_npk[MAX_PATH];
	char name_nif[MAX_PATH];

	// create pathes
	{
		// Maik Jurkait  01.06.2004 18:02:43
		// ------------------------------------------
		//	get path only
		// ------------------------------------------
		strcpy(str_textpath,path);
		char *pos = strrchr(str_textpath,'\\');
		if (pos) { *pos++ = '\\'; *pos = 0;}
		pos = strrchr(str_textpath,'\\');
		if (pos) { *pos++ = '\\'; *pos = 0;}


		// Maik Jurkait  16.06.2004 15:22:41
		// ------------------------------------------
		//	truncate filename;
		// ------------------------------------------
		char str_filenameonly[MAX_PATH];
		strcpy(str_filenameonly,filename);
		pos = strrchr(str_filenameonly,'.');
		if (pos) { *pos = 0;}


		// Maik Jurkait  16.06.2004 15:21:34
		// ------------------------------------------
		//	create NPK name
		// ------------------------------------------
		strcpy(name_npk,str_textpath);
		strcat(name_npk,str_filenameonly);
		strcat(name_npk,".npk");


		// Maik Jurkait  16.06.2004 15:21:41
		// ------------------------------------------
		//	creat NIF name
		// ------------------------------------------
		strcpy(name_nif,str_filenameonly);
		strcat(name_nif,".nif");
	}



	// Maik Jurkait  01.06.2004 18:05:29
	// ------------------------------------------
	//	Add texture path
	// ------------------------------------------
	gpc_texturemanager->AddSearchPath(str_textpath);


	// Maik Jurkait  16.06.2004 15:21:03
	// ------------------------------------------
	//	look for the 2 cases
	// ------------------------------------------
	CDirList	dir;
	if (!dir.Open(name_npk))
		dir.Open(str_textpath);


	char*	p_memory = NULL;
	size_t	n_size = 0;
	dir.ReadFile(name_nif,p_memory, n_size);




	// Maik Jurkait  01.06.2004 18:05:47
	// ------------------------------------------
	// Create object
	// ------------------------------------------
	object pc = NULL;
	
	if (p_memory)
	{
		pc = CreateObjectFromMemory(p_memory, n_size);
		delete (p_memory);
	}

	
	// Maik Jurkait  12.06.2004 10:39:54
	// ------------------------------------------
	//	Apply skin
	// ------------------------------------------
	AssignSkin(pc);



	// Maik Jurkait  01.06.2004 18:39:57
	// ------------------------------------------
	//	Create
	// ------------------------------------------
	((CNode*)pc)->Create();


	// Maik Jurkait  30.06.2004 10:09:40
	// ------------------------------------------
	//	PostCreate 
	// ------------------------------------------
	((CNode*)pc)->UpdateStatic();

    
	// Maik Jurkait  02.06.2004 12:39:05
	// ------------------------------------------
	//	Debug Stuff
	// ------------------------------------------
	#ifdef ANALYSE_NIF
		if (gpc_wnd_debug)
		{
			gpc_wnd_debug->TreeClear();
			gpc_wnd_debug->TreeBuild((CNode*)pc);
		}
	#endif



	// Maik Jurkait  01.06.2004 18:06:22
	// ------------------------------------------
	//	Remove search path
	// ------------------------------------------
	gpc_texturemanager->RemoveSearchPath(str_textpath);



	return pc;
}




/**
*	CREATING
**/
object	CreateObjectFromMemory(void* p_memory, size_t n_size)
{
	// call Create first
	assert(gh_renderwnd);



	// Maik Jurkait  24.05.2004 17:08:41
	// ------------------------------------------
	// Init the NifStream
	// ------------------------------------------
	assert(p_memory);// "no memory given"
	if (!p_memory) return NULL;

	CNifStream nif_stream((char*)p_memory,n_size);
	if (nif_stream.Version()==0) return NULL;





	// Maik Jurkait  24.05.2004 18:52:27
	// ------------------------------------------
	//	Read chunks
	// ------------------------------------------
	int		n_nof_nodes = 0;
	CNode*	ap_nodes[MAX_NODES];
	int		n_base_node = 0;

	while (nif_stream.Size()>0)
	{
		// Maik Jurkait  24.05.2004 18:56:50
		// ------------------------------------------
		//	Get chunk name
		// ------------------------------------------
		char *chunk_name = nif_stream.readstring(30);

		assert(chunk_name != NULL);
		if (chunk_name == NULL )
		{
			assert(nif_stream.Size()<=6);// no chunk name
			// to prevent another assert
			nif_stream.readSkip(nif_stream.Size());
			break;
		}



		// Maik Jurkait  24.05.2004 18:56:56
		// ------------------------------------------
		//	Special chunks
		// ------------------------------------------
		// V4 got no endmark
		if (chunk_name[0]==0)
		{
			assert(nif_stream.Size()<=6);// empty chunk name but data-in stream
			// to prevent another assert
			nif_stream.readSkip(nif_stream.Size());
			delete [] chunk_name;
			break;
		}

		// ignore this "Top Level Object"
		if (strcmp(chunk_name,"Top Level Object")==0)
		{
			// set next node as base
			n_base_node = n_nof_nodes;
			delete [] chunk_name;
			continue;
		}

		// "End Of File"
		if (strcmp(chunk_name,"End Of File")==0)
		{
			delete [] chunk_name;
			break;
		}

		
		// Maik Jurkait  24.05.2004 18:57:05
		// ------------------------------------------
		//	Create Node
		// ------------------------------------------
		CNode* pc_node = CreateNode(chunk_name);
		assert(pc_node != NULL);
		if (!pc_node) 
		{
			TRACE1("unknown node: %s\n",chunk_name);
			Recover(nif_stream); 
			continue;
		}

		// add and load
		ap_nodes[n_nof_nodes] = pc_node;
		bool r = pc_node->Load(nif_stream,n_nof_nodes++);

		// ERROR
		assert(r);
		if (!r)			{ Recover(nif_stream); continue;}

		assert(n_nof_nodes<MAX_NODES);

	}


	assert(n_base_node<n_nof_nodes);// don't knwo which is the base node
	assert(nif_stream.Size()==0);	// all chunks readed but still data in stream



	// Maik Jurkait  25.05.2004 17:15:28
	// ------------------------------------------
	//	Build the hierachy
	// ------------------------------------------
	for (int t=0;t<n_nof_nodes;t++)
	{
		ap_nodes[t]->CreateHierachy(ap_nodes,n_nof_nodes);
	}



	// Maik Jurkait  25.05.2004 17:39:02
	// ------------------------------------------
	//	Check un-referenzed nodes
	// ------------------------------------------
	// we have to collect unconnected chunks first - else we would run into problems with chunks with childs
	TArray<CNode*>	nodes_to_delete;

	for (int t=0;t<n_nof_nodes;t++)
	{
		if (t == n_base_node) continue;

		if (ap_nodes[t]->mn_referenzes == 0)
		{
			TRACE2("NOT CONNECTED: (%x) %s\n", ap_nodes[t]->GetID(), ap_nodes[t]->GetClassName());
			nodes_to_delete.AddLast(ap_nodes[t]);
		}
	}

	//	now move all unconnected to a dummy node
	if (nodes_to_delete.Count()>0)
	{
		CDebugNodeUnconnected * pc_dbg_node = new CDebugNodeUnconnected();

		for (int t=0;t<nodes_to_delete.Count();t++)
			pc_dbg_node->AddNode(nodes_to_delete[t]);

		// now connect it
		CNiNode* pc_new_base = new CNiNode();
		pc_new_base->SetName("ERROR-NODE(caused by unconneted)");
		pc_new_base->AddChild(pc_dbg_node);
		pc_new_base->AddChild(ap_nodes[n_base_node]);

		assert(n_nof_nodes+1<MAX_NODES);
		ap_nodes[n_nof_nodes] = pc_new_base;
		n_base_node = n_nof_nodes;
	}



	// some chunk were not connected
	assert(nodes_to_delete.Count()==0);



	return ap_nodes[n_base_node];
}





/*
*	RELEASE
**/
NIFLIB_API void	NIF::ReleaseObject(object pc_data)
{
	#ifdef ANALYSE_NIF
		if (gpc_wnd_debug)	gpc_wnd_debug->TreeClear();
	#endif

	// not created
	assert(gh_renderwnd);

	delete (CNode*)pc_data;
}










NIFLIB_API bool	NIF::GetBoundingBox(object pc_data, float& x1, float& y1, float& z1, float& x2, float& y2, float& z2)
{
	x1=y1=z1=x2=y2=z2=0.0f;

	if (pc_data==NULL) return false;

	CVector min,max;
	bool res = ((CNode*)pc_data)->GetBoundingbox(min,max);
	
	x1 = min.GetX();
	y1 = min.GetY();
	z1 = min.GetZ();

	x2 = max.GetX();
	y2 = max.GetY();
	z2 = max.GetZ();

	return res;
}






bool		LoadSkin(int nr, const char* archive, const char* name)
{
	if (man_skins[nr] != -1)
		gpc_texturemanager->ReleaseTexture(man_skins[nr]);

	/// \todo add texturemanager: filenotfound flag

	gpc_texturemanager->AddSearchPath(archive);
	man_skins[nr] = gpc_texturemanager->GetTexture(name);
	gpc_texturemanager->RemoveSearchPath(archive);

	return true;
}


void		ClearSkin()
{
	for (int t=0;t<m_number_of_skins;t++)
	{
		if (man_skins[t] != -1)
			gpc_texturemanager->ReleaseTexture(man_skins[t]);

		man_skins[t] = -1;
	}
}






// ----------------------------------------------------------------------
// - DAOC INTERFACE-FUNCTION ---------------------------------------------
// ----------------------------------------------------------------------

NIFLIB_API void	NIF::SetRootDirectory(const char* path)
{
	char temp[MAX_PATH];

	// Maik Jurkait  04.06.2004 18:16:04
	// ------------------------------------------
	//	Remove old TextureSearch Path
	// ------------------------------------------
	if (gpc_base_directory)
	{
		strcpy(temp, gpc_base_directory);
		strcat(temp, "zones\\nifs\\");
		gpc_texturemanager->RemoveSearchPath(gpc_base_directory);

		delete gpc_base_directory;
	}

	
	// Maik Jurkait  04.06.2004 18:17:29
	// ------------------------------------------
	//	copy path
	// ------------------------------------------
	gpc_base_directory = new char[strlen(path)+2];
	strcpy(gpc_base_directory,path);

	if (gpc_base_directory[strlen(gpc_base_directory)-1]!='\\' &&
		gpc_base_directory[strlen(gpc_base_directory)-1]!='/')
			strcat(gpc_base_directory,"/");


	// Maik Jurkait  04.06.2004 18:19:00
	// ------------------------------------------
	//	Add search path
	// ------------------------------------------
	strcpy(temp, gpc_base_directory);
	strcat(temp, "zones\\nifs\\");
	gpc_texturemanager->AddSearchPath(temp);


	// Maik Jurkait  08.06.2004 12:36:20
	// ------------------------------------------
	//	Open the CSV
	// ------------------------------------------
	CreateCReaderCSV(MONSTERS,	"gamedata.mpk","monsters.csv");
	CreateCReaderCSV(MONNIFS,	"gamedata.mpk","monnifs.csv");
	CreateCReaderCSV(OBJECTS,	"gamedata.mpk","objects.csv");
	CreateCReaderCSV(ITEMS,		"gamedata.mpk","items.csv");
	CreateCReaderCSV(SKINS,		"gamedata.mpk","skins.csv");


	
	// Maik Jurkait  16.06.2004 09:38:56
	// ------------------------------------------
	//	Init reader test
	// ------------------------------------------
	{
		CDirList m_gamedata;
		char	path[MAX_PATH];
		strcpy(path,gpc_base_directory);
		strcat(path,"zones\\zones.mpk");
		m_gamedata.Open(path);

		char*	mem_buffer;
		size_t	mem_size;
		m_gamedata.ReadFile("zones.dat",mem_buffer,mem_size);

		gpc_zones = new CReaderINI(mem_buffer,mem_size);
	}
}






// - DAOC Monster-FUNCTION ---------------------------------------------

NIFLIB_API int		NIF::DAoC_GetMonsterIDs(int* p_target_buffer, int target_buffer_size)
{
	if (gapc_cvs[MONSTERS] == NULL) return 0;


	// Maik Jurkait  08.06.2004 10:26:31
	// ------------------------------------------
	//	if nothing given just count the line
	// ------------------------------------------
	if (p_target_buffer == NULL || target_buffer_size == 0)
	{
		if (!gapc_cvs[MONSTERS]->GotoFirstLine()) return 0;
		int counter=0;
		do
		{
			// filter empty lines
			if (gapc_cvs[MONSTERS]->GetInt(2)!=0) counter++;
		} while (gapc_cvs[MONSTERS]->GotoNextLine());
		return counter;
	}


	// Maik Jurkait  08.06.2004 10:48:41
	// ------------------------------------------
	//	
	// ------------------------------------------
	memset(p_target_buffer,0, target_buffer_size*sizeof(int));


	// Maik Jurkait  08.06.2004 10:39:55
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[MONSTERS]->GotoFirstLine()) return 0;

	int counter = 0;
	do
	{
		// filter empty lines
		if (gapc_cvs[MONSTERS]->GetInt(2)!=0)
			p_target_buffer[counter++] = gapc_cvs[MONSTERS]->GetInt(0);
	}
	while (counter < target_buffer_size && gapc_cvs[MONSTERS]->GotoNextLine());
	

	return counter;
}




NIFLIB_API bool		NIF::DAoC_GetMonsterName(int ID, char* pc_buffer, int size)
{
	if (gapc_cvs[MONSTERS] == NULL) return 0;

	assert(pc_buffer);
	*pc_buffer = 0;

	
	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[MONSTERS]->GotoIndexLine(ID)) return false;

	// Maik Jurkait  08.06.2004 10:47:57
	// ------------------------------------------
	//	copy string
	// ------------------------------------------
	char* str = gapc_cvs[MONSTERS]->GetString(1);
	strncpy(pc_buffer,str,min((int)strlen(str)+1,size));

	if (size-1<(int)strlen(str))
	{
		pc_buffer[size-1]=0;
		return false;
	}

	return true;
}

NIFLIB_API bool		NIF::DAoC_GetMonsterExtraInfo(int ID, char* pc_buffer, int size)
{
	if (gapc_cvs[MONSTERS] == NULL) return 0;

	assert(pc_buffer);
	*pc_buffer = 0;

	
	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[MONSTERS]->GotoIndexLine(ID)) return false;


	// Maik Jurkait  16.06.2004 14:18:20
	// ------------------------------------------
	//	no special info of monsters atm
	// ------------------------------------------
	return true;
}





NIFLIB_API object	NIF::DAoC_LoadMonster(int ID)
{
	// Maik Jurkait  08.06.2004 12:59:26
	// ------------------------------------------
	//	Base checkings
	// ------------------------------------------
	if (!gpc_base_directory) return NULL;
	if (gapc_cvs[MONSTERS] == NULL) return NULL;
	if (gapc_cvs[MONNIFS] == NULL) return NULL;


	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	Get NIF-ID
	// ------------------------------------------
	if (!gapc_cvs[MONSTERS]->GotoIndexLine(ID)) return NULL;
	int nif_ID = gapc_cvs[MONSTERS]->GetInt(2);


	// Maik Jurkait  12.06.2004 10:27:57
	// ------------------------------------------
	//	Setup Skin
	// ------------------------------------------
	ClearSkin();
	if (gapc_cvs[SKINS] != NULL)
	{
		for (int t=0;t<m_number_of_skins;t++)
		{
			int skin_id = gapc_cvs[MONSTERS]->GetInt(3+t);
			if (skin_id!=0)
			{
				if (!gapc_cvs[SKINS]->GotoIndexLine(skin_id)) continue;

				char path[MAX_PATH];
				sprintf(path,"%sfigures\\skins\\skin%03i.mpk",gpc_base_directory, gapc_cvs[SKINS]->GetInt(4));

				LoadSkin(t,path,gapc_cvs[SKINS]->GetString(2));
			}
		}
	}



	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	Get NIF-Name
	// ------------------------------------------
	if (!gapc_cvs[MONNIFS]->GotoIndexLine(nif_ID)) return NULL;






	char	path[MAX_PATH];
	strcpy(path,gpc_base_directory);
	strcat(path,"figures\\");

	char	filename[MAX_PATH];
	strcpy(filename,gapc_cvs[MONNIFS]->GetString(2));
	strcat(filename,".nif");

	object pc = NIF::CreateObject(path,filename);

	ShowStyle(pc);
	

	return pc;
}







NIFLIB_API int		NIF::DAoC_GetItemIDs(int* p_target_buffer, int target_buffer_size)
{
	if (gapc_cvs[OBJECTS] == NULL) return 0;

	// Maik Jurkait  08.06.2004 10:26:31
	// ------------------------------------------
	//	if nothing given just count the line
	// ------------------------------------------
	if (p_target_buffer == NULL || target_buffer_size == 0)
		return gapc_cvs[OBJECTS]->GetLineCount();


	// Maik Jurkait  08.06.2004 10:48:41
	// ------------------------------------------
	//	safty clear
	// ------------------------------------------
	memset(p_target_buffer,0, target_buffer_size*sizeof(int));



	// Maik Jurkait  08.06.2004 10:39:55
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[OBJECTS]->GotoFirstLine()) return 0;

	int counter = 0;
	do
	{
		p_target_buffer[counter++] = gapc_cvs[OBJECTS]->GetInt(0);
	}
	while (counter < target_buffer_size && gapc_cvs[OBJECTS]->GotoNextLine());
	

	return counter;
}



NIFLIB_API bool		NIF::DAoC_GetItemName(int ID, char*pc_buffer, int size)
{
	if (gapc_cvs[OBJECTS] == NULL) return 0;

	assert(pc_buffer);
	*pc_buffer = 0;

	
	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[OBJECTS]->GotoIndexLine(ID)) return false;

	// Maik Jurkait  08.06.2004 10:47:57
	// ------------------------------------------
	//	copy string
	// ------------------------------------------
	char* str = gapc_cvs[OBJECTS]->GetString(1);
	strncpy(pc_buffer,str,min((int)strlen(str)+1,size));

	if (size-1<(int)strlen(str))
	{
		pc_buffer[size-1]=0;
		return false;
	}

	return true;
}


NIFLIB_API bool		NIF::DAoC_GetItemExtraInfo(int ID, char* pc_buffer, int size)
{
	if (gapc_cvs[OBJECTS] == NULL) return 0;

	assert(pc_buffer);
	*pc_buffer = 0;

	
	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	
	// ------------------------------------------
	if (!gapc_cvs[OBJECTS]->GotoIndexLine(ID)) return false;



	// Maik Jurkait  08.06.2004 10:47:57
	// ------------------------------------------
	//	Housing?
	// ------------------------------------------
	int	n_housing_id = gapc_cvs[OBJECTS]->GetInt(36);

	if (n_housing_id>0 && size>(int)strlen(pc_buffer)+7)
	{
		strcat(pc_buffer,"Housing");
	}


	return true;
}


NIFLIB_API object	NIF::DAoC_LoadItem(int ID)
{
	bool b_housing_dir = false;


	// Maik Jurkait  08.06.2004 12:59:26
	// ------------------------------------------
	//	Base checkings
	// ------------------------------------------
	if (!gpc_base_directory) return NULL;
	if (gapc_cvs[OBJECTS] == NULL) return NULL;
	if (gapc_cvs[ITEMS] == NULL) return NULL;


	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	Get NIF-ID
	// ------------------------------------------
	if (!gapc_cvs[OBJECTS]->GotoIndexLine(ID)) return NULL;
	int nif_ID		 = gapc_cvs[OBJECTS]->GetInt(2);
	int	n_housing_id = gapc_cvs[OBJECTS]->GetInt(36);


	// Maik Jurkait  16.06.2004 13:33:19
	// ------------------------------------------
	//	Housing item
	// ------------------------------------------
	if (n_housing_id != 0)
	{
		nif_ID = n_housing_id;
		b_housing_dir = true;
	}



	// Maik Jurkait  08.06.2004 10:47:08
	// ------------------------------------------
	//	Get NIF-Name
	// ------------------------------------------
	if (!gapc_cvs[ITEMS]->GotoIndexLine(nif_ID)) return NULL;

	

	// Maik Jurkait  12.06.2004 10:36:46
	// ------------------------------------------
	//	Skin
	// ------------------------------------------
	ClearSkin();



	// Maik Jurkait  12.06.2004 10:37:15
	// ------------------------------------------
	//	Load
	// ------------------------------------------
	char	path[MAX_PATH];
	strcpy(path,gpc_base_directory);

	if (b_housing_dir)
		strcat(path,"phousing\\nifs\\");
	else
		strcat(path,"items\\");


	char	filename[MAX_PATH];
	strcpy(filename,gapc_cvs[ITEMS]->GetString(2));
	strcat(filename,".nif");

	return NIF::CreateObject(path,filename);
}






