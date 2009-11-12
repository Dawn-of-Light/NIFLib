/**
* NIFLib
* Interface functions
**/
#include "stdafx.h"
#include <math.h>
#include "TArray.h"
#include "CReaderCSV.h"
#include "CReaderINI.h"
#include "CDebugWindow.h"
#include "CDirList.h"
#include "NIFLib_base.h"
#include "NIFLib_zones.h"
#include "CTextureManager.h"

#include "CDebugProgress.h"
#include "CDebugNode.h"

#include "CNiNode.h"
#include "CNiTriShapeData.h"
#include "CNiTriShape.h"
#include "CNiTexturingProperty.h"
#include "CNiSourceTexture.h"


const float PI =  3.141592654f;
const float RADDEG = PI/180.0f;


using namespace NIF;



s_zoneinformation::s_zoneinformation()
{
		// set default value
		r_expansion = 0;
		r_phousing = 0;
		r_proxy_region = 0;
		r_entry_music = 0;	
		r_grass_defs = 0;
		r_grasscsv = NULL;
		r_grassmap = NULL;
		r_grasscsv1 = NULL;
		r_grassmap1 = NULL;
		r_GRASSDENSITY = 0;
		r_skydome  = NULL;
		r_detailmap  = NULL;
		z_enabled = 1;

		z_id = 0;
		z_type = 0;
		z_region = 0;
		z_proxy_zone = -1;
		z_name = NULL;
		z_region_offset_x = 0;
		z_region_offset_y = 0;
		z_width = 0;
		z_height = 0;
		z_entry_music = 0;
		z_skydome = NULL;
		z_override_time = 0;
		z_temperature = 0;
		z_frontier = 0;
}

s_zoneinformation::~s_zoneinformation()
{
		SAFE_DELETE(r_grasscsv);
		SAFE_DELETE(r_grassmap);
		SAFE_DELETE(r_grasscsv1);
		SAFE_DELETE(r_grassmap1);
		SAFE_DELETE(r_skydome);
		SAFE_DELETE(r_detailmap);
		SAFE_DELETE(z_name);
		SAFE_DELETE(z_skydome);
}



struct s_nif_info
{
	int		id;
	object	obj;
	int		used;
};





NIFLIB_API void	NIF::DAoC_LoadAllZones()
{
	int* ppn;
	int size = NIF::DAoC_GetZoneIDs(0,0);
	ppn = new int[size];
	NIF::DAoC_GetZoneIDs(ppn,size);

	for (int t1=0;t1<size-1;t1++)
		for (int t2=t1+1;t2<size;t2++)
			if (ppn[t1]>ppn[t2])
			{
				int d = ppn[t1];
				ppn[t1] = ppn[t2];
				ppn[t2] = d;
			}

	CDebugProgress prog;
	prog.Create(size);

	for (int t=0;t<size;t++)
	{
		prog.Step();
		object ob = NIF::DAoC_LoadZone(ppn[t]);
		NIF::ReleaseObject(ob);
	}
}


//#define EXPORT_DOORS

// don't know why but "LoadNIF" waste's more time on dungeons or cities.
// the differenzes to NIF::CreateObject:
//  - no texture addsearchpath (adding a searchpath is alway a directory scan)
//  - no create call (since create needs textures)
//  - no updatestatic call (terrain load's does it anyway)
#define USE_LOADNIF
NIF::object	LoadNIF(const char* path, const char* filename)
{
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





	// Maik Jurkait  16.06.2004 15:21:03
	// ------------------------------------------
	//	look for the 2 cases
	// ------------------------------------------
	CDirList	dir;
	if (!dir.Open(name_npk))
		dir.Open(str_textpath,true);


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


	return pc;
}




NIFLIB_API int		NIF::DAoC_GetZoneIDs(int* ppn, int size)
{
	assert(gpc_zones); // zones.dat not loaded

	// Maik Jurkait  16.06.2004 14:43:24
	// ------------------------------------------
	//	Get all section names
	// ------------------------------------------
	TArray<char*> names;
	gpc_zones->GetSectionList(names);


	// Maik Jurkait  16.06.2004 14:43:31
	// ------------------------------------------
	//	count 'zone' sections
	// ------------------------------------------
	int counter = 0;
	for (int t=0;t<names.Count();t++)
	{
		if (strnicmp(names[t],"zone",4)==0)
		{
			int id = atoi(&names[t][4]);
			if (size==0) counter++;
			if (size>counter)
			{
				ppn[counter++]=id;
			}
		}

		delete (names[t]);
	}

	return counter;
}






NIFLIB_API bool		NIF::DAoC_GetZoneName(int ID, char*name, int size)
{
	assert(gpc_zones); // zones.dat not loaded

	// Maik Jurkait  16.06.2004 14:43:48
	// ------------------------------------------
	//	Get name
	// ------------------------------------------
	char temp[64];
	sprintf(temp,"zone%03i",ID);

	char* res = gpc_zones->GetEntryString(temp,"name","unknown");
	strncpy(name,res,size-1);
	name[size-1]=0;
	delete (res);

	if (stricmp(name,"unknown")==0) return false;


	// Maik Jurkait  16.06.2004 14:43:55
	// ------------------------------------------
	//	Check if clone
	// ------------------------------------------
	int proxy = gpc_zones->GetEntryIntThisSection("proxy_zone",-1);
	if (proxy != -1)
	{
		if (size>(int)strlen(name)+8)
			strcat(name,"(clone)");
	}


	return true;
}






NIFLIB_API bool		NIF::DAoC_GetZoneExtraInfo(int ID, char*pc_buffer, int size)
{
	assert(gpc_zones); // zones.dat not loaded
	*pc_buffer = 0;


	// Maik Jurkait  16.06.2004 14:44:07
	// ------------------------------------------
	//	Get zone type
	// ------------------------------------------
	char temp[128];
	sprintf(temp,"zone%03i",ID);

	if (size >= (int)strlen(pc_buffer)+11) 
	{
		switch (gpc_zones->GetEntryInt(temp,"type",0))
		{
			case 0: strcat(pc_buffer,"terrain"); break;
			case 1: strcat(pc_buffer,"city"); break;
			case 2: strcat(pc_buffer,"dungeon"); break;
			case 3: strcat(pc_buffer,"housing"); break;
			case 4: strcat(pc_buffer,"sky city"); break;
		}
	}

	if (gpc_zones->GetEntryIntThisSection("frontiers",0)==1)
		strcat(pc_buffer,", frontier");

	return true;
}






bool	LoadPCX(CDirList* dir , const char* filename, BYTE*& pc_data, int& size_x, int& size_y)
{
	bool res;

	// read file
	char *buffer;
	size_t size;

	res = dir->ReadFile(filename,buffer,size);
	ASSERT(res);
	if (!res) return false;

	// convert
#pragma pack(1)
	struct s_head
	{
		BYTE	Manufacturer;	//  Constant Flag, 10 = ZSoft .pcx 
		BYTE	Version;		//  Version information 
								// 0 = Version 2.5 of PC Paintbrush 
								// 2 = Version 2.8 w/palette information 
								// 3 = Version 2.8 w/o palette information 
								// 4 = PC Paintbrush for Windows(Plus for Windows uses Ver 5) 
								// 5 = Version 3.0 
		BYTE	Encoding;		// 1 = .PCX run length encoding 
		BYTE	BitsPerPixel;	//  Number of bits to represent a pixel (per Plane) - 1, 2, 4, or 8 
		WORD	Xmin,Ymin,Xmax,Ymax; //  Image Dimensions: Xmin,Ymin,Xmax,Ymax 
		WORD	HDpi;			//  Horizontal Resolution of image in DPI* 
		WORD	VDpi;			//  Vertical Resolution of image in DPI* 
		BYTE	Colormap[48];	//  Color palette setting
		BYTE    Reserved;		//  Should be set to 0. 
		BYTE    NPlanes;		//  Number of color planes 
		WORD    BytesPerLine;	// Number of bytes to allocate for a scanline plane.  MUST be an EVEN number.  Do NOT calculate from Xmax-Xmin. 
		WORD	PaletteInfo;	// How to interpret palette- 1 = Color/BW,  2 = Grayscale (ignored in PB IV/ IV +) 
		WORD	HscreenSize;	//  Horizontal screen size in pixels. New field found only in PB IV/IV Plus 
		WORD	VscreenSize;	// Vertical screen size in pixels. New field found only in PB IV/IV Plus 
		BYTE    Filler[54];		// Blank to fill out 128 byte header.  Set all bytes to 0 
	};
#pragma pack()

	s_head* head = (s_head*)buffer;
	unsigned char* source = (unsigned char*)(buffer+sizeof(s_head));

	if (head->Manufacturer != 10)	{ TRACE("not a pcx\n");delete [] buffer;	return false;}
	if (head->Version != 5)			{ TRACE("wrong version\n");delete [] buffer;	return false;}
	if (head->BitsPerPixel != 8)	{ TRACE("wrong colordepth\n");delete [] buffer;	return false;}
	if (head->Encoding != 1)		{ TRACE("not encoded\n");delete [] buffer;	return false;}

	size_x = head->Xmax - head->Xmin + 1;
	size_y = head->Ymax - head->Ymin + 1;
	BYTE* dest = new BYTE[size_x * size_y];
	pc_data = dest;
	char* palette = buffer+size-766;

	int max = size_x * size_y;
	while (max)
	{
		if ((*source & 0xc0) == 0xc0)
		{
			int len = min(max,*source & 0x3F);
			source++;
			memset(dest,palette[*source*3],len);
			source++;
			dest+=len;
			max -=len;
		}
		else
		{
			*dest++ = palette[*source*3];
			source++;
			max--;
		}
	}
	ASSERT((char*)source-buffer<=(int)size);

	delete [] buffer;


	return true;
}





object	LoadCity(const s_zoneinformation& zinfo)
{
	#ifdef EXPORT_DOORS
	return NULL;
	#endif

	// Maik Jurkait  16.06.2004 15:13:13
	// ------------------------------------------
	//	generate NIF pathes
	// ------------------------------------------
	char nif_path_local[MAX_PATH];
	sprintf(nif_path_local,"%szones\\zone%03i\\nifs\\",gpc_base_directory,zinfo.z_data_id);



	// Maik Jurkait  16.06.2004 14:59:00
	// ------------------------------------------
	//	Open datXXX dir
	// ------------------------------------------
	char path[MAX_PATH];
	sprintf(path,"%szones\\zone%03i\\dat%03i.mpk",gpc_base_directory,zinfo.z_data_id,zinfo.z_data_id);

	CDirList datdir;
	if (!datdir.Open(path)) return NULL;


	// Maik Jurkait  16.06.2004 15:03:49
	// ------------------------------------------
	//	Open city.dat
	// ------------------------------------------
	char*	mbuffer;
	size_t	msize;
	if (!datdir.ReadFile("city.csv",mbuffer,msize)) return NULL;

	CReaderCSV citydat(mbuffer,msize);



	// Maik Jurkait  30.06.2004 10:44:55
	// ------------------------------------------
	//	Progress window;
	// ------------------------------------------
	CDebugProgress	progress;
	progress.Create(citydat.GetLineCount()+3);


	// Maik Jurkait  16.06.2004 15:05:15
	// ------------------------------------------
	//	skip first line 
	// ------------------------------------------
	citydat.GotoFirstLine();
	if (!citydat.GotoNextLine()) return NULL;


	// Maik Jurkait  16.06.2004 15:07:12
	// ------------------------------------------
	//	Create scene root
	// ------------------------------------------
	CNiNode* pc_root = new CNiNode();
	pc_root->SetName("City root");


	#ifdef ANALYSE_NIF
		// disable debug tree
		// (else it would be update by every object)
		CDebugWindow* pc_safed = gpc_wnd_debug;
		gpc_wnd_debug = NULL;
	#endif


	// Maik Jurkait  16.06.2004 15:06:49
	// ------------------------------------------
	//	read all nifs
	// ------------------------------------------
	do
	{
		object pc_nif;

		// Try load here
		TRACE1("Loading: %s\n",citydat.GetString(1));

		progress.Step(citydat.GetString(1));

#ifdef USE_LOADNIF
		pc_nif = LoadNIF(nif_path_local,citydat.GetString(1));
#else
		pc_nif = NIF::CreateObject(nif_path_local,citydat.GetString(1));
#endif
		assert(pc_nif);
		if (pc_nif == NULL) continue;
		((CNiNode*)pc_root)->SetName(citydat.GetString(1));

		pc_root->AddChild((CNode*)pc_nif);

	} while (citydat.GotoNextLine());




	// Maik Jurkait  01.06.2004 18:05:29
	// ------------------------------------------
	//	Create all
	// ------------------------------------------
#ifdef USE_LOADNIF
	progress.Step("creating");
	gpc_texturemanager->AddSearchPath(nif_path_local);
	pc_root->Create();
	gpc_texturemanager->RemoveSearchPath(nif_path_local);
#endif


	// Maik Jurkait  13.07.2004 09:52:24
	// ------------------------------------------
	//	Debug view
	// ------------------------------------------
	progress.Step();
	#ifdef ANALYSE_NIF
		progress.SetText("update debug-view");
		// re-enabel debug view
		gpc_wnd_debug = pc_safed;
		if (gpc_wnd_debug)
		{
			gpc_wnd_debug->TreeClear();
		//	gpc_wnd_debug->TreeBuild(pc_root);
		}
	#endif


	// Maik Jurkait  30.06.2004 10:20:58
	// ------------------------------------------
	//	optimize
	// ------------------------------------------
	progress.Step("optimize");
	pc_root->UpdateStatic();


	return pc_root;
}





object	LoadDungeon(const s_zoneinformation& zinfo)
{
	#ifdef EXPORT_DOORS
	return NULL;
	#endif

	// Maik Jurkait  16.06.2004 14:59:00
	// ------------------------------------------
	//	create NIF path
	// ------------------------------------------
	char nifpath[MAX_PATH];
	sprintf(nifpath,"%szones\\Dnifs\\",gpc_base_directory,zinfo.z_data_id,zinfo.z_data_id);



	// Maik Jurkait  16.06.2004 14:59:00
	// ------------------------------------------
	//	Open datXXX dir
	// ------------------------------------------
	char path[MAX_PATH];
	sprintf(path,"%szones\\zone%03i\\dat%03i.mpk",gpc_base_directory,zinfo.z_data_id,zinfo.z_data_id);

	CDirList datdir;
	if (!datdir.Open(path)) return NULL;


	// Maik Jurkait  16.06.2004 15:03:49
	// ------------------------------------------
	//	Open dungeon.chunk
	// ------------------------------------------
	char*	mbuffer;
	size_t	msize;
	if (!datdir.ReadFile("dungeon.chunk",mbuffer,msize)) return NULL;

	CReaderCSV nifs(mbuffer,msize,true);

	// Maik Jurkait  13.07.2004 07:57:02
	// ------------------------------------------
	//	NIFProxy
	// ------------------------------------------
	datdir.ReadFile("NIFPROXY.csv",mbuffer,msize);
	CReaderCSV nifproxy(mbuffer,msize,true);


	// Maik Jurkait  16.06.2004 16:37:09
	// ------------------------------------------
	//	Open dungeon.place
	// ------------------------------------------
	if (!datdir.ReadFile("dungeon.place",mbuffer,msize)) return NULL;

	CReaderCSV places(mbuffer,msize);


	// Maik Jurkait  30.06.2004 10:44:55
	// ------------------------------------------
	//	Progress window;
	// ------------------------------------------
	CDebugProgress	progress;
	progress.Create(nifs.GetLineCount()+4);


	#ifdef ANALYSE_NIF
		// disable debug tree
		// (else it would be update by every object)
		CDebugWindow* pc_safed = gpc_wnd_debug;
		gpc_wnd_debug = NULL;
	#endif


	// Maik Jurkait  16.06.2004 16:37:29
	// ------------------------------------------
	//	Load all chunks
	// ------------------------------------------
	TArray <s_nif_info>	all_nifs;
	nifs.GotoFirstLine();
	do
	{

		char* nifname = nifs.GetString(0);
		TRACE1("Loading: %s\n",nifname);
		progress.Step(nifname);

		// check nifproxy (added patch 1.68)
		nifproxy.GotoFirstLine();
		do
		{
			if (stricmp(nifproxy.GetString(0),nifname)==0)
			{
				nifname = nifproxy.GetString(1);
				TRACE1("-> using: %s\n",nifname);
				progress.SetText(nifname);
				break;
			}
		}
		while (nifproxy.GotoNextLine());

		// load
#ifdef USE_LOADNIF
		object obj = LoadNIF(nifpath,nifname);
#else
		object obj = CreateObject(nifpath,nifname);
#endif
		assert(obj);

		s_nif_info newinfo;
		newinfo.obj = obj;
		newinfo.used = 0;
		all_nifs.AddLast(newinfo);

	} while (nifs.GotoNextLine());




	// Maik Jurkait  16.06.2004 15:07:12
	// ------------------------------------------
	//	Create scene root
	// ------------------------------------------
	CNiNode* pc_root = new CNiNode();
	pc_root->SetName("Dungeon root");



	// Maik Jurkait  16.06.2004 16:41:42
	// ------------------------------------------
	//	Place the stuff
	// ------------------------------------------
	progress.Step("placing");
	places.GotoFirstLine();
	do
	{
		int		nifid = places.GetInt(0);
		float	tx = places.GetFloat(1);
		float	ty = places.GetFloat(2);
		float	tz = places.GetFloat(3);
		float	rz = places.GetFloat(4);

		float	u1 = places.GetFloat(5);
		float	u2 = places.GetFloat(6);
		float	u3 = places.GetFloat(7);

		if (all_nifs[nifid].obj == NULL)
		{
			TRACE("ERROR: object not loaded\n");
			continue;
		}

		char temp[256];
		sprintf(temp,"%f,%f,%0.2f,%0.2f",u1,u2,u3,rz);

		CNiNode* pc_trans = new CNiNode();
		pc_trans->SetName(temp);//"trans");
		pc_trans->GetMatrix().AddTranslation(tx,ty,tz);
		if (u1==0)		pc_trans->GetMatrix().AddRotation(0,0,rz);
		if (u2>0.5f)	pc_trans->GetMatrix().AddScale(-1,1,1);
		if (u3>0)		pc_trans->GetMatrix().AddRotation(0,0,PI);

		pc_trans->AddChild((CNode*)all_nifs[nifid].obj);
		all_nifs[nifid].used++;

		pc_root->AddChild(pc_trans);
		
	} while (places.GotoNextLine());


	// Maik Jurkait  13.07.2004 08:12:26
	// ------------------------------------------
	//	delete unused
	// ------------------------------------------
	for (int t=0;t<all_nifs.Count();t++)
	{
		if (all_nifs[t].used ==0 )
			NIF::ReleaseObject(all_nifs[t].obj);
	}


	// Maik Jurkait  01.06.2004 18:05:29
	// ------------------------------------------
	//	Create all
	// ------------------------------------------
#ifdef USE_LOADNIF	
	progress.Step("creating");
	gpc_texturemanager->AddSearchPath(nifpath);
	pc_root->Create();
	gpc_texturemanager->RemoveSearchPath(nifpath);
#endif


	// Maik Jurkait  13.07.2004 10:08:07
	// ------------------------------------------
	//	update debug-view
	// ------------------------------------------
	progress.Step();
	#ifdef ANALYSE_NIF
		progress.SetText("update debug-view");
		// re-enable debug-view
		gpc_wnd_debug = pc_safed;
		if (gpc_wnd_debug)
		{
			gpc_wnd_debug->TreeClear();
			//gpc_wnd_debug->TreeBuild(pc_root);
		}
	#endif

	// test
	progress.Step("test");
//	DoHeighCheck(pc_root);


	// Maik Jurkait  30.06.2004 10:20:58
	// ------------------------------------------
	//	optimize
	// ------------------------------------------
	progress.Step("optimize");
	pc_root->UpdateStatic();



	return pc_root;
}




float	calcHight(float* mpc_highmap, float grid_size, float fx, float fy)
{
	float h1;
	float h2;
	float h3;
	float h4;

	int x = (int)(fx/grid_size);
	int y = (int)(fy/grid_size);

	x = min(255,x);
	y = min(255,y);

	h1 = mpc_highmap[x+(256-y)*256];
	h2 = mpc_highmap[x+1+(256-y)*256];
	h3 = mpc_highmap[x+256+(256-y)*256];
	h4 = mpc_highmap[x+257+(256-y)*256];

	return -h1;


	// wrong
	fx = fmod(fx,grid_size);
	fy = fmod(fy,grid_size);

	if (fy<fx)
	{
		float hx = h1 + (h2-h1) * (fx/grid_size);
		float hy = h1 + (h3-h1) * (fy/grid_size);
		return -(hx+hy)/2;
	}

	float hx = h3 + (h4-h3) * (fx/grid_size);
	float hy = h2 + (h4-h2) * (fy/grid_size);
	return -(hx+hy)/2;
}




NIF::object		LoadTerrain(const s_zoneinformation& zinfo)
{
	float	scalefactor = 8;
	float	offsetfactor = 32;
	int		textureset = zinfo.z_data_id;
	int		flip_x = 0;
	int		flip_y = 0;

	const float main_scale = 10.0f;

	const float grid_size = 32.0f;
	const float text_step = (1.0f / grid_size); 
	const float grid_object_pos_scale = 1.0f/main_scale;//1.0f/8.0f;
	const float grid_object_scale = 1.25f / 1000.0f;
//	const float high_scale = -8.0f/main_scale;//(-1.0f/1024.0f);
	const float high_scale = -1.0f/main_scale;//(-1.0f/1024.0f);
	const float grid_scale = 8.0f/main_scale;


	// Maik Jurkait  06.07.2004 09:00:54
	// ------------------------------------------
	//	create NIF search pathes
	// ------------------------------------------
	// housing:
	//	phousing\\zones\\zoneXXX\\nifs
	//	phousing\\nifs
	//  zones\\nifs
	//
	// frontier:
	//	frontiers\\zones\\zoneXXX\\nifs
	//	frontiers\\nifs
	//  zones\\nifs
	//
	// normal:
	// frontier:
	//	zones\\zoneXXX\\nifs
	//	zones\\nifs
	//  zones\\nifs
	//
	char dir_zone_base[MAX_PATH];
	sprintf(dir_zone_base,"%szones\\",gpc_base_directory);
	if (zinfo.z_type==3)		sprintf(dir_zone_base,"%sphousing\\zones\\",gpc_base_directory);
	if (zinfo.z_frontier == 1)	sprintf(dir_zone_base,"%sfrontiers\\zones\\",gpc_base_directory);


	char nifpath1[MAX_PATH];
	char nifpath2[MAX_PATH];
	char nifpath3[MAX_PATH];
	sprintf(nifpath1,"%szone%03i\\nifs\\",dir_zone_base,zinfo.z_data_id);
	sprintf(nifpath2,"%s\\empty",gpc_base_directory); // just a dummy
	sprintf(nifpath3,"%szones\\nifs\\",gpc_base_directory);

	if (zinfo.z_type==3)
	{
		sprintf(nifpath2,"%sphousing\\nifs\\",gpc_base_directory);
	}
	if (zinfo.z_frontier == 1)
	{
		sprintf(nifpath2,"%sfrontiers\\nifs\\",gpc_base_directory);
	}



	// Maik Jurkait  16.06.2004 14:59:00
	// ------------------------------------------
	//	Open datXXX dir
	// ------------------------------------------
	char path[MAX_PATH];
	sprintf(path,"%szone%03i\\dat%03i.mpk",dir_zone_base,zinfo.z_data_id,zinfo.z_data_id);

	CDirList datdir;
	if (!datdir.Open(path)) return NULL;



	// Maik Jurkait  16.06.2004 19:23:46
	// ------------------------------------------
	//	reading sector.dat
	// ------------------------------------------
	char*	mbuffer;
	size_t	msize;
	if (!datdir.ReadFile("sector.dat",mbuffer,msize)) return NULL;
	CReaderINI secdat(mbuffer,msize);

	scalefactor		= secdat.GetEntryFloat("terrain","scalefactor",8);
	offsetfactor	= secdat.GetEntryFloatThisSection("offsetfactor",32);
	textureset		= secdat.GetEntryIntThisSection("use_texture",textureset);
	flip_x			= secdat.GetEntryIntThisSection("flip_x",0);
	flip_y			= secdat.GetEntryIntThisSection("flip_y",0);


	// Maik Jurkait  02.07.2004 14:40:01
	// ------------------------------------------
	//	open Nifs + fixtures
	// ------------------------------------------
	if (!datdir.ReadFile("nifs.csv",mbuffer,msize)) return NULL;
	CReaderCSV nifs(mbuffer,msize);

	if (!datdir.ReadFile("fixtures.csv",mbuffer,msize)) return NULL;
	CReaderCSV fixtures(mbuffer,msize);


	// Maik Jurkait  30.06.2004 10:44:55
	// ------------------------------------------
	//	Progress window;
	// ------------------------------------------
	CDebugProgress	progress;
	progress.Create(2+4 + 
					nifs.GetLineCount()
					);




	// Maik Jurkait  16.06.2004 19:35:01
	// ------------------------------------------
	//	Load HighMaps
	// ------------------------------------------
	progress.Step("loading highmaps");
	float* mpc_highmap;
	int		x,y;
	bool	res;
	BYTE *	pc_terrain;
	BYTE *	pc_offset;

	res = LoadPCX(&datdir,"terrain.pcx",pc_terrain,x,y);
	ASSERT(res); if (!res) return NULL;

	res = LoadPCX(&datdir,"offset.pcx",pc_offset,x,y);
	ASSERT(res); if (!res) return NULL;

	mpc_highmap = new float[256*257+1];

	for (int x = 0;x<256;x++)
	for (int y = 0;y<256;y++)
			mpc_highmap[(x)+y*256] = (pc_terrain[x+y*256] * scalefactor + pc_offset[x+y*256]*offsetfactor)*high_scale;

	for (int x = 0;x<257;x++)	mpc_highmap[x+256*256]=mpc_highmap[x+256*255];
		
	delete [] (pc_terrain);
	delete [] (pc_offset);




	#ifdef ANALYSE_NIF
		// disable debug tree
		// (else it would be update by every object)
		CDebugWindow* pc_safed = gpc_wnd_debug;
		gpc_wnd_debug = NULL;
	#endif



	// Maik Jurkait  16.06.2004 19:41:57
	// ------------------------------------------
	//	Create Scene-Root
	// ------------------------------------------
	CNiNode* pc_root = new CNiNode();
	pc_root->SetName("Terrain root");


#ifndef EXPORT_DOORS

	// Maik Jurkait  16.06.2004 19:38:27
	// ------------------------------------------
	//	create terrain mesh
	// ------------------------------------------
	progress.Step("Terrain mesh");

	CNiNode* pc_tnode = new CNiNode();
	pc_tnode->SetName("Floor");
	pc_root->AddChild(pc_tnode);

	for (int block_x = 0; block_x<8;block_x++)
	for (int block_y = 0; block_y<8;block_y++)
	{

		// Maik Jurkait  16.06.2004 19:47:08
		// ------------------------------------------
		//	Create new mesh nodes
		// ------------------------------------------
		CNiTriShape*		pc_tri = new CNiTriShape();


		// Maik Jurkait  16.06.2004 19:47:17
		// ------------------------------------------
		//	Add texture
		// ------------------------------------------
		CNiTexturingProperty*	pc_textureporb = new CNiTexturingProperty();
		CNiSourceTexture*		pc_texturesource = new CNiSourceTexture();

		char texturename[256];
		sprintf(texturename,"tex%02i-%02i.bmp",block_x,block_y);
		pc_texturesource->SetTexture(texturename);
		pc_textureporb->AddTexture(pc_texturesource);
		pc_tri->AddProperty(pc_textureporb);
		pc_tnode->AddChild(pc_tri);


		// Maik Jurkait  17.06.2004 11:23:54
		// ------------------------------------------
		//	Create the mesh
		// ------------------------------------------
		pc_tri->CreateManual(texturename,33*33,32*32*2,1,false);

		for (int y = 0;y<33;y++)
		for (int x = 0;x<33;x++)
		{
				float height = -mpc_highmap[block_x*32+x+((block_y)*32+32-y)*256];
				float text_x = x*text_step;
				float text_y = 1.0f-y*text_step;
				float coor_x = block_x*32.0f*grid_size + x*grid_size;
				float coor_y = (7-block_y)*32.0f*grid_size + y*grid_size;

				pc_tri->SetPoint(x+33*y,CVector(coor_x*grid_scale,coor_y*grid_scale,height));
				pc_tri->SetPointTexture(x+33*y,text_x,text_y,0);
		}

		for (int y = 0;y<32;y++)
		for (int x = 0;x<32;x++)
		{
			pc_tri->SetTriangle((x+32*y)*2,		x+y*33,x+y*33+1,x+(y+1)*33);
			pc_tri->SetTriangle((x+32*y)*2+1,	x+y*33+1,x+1+(y+1)*33,x+(y+1)*33);
		}

	}




	// Maik Jurkait  02.07.2004 14:38:00
	// ------------------------------------------
	//	Create Water
	// ------------------------------------------
	/// \todo create terrain rivers


	// Maik Jurkait  16.06.2004 19:30:53
	// ------------------------------------------
	//	call create to load all terrain textures
	// ------------------------------------------
	progress.Step("loading terrain textures");

	char texturepath[MAX_PATH];
	sprintf(texturepath,"%szone%03i\\tex%03i.mpk",dir_zone_base,textureset,textureset);
	gpc_texturemanager->AddSearchPath(texturepath);

	pc_root->Create();

	gpc_texturemanager->RemoveSearchPath(texturepath);
#endif




	// Maik Jurkait  02.07.2004 14:38:11
	// ------------------------------------------
	//	Load Nifs
	// ------------------------------------------
	TArray <s_nif_info>	all_nifs;
	nifs.GotoFirstLine();
	do
	{
		TRACE1("Loading: %s\n",nifs.GetString(2));
		progress.Step(nifs.GetString(2));
		object obj;

#ifdef USE_LOADNIF
		obj = LoadNIF(nifpath1,nifs.GetString(2));
		if (obj == NULL)	obj = LoadNIF(nifpath2,nifs.GetString(2));
		if (obj == NULL)	obj = LoadNIF(nifpath3,nifs.GetString(2));
#else
		obj = CreateObject(nifpath1,nifs.GetString(2));
		if (obj == NULL)	obj = CreateObject(nifpath2,nifs.GetString(2));
		if (obj == NULL)	obj = CreateObject(nifpath3,nifs.GetString(2));
#endif

		if (!obj)
		{
			TRACE1("Error loading: %s\n",nifs.GetString(2));
			continue;
		}


		s_nif_info info;
		info.id = nifs.GetInt(0);
		info.obj = obj;
		info.used = 0;
		all_nifs.AddLast(info);

	} while (nifs.GotoNextLine());





	// Maik Jurkait  02.07.2004 14:43:43
	// ------------------------------------------
	//	Place objects
	// ------------------------------------------
	fixtures.GotoFirstLine();
	int counter = 1;
	do
	{
		// Maik Jurkait  02.07.2004 18:25:16
		// ------------------------------------------
		//	 create new node
		// ------------------------------------------
		CNiNode* pc_fixture = new CNiNode();
		char temp[256];
		sprintf(temp,"Fixture_%i",counter++);
		pc_fixture->SetName(temp);
		pc_root->AddChild(pc_fixture);

		// Maik Jurkait  02.07.2004 18:25:23
		// ------------------------------------------
		//	Read info
		// ------------------------------------------
		int		nif_id = fixtures.GetInt(1);
		float	x = fixtures.GetFloat(3) *grid_object_pos_scale;
		float	y = (8.0f*32.0f*grid_size*grid_scale) - fixtures.GetFloat(4) *grid_object_pos_scale;
		float	z = fixtures.GetFloat(5) * high_scale;
		float	a = fixtures.GetFloat(6);
		float	scale = fixtures.GetFloat(7) *grid_object_scale;


		// Maik Jurkait  05.07.2004 21:54:29
		// ------------------------------------------
		/// \todo atm the high in of objects in terrain is calculated. maybe there is a way to use the saved-value
		// ------------------------------------------
		z = calcHight(mpc_highmap, grid_size, x/grid_scale, y/grid_scale);


		// Maik Jurkait  02.07.2004 18:42:03
		// ------------------------------------------
		//	Find nif-obj
		// ------------------------------------------
		int obj_id = 0;
		for (;obj_id<all_nifs.Count();obj_id++)
			if (all_nifs[obj_id].id == nif_id) break;

		//ASSERT(obj_id<all_nifs.Count());
		if (obj_id>=all_nifs.Count())
		{
			int id = fixtures.GetInt(1); // workarround 'cause CSVReader uses only one str-buffer
			TRACE2("object not found: %i %s\n",id,fixtures.GetString(2));
			continue;
		}


		// Maik Jurkait  11.07.2004 13:57:28
		// ------------------------------------------
		//	look for doors
		// ------------------------------------------
	#ifdef EXPORT_DOORS
		CNode* nif = (CNode*)all_nifs[obj_id].obj;
		CNode* result = NULL;
		FILE *door_log = fopen("doors.txt","at");
		while ((result = nif->FindNode("door",&result,false))!=NULL)
		{
			CNiNode*pc_node = (CNiNode*)result;
			
			if (!isdigit(pc_node->GetNodeName()[4]))  continue;
			int id = fixtures.GetInt(0);
			int idnif = fixtures.GetInt(1);
			int idnifun = fixtures.GetInt(14);
			int num = atoi(&pc_node->GetNodeName()[4]);

			fprintf(door_log,"%i;%s;%s;%i;%i;%i;%i;%s\n",
							zinfo.z_id*100000 + idnifun*100+num,
							fixtures.GetString(2),pc_node->GetNodeName(),
							idnifun,id,idnif,
							zinfo.z_id,zinfo.z_name);

			TRACE2("DOOR: %s,%s,",fixtures.GetString(2),pc_node->GetNodeName());
			TRACE3("%i,%i,%i",idnifun,id,idnif);
			TRACE2(" (%i %s) ->",zinfo.z_id,zinfo.z_name);
			TRACE1("%i\n",zinfo.z_id*100000 + idnifun*100+num);
		}
		fclose(door_log);
	#endif



		// Maik Jurkait  02.07.2004 18:27:04
		// ------------------------------------------
		//	Add object
		// ------------------------------------------
		all_nifs[obj_id].used++;
		pc_fixture->AddChild((CNode*)all_nifs[obj_id].obj);

		pc_fixture->GetMatrix().SetTranslation(	x,
												y,
												z);
		pc_fixture->GetMatrix().AddRotation(0,0,-a*RADDEG);
		pc_fixture->GetMatrix().AddScale(scale,scale,scale);

	} while (fixtures.GotoNextLine());


	// Maik Jurkait  02.07.2004 18:52:15
	// ------------------------------------------
	//	release unused
	// ------------------------------------------
	for (int t=0;t<all_nifs.Count();t++)
		if (all_nifs[t].used == 0)
			ReleaseObject(all_nifs[t].obj);


	// Maik Jurkait  01.06.2004 18:05:29
	// ------------------------------------------
	//	Create all
	// ------------------------------------------
#ifdef USE_LOADNIF
#ifndef EXPORT_DOORS

	progress.Step("creating");
	gpc_texturemanager->AddSearchPath(nifpath1);
	gpc_texturemanager->AddSearchPath(nifpath2);
	gpc_texturemanager->AddSearchPath(nifpath3);
	pc_root->Create();
	gpc_texturemanager->RemoveSearchPath(nifpath3);
	gpc_texturemanager->RemoveSearchPath(nifpath2);
	gpc_texturemanager->RemoveSearchPath(nifpath1);
#endif
#endif


	// Maik Jurkait  02.07.2004 18:45:33
	// ------------------------------------------
	//	re-activate debug-view
	// ------------------------------------------
	progress.Step();
	#ifdef ANALYSE_NIF
		progress.SetText("updating debug-view");
		gpc_wnd_debug = pc_safed;
		if (gpc_wnd_debug)
		{
			gpc_wnd_debug->TreeClear();
			// to heavy
			//gpc_wnd_debug->TreeBuild(pc_root);
		}
	#endif


	// Maik Jurkait  05.07.2004 21:51:42
	// ------------------------------------------
	//	highmap not needed anymore
	// ------------------------------------------
	delete mpc_highmap;



	// Maik Jurkait  30.06.2004 10:20:58
	// ------------------------------------------
	//	optimize
	// ------------------------------------------
	progress.Step("optimize");
#ifndef EXPORT_DOORS
	pc_root->UpdateStatic();
#endif


	return pc_root;
}





void	ReadRegionInfo(s_zoneinformation& zone_info, int ID)
{
	assert(gpc_base_directory); // no base dir
	assert(gpc_zones);			// zones.dat not loaded

	char temp[64];
	sprintf(temp,"region%03i", ID);


	// Maik Jurkait  16.06.2004 19:09:12
	// ------------------------------------------
	//	[REGION] stuff
	// ------------------------------------------
	zone_info.r_proxy_region		= gpc_zones->GetEntryInt(temp,"proxy_region",		-1);

	if (zone_info.r_proxy_region != -1)
		ReadRegionInfo(zone_info, zone_info.r_proxy_region);

	zone_info.r_expansion	= gpc_zones->GetEntryIntThisSection("expansion",	zone_info.r_expansion);
	zone_info.r_phousing	= gpc_zones->GetEntryIntThisSection("phousing",		zone_info.r_phousing);
	zone_info.r_entry_music	= gpc_zones->GetEntryIntThisSection("entry_music",	zone_info.r_entry_music);
	zone_info.r_grass_defs	= gpc_zones->GetEntryIntThisSection("grass_defs",	zone_info.r_grass_defs);
	zone_info.r_GRASSDENSITY= gpc_zones->GetEntryIntThisSection("GRASSDENSITY",	zone_info.r_GRASSDENSITY);

	SAFE_DELETE(zone_info.r_grasscsv);
	SAFE_DELETE(zone_info.r_grassmap);
	SAFE_DELETE(zone_info.r_grasscsv1);
	SAFE_DELETE(zone_info.r_grassmap1);
	SAFE_DELETE(zone_info.r_skydome);
	SAFE_DELETE(zone_info.r_detailmap);

	zone_info.r_grasscsv	= gpc_zones->GetEntryStringThisSection("grasscsv","");
	zone_info.r_grassmap	= gpc_zones->GetEntryStringThisSection("grassmap","");
	zone_info.r_grasscsv1	= gpc_zones->GetEntryStringThisSection("grasscsv1","");
	zone_info.r_grassmap1	= gpc_zones->GetEntryStringThisSection("grassmap1","");
	zone_info.r_skydome		= gpc_zones->GetEntryStringThisSection("skydome","");
	zone_info.r_detailmap	= gpc_zones->GetEntryStringThisSection("detailmap","");
}





void	ReadZoneInfo(s_zoneinformation& zone_info, int ID)
{
	assert(gpc_base_directory); // no base dir
	assert(gpc_zones);			// zones.dat not loaded

	char temp[64];
	sprintf(temp,"zone%03i",ID);


	// Maik Jurkait  16.06.2004 19:06:50
	// ------------------------------------------
	//	[ZONE] stuff
	// ------------------------------------------
	zone_info.z_region		= gpc_zones->GetEntryInt(temp,"region",				zone_info.z_region);
	zone_info.z_proxy_zone	= gpc_zones->GetEntryIntThisSection("proxy_zone",	-1);

	zone_info.z_data_id = ID;
	if (zone_info.z_proxy_zone != -1)
	{
		zone_info.z_data_id = zone_info.z_proxy_zone;
		ReadZoneInfo(zone_info, zone_info.z_proxy_zone);
	}

	zone_info.z_id = ID;

	zone_info.z_enabled			= gpc_zones->GetEntryIntThisSection("enabled",			zone_info.z_enabled);
	zone_info.z_type			= gpc_zones->GetEntryIntThisSection("type",				zone_info.z_type);
	zone_info.z_region_offset_x	= gpc_zones->GetEntryIntThisSection("region_offset_x",	zone_info.z_region_offset_x);
	zone_info.z_region_offset_y	= gpc_zones->GetEntryIntThisSection("region_offset_y",	zone_info.z_region_offset_y);
	zone_info.z_width			= gpc_zones->GetEntryIntThisSection("width",			zone_info.z_width);
	zone_info.z_height			= gpc_zones->GetEntryIntThisSection("height",			zone_info.z_height);
	zone_info.z_entry_music		= gpc_zones->GetEntryIntThisSection("entry_music",		zone_info.z_entry_music);
	zone_info.z_override_time	= gpc_zones->GetEntryIntThisSection("override_time",	zone_info.z_override_time);
	zone_info.z_temperature		= gpc_zones->GetEntryIntThisSection("temperature",		zone_info.z_temperature);
	zone_info.z_frontier		= gpc_zones->GetEntryIntThisSection("frontiers",		zone_info.z_frontier);

	char * zname = gpc_zones->GetEntryStringThisSection("name","");
	char * zsky  = gpc_zones->GetEntryStringThisSection("skydome","");
	if (zname[0])
	{
		SAFE_DELETE(zone_info.z_name);
		zone_info.z_name = zname;
		zname = NULL;
	}
	if (zsky[0])
	{
		SAFE_DELETE(zone_info.z_skydome);
		zone_info.z_skydome = zsky;
		zsky = NULL;
	}
	SAFE_DELETE(zname);
	SAFE_DELETE(zsky);



	// Maik Jurkait  16.06.2004 19:11:11
	// ------------------------------------------
	//	and region stuff
	// ------------------------------------------
    ReadRegionInfo(zone_info, zone_info.z_region);
}






NIFLIB_API object	NIF::DAoC_LoadZone(int ID)
{
	assert(gpc_base_directory); // no base dir
	assert(gpc_zones);			// zones.dat not loaded



	// Maik Jurkait  16.06.2004 14:44:19
	// ------------------------------------------
	//	Get zone parameters
	// ------------------------------------------
	s_zoneinformation zone_info;
	ReadZoneInfo(zone_info,  ID);


	switch (zone_info.z_type)
	{
		case 0:	return LoadTerrain(zone_info);
		case 1:	return LoadCity(zone_info);
		case 2:	return LoadDungeon(zone_info);
		case 3:	return LoadTerrain(zone_info);
	}

	return NULL;
}




#define DETAIL 256
#define HEIGHMAPS 4
void	DoHeighCheck(CNode*pc_root)
{

	CDebugNode* pc_n = new CDebugNode();
	((CNiNode*)pc_root)->AddChild(pc_n);
	CVector vmin,vmax;

	pc_root->GetBoundingbox(vmin,vmax);
	float x_steps = (vmax.GetX()-vmin.GetX())/DETAIL;
	float y_steps = (vmax.GetY()-vmin.GetY())/DETAIL;

	TArray <CVector> points;
	CMatrix world;

	float* heighmap[HEIGHMAPS];
	for (int t=0;t<HEIGHMAPS;t++) heighmap[t] = new float [DETAIL*DETAIL];
	const float nothing = -99999.0f;

	// Maik Jurkait  13.07.2004 14:07:35
	// ------------------------------------------
	//	find one
	// ------------------------------------------
	for (float y=vmin.GetY();y<vmax.GetY();y+=y_steps)
	for (float x=vmin.GetX();x<vmax.GetX();x+=x_steps)
	{
		points.SetSize(0);
		pc_root->Collision(world, CVector(x,y,vmin.GetZ()),CVector(0,0,vmax.GetZ()-vmin.GetZ()), points); 

		for (int t1=0;t1<points.Count()-1;t1++)
			for (int t2=t1+1;t2<points.Count();t2++)
				if (points[t1].GetZ()>points[t2].GetZ())
				{
					CVector temp = points[t1];
					points[t1] = points[t2];
					points[t2] = temp;
				}

		for (int t=0;t<HEIGHMAPS;t++)
		{
			float *h = &heighmap[t][(int)((x-vmin.GetX())/x_steps) + (int)((y-vmin.GetY())/y_steps)*DETAIL];
			*h=nothing;
			if (points.Count()>t*2)
			{
				*h = points[t*2].GetZ();
				pc_n->AddPoint(points[t*2]);
			}
		}
	}


	// Maik Jurkait  13.07.2004 14:34:09
	// ------------------------------------------
	//	and now save it as bmp
	// ------------------------------------------
	for (int h = 0;h<HEIGHMAPS;h++)
	{
		float imax = 0;
		float imin = 0;
		int t = 0;
		for (;t<DETAIL*DETAIL;t++)
			if (heighmap[h][t]>nothing)
			{
				imax = heighmap[h][t];
				imin = heighmap[h][t];
				break;
			}

		for (;t<DETAIL*DETAIL;t++)
		{
			if (heighmap[h][t]>nothing)
			{
				imax = max(imax,heighmap[h][t]);
				imin = min(imin,heighmap[h][t]);
			}
		}


		BYTE pixels[DETAIL*DETAIL*3];
		BYTE *p = pixels;
		for (int t=0;t<DETAIL*DETAIL;t++)
		{
			BYTE col = 0;
			if (heighmap[h][t]>nothing)
				col = 1+(heighmap[h][t]-imin)*254/(imax-imin);

			*p++ = col;
			*p++ = col;
			*p++ = col;
		}


		BITMAPFILEHEADER	head;
		BITMAPINFOHEADER	info;

		head.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+DETAIL*DETAIL*3;
		head.bfType = 'MB';
		head.bfReserved1 = 0;
		head.bfReserved2 = 0;
		head.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

		info.biSize = sizeof(BITMAPINFOHEADER);
		info.biWidth = DETAIL;
		info.biHeight = DETAIL;
		info.biPlanes = 1;
		info.biBitCount = 24;
		info.biCompression = BI_RGB;
		info.biSizeImage = 0;
		info.biXPelsPerMeter = DETAIL;
		info.biYPelsPerMeter = DETAIL;
		info.biClrUsed = 0;
		info.biClrImportant = 0;


		char name[256];
		sprintf(name,"_h%i.bmp",h);

		FILE* outf;
		outf= fopen(name,"wb");
		fwrite(&head,sizeof(BITMAPFILEHEADER),1,outf);
		fwrite(&info,sizeof(BITMAPINFOHEADER),1,outf);
		fwrite(pixels,DETAIL*DETAIL*3,1,outf);
		fclose(outf);
	}

	for (int t=0;t<HEIGHMAPS;t++) delete(heighmap[t]);


}
