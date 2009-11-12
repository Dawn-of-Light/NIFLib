/**
* NIFLib Zones
* Interface functions
**/

#include "niflib.h"

// ----------------------------------------------------------
// DLL Functions
// ----------------------------------------------------------
NIFLIB_API int			NIF::DAoC_GetZoneIDs(int* ppn, int size);
NIFLIB_API bool			NIF::DAoC_GetZoneName(int ID, char*name, int size);
NIFLIB_API bool			NIF::DAoC_GetZoneExtraInfo(int ID, char*pc_buffer, int size);
NIFLIB_API NIF::object	NIF::DAoC_LoadZone(int ID);

// for testing
NIFLIB_API void			NIF::DAoC_LoadAllZones();




// ----------------------------------------------------------
// Internal functions
// ----------------------------------------------------------
struct	s_zoneinformation
{
	// Region stuff
	int		r_expansion;		/// 0 -> classic, 1 -> SI, 2 -> TOA
	int		r_phousing;			/// 0 -> normal, 1 -> housingzone
	int		r_proxy_region;		/// only needed while reading
	int		r_entry_music;	
	int		r_grass_defs;		/// Number of grass layers
	char*	r_grasscsv;
	char*	r_grassmap;
	char*	r_grasscsv1;
	char*	r_grassmap1;
	int		r_GRASSDENSITY;
	char*	r_skydome;
	char*	r_detailmap;

	// Zone stuff
	int		z_enabled;			/// always 1
	int		z_type;				///	0 -> normal Terrain;1 -> City;	2 -> Dungeon;3 -> Housing (basicaly like 0);4 -> Aerus sky city
	int		z_region;			/// only needed while reading
	int		z_proxy_zone;		/// only needed while reading
	char*	z_name;
	int		z_region_offset_x;
	int		z_region_offset_y;
	int		z_width;
	int		z_height;
	int		z_entry_music;
	char*	z_skydome;
	int		z_override_time;
	int		z_temperature;
	int		z_frontier;
	int		z_id;

	int		z_data_id;			// which ID for zone-data loading


	s_zoneinformation();
	~s_zoneinformation();
};



// ----------------------------------------------------------
// Internal functions
// ----------------------------------------------------------
NIF::object		LoadCity(const s_zoneinformation&);
NIF::object		LoadDungeon(const s_zoneinformation&);
NIF::object		LoadTerrain(const s_zoneinformation&);

/**
*	load nif's without creating it
**/
NIF::object		LoadNIF(const char* path, const char* filename);

bool			LoadPCX(CDirList* dir , const char* filename, BYTE*& pc_data, int& size_x, int& size_y);

void			ReadRegionInfo(s_zoneinformation& zone_info, int ID);
void			ReadZoneInfo(s_zoneinformation& zone_info, int ID);

void			DoHeighCheck(CNode*pc_root);

