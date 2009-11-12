#include "StdAfx.h"
#include ".\creaderini.h"
#include <stdlib.h>

CReaderINI::CReaderINI(void)
{
	mpc_memfile = NULL;
	mpc_mempointer = NULL;
	mpc_memcurrentsection = NULL;
}

CReaderINI::~CReaderINI(void)
{
	SAFE_DELETE(mpc_memfile);
}

CReaderINI::CReaderINI(char*data, size_t size)
{
	Open(data,size);
}

void	CReaderINI::Open(char*data, size_t size)
{
	assert(data); // no line set
	
	// Maik Jurkait  08.06.2004 09:49:30
	// ------------------------------------------
	// copy block + add eof	
	// ------------------------------------------
	mpc_memfile = new unsigned char[size+1];
	memcpy(mpc_memfile,	data, size);
	mpc_memfile[size]=0;
	delete (data);

	mpc_mempointer = mpc_memfile;
	mpc_memcurrentsection = NULL;
}


void	CReaderINI::GotoStart()
{
	mpc_mempointer = mpc_memfile;
}


void	CReaderINI::GotoNextSection()
{
	while (*mpc_mempointer && *mpc_mempointer !='[')
	{
		while (iswspace(*mpc_mempointer))		mpc_mempointer++;
		while (*mpc_mempointer && *mpc_mempointer!=0x0a) mpc_mempointer++;
		while (iswspace(*mpc_mempointer))		mpc_mempointer++;
		mpc_memcurrentsection = mpc_mempointer;
	}
}


void	CReaderINI::FindSection(const char*name)
{

	char temp[128];
	strcpy(temp,"[");
	strcat(temp,name);
	strcat(temp,"]");
	int len = (int)strlen(temp);

	GotoStart();
	do
	{
		GotoNextSection();
		if (strnicmp((char*)mpc_mempointer,temp,len)==0)
		{
			while (*mpc_mempointer && *mpc_mempointer!=0x0a) mpc_mempointer++;
			while (iswspace(*mpc_mempointer))			mpc_mempointer++;
			mpc_memcurrentsection = mpc_mempointer;
			return;
		}
		if (*mpc_mempointer) mpc_mempointer++;

	} while (*mpc_mempointer);
}


bool	CReaderINI::FindEntry(const char*name)
{
	assert(mpc_memcurrentsection);
	mpc_mempointer = mpc_memcurrentsection;

	do
	{
		while (iswspace(*mpc_mempointer))			mpc_mempointer++;
		if (*mpc_mempointer == '[') return false;

		if (strnicmp((char*)mpc_mempointer,name,strlen(name))==0)
		{
			mpc_mempointer += strlen(name);
			while (iswspace(*mpc_mempointer)&&*mpc_mempointer!=0x0a)	mpc_mempointer++;
			if (*mpc_mempointer == '=') mpc_mempointer++;
			while (iswspace(*mpc_mempointer)&&*mpc_mempointer!=0x0a)	mpc_mempointer++;

			return true;
		}

		while (*mpc_mempointer && *mpc_mempointer!=0x0a)			mpc_mempointer++;

	} while(*mpc_mempointer);

	return false;
}




void	CReaderINI::GetSectionList(TArray<char*>& stringarray)
{
	GotoStart();

	for (int t=0;t<stringarray.Count();t++)
		delete (stringarray[t]);
	stringarray.RemoveAll();

	do
	{
		GotoNextSection();
		if (!*mpc_mempointer) return;
		mpc_mempointer++;
		unsigned char* endpos=mpc_mempointer;
		while (*endpos && *endpos!=']' && *endpos !=0x0a) endpos++;

		char* name = new char[(int)(endpos-mpc_mempointer)+1];
		memcpy(name,mpc_mempointer,(int)(endpos-mpc_mempointer));
		name[(int)(endpos-mpc_mempointer)]=0;

		stringarray.AddLast(name);

	} while(*mpc_mempointer);
}


char*	CReaderINI::GetEntryStringThisSection(const char* key, const char* default_value)
{
	if (FindEntry(key))
	{
		unsigned char*endpos = mpc_mempointer;
		while (*endpos && *endpos!=0x0a) endpos++;
		endpos--;
		while (endpos>=mpc_mempointer && iswspace(*endpos)) endpos--;
		
		int len = (int)(endpos-mpc_mempointer)+1;
		char* value = new char[len+1];
		memcpy(value, mpc_mempointer,len);
		value[len]=0;

		return value;
	}
	

	char*defval = new char[strlen(default_value)+1];
	strcpy(defval,default_value);
	return defval;
}


char*	CReaderINI::GetEntryString(const char* section, const char* key, const char* default_value)
{
	FindSection(section);

	return GetEntryStringThisSection(key,default_value);
}


int		CReaderINI::GetEntryInt(const char* section, const char* key, int default_value)
{
	char* str_val = GetEntryString(section,key,"");

	int result = default_value;
	if (str_val[0]!=0) result = atoi(str_val);
	delete str_val;

	return result;
}

float	CReaderINI::GetEntryFloat(const char* section, const char* key, float default_value)
{
	char* str_val = GetEntryString(section,key,"");

	float result = default_value;
	if (str_val[0]!=0) result = (float)atof(str_val);
	delete str_val;

	return result;
}

int		CReaderINI::GetEntryIntThisSection(const char* key, int default_value)
{
	char* str_val = GetEntryStringThisSection(key,"");

	int result = default_value;
	if (str_val[0]!=0) result = atoi(str_val);
	delete str_val;

	return result;
}


float	CReaderINI::GetEntryFloatThisSection(const char* key, float default_value)
{
	char* str_val = GetEntryStringThisSection(key,"");

	float result = default_value;
	if (str_val[0]!=0) result = (float)atof(str_val);
	delete str_val;

	return result;
}
