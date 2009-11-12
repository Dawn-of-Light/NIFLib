#pragma once

/**
*	Directory List
*	Mangages File access indepently if it's a Directory or a mpk/npk
**/
class CDirList
{
	private:

		#pragma pack(1)
		struct s_entry
		{
			char name[0x100];

			unsigned int time;
			unsigned int attrib;
			unsigned int base_offset;
			unsigned int size_unpacked;
			unsigned int offset;
			unsigned int size_packed;
			unsigned int crc;
		};


		struct s_head
		{
			unsigned id;				// == MPAK
			unsigned char version;		// == 2 .. maybe version

			union
			{
				unsigned char shredderd[16];
				struct
				{
					int  crc;
					int  size_dir;
					int  size_name;
					int	 nof_files;
				};
			} data;
		};
		#pragma pack ()


	public:
		CDirList(void);
		~CDirList(void);

		/**
		*	set b_no_contensread to true if you just need it to read a file
		**/
		bool	Open(const char*, bool b_no_contensread=false);
		void	Close();

		bool	IsPath(const char*);

		bool	FileExists(const char* filename);

		bool	ReadFile(const char* file, char* &buffer, size_t& buffersize);

	private:

		bool	OpenDirectory();
		bool	OpenDirectoryMPK();
		bool	ReadFileMPK(const char* file, char* &buffer, size_t& buffersize);


		bool				mb_is_packed_file;
		char*				mstr_dirname;
		unsigned int		mn_header_size;

		TArray <s_entry>	mas_files;

/*
		int		GetNOFFiles();
		void	GetFile(int n, LPWIN32_FIND_DATA);


		bool	ReadFileText(int n, char* &buffer, size_t& buffersize);
		bool	ReadFileText(const CString& file, char* &buffer, size_t& buffersize);

*/

};
