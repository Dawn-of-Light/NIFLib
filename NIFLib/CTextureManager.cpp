#include "StdAfx.h"
#include "ctexturemanager.h"
#include "CDirList.h"
#include "resource.h"


#ifdef NIFLIB_DLL_EXPORTS
	#pragma comment( lib, "opengl32" )
	#pragma comment( lib, "glu32" )
	#pragma comment( lib, "glaux" )

	#define GLH_EXT_SINGLE_FILE
	#include <GL/glext.h>
#endif
	#include <GLH/glh_extensions.h>

/**
*	extended version of assert
**/
#define RASSERT(x,y)	ASSERT(x); if (!(x)) return y;


CTextureManager* gpc_texturemanager = NULL;



CTextureManager::CTextureManager(void)
{
	// Maik Jurkait  01.06.2004 12:15:03
	// ------------------------------------------
	//	Just set current active to NONE
	// ------------------------------------------
	mn_current_active = -1;
}




CTextureManager::~CTextureManager(void)
{

	for (int i=0;i<ma_searchpath.Count();i++)
	{
		delete (ma_searchpath[i]);
	}
	

	// Maik Jurkait  01.06.2004 12:15:20
	// ------------------------------------------
	//	Release all texture
	// ------------------------------------------
	for (int i=0;i<ms_textures.Count();i++)
	{
		#ifdef _DEBUG
		if (ms_textures[i].references>0) TRACE1("Texture \"%s\" has still referenzes\n",ms_textures[i].name);
		#endif
		glDeleteTextures(1, &(ms_textures[i].id));
	}
}



int		CTextureManager::FindTexture(const char* name)
{

	// Maik Jurkait  01.06.2004 12:33:00
	// ------------------------------------------
	//	do binary search
	// ------------------------------------------
	int low  = -1;
	int	high = ms_textures.Count()-1;


	while (high-low > 1)
	{
		int pos = (low + high)/2;

		int c = stricmp(ms_textures[pos].name,name);
		if (c==0)	return pos;
		if (c<0)	low = pos;
		else		high = pos;
	}


	// Maik Jurkait  01.06.2004 12:33:09
	// ------------------------------------------
	//	maybe it's the last entry
	//  (since an empty texture arry is rare I'll only check it here)
	// ------------------------------------------
	if (high>-1 && stricmp(ms_textures[high].name,name) ==0)
		return high;


	return -1;
}





void	CTextureManager::ActivateTexture(GLuint id)
{
	ActivateTexture(id, 0);

/*
	if (mn_current_active == id) return;
	mn_current_active = id;
	glBindTexture(GL_TEXTURE_2D, id);
*/
}



void	CTextureManager::ActivateTexture(GLuint id, int channel)
{
	ASSERT(channel<MAX_TEXTURES);
	if (mn_current_texture[channel] == id) return;
	mn_current_texture[channel] = id;

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
}


void	CTextureManager::DeActivateTexture(int channel)
{
	ASSERT(channel<MAX_TEXTURES);
	mn_current_texture[channel] = -1;

	glActiveTextureARB(GL_TEXTURE0_ARB+channel);
	glDisable(GL_TEXTURE_2D);
}



void	CTextureManager::TextureChanged()
{
	mn_current_active = -1;

	for (int t=0;t<MAX_TEXTURES;t++)
		mn_current_texture[t] = -1;
}




GLuint	CTextureManager::GetTexture(const char* filename)
{
	// Maik Jurkait  01.06.2004 12:39:56
	// ------------------------------------------
	//	Search it
	// ------------------------------------------
	int n_texture = FindTexture(filename);

	// Maik Jurkait  01.06.2004 12:40:02
	// ------------------------------------------
	//	Load it
	// ------------------------------------------
	if (n_texture == -1)
	{
		n_texture = LoadTexture(filename);

		// should never happen 'cause NOT_FOUND will be loaded
		RASSERT(n_texture != -1, -1);
	}

	// Maik Jurkait  01.06.2004 12:41:18
	// ------------------------------------------
	//	Increase REF
	// ------------------------------------------
	ms_textures[n_texture].references++;
	return ms_textures[n_texture].id;
}





GLuint	CTextureManager::GetTexture(GLuint id)
{
	// find texture
	int pos;
	for ( pos=0; pos<ms_textures.Count(); pos++)
			if (ms_textures[pos].id == id) break;

	RASSERT(pos<ms_textures.Count(),-1);//"Texture not loaded");

	ms_textures[pos].references++;

	return id;
}



void	CTextureManager::ReleaseTexture(GLuint id)
{
	// Maik Jurkait  01.06.2004 12:50:45
	// ------------------------------------------
	//	Find texture
	// ------------------------------------------
	int pos;
	for ( pos=0; pos<ms_textures.Count(); pos++)
			if (ms_textures[pos].id == id) break;

	RASSERT(pos<ms_textures.Count(), ); // Texture not loaded

	// Maik Jurkait  01.06.2004 12:55:24
	// ------------------------------------------
	//	Ref + release
	// ------------------------------------------
	// release
	ASSERT(ms_textures[pos].references>0);//"Texture not referenzed.");
	if (--ms_textures[pos].references == 0)
	{
		glDeleteTextures(1, &(ms_textures[pos].id));

		ms_textures.Delete(pos);
	}
}



void	CTextureManager::AddSearchPath(const char*path)
{
	CDirList* pc_new_dir = new CDirList;
	pc_new_dir->Open(path);

	ma_searchpath.AddLast(pc_new_dir);
}



void	CTextureManager::RemoveSearchPath(const char*path)
{
	for (int t=0;t<ma_searchpath.Count();t++)
		if (ma_searchpath[t]->IsPath(path))
		{
			delete (ma_searchpath[t]);
			ma_searchpath.Delete(t);
		}
}



extern HINSTANCE gh_instance;

int	CTextureManager::LoadTexture(const char* name)
{
	char	str_cutname[MAX_PATH];
	char*	p_add_point;

	// Maik Jurkait  01.06.2004 17:17:55
	// ------------------------------------------
	//	possible extentions
	// ------------------------------------------
	const char* ext[]={".dds",".tga",".bmp"};
	const int nof_ext = sizeof(ext)/sizeof(ext[0]);




	// Maik Jurkait  01.06.2004 17:18:03
	// ------------------------------------------
	//	cutname
	// ------------------------------------------
	strcpy(str_cutname,name);
	p_add_point = strrchr(str_cutname,'.');
	if (p_add_point) *p_add_point = 0;


	// Maik Jurkait  01.06.2004 17:19:24
	// ------------------------------------------
	//	Search it
	// ------------------------------------------
	char *	membuf = NULL;
	size_t	memsize = 0;
	int		f_type = 0;

	bool b_not_found = true;
	for (int n_dir_index=ma_searchpath.Count()-1;n_dir_index>=0 && b_not_found;n_dir_index--)
		for (f_type=0;f_type<nof_ext;f_type++)
		{
			strcat(str_cutname,ext[f_type]);
			if (ma_searchpath[n_dir_index]->FileExists(str_cutname))
			{
				b_not_found = false;
				ma_searchpath[n_dir_index]->ReadFile(str_cutname,membuf,memsize);
				break;
			}

			if (p_add_point) *p_add_point = 0;
		}



	// Maik Jurkait  01.06.2004 17:35:30
	// ------------------------------------------
	//	texture not found
	//	\todo could be loaded at startup but this is only needed if many textures are not found
	// ------------------------------------------
	if (b_not_found)
	{
		TRACE1("MISSING TEXTURE: %s\n",name);

		// Maik Jurkait  01.06.2004 19:38:45
		// ------------------------------------------
		//	Open "texture not found" resource
		// ------------------------------------------
		LPVOID	lpRes; 
		HRSRC	hResInfo;
		HANDLE	hRes; 
 
		// Find the resource. 
		hResInfo = FindResource(gh_instance, MAKEINTRESOURCE(IDB_TEXTURE_NOT_FOUND),RT_BITMAP); 
		if (hResInfo == NULL)       return -1; 
 
		 // Load the resource. 
		hRes = LoadResource(gh_instance, hResInfo); 
		if (hRes == NULL)			return -1; 
 
		// Lock the resource. 
		lpRes = LockResource(hRes); 
		if (lpRes == NULL)			return -1; 


		// Maik Jurkait  02.06.2004 10:03:51
		// ------------------------------------------
		//	copy the memory + recreate fileheader
		// ------------------------------------------
		memsize = SizeofResource(gh_instance, hResInfo);
		membuf = new char[memsize + sizeof(BITMAPFILEHEADER)];
		memcpy(membuf+sizeof(BITMAPFILEHEADER),lpRes,memsize);
		BITMAPFILEHEADER*	head = (BITMAPFILEHEADER*)membuf;
		head->bfType = 'MB';
		memsize += sizeof(BITMAPFILEHEADER);

		// Maik Jurkait  02.06.2004 10:04:11
		// ------------------------------------------
		// free resource	
		// ------------------------------------------
		UnlockResource(hRes); 
		FreeResource(hRes);  

		f_type = 2; // BMP
	}



	// Maik Jurkait  01.06.2004 17:38:18
	// ------------------------------------------
	//	Create
	// ------------------------------------------
	image_data *	pc_data = NULL;
	switch(f_type)
	{
		case 0:		pc_data = _LoadDDS(membuf,memsize);	break;
		case 1:		pc_data = _LoadTGA(membuf,memsize);	break;
		case 2:		pc_data = _LoadBMP(membuf,memsize);	break;
	}

	ASSERT(pc_data != NULL);//, "error texture loading file");
	delete [] membuf;


	int index = CreateAndInsertTexture(pc_data,	name);
	free(pc_data->data);
	delete(pc_data);
	return index;
}







GLuint	CTextureManager::CreateTexture(int x, int y, unsigned char* data, int channels)
{
	// Maik Jurkait  01.06.2004 12:10:40
	// ------------------------------------------
	//	Fill image structure
	// ------------------------------------------
	image_data pc_data;;
	pc_data.channels = channels;
	pc_data.sizeX = x;
	pc_data.sizeY = y;
	pc_data.data = data;


	// Maik Jurkait  01.06.2004 12:10:52
	// ------------------------------------------
	//	set a name. just for debug needed
	// ------------------------------------------
	static int counter = 1;
	char name[128];
	sprintf(name,"gen%i",counter++);


	// Maik Jurkait  01.06.2004 12:12:42
	// ------------------------------------------
	//	Create it
	// ------------------------------------------
	int index = CreateAndInsertTexture(&pc_data,	name);


	// Maik Jurkait  01.06.2004 12:12:51
	// ------------------------------------------
	//	Clean up
	// ------------------------------------------
	free(pc_data.data); 


	// Maik Jurkait  01.06.2004 12:12:59
	// ------------------------------------------
	//	Return it
	// ------------------------------------------
	ms_textures[index].references = 1;
	return ms_textures[index].id;
}




int	CTextureManager::CreateAndInsertTexture(image_data*pc_data,	const char* name)
{

	// Maik Jurkait  01.06.2004 12:36:25
	// ------------------------------------------
	//	Find insert position
	// ------------------------------------------
	// find pos
	int insertpos = 0;
	if (ms_textures.Count()>0)
	{
		int low = -1; int high = ms_textures.Count()-1;
		while (high-low > 1)
		{
			int pos = (low + high)/2;
			int c = stricmp(ms_textures[pos].name,name);
			ASSERT(c!=0); // texture already loaded
			if (c<0)	low = pos;
			else		high = pos;
		}
		
		insertpos = high;
		if (stricmp(ms_textures[high].name,name)>0) insertpos++;
	}


	// Maik Jurkait  01.06.2004 13:04:51
	// ------------------------------------------
	//	create struce
	// ------------------------------------------
	s_texture new_texture;

	strncpy(new_texture.name,name,120);
	new_texture.references = 0;


	// Maik Jurkait  01.06.2004 13:06:55
	// ------------------------------------------
	//	Create GL-Texture
	// ------------------------------------------
	glGenTextures(1, &new_texture.id);
	glBindTexture(GL_TEXTURE_2D, new_texture.id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);



	switch (pc_data->channels)
	{
		case 3:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pc_data->sizeX, pc_data->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pc_data->data);
			break;
		case 4:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pc_data->sizeX, pc_data->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, pc_data->data);
			break;
		case 0:
			{
				// load the .dds file
				int offset = 0;
				int blockSize = (pc_data->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;

				// load the mipmaps 
				for (unsigned i = 0; i < pc_data->nof_mipmaps && (pc_data->sizeX || pc_data->sizeY); ++i)
				{
					if (pc_data->sizeX == 0)	pc_data->sizeX = 1;
					if (pc_data->sizeY == 0)	pc_data->sizeY = 1;
					int size = ((pc_data->sizeX+3)/4)*((pc_data->sizeY+3)/4)*blockSize;
					glCompressedTexImage2DARB(GL_TEXTURE_2D, i, pc_data->format, pc_data->sizeX, pc_data->sizeY,0, size, pc_data->data + offset);
					offset += size;
					pc_data->sizeX >>= 1;
					pc_data->sizeY >>= 1;
				}
			}
			break;

		default:
			ASSERT(false);//,"illegal format");
	}



	// Maik Jurkait  01.06.2004 13:03:36
	// ------------------------------------------
	//	Insert
	// ------------------------------------------
	ms_textures.Insert(insertpos,new_texture);



	return insertpos;
}




#include <ddraw.h> // for DDSURFACEDESC2
CTextureManager::image_data*	CTextureManager::_LoadDDS(char*mem, size_t size)
{
	
	DDSURFACEDESC2* ddsd;
	int				bufsize;

	if (memcmp(mem, "DDS ", 4) != 0)	return NULL;


	image_data*p = new image_data;


	// get the surface desc
	ddsd = (DDSURFACEDESC2*)(mem+4);
	mem += sizeof(DDSURFACEDESC2)+4;
	size -= sizeof(DDSURFACEDESC2)+4;


	// how big is it going to be including all mipmaps?
	bufsize = ddsd->dwMipMapCount > 1 ? ddsd->dwLinearSize * 2 : ddsd->dwLinearSize;
	p->data = (unsigned char*)malloc(bufsize * sizeof(unsigned char));

	/// \test check this: assert(size <= bufsize * sizeof(unsigned char));
	/// atleast on a 4x4 (3mips, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	/// bufsize is to small to take all bytes in the file stream.
	/// (Nvidia's dds-loader will fail too)
	/// so I'd changed it to take the lesser value - don't know if texture is still correct
	memcpy(p->data, mem, min(size,bufsize * sizeof(unsigned char)) );


	// close the file pointer 
	switch(ddsd->ddpfPixelFormat.dwFourCC)
	{
		case FOURCC_DXT1:	p->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;		break;
		case FOURCC_DXT3:	p->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;		break;
		case FOURCC_DXT5:	p->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;		break;
		default:
			free(p->data);
			delete(p);
			return NULL;
	}

	p->nof_mipmaps = max(1,ddsd->dwMipMapCount);
	p->channels = 0;
	p->sizeX = ddsd->dwWidth;
	p->sizeY = ddsd->dwHeight;
	
	return p;
}




CTextureManager::image_data* CTextureManager::_LoadBMP(char*mem, size_t size)
{
	BITMAPFILEHEADER*	head;
	BITMAPINFOHEADER*	info;
	RGBQUAD*			palette;
	unsigned char*		dibbits;

	// get all pointers
	head = (BITMAPFILEHEADER*)mem;
	if (head->bfType != 'MB') return false;

	info = (BITMAPINFOHEADER*)(mem + sizeof(BITMAPFILEHEADER));
	palette = (RGBQUAD*)(mem + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));

	int PaletteEntries = 1 << info->biBitCount;
	if( info->biBitCount > 8 )		PaletteEntries = 0;
	else if( info->biClrUsed != 0 )	PaletteEntries = info->biClrUsed;

	dibbits = (unsigned char*)palette+ PaletteEntries*sizeof(RGBQUAD);





	unsigned char *data = new unsigned char[info->biWidth * info->biHeight*3];

	unsigned char*line_dest;
	unsigned char*line_src;
	int line_size = info->biWidth*3;
	int pitch = info->biWidth*info->biBitCount/8;
	if (pitch &3) pitch = 4+(pitch&~3);


	if (info->biBitCount == 24)
	{
		for (int y=0;y<info->biHeight;y++)
		{
			line_dest = &data[line_size*(info->biHeight-y-1)];
			line_src  = &dibbits[pitch*y];
			memcpy(line_dest,line_src,line_size);

			//swap bgr
			for (int x=0;x<info->biWidth;x++)
			{
				unsigned char t = line_dest[0];
				line_dest[0] = line_dest[2];
				line_dest[2] = t;
				line_dest+=3;
			}
		}
	}
	else
	if (info->biBitCount == 16)
	{
		ASSERT(info->biCompression == BI_RGB);
		for (int y=0;y<info->biHeight;y++)
		{
			line_dest = &data[line_size*(info->biHeight-y-1)];
			line_src  = &dibbits[pitch*y];
			WORD	col;

			// decode
			for (int x=0;x<info->biWidth;x++)
			{
				col = *(WORD*)line_src; line_src+=2;
				*line_dest++ = (col&0x7c00)>>7;
				*line_dest++ = (col&0x03e0)>>2;
				*line_dest++ = (col&0x001f)<<3;
			}
		}
	}
	else
	if (info->biBitCount == 8)
	{
		ASSERT(info->biCompression == BI_RGB);
		for (int y=0;y<info->biHeight;y++)
		{
			line_dest = &data[line_size*(info->biHeight-y-1)];
			line_src  = &dibbits[pitch*y];

			// decode
			for (int x=0;x<info->biWidth;x++)
			{
				*line_dest++ = palette[*line_src].rgbRed;
				*line_dest++ = palette[*line_src].rgbGreen;
				*line_dest++ = palette[*line_src].rgbBlue;
				line_src++;
			}
		}
	}
	

	image_data*p = new image_data;
	p->channels = 3;
	p->sizeX = info->biWidth;
	p->sizeY = info->biHeight;
	p->data = data;
	return p;
}



/****************************************************************************************
*************   Function and Variable Definitions for TARGA Image Loading   *************
*************   Supports 16, 24, 32 -Bit Targa Images and RLE Compression   ************* 
****************************************************************************************/
//...Definitions for types of TGA files...
#define TGA_RGB      2		//...Normal targa file (BGR)...
#define TGA_A		 3		//...Alpha file...
#define TGA_RLE		10		//...Compressed with Run Lenght Encoded(RLE)...



//...Loads a targa image and returns a pointer to the read image structure...
CTextureManager::image_data* CTextureManager::_LoadTGA(char*mem, size_t size)
{
	image_data *pImageData = NULL;		//...Image data...
	int channels = 0;					//...Channels of the image(3=RGA, 4=RGBA)...
	int stride = 0;						//...The stride (channels * width)...
	int i = 0;							//...A counter...
	 

#pragma pack(1)
	struct s_tga_head
	{
		BYTE	length;
		BYTE	colormap;
		BYTE	imageType;
		WORD	colormap_first_entry;
		WORD	colormap_lenght;
		BYTE	colormap_entrysize;
		WORD	ori_x;
		WORD	ori_y;
		WORD	width;
		WORD	height;
		BYTE	bits;
	};
#pragma pack()



	pImageData = new image_data;
	
	// read head
	s_tga_head* head = (s_tga_head*)mem;
	mem += sizeof(s_tga_head)+1+head->length;


	// UNcompressed FILE
	if(head->imageType != TGA_RLE)
	{ 
		if(head->bits == 24 || head->bits == 32)
		{ //...Handle the in the same way...
			channels = head->bits / 8;
			stride = channels * head->width;
			pImageData->data = new unsigned char[stride * head->height];
			for(int y = 0; y < head->height; y++){
				unsigned char *pLine = &(pImageData->data[stride * (head->height-y-1)]);
				memcpy(pLine,mem, stride);
				mem+=stride;
				//...Targa uses BGR type storage. Thats why convert BGR to RGB...
				for(i = 0; i < stride; i += channels){
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}

		//...If the image depth is 16 bits(RGB stored in 1 unsigned short)
		else
		if(head->bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;
			channels = 3;
			stride = channels * head->width;
			pImageData->data = new unsigned char[stride * head->height];
			for(int y = 0; y < head->height; y++)
			{
				unsigned char *pLine = &(pImageData->data[stride * (head->height-y-1)]);
				for(int i = 0; i < head->width; i++)
				{
					pixels = *(unsigned short*)mem;
					mem+=2;
					b = (pixels & 0x1f) << 3;
					g = ((pixels >> 5) & 0x1f) << 3;
					r = ((pixels >> 10) & 0x1f) << 3;
					*pLine++ = r;
					*pLine++ = g;
					*pLine++ = b;
				}
			}
		}	
		else
		if(head->bits == 8)
		{
			if (head->colormap==1)
			{
				ASSERT(head->colormap_entrysize==24);
				char* colormap = mem;
				mem += head->colormap_lenght*head->colormap_entrysize/8;

				channels = 3;
				stride = channels * head->width;
				pImageData->data = new unsigned char[stride * head->height];
				for(int y = 0; y < head->height; y++)
				{
					unsigned char *pLine = &(pImageData->data[stride * (head->height-y-1)]);
					for(int i = 0; i < head->width; i++)
					{
						*pLine++ = colormap[(unsigned char)(*mem)];
						*pLine++ = colormap[(unsigned char)(*mem)];
						*pLine++ = colormap[(unsigned char)(*mem)];
						mem++;
					}
				}
			}
			else
			{
				ASSERT(head->colormap==0);
				ASSERT(head->colormap_entrysize==0);
				char* colormap = mem;
				mem += head->colormap_lenght*head->colormap_entrysize/8;

				channels = 3;
				stride = channels * head->width;
				pImageData->data = new unsigned char[stride * head->height];
				for(int y = 0; y < head->height; y++)
				{
					unsigned char *pLine = &(pImageData->data[stride * (head->height-y-1)]);
					for(int i = 0; i < head->width; i++)
					{
						*pLine++ = colormap[(unsigned char)(*mem)*3+2];
						*pLine++ = colormap[(unsigned char)(*mem)*3+1];
						*pLine++ = colormap[(unsigned char)(*mem)*3];
						mem++;
					}
				}
			}

		}
		else
		{
			TRACE("Error..: Unsupported targa file\nPlease use a 16, 24, or 32 bit targa image\n");
			delete (pImageData);
			return NULL;
		}
	}
	// COMPRESSED File
	else{ 

		TRACE("WARNING: COMPRESSED TGA...IS UPSIDEDOWN\n");

		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = head->bits / 8;
		stride = channels * head->width;
		pImageData->data = new unsigned char[stride * head->height];
		while(i < head->width*head->height)
		{
			rleID = *mem++;
			if(rleID < 128)
			{
				rleID++;
				while(rleID)
				{
					pImageData->data[colorsRead + 0] = mem[2];
					pImageData->data[colorsRead + 1] = mem[1];
					pImageData->data[colorsRead + 2] = mem[0];
					mem +=3;
					if(head->bits == 32)	pImageData->data[colorsRead + 3] = *mem++;
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				rleID -= 127;
				while(rleID)
				{
					pImageData->data[colorsRead + 0] = mem[2];
					pImageData->data[colorsRead + 1] = mem[1];
					pImageData->data[colorsRead + 2] = mem[0];
					mem +=3;
					if(head->bits == 32)	pImageData->data[colorsRead + 3] = *mem++;
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
	}

	pImageData->channels = channels;
	pImageData->sizeX    = head->width;
	pImageData->sizeY    = head->height;

	return pImageData;
}




 