#include "StdAfx.h"
#include "cdirlist.h"
#include "../zlib/zlib.h"



CDirList::CDirList(void)
{
	mb_is_packed_file = false;
	mstr_dirname = NULL;
}


CDirList::~CDirList(void)
{
	Close();
}



void CDirList::Close()
{
	SAFE_DELETE(mstr_dirname);
	mas_files.RemoveAll();
	mas_files.CleanUp();
}




bool CDirList::Open(const char* path, bool b_no_contensread /*=false*/)
{
	// Maik Jurkait  01.06.2004 15:16:55
	// ------------------------------------------
	//	Clear
	// ------------------------------------------
	Close();

	// Maik Jurkait  01.06.2004 15:17:30
	// ------------------------------------------
	//	CHECK if MPK or DIR	
	// ------------------------------------------
	mb_is_packed_file = false;

	if (strlen(path)>3)
	{
		char* p = (char*)&path[strlen(path)-4];
		if ( stricmp(p,".mpk")==0 || stricmp(p,".npk")==0)	mb_is_packed_file = true;
	}


	// Maik Jurkait  01.06.2004 15:18:03
	// ------------------------------------------
	//	Store dir-name
	// ------------------------------------------
	mstr_dirname = new char[strlen(path)+2]; // maybe we have to add '\\'
	strcpy(mstr_dirname,path);

	if (!mb_is_packed_file)
	{
		if (mstr_dirname[strlen(path)-1] != '\\' && mstr_dirname[strlen(path)-1] != '/')
			strcat(mstr_dirname,"\\");
	}


	// Maik Jurkait  01.06.2004 15:19:41
	// ------------------------------------------
	// read contents	
	// ------------------------------------------
	if (mb_is_packed_file)
		return OpenDirectoryMPK();
	else
	{
		if (b_no_contensread) return true;
		return OpenDirectory();
	}
}


bool	CDirList::IsPath(const char* dir_name)
{

	// Maik Jurkait  01.06.2004 17:11:44
	// ------------------------------------------
	//	Maybe a '\\' was added, so generate a temp string
	// ------------------------------------------
	int len = (int)strlen(dir_name);
	char *temp = new char[len+2]; // maybe we have to add '\\'
	strcpy(temp,dir_name);

	char* p = (char*)&dir_name[len-4];
	if ( len>3 && stricmp(p,".mpk")!=0 && stricmp(p,".npk")!=0)	
		if (temp[len-1] != '\\' && temp[len-1] != '/')
				strcat(temp,"\\");

	bool res = stricmp(temp,mstr_dirname)==0;

	delete (temp);

	return res;
}





bool	CDirList::OpenDirectory()
{
	// Maik Jurkait  01.06.2004 15:21:11
	// ------------------------------------------
	//	generate search string
	// ------------------------------------------
	char search[MAX_PATH];
	strcpy(search, mstr_dirname);
	strcat(search,"*.*");


	// Maik Jurkait  01.06.2004 15:22:35
	// ------------------------------------------
	//	search all files
	// ------------------------------------------
	WIN32_FIND_DATA FindFileData;
	HANDLE			hFind;

	hFind = FindFirstFile(search, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return false;

	do
	{
		// Maik Jurkait  01.06.2004 15:25:00
		// ------------------------------------------
		//	ignore: dir's
		// ------------------------------------------
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		// Maik Jurkait  01.06.2004 15:23:21
		// ------------------------------------------
		//	ignore: .mpk + .npk files
		// ------------------------------------------
		if (strlen(FindFileData.cFileName)>3)
		{
			char* p = &FindFileData.cFileName[strlen(FindFileData.cFileName)-4];
			if ( stricmp(p,".mpk")==0 || stricmp(p,".npk")==0)	continue;
		}

		// Maik Jurkait  01.06.2004 15:25:16
		// ------------------------------------------
		//	Add to list
		// ------------------------------------------
		s_entry	new_entry;
		strncpy(new_entry.name,FindFileData.cFileName,99);
		new_entry.name[99]=0;
		new_entry.size_unpacked =  FindFileData.nFileSizeLow;

		mas_files.AddLast(new_entry);

	} while (FindNextFile(hFind,&FindFileData));


	FindClose(hFind);

	return true;
}





bool	CDirList::OpenDirectoryMPK()
{
	// Maik Jurkait  01.06.2004 15:25:25
	// ------------------------------------------
	//	open file
	// ------------------------------------------
	FILE *fp;

	if (!(fp = fopen(mstr_dirname, "rb")))
		return false;


	// Maik Jurkait  01.06.2004 15:25:47
	// ------------------------------------------
	//	Get size
	// ------------------------------------------
	fseek(fp,0,SEEK_END);
	size_t s = ftell(fp);
	fseek(fp,0,SEEK_SET);


	// Maik Jurkait  01.06.2004 15:27:23
	// ------------------------------------------
	//	Read Head
	// ------------------------------------------
	s_head	header;
	fread(&header,sizeof(s_head),1,fp);

	if (header.id != 'KAPM')
	{
		fclose(fp);
		return false;
	}

	// de-shredder
	for(unsigned char i = 0; i < 16; ++i)
	{
		header.data.shredderd[i] ^= i;
	}


	// Maik Jurkait  01.06.2004 15:40:50
	// ------------------------------------------
	//	skip name
	// ------------------------------------------
	fseek(fp,header.data.size_name,SEEK_CUR);
	/*
		USE something like that:
		fread(&name_compresseed,1,header.data.size_name,fp);
		uncompress((Bytef*)name, &l, (Bytef*)name_compresseed, header.data.size_name);

	*/


	// Maik Jurkait  01.06.2004 16:30:34
	// ------------------------------------------
	//	read files list
	// ------------------------------------------
	Bytef*		buffer = new Bytef[header.data.size_dir];
	mas_files.SetSize(header.data.nof_files);
    
	fread(buffer,1,header.data.size_dir,fp);
	uLongf insize = header.data.size_dir; 
	uLongf outsize = header.data.nof_files * sizeof(s_entry);
	long res = uncompress((Bytef*)mas_files.GetData(), &outsize, (Bytef*)buffer, &insize);
	ASSERT(res==0);

	delete(buffer);

	// Maik Jurkait  01.06.2004 16:53:34
	// ------------------------------------------
	//	store current position... this is the base offset
	// ------------------------------------------
	mn_header_size = ftell(fp);

	fclose(fp);


	return true;
}



bool	CDirList::FileExists(const char* filename)
{
	for (int n=0;n<mas_files.Count();n++)
		if (stricmp(mas_files[n].name,filename)==0)
			return true;

	return false;

}


bool	CDirList::ReadFile(const char* filename, char* &buffer, size_t& buffersize)
{
	// Maik Jurkait  13.07.2004 08:32:04
	// ------------------------------------------
	//	rest vars
	// ------------------------------------------
	buffer = NULL;
	buffersize = 0;


	// Maik Jurkait  01.06.2004 16:58:03
	// ------------------------------------------
	//	if MPAK take ReadFileMPK
	// ------------------------------------------
	if (mb_is_packed_file)
		return ReadFileMPK(filename, buffer, buffersize);


	// Maik Jurkait  01.06.2004 16:58:17
	// ------------------------------------------
	//	generate full pathname
	// ------------------------------------------
	char pathname[1024];
	strcpy(pathname,mstr_dirname);
	strcat(pathname,filename);


	// Maik Jurkait  01.06.2004 16:58:25
	// ------------------------------------------
	//	Open
	// ------------------------------------------
	FILE* fp;
	if (!(fp = fopen(pathname, "rb")))			return false;

	// Maik Jurkait  01.06.2004 16:58:28
	// ------------------------------------------
	//	Get Size
	// ------------------------------------------
	fseek(fp,0,SEEK_END);
	buffersize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	// ... here we could check what is stored in ms_files list..but this will only cost performance


	// Maik Jurkait  01.06.2004 16:58:32
	// ------------------------------------------
	//	Read whole buffer
	// ------------------------------------------
	buffer = new char[buffersize];
	buffersize = fread(buffer,1,buffersize,fp);
	fclose(fp);
	return true;
}





bool	CDirList::ReadFileMPK(const char* file, char* &buffer, size_t& buffersize)
{
	ASSERT(mb_is_packed_file);

	// Maik Jurkait  01.06.2004 16:59:42
	// ------------------------------------------
	//	Find entry
	// ------------------------------------------
	int n_index;
	for (n_index=0;n_index<mas_files.Count();n_index++)
		if (stricmp(mas_files[n_index].name,file)==0) break;

	if (n_index>=mas_files.Count()) return false;


	// Maik Jurkait  01.06.2004 17:00:00
	// ------------------------------------------
	//	open pak
	// ------------------------------------------
	FILE* fp;
	if (!(fp = fopen(mstr_dirname, "rb")))			return false;


	// Maik Jurkait  01.06.2004 17:00:31
	// ------------------------------------------
	//	go to right position
	// ------------------------------------------
	fseek(fp,mas_files[n_index].offset + mn_header_size,SEEK_SET);

	// Maik Jurkait  01.06.2004 17:01:28
	// ------------------------------------------
	//	create read + decompress buffer
	// ------------------------------------------
	buffersize = mas_files[n_index].size_unpacked;
	Bytef* inbuf = (Bytef*)malloc(mas_files[n_index].size_packed);
	buffer = new char[buffersize];


	// Maik Jurkait  01.06.2004 17:02:11
	// ------------------------------------------
	//	Read compressed
	// ------------------------------------------
	fread(inbuf,mas_files[n_index].size_packed,1,fp);
	fclose(fp);


	// Maik Jurkait  01.06.2004 17:02:22
	// ------------------------------------------
	//	decompress
	// ------------------------------------------
	uLongf insize = mas_files[n_index].size_packed;
	uLongf l = buffersize;
	long res = uncompress((Bytef*)(buffer), &l, inbuf, &insize);
	ASSERT(res == Z_OK);// uncompress error
	free(inbuf);

	return (res == Z_OK);
}


