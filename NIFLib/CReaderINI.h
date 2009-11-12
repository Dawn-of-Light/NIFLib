#pragma once

class CReaderINI
{
	public:
		CReaderINI(void);
		CReaderINI(char*data, size_t size);
		~CReaderINI(void);

		void	Open(char*data, size_t size);

		void	GetSectionList(TArray<char*>& stringarray);

		char*	GetEntryString(const char* section, const char* key, const char* default_value);
		int		GetEntryInt(const char* section, const char* key, int default_value);
		float	GetEntryFloat(const char* section, const char* key, float default_value);

		char*	GetEntryStringThisSection(const char* key, const char* default_value);
		int		GetEntryIntThisSection(const char* key, int default_value);
		float	GetEntryFloatThisSection(const char* key, float default_value);

	private:
		void	GotoStart();
		void	GotoNextSection();
		void	FindSection(const char*);
		bool	FindEntry(const char*);


	private:
		/**
		*	file memory block
		**/
		unsigned char*	mpc_memfile;

		/**
		*	file pointer runner
		**/
		unsigned char*	mpc_mempointer;

		/**
		*	file pointer current section
		**/
		unsigned char*	mpc_memcurrentsection;

};
