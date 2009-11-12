#include "StdAfx.h"
#include "CDebugWindow.h"
#include "resource.h"
#include "CNiNode.h"



#ifdef ANALYSE_NIF

/**
*	since later this window is only in debug version
*   lib is included here
**/
#include "commctrl.h"
#pragma comment (lib,"comctl32")


extern HINSTANCE	gh_instance;
CDebugWindow*		pc_current_win;

CDebugWindow::CDebugWindow(void)
{
	pc_current_win = this;
	mhwnd = NULL;
	mpc_tree_root = new CDebugTreeData();
	mdw_selection_render_flags = 0;
	mb_selection_recursive = true;
	mb_no_selection_view = false;
	mdw_display_flags = 0;
}

CDebugWindow::~CDebugWindow(void)
{
	delete mpc_tree_root;
	pc_current_win = NULL;
}


void	CDebugWindow::Create()
{
	assert(!mhwnd);

	// Maik Jurkait  02.06.2004 12:30:57
	// ------------------------------------------
	//	Create the window
	// ------------------------------------------
	InitCommonControls();
	mhwnd = CreateDialog(gh_instance,MAKEINTRESOURCE(IDD_DEBUG_WIN),NULL,MsgProc);
	if (!mhwnd) return;


	// Maik Jurkait  02.06.2004 12:52:06
	// ------------------------------------------
	//	Reposition
	// ------------------------------------------
	RECT r;
	GetWindowRect(mhwnd,&r);
	SetWindowPos(mhwnd,NULL,	GetSystemMetrics(SM_CXFULLSCREEN)-GetSystemMetrics(SM_CXBORDER)-r.right+r.left,
								GetSystemMetrics(SM_CYFULLSCREEN)-GetSystemMetrics(SM_CYBORDER)-r.bottom+r.top,
								0,0,
								SWP_NOZORDER | SWP_NOSIZE);


	// Maik Jurkait  02.06.2004 12:52:20
	// ------------------------------------------
	//	Add icons
	// ------------------------------------------
	HBITMAP hbmp;     // handle to bitmap 
	HIMAGELIST himl = ImageList_Create(16,16,ILC_MASK|ILC_COLOR24,16,4); 
    hbmp = LoadBitmap(gh_instance, MAKEINTRESOURCE(IDB_NODE_ICONS)); 
    ImageList_AddMasked(himl, hbmp, RGB(255,0,255)); 
	ImageList_SetBkColor(himl,CLR_NONE);
    DeleteObject(hbmp); 
	
	ImageList_SetOverlayImage(himl,28,TICST_INVESTIGATE);
	ImageList_SetOverlayImage(himl,29,TICST_UNKNOWNDATA);
	ImageList_SetOverlayImage(himl,30,TICST_UNKNOWNDATA_LISTED);
	ImageList_SetOverlayImage(himl,31,TICST_UNKNOWNDATA_NOT_CRITICAL);

	SendMessage(GetDlgItem(mhwnd,IDC_OBJ_TREE),	TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)himl);
}



void	CDebugWindow::Show()
{
	ShowWindow(mhwnd,SW_SHOW);
}


void	CDebugWindow::Hide()
{
	ShowWindow(mhwnd,SW_HIDE);
}


void	CDebugWindow::ModelUpdate()
{
	if (!mpc_tree_root) return;
	for (int t=0;t<mpc_tree_root->childs.Count();t++)
		if (mpc_tree_root->childs[t]->pc_node)
			mpc_tree_root->childs[t]->pc_node->UpdateStatic();
}


void	CDebugWindow::SwitchVisibiliy(HWND hwndDlg)
{
	HWND tree = GetDlgItem(hwndDlg,IDC_OBJ_TREE);

	// Get selection
	HTREEITEM hitem = TreeView_GetSelection(tree);
	if (hitem==NULL) return;

	// Get CNode
	TVITEM it;
	it.mask = TVIF_PARAM;
	it.hItem = hitem;
	TreeView_GetItem(tree,&it);

	CDebugTreeData* pts = (CDebugTreeData*)it.lParam;

	if (pts && pts->pc_node != NULL) 
	{
		if (pts->pc_node->IsClass("NiNode"))
		{
			((CNiNode*)pts->pc_node)->SetVisible(!((CNiNode*)pts->pc_node)->IsVisible());
			pc_current_win->ModelUpdate();
		}
	}
	
}


void	CDebugWindow::Matrix(HWND hwndDlg,float fx,float fy,float fz, float rot)
{
	HWND tree = GetDlgItem(hwndDlg,IDC_OBJ_TREE);

	// Get selection
	HTREEITEM hitem = TreeView_GetSelection(tree);
	if (hitem==NULL) return;

	// Get CNode
	TVITEM it;
	it.mask = TVIF_PARAM;
	it.hItem = hitem;
	TreeView_GetItem(tree,&it);

	if (it.lParam != NULL)
	{
		CDebugTreeData* pts = (CDebugTreeData*)it.lParam;
		if (pts->pc_node != NULL) 
		{
			if (stricmp(pts->pc_node->GetClassName(),"NiNode")==0)
			{
				CNiNode*pc = (CNiNode*)pts->pc_node;
				pc->GetMatrix().AddScale(fx,fy,fz);
				pc->GetMatrix().AddRotation(0,0,rot);

				// force tree
				ModelUpdate();
			}
		}
	}

}


void	CDebugWindow::HighlightCur(HWND hwndDlg, bool highlight)
{
	HWND tree = GetDlgItem(hwndDlg,IDC_OBJ_TREE);

	// Get selection
	HTREEITEM hitem = TreeView_GetSelection(tree);
	if (hitem==NULL) return;

	// Get CNode
	TVITEM it;
	it.mask = TVIF_PARAM;
	it.hItem = hitem;
	TreeView_GetItem(tree,&it);

	Highlight(it , highlight);
}


void	CDebugWindow::Highlight(TVITEM& item ,bool highlight)
{
	if (item.lParam != NULL)
	{
		CDebugTreeData* pts = (CDebugTreeData*)item.lParam;
		if (pts->pc_node != NULL) 
		{
			DWORD cur_flags = pts->pc_node->GetRenderFlags();
			DWORD new_flags = cur_flags;
			if (highlight && (!pc_current_win->mb_no_selection_view))
				new_flags |= pc_current_win->mdw_selection_render_flags;
			else
				new_flags &= ~(pc_current_win->mdw_selection_render_flags);

			if (new_flags != cur_flags)
			{
				pts->pc_node->SetRenderFlags(new_flags,	pc_current_win->mb_selection_recursive);
				pc_current_win->ModelUpdate();
			}
		}
	}
}


void	CDebugWindow::UpdateMenuState(HWND hwnd)
{
	HMENU menu = GetMenu(hwnd);

	// --------- SELECTION MENU
	if (pc_current_win->mb_no_selection_view)
	{
		CheckMenuItem(menu,ID_SELECTIONS_DONTSHOW, MF_BYCOMMAND|MF_CHECKED);
		EnableMenuItem( menu,ID_SELECTIONS_SHOWBOUNDING,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem( menu,ID_SELECTIONS_SHOWSELECTED,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem( menu,ID_SELECTIONS_RECURSIVE,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		CheckMenuItem(menu,ID_SELECTIONS_DONTSHOW, MF_BYCOMMAND|MF_UNCHECKED);
		EnableMenuItem( menu,ID_SELECTIONS_SHOWBOUNDING,MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem( menu,ID_SELECTIONS_SHOWSELECTED,MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem( menu,ID_SELECTIONS_RECURSIVE,MF_BYCOMMAND | MF_ENABLED);
	}

	if (mdw_selection_render_flags & RF_SHOW_BOUNDINGBOX)
			CheckMenuItem(menu,ID_SELECTIONS_SHOWBOUNDING, MF_BYCOMMAND|MF_CHECKED);
	else
			CheckMenuItem(menu,ID_SELECTIONS_SHOWBOUNDING, MF_BYCOMMAND|MF_UNCHECKED);

	if (mdw_selection_render_flags & RF_SHOW_SELECTED)
			CheckMenuItem(menu,ID_SELECTIONS_SHOWSELECTED, MF_BYCOMMAND|MF_CHECKED);
	else
			CheckMenuItem(menu,ID_SELECTIONS_SHOWSELECTED, MF_BYCOMMAND|MF_UNCHECKED);

	if (mb_selection_recursive)
			CheckMenuItem(menu,ID_SELECTIONS_RECURSIVE, MF_BYCOMMAND|MF_CHECKED);
	else
			CheckMenuItem(menu,ID_SELECTIONS_RECURSIVE, MF_BYCOMMAND|MF_UNCHECKED);

	// ----------- VIEW Menu
	if (mdw_display_flags & SHOW_ONLY_VISIBLE)
			CheckMenuItem(menu,ID_SHOW_ONLYVISIBLE, MF_BYCOMMAND|MF_CHECKED);
	else
			CheckMenuItem(menu,ID_SHOW_ONLYVISIBLE, MF_BYCOMMAND|MF_UNCHECKED);

	

}



INT_PTR CALLBACK CDebugWindow::MsgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
				pc_current_win->UpdateMenuState(hwndDlg);
				return TRUE;


		case WM_COMMAND:
			if (LOWORD(wParam)==IDCANCEL || LOWORD(wParam)==IDOK)	ShowWindow(hwndDlg,SW_HIDE);

			if (LOWORD(wParam)==ID_SELECTIONS_DONTSHOW)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mb_no_selection_view = !pc_current_win->mb_no_selection_view;
				pc_current_win->UpdateMenuState(hwndDlg);
				HighlightCur(hwndDlg, true);
			};

			if (LOWORD(wParam)==ID_SELECTIONS_SHOWBOUNDING)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mdw_selection_render_flags ^= RF_SHOW_BOUNDINGBOX;
				pc_current_win->UpdateMenuState(hwndDlg);
				HighlightCur(hwndDlg, true);
			};

			if (LOWORD(wParam)==ID_SELECTIONS_SHOWSELECTED)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mdw_selection_render_flags ^= RF_SHOW_SELECTED;
				pc_current_win->UpdateMenuState(hwndDlg);
				HighlightCur(hwndDlg, true);
			};

			if (LOWORD(wParam)==ID_SELECTIONS_RECURSIVE)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mb_selection_recursive = ! pc_current_win->mb_selection_recursive;
				pc_current_win->UpdateMenuState(hwndDlg);
				HighlightCur(hwndDlg, true);
			};

			if (LOWORD(wParam)==ID_SHOW_ONLYVISIBLE)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mdw_display_flags ^= SHOW_ONLY_VISIBLE;
				pc_current_win->ShowTree();
				pc_current_win->UpdateMenuState(hwndDlg);
			}
			if (LOWORD(wParam)==ID_SHOW_ALL)
			{
				HighlightCur(hwndDlg, false);
				pc_current_win->mdw_display_flags = 0;
				pc_current_win->ShowTree();
				pc_current_win->UpdateMenuState(hwndDlg);
			}

			if (LOWORD(wParam)==ID_SELECTION_HIDE)
			{
				pc_current_win->SwitchVisibiliy(hwndDlg);
			}

			if (LOWORD(wParam)==ID_FLIP_X)		pc_current_win->Matrix(hwndDlg,-1,1,1,0);
			if (LOWORD(wParam)==ID_FLIP_Y)		pc_current_win->Matrix(hwndDlg,1,-1,1,0);
			if (LOWORD(wParam)==ID_FLIP_Z)		pc_current_win->Matrix(hwndDlg,1,1,-1,0);
			if (LOWORD(wParam)==ID_ROTATE_CW)	pc_current_win->Matrix(hwndDlg,1,1,1,3.12f/2.0f);
			if (LOWORD(wParam)==ID_ROTATE_CCW)	pc_current_win->Matrix(hwndDlg,1,1,1,-3.12f/2.0f);


			

			return TRUE;


		case WM_NOTIFY:
			if (wParam == IDC_OBJ_TREE)
			{
				NMHDR* ph = (NMHDR*)lParam;
				if (ph->code == TVN_SELCHANGED)
				{
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
					
					Highlight(pnmtv->itemOld ,false);
					Highlight(pnmtv->itemNew ,true);
				}
			}

			return TRUE;

		case WM_SIZE:
			// Maik Jurkait  02.06.2004 12:37:28
			// ------------------------------------------
			//	Reposition
			// ------------------------------------------
			MoveWindow(GetDlgItem(hwndDlg,IDC_OBJ_TREE),0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
			return TRUE;
	}

	return FALSE;
}


void	CDebugWindow::TreeClear()
{
	SendMessage( GetDlgItem(pc_current_win->mhwnd,IDC_OBJ_TREE), TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT );

	delete mpc_tree_root;
	mpc_tree_root = new CDebugTreeData();
}



void*		CDebugWindow::TreeAdd(void* par, CNode* node, char* text, int image)
{
	CDebugTreeData* parent = (CDebugTreeData*) par;

	// Maik Jurkait  04.06.2004 19:46:48
	// ------------------------------------------
	//	add to internal tree
	// ------------------------------------------
	CDebugTreeData* newnode = new CDebugTreeData();
	newnode->name = new char[strlen(text)+1];
	strcpy(newnode->name,text);
	newnode->image = image;
	newnode->pc_node = node;

	parent->AddChild(newnode);

	return (void*)newnode;
}




void	CDebugWindow::ShowTree()
{
	SendMessage( GetDlgItem(pc_current_win->mhwnd,IDC_OBJ_TREE), TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT );
	ShowTree(TVI_ROOT,mpc_tree_root);

}




void	CDebugWindow::ShowTree(HTREEITEM parent,CDebugTreeData* pc_data)
{
	HWND tree = GetDlgItem(pc_current_win->mhwnd,IDC_OBJ_TREE);

	for (int t=0;t<pc_data->childs.Count();t++)
	{
		if (!IsShowFlagOkay(pc_data->childs[t])) continue;


		// tranlate overlay images
		int overlay = TICST_INVESTIGATE;
		int ov_flag = (pc_data->childs[t]->image&0xffff0000); 
		if (ov_flag == TICS_INVESTIGATE)						overlay = TICST_INVESTIGATE;
		else if (ov_flag == TICS_FINISHED)						overlay = TICST_FINISHED;
		else if (ov_flag == TICS_FINISHED_RENDERING_MISSING)	overlay = TICST_FINISHED;
		else if (ov_flag == TICS_UNKNOWNDATA)					overlay = TICST_UNKNOWNDATA;
		else if (ov_flag == TICS_UNKNOWNDATA_LISTED)			overlay = TICST_UNKNOWNDATA_LISTED;
		else if (ov_flag == TICS_UNKNOWNDATA_NOT_CRITICAL)		overlay = TICST_UNKNOWNDATA_NOT_CRITICAL;

		


		// fill insert struct
		TVINSERTSTRUCT	s_item;
		s_item.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_STATE;
		s_item.item.iImage = (pc_data->childs[t]->image)&0xffff;
		s_item.item.iSelectedImage = s_item.item.iImage;
		s_item.item.pszText = pc_data->childs[t]->name;
		s_item.item.lParam = (LPARAM)pc_data->childs[t];
		s_item.item.state = INDEXTOOVERLAYMASK(overlay);
		s_item.item.stateMask = TVIS_OVERLAYMASK;
		s_item.hParent = parent;
		s_item.hInsertAfter = TVI_LAST;

		// insert
		HTREEITEM cur_item = (HTREEITEM)SendMessage( tree, TVM_INSERTITEM,   0, (LPARAM)&s_item);

		// proceed childs
		ShowTree(cur_item, pc_data->childs[t]);
	}


	SendMessage( GetDlgItem(pc_current_win->mhwnd,IDC_OBJ_TREE), TVM_SORTCHILDREN, TRUE, (LPARAM)parent );

}


bool	CDebugWindow::IsShowFlagOkay(CDebugTreeData* pc_tnode)
{
	if (mdw_display_flags & SHOW_NO_CLASS_INFO)
		if (pc_tnode->image == TIC_CLASS) return false;

	if (mdw_display_flags & SHOW_ONLY_VISIBLE)
		if (!IsVisible(pc_tnode)) return false;

	return true;
}



bool	CDebugWindow::IsVisible(CDebugTreeData* pc_tnode)
{	
	if (pc_tnode->pc_node != NULL) 
	{
		if (pc_tnode->pc_node->IsClass("NiNode"))
			return ((CNiNode*)pc_tnode->pc_node)->IsVisible();
	}

	return true;
}



void	CDebugWindow::TreeBuild(CNode* pc_node)
{
	pc_node->CreateDbgTree(mpc_tree_root,TreeAdd);

	ShowTree();
}



#endif