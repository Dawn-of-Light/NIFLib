#include "StdAfx.h"
#include "cdebugprogress.h"
#include "resource.h"


extern HINSTANCE	gh_instance;

CDebugProgress::CDebugProgress(void)
{
	mh_exitevent = CreateEvent(NULL,TRUE,FALSE,NULL);
	mhwnd = NULL;
}

CDebugProgress::~CDebugProgress(void)
{
	SetEvent(mh_exitevent);
	Sleep(0);
	WaitForSingleObject(mh_thread,1000);

	CloseHandle(mh_thread);
	CloseHandle(mh_exitevent);
}


void	CDebugProgress::Create(int steps)
{

	if (mhwnd)
	{
		SendMessage(GetDlgItem(mhwnd,IDC_PROGRESS1),	PBM_SETRANGE,0,MAKELPARAM(0, steps));
		return;
	}

	mh_thread = CreateThread(NULL,0,ThreadProc,this,0,NULL);
	Sleep(100);

	if (!mhwnd) return;

	ShowWindow(mhwnd,SW_SHOW);
	SendDlgItemMessage(mhwnd,IDC_PROGRESS1,	PBM_SETRANGE,0,MAKELPARAM(0, steps));
	SendDlgItemMessage(mhwnd,IDC_PROGRESS1,	PBM_SETSTEP,1,0);

}


DWORD WINAPI CDebugProgress::ThreadProc( LPVOID lpParameter)
{
	HANDLE h_exit = ((CDebugProgress*)lpParameter)->mh_exitevent;

	HWND wnd = CreateDialog(gh_instance,MAKEINTRESOURCE(IDD_PROGRESS),NULL,MsgProc);
	((CDebugProgress*)lpParameter)->mhwnd = wnd;

	MSG msg;
	do
	{
		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
	        TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	while (WaitForSingleObject(h_exit,0) != WAIT_OBJECT_0);

	DestroyWindow(wnd);


	return 1;
}



void	CDebugProgress::SetPos(int pos)
{
	SendDlgItemMessage(mhwnd,IDC_PROGRESS1,	PBM_SETPOS,pos,0);
	UpdateWindow(mhwnd);
}

void	CDebugProgress::SetText(const char*text)
{
	SetDlgItemText(mhwnd,IDC_TEXT,text);
	UpdateWindow(mhwnd);
}

void	CDebugProgress::SetPos(int pos, const char*text)
{
	SetPos(pos);
	SetText(text);
}

void	CDebugProgress::Step()
{
	SendDlgItemMessage(mhwnd,IDC_PROGRESS1,	PBM_STEPIT,0,0);
}

void	CDebugProgress::Step(const char*text)
{
	Step();
	SetText(text);
}


INT_PTR CALLBACK	CDebugProgress::MsgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
				return TRUE;
	}

	return FALSE;
}
