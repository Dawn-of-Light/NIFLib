#pragma once

class CReaderCSV
{
	public:
		CReaderCSV();
		CReaderCSV(char*data, size_t size, bool has_no_index = false);
		~CReaderCSV(void);

		void	Open(char*data, size_t size, bool has_no_index = false);


		/**
		*	go to the line with given index
		*	\param int i				index to find
		*	\param int sub_nr_of_index	which index contains the index (nrl:0)
		**/
		bool	GotoIndexLine(int i, int sub_nr_of_index = 0);

		/**
		*	go to first line
		**/
		bool	GotoFirstLine();

		/**
		*	go to next line
		**/
		bool	GotoNextLine();

		/**
		*	count the line
		*	!! resets current line !!
		**/
		int		GetLineCount();

		/**
		*	Get a string
		*	Before you have to select a line (with one of the Goto..Line() functions)
		*	\param int index  colume
		**/
		char*	GetString(int index);

		/**
		*	Get a int
		*	Before you have to select a line (with one of the Goto..Line() functions)
		*	\param int index  colume
		**/
		int		GetInt(int index);

		/**
		*	Get a float
		*	Before you have to select a line (with one of the Goto..Line() functions)
		*	\param int index  colume
		**/
		float	GetFloat(int index);


	private:


		/**
		*	if true it's not required that first char in line is a digit
		**/
		bool			mb_has_no_index;

		/**
		*	file pointer
		**/
		unsigned char*	mpc_memfile;

		/**
		*	current line
		**/
		unsigned char*	mpc_cur_line;

		/**
		*	
		**/
		int		mn_cur_line_index;

		/**
		*	
		**/
		unsigned char*	mpc_cur_line_pos;


		/**
		*	holds the return string
		*	to keep memory-managment easier
		**/
		char	mstr_returnbuffer[128];
};
