#include "StdAfx.h"
#include "CReaderCSV.h"
#include <stdlib.h>


CReaderCSV::CReaderCSV()
{
	mpc_cur_line = NULL;
	mpc_memfile = NULL;
	mb_has_no_index = false;
}


CReaderCSV::CReaderCSV(char*data, size_t size, bool has_no_index/* = false*/)
{
	Open(data, size, has_no_index);
}


CReaderCSV::~CReaderCSV(void)
{
	SAFE_DELETE(mpc_memfile);
}



void	CReaderCSV::Open(char*data, size_t size, bool has_no_index/* = false*/)
{
	mb_has_no_index =has_no_index;

	// Maik Jurkait  08.06.2004 09:49:39
	// ------------------------------------------
	//	reset line pointer
	// ------------------------------------------
	mpc_cur_line = NULL;

	// Maik Jurkait  08.06.2004 09:49:30
	// ------------------------------------------
	// copy block + add eof	
	// ------------------------------------------
	mpc_memfile = new unsigned char[size+1];
	memcpy(mpc_memfile,	data, size);
	mpc_memfile[size]=0;
	SAFE_DELETE(data);
}




bool	CReaderCSV::GotoIndexLine(int i, int sub_nr_of_index /*= 0*/)
{
	if (!GotoFirstLine()) return false;

	while (GetInt(sub_nr_of_index)!=i)
	{
		if (!GotoNextLine()) return false;
	}
	return true;
}




bool	CReaderCSV::GotoFirstLine()
{
	if (!mpc_memfile) return false;

	mpc_cur_line = mpc_memfile;
	mn_cur_line_index = 0;
	mpc_cur_line_pos = mpc_cur_line;

	// Maik Jurkait  08.06.2004 09:50:47
	// ------------------------------------------
	//	eat whitespaces
	// ------------------------------------------
	while (iswspace(*mpc_cur_line) && *mpc_cur_line) mpc_cur_line++;
	if (!*mpc_cur_line) return false;

	if (*mpc_cur_line == ';') 	return GotoNextLine();


	if (!mb_has_no_index)
	{
		while (!isdigit(*GetString(0)))
		{
			if (!GotoNextLine()) return false;
		}
	}
	return true;
}


bool	CReaderCSV::GotoNextLine()
{
	if (!mpc_memfile) return false;

	assert(mpc_cur_line); // no line set


	// Maik Jurkait  08.06.2004 09:50:31
	// ------------------------------------------
	//	already at file end
	// ------------------------------------------
	if (!*mpc_cur_line) return false;


	// Maik Jurkait  08.06.2004 09:50:39
	// ------------------------------------------
	//	Skip line
	// ------------------------------------------
	while (*mpc_cur_line != 0x0d && *mpc_cur_line) mpc_cur_line++;
	if (!*mpc_cur_line) return false;
	mpc_cur_line++;


	// Maik Jurkait  08.06.2004 09:50:47
	// ------------------------------------------
	//	eat whitespaces
	// ------------------------------------------
	while (iswspace(*mpc_cur_line) && *mpc_cur_line) mpc_cur_line++;
	if (!*mpc_cur_line) return false;


	// Maik Jurkait  08.06.2004 09:50:57
	// ------------------------------------------
	//	reset line index
	// ------------------------------------------
	mn_cur_line_index = 0;
	mpc_cur_line_pos = mpc_cur_line;


	// Maik Jurkait  08.06.2004 09:57:20
	// ------------------------------------------
	//	Comment line?
	// ------------------------------------------
	if (*mpc_cur_line == ';') 	return GotoNextLine();

	return true;
}



int		CReaderCSV::GetLineCount()
{
	int n_count = 0;

	if (!GotoFirstLine()) return 0;

	do
	{
		n_count++;
	} while (GotoNextLine());

	return n_count;
}



char*	CReaderCSV::GetString(int index)
{
	assert(mpc_cur_line);
	assert(mpc_cur_line_pos);

	// Maik Jurkait  08.06.2004 09:51:06
	// ------------------------------------------
	//	maybe restart at line begin
	// ------------------------------------------
	if (mn_cur_line_index > index)
	{
		mn_cur_line_index = 0;
		mpc_cur_line_pos = mpc_cur_line;
	}


	// Maik Jurkait  08.06.2004 09:51:35
	// ------------------------------------------
	//	goto sub index	
	// ------------------------------------------
	while (mn_cur_line_index < index)
	{
		while (*mpc_cur_line_pos != ',' && *mpc_cur_line_pos && *mpc_cur_line_pos != 0x0d) mpc_cur_line_pos++;

		mn_cur_line_index++;

		if (!*mpc_cur_line_pos || *mpc_cur_line_pos == 0x0d)
		{
			mstr_returnbuffer[0]=0;
			return mstr_returnbuffer;
		}

		mpc_cur_line_pos++; // skip ','

		while (isspace(*mpc_cur_line_pos)) mpc_cur_line_pos++;
	}

	// Maik Jurkait  08.06.2004 09:51:43
	// ------------------------------------------
	//	Read field
	// ------------------------------------------
	int idx = 0;
	while (*mpc_cur_line_pos != ',' && *mpc_cur_line_pos && *mpc_cur_line_pos != 0x0d)
	{
		mstr_returnbuffer[idx++] = *mpc_cur_line_pos++;
		assert(idx<127);
	}
	mstr_returnbuffer[idx] = 0;

	// Maik Jurkait  08.06.2004 09:51:52
	// ------------------------------------------
	//	eat white space of next subindex
	// ------------------------------------------
	mn_cur_line_index++;
	if (!*mpc_cur_line_pos || *mpc_cur_line_pos == 0x0d)
		return mstr_returnbuffer;

	mpc_cur_line_pos++; // skip ','

	while (isspace(*mpc_cur_line_pos)) mpc_cur_line_pos++;

	return mstr_returnbuffer;
}



int		CReaderCSV::GetInt(int index)
{
	return atoi(GetString(index));
}


float	CReaderCSV::GetFloat(int index)
{
	return (float)atof(GetString(index));
}

