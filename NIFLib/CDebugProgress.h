#pragma once

class CDebugProgress
{
	public:
		CDebugProgress(void);
		~CDebugProgress(void);

		void	Create(int steps);
		void	SetPos(int pos);
		void	SetPos(int pos, const char*);
		void	SetText(const char*);

		void	Step();
		void	Step(const char*);

		static  INT_PTR CALLBACK	MsgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static	DWORD WINAPI		ThreadProc( LPVOID lpParameter);

	private:
		HWND	mhwnd;
		HANDLE	mh_exitevent;
		HANDLE	mh_thread;
};
