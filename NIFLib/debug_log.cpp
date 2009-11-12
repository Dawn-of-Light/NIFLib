#include "stdafx.h"
#include "debug_log.h"


#ifdef _DEBUG
	#include "TArray.h"
	#include <crtdbg.h>
	#pragma comment(lib,"version.lib")
#endif 



namespace debug
{
// just to make sure nothing from here is added in release
#ifdef _DEBUG


// log filename
#define LOGFILE	"log.txt"

// if defined old log will be deleted
#define CREATE_NEW_LOG


// store time counter
DWORD	dw_start_tick;

bool  b_newline = true;


void	_initlog()
{
	//_CrtSetBreakAlloc(366277);

	// since nobody is perfect: enable leak checking
	{
		int temp=_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			temp |= _CRTDBG_ALLOC_MEM_DF;
			temp |= _CRTDBG_LEAK_CHECK_DF ;
			temp &=~_CRTDBG_CHECK_CRT_DF;
			temp &=~_CRTDBG_DELAY_FREE_MEM_DF;
		_CrtSetDbgFlag(temp);
	}


	// store start-time
	dw_start_tick = GetTickCount();

#ifdef CREATE_NEW_LOG
	DeleteFile(LOGFILE);
#endif


	// Generate HEADER
	_logoutfile("----------------------------------------------------------------------\n");

	// EXE-Info
	char buffer[MAX_PATH];
	if (GetModuleFileName( 0, buffer, MAX_PATH ))
	{
		char *p;
		for ( p = buffer + strlen( buffer ) - 1; p >= buffer; -- p )
		{
			// locate the rightmost path separator
			if ( *p == '\\' || *p == '/' || *p == ':' )		{	p++;	break;	}
		}


		HANDLE						h_file;
		BY_HANDLE_FILE_INFORMATION	h_info;

		h_file = CreateFile(buffer, 0, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING,0,NULL);

		if (h_file != INVALID_HANDLE_VALUE) 
		{
			GetFileInformationByHandle(h_file, &h_info);
			CloseHandle(h_file);
		}

		SYSTEMTIME f_time;
		FileTimeToSystemTime(&h_info.ftLastWriteTime,&f_time);
	

		_logoutfile("%s (%02i.%02i.%04i  %02i:%02i:%02i)\n", p,
						f_time.wDay, f_time.wMonth, f_time.wYear, 
						f_time.wHour, f_time.wMinute, f_time.wSecond);

		DWORD size, dummy;
	    GetModuleFileName(NULL, buffer, MAX_PATH);
   		size = GetFileVersionInfoSize(buffer, &dummy);
        		if (size)
				{
        			char *buf = (char *)malloc(size);
        			GetFileVersionInfo(buffer, NULL, size, buf);
        			VS_FIXEDFILEINFO *qbuf;
        			UINT len;
        			if (VerQueryValue(buf, "\\", (void **)&qbuf, &len)) {
						_logoutfile( "Version %d.%02d.%02d.%04d\n", 
								HIWORD(qbuf->dwFileVersionMS),
								LOWORD(qbuf->dwFileVersionMS), 
								HIWORD(qbuf->dwFileVersionLS), 
								LOWORD(qbuf->dwFileVersionLS));
	       			}
        			free(buf);
        		}

		}


	// OS-info
	char* osstrings[] =
	{
		"unknown OS",
		"Windows 95",
		"Windows 98",
		"Windows Me",
		"Windows NT 3.51",
		"Windows NT 4.0",
		"Windows 2000",
		"Windows XP",
		"Windows Server 2003 family"
	};


	OSVERSIONINFO VersionInfo;
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&VersionInfo);

	int osidx = 0;
	switch (VersionInfo.dwMajorVersion)
	{
		case 3: if (VersionInfo.dwMinorVersion==51) osidx = 4;
				break;
		case 4: switch (VersionInfo.dwMinorVersion)
				{
					case 0: osidx = 1; 
							if (VersionInfo.dwPlatformId ==VER_PLATFORM_WIN32_NT) osidx = 5; 
							break;
					case 10: osidx = 2; break;
					case 90: osidx = 3; break;
				}
				break;
		case 5:
				switch (VersionInfo.dwMinorVersion)
				{
					case 0: osidx = 6; break;
					case 1: osidx = 7; break;
					case 2: osidx = 8; break;
				}
				break;
	}

	char temp[1024];
	sprintf(temp,"%s (v%i.%i Build:%i %s)\n",osstrings[osidx],VersionInfo.dwMajorVersion,VersionInfo.dwMinorVersion,
			VersionInfo.dwBuildNumber,VersionInfo.szCSDVersion);
	_logoutfile(temp);

	_logoutfile("----------------------------------------------------------------------\n");


	// Start WINDOW-LOG-Thread - only use it if something went completly wrong
//	_protocol_windows_init();
}



void	_endlog()
{
	_protocol_windows_end();
}


void	_logoutfile(char* fmt,...)
{
	va_list		ap;				
	if (fmt == NULL)	return;					

	FILE* outf = fopen(LOGFILE,"at");
	fprintf(outf,"%08i: ",GetTickCount()-dw_start_tick);
	va_start(ap, fmt);
		vfprintf(outf, fmt, ap);
	va_end(ap);

	fclose(outf);
}


void	_logout(const char*file, int line, char* fmt,...)
{
	char debugline[1024];

	va_list		ap;				
	if (fmt == NULL)	return;					

	FILE* outf = fopen(LOGFILE,"at");

	if (b_newline)
	{
		fprintf(outf,"%s(%i): ",file,line);
		sprintf(debugline,"%s(%i): ",file,line); OutputDebugString(debugline);
	}
	b_newline = false;


	va_start(ap, fmt);
		vsprintf(debugline, fmt, ap);
	va_end(ap);

	fprintf(outf, "%s",debugline);
	OutputDebugString(debugline);
	fclose(outf);

	if (strchr(debugline,'\n')!=NULL) b_newline = true;
}



// ---------------------------------------------------------------------------------------
// now follows a quick&hacked window enumeration thread
// DON'T USE IT  -  it's more just for me if someone good strange probs :)
// there are some bugs inside (memory-leak and some timing probs)
// ---------------------------------------------------------------------------------------


HANDLE log_thread = NULL;


TArray <char*>	ga_windowclasses;
TArray <char*>	ga_windownames;
TArray <bool>	a_found;


BOOL CALLBACK EnumWindowsProc(HWND hwnd,   LPARAM lParam )
{
	BOOL writelog = (BOOL)lParam;

	char* buffer_class = new char[256];
	if (buffer_class == NULL)	{ _logoutfile("can't allocate memory"); return FALSE;}
	char* buffer_name  = new char[256];
	if (buffer_name == NULL)	{ delete [] (buffer_class); _logoutfile("can't allocate memory"); return FALSE;}
	GetClassName(hwnd,buffer_class,255);
	GetWindowText(hwnd,buffer_name,255);

	int idx;
	for (idx=0;idx<ga_windowclasses.Count();idx++)
		if ((strcmp(ga_windowclasses[idx],buffer_class)==0) &&
			(strcmp(ga_windownames[idx],buffer_name)==0)) break;

	if (idx>=ga_windowclasses.Count())
	{
		if (writelog)
		{
			_logoutfile("WINDOWS: New Window: Name=\"%s\" Class=\"%s\"",buffer_name, buffer_class);
		}

		ga_windowclasses.AddLast(buffer_class);
		ga_windownames.AddLast(buffer_name);
		a_found.AddLast(true);
	}
	else
	{
		a_found[idx] = true;
		delete [] (buffer_class);
		delete [] (buffer_name);
		
	}



	return TRUE;
}


void	_protocol_windows()
{
	EnumWindows(EnumWindowsProc,(LPARAM)TRUE);
}


DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
	do
	{
		a_found.SetSize(ga_windowclasses.Count());
		for (int t=0;t<ga_windowclasses.Count();t++)	a_found[t]=false;

		EnumWindows(EnumWindowsProc,(LPARAM)TRUE);

		// closed windows
		for (int t=0;t<ga_windowclasses.Count();t++)
			if (a_found[t]==false)
			{
				_logoutfile("WINDOWS: Closed Window: Name=\"%s\" Class=\"%s\"",ga_windownames[t], ga_windowclasses[t]);
				delete [] (ga_windownames[t]);
				delete [] (ga_windowclasses[t]);
				ga_windownames.Delete(t);
				ga_windowclasses.Delete(t);
				a_found.Delete(t);
				t--;
			}

		Sleep(5);
	}
	while(1);

	return 0;
}


void	_protocol_windows_init()
{
	EnumWindows(EnumWindowsProc,(LPARAM)FALSE);
	log_thread = CreateThread(NULL,0,ThreadProc,NULL,0,NULL);
}

void	_protocol_windows_end()
{
	if (log_thread!=NULL)
	{
		CloseHandle(log_thread);
	}

	int idx;
	for (idx=0;idx<ga_windowclasses.Count();idx++)
	{
		delete [] (ga_windowclasses[idx]);
		delete [] (ga_windownames[idx]);
	}

}


#endif

} // end namespace