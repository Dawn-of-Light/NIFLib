#pragma once

#ifdef ANALYSE_NIF

class CNode;



/**
*	internal tree
*	used filter nodes
*	since it's just for debug and only for debugwindow everything is public
**/
class	CDebugTreeData
{
	public:
		char*					name;
		int						image;
		CNode*					pc_node;	
		TArray<CDebugTreeData*>	childs;

		CDebugTreeData()
		{
			name = NULL;
			image = 0;
			pc_node = NULL;	
		}

		~CDebugTreeData()
		{
			SAFE_DELETE(name);
			for(int t=0;t<childs.Count();t++)
				delete(childs[t]);
		}

		void	CDebugTreeData::AddChild(CDebugTreeData* pc_data)
		{
			childs.AddLast(pc_data);
		}
};


#define SHOW_ONLY_VISIBLE	(1<<0)
#define SHOW_NO_CLASS_INFO	(1<<1)

class CDebugWindow
{
	private:




	public:
		CDebugWindow(void);
		~CDebugWindow(void);

		void	Create();
		void	Show();
		void	Hide();

		void	TreeClear();
		void	TreeBuild(CNode*);
		void	ShowTree();

		static  INT_PTR CALLBACK	MsgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static	void*		TreeAdd(void*, CNode* node, char* text, int image);


	private:
		void	ModelUpdate();
		void	UpdateMenuState(HWND wnd);
		void	Matrix(HWND wnd,float fx,float fy,float fz, float rot);

		void	ShowTree(HTREEITEM,CDebugTreeData*);
		bool	IsShowFlagOkay(CDebugTreeData*);
		bool	IsVisible(CDebugTreeData*);

		static	void	HighlightCur(HWND ,bool highlight);
		static	void	Highlight(TVITEM& ,bool highlight);
		static	void	SwitchVisibiliy(HWND hwndDlg);

		HWND			mhwnd;
		CDebugTreeData*	mpc_tree_root;

		DWORD			mdw_display_flags;
		bool			mb_no_selection_view;
		DWORD			mdw_selection_render_flags;
		bool			mb_selection_recursive;
};

#endif