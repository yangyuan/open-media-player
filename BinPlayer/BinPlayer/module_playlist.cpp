#include "module_playlist.h"

TCHAR CurrentPlayListPath[MAX_PATH];

HWND CreateListView (HWND hwndParent) {     
    RECT rcl; 
    INITCOMMONCONTROLSEX icex;
    
    // Ensure that the common control DLL is loaded. 
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex); 
    
    // Create the list-view window in report view with label editing enabled.
    GetClientRect (hwndParent, &rcl); 
    
    HWND hWndListView = CreateWindow(WC_LISTVIEW, 
        TEXT(""), 
        WS_CHILD | LVS_REPORT | WS_VISIBLE | LVS_NOCOLUMNHEADER |LVS_OWNERDRAWFIXED, 
        0, 
        0, 
        0, 
        0, 
        hwndParent, 
        (HMENU)0, 
        hInst, 
        NULL); 

	HIMAGELIST ImageListTemp =  ImageList_Create( 15, 15, ILC_COLOR, 0, 0);
	ListView_SetImageList(hWndListView, ImageListTemp, LVSIL_STATE);

	DWORD StyleTemp;
	StyleTemp = ListView_GetExtendedListViewStyle(hWndListView);
	ListView_SetExtendedListViewStyle(hWndListView, StyleTemp | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
    return (hWndListView);
}

BOOL InitListViewColumns(HWND hWndListView) 
{ 
    TCHAR szText[256];     // temporary buffer 
    LVCOLUMN lvc; 
    int iCol; 

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text, and subitem members
    // of the structure are valid. 
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 
	  
    // Add the columns
    for (iCol = 0; iCol < 2; iCol++) 
    { 
        lvc.iSubItem = iCol;
        lvc.pszText = szText;	
        lvc.cx = 78;     // width of column in pixels

        if ( iCol < 2 )
            lvc.fmt = LVCFMT_LEFT;  // left-aligned column
        else
            lvc.fmt = LVCFMT_RIGHT; // right-aligned column		                         

        LoadString(hInst, 
                   60000 + iCol, 
                   szText, 
                   sizeof(szText)/sizeof(szText[0]));

        if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1) 
            return FALSE; 
    } 


    return TRUE; 
} 

HWND ListView;
RECT ListViewClientRect;
void OnPlayListDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	static TCHAR String[MAX_PATH];
	static HBRUSH BrushSelectIn;
	static HBRUSH BrushSelectOut;
	static HBRUSH Brush1;
	static HBRUSH Brush0;
	static bool inied = false;
	if (inied == false) {
		BrushSelectIn = CreateSolidBrush(0xFBE6D2);
		BrushSelectOut = CreateSolidBrush(0xc56a31);
		//odd
		Brush1 = CreateSolidBrush(0xF2F6FB);
		Brush0 = CreateSolidBrush(0xFFFFFF);
		inied = true;
	}
	if (lpDrawItemStruct->hwndItem != ListView) return;
	//int nItem = lpDrawItemStruct->itemID;
	//RECT rcItem = lpDrawItemStruct->rcItem;
	//POSITION pos = FindPos(nItem);
	bool fSelected;
	//CPlaylistItem& pli = m_pl.GetAt(pos);

	if(fSelected = ListView_GetItemState(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, LVIS_SELECTED)) {
		FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, BrushSelectIn);
		//FrameRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, BrushSelectOut);
	} else {
		if (lpDrawItemStruct->itemID%2)	FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, Brush1);
		else FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, Brush0);
	}

	COLORREF textcolor = fSelected?0x0000D4:0x663300;
	SetTextColor(lpDrawItemStruct->hDC, textcolor);
	ListView_GetItemText(ListView,lpDrawItemStruct->itemID,0,String, MAX_PATH);
	lpDrawItemStruct->rcItem.left += 3;
//	DrawTextEx(lpDrawItemStruct->hDC,String, -1 , &lpDrawItemStruct->rcItem,TA_LEFT|DT_END_ELLIPSIS |DT_VCENTER, NULL);

	LVITEM lvItem;
	lvItem.iItem = lpDrawItemStruct->itemID;
	lvItem.mask   =   LVIF_PARAM;
	ListView_GetItem(ListView,&lvItem);
	DrawTextEx(lpDrawItemStruct->hDC,((PlayListItem *)(lvItem.lParam))->Title, -1 , &lpDrawItemStruct->rcItem,TA_LEFT|DT_END_ELLIPSIS |DT_VCENTER, NULL);

	//if(pli.m_fInvalid) textcolor |= 0xA0A0A0;

	//CString time = !pli.m_fInvalid ? m_list.GetItemText(nItem, COL_TIME) : _T("Invalid");
	//CSize timesize(0, 0);
	//CPoint timept(rcItem.right, 0);
	//if(time.GetLength() > 0)
	//{
	//	timesize = pDC->GetTextExtent(time);
	//	if((3+timesize.cx+3) < rcItem.Width()/2)
	//	{
	//		timept = CPoint(rcItem.right-(3+timesize.cx+3), (rcItem.top+rcItem.bottom-timesize.cy)/2);

	//		pDC->SetTextColor(textcolor);
	//		pDC->TextOut(timept.x, timept.y, time);
	//	}
	//}

	//CString fmt, file;
	//fmt.Format(_T("%%0%dd. %%s"), (int)log10(0.1+m_pl.GetCount())+1);
	//file.Format(fmt, nItem+1, m_list.GetItemText(nItem, COL_NAME));
	//CSize filesize = pDC->GetTextExtent(file);
	//while(3+filesize.cx+6 > timept.x && file.GetLength() > 3)
	//{
	//	file = file.Left(file.GetLength()-4) + _T("...");
	//	filesize = pDC->GetTextExtent(file);
	//}

	//if(file.GetLength() > 3)
	//{
	//	pDC->SetTextColor(textcolor);
	//	pDC->TextOut(rcItem.left+3, (rcItem.top+rcItem.bottom-filesize.cy)/2, file);
	//}
}
LRESULT CALLBACK WinProcPlayList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	int wmId, wmEvent;
	NMLVDISPINFO* plvdi;
	LPNMITEMACTIVATE lpnmitem;
	HDC hdc;
	POINT MousePoint;
	switch (message) {
	case WM_CREATE:
		ListView = CreateListView(hWnd);
		InitListViewColumns(ListView);
		break;
	case WM_SIZE:
		GetClientRect(hWnd,&ListViewClientRect);
		MoveWindow(ListView, 0, 0, ListViewClientRect.right, ListViewClientRect.bottom, true);
		ListView_SetColumnWidth(ListView, 0, ListViewClientRect.right-50);
		ListView_SetColumnWidth(ListView, 1, LVSCW_AUTOSIZE_USEHEADER);
		break;
	case WM_DRAWITEM:
		OnPlayListDrawItem((LPDRAWITEMSTRUCT) lParam);
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code) {
		case LVN_GETDISPINFO:
			plvdi = (NMLVDISPINFO*)lParam;
			switch (plvdi->item.iSubItem) {
			case 0:
				plvdi->item.pszText = ((PlayListItem *)plvdi->item.lParam)->Title;
				break;
			case 1:
//				plvdi->item.pszText = ((PlayListItem *)plvdi->item.lParam)->szTime;
				break;
			default:
				break;
			}
			return 0;
		case NM_DBLCLK:
			lpnmitem = (LPNMITEMACTIVATE) lParam;
			if (lpnmitem->iItem == -1) break;
			else on_event_PlayListItem_DoubleClick(lpnmitem->iItem);
			break;
		case NM_RCLICK:
			lpnmitem = (LPNMITEMACTIVATE) lParam;
			if (lpnmitem->iItem == -1) {
				EnableMenuItem(hMenuPlayListPop,ID_PLAYLISTWITHFILESELECTED_PROPERTY, MF_BYCOMMAND| MF_GRAYED);
				EnableMenuItem(hMenuPlayListPop,3, MF_BYPOSITION | MF_GRAYED);
			} else {
				EnableMenuItem(hMenuPlayListPop,ID_PLAYLISTWITHFILESELECTED_PROPERTY, MF_BYCOMMAND| MF_ENABLED);
				EnableMenuItem(hMenuPlayListPop,3, MF_BYPOSITION | MF_ENABLED);
			}
			EnableMenuItem(hMenuPlayListPop,ID_PLAYLIST_REMOVEREDUNDANCY, MF_BYCOMMAND| MF_GRAYED);
			EnableMenuItem(hMenuPlayListPop,ID_PLAYLISTWITHFILESELECTED_PROPERTY, MF_BYCOMMAND| MF_GRAYED);
			GetCursorPos(&MousePoint);
			TrackPopupMenuEx (hMenuPlayListPop, 0, MousePoint.x, MousePoint.y, hWnd, NULL);
			break;
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		SetFocus(hWnd);
		// Parse the menu selections:
		switch (wmId) {
		case ID_PLAYLIST_CLEARALL:
			playList_clear();
			break;
		case ID_PLAYLIST_DELETEFROMLIST:
			playList_DeleteSelected(FALSE);
			break;
		case ID_PLAYLIST_DELETEFROMDISK:
			playList_DeleteSelected(TRUE);
			break;
		case ID_PLAYLIST_ADDFILE:
			on_event_Menu_MainPlayList_AddFile();
			break;
		case ID_ADD_FROMPLAYLIST:
			on_event_Menu_MainPlayList_AddFileFromPlayList();
			break;
		case ID_PLAYLIST_LOADPLAYLIST:
			on_event_Menu_MainPlayList_LoadPlayList();
			break;
		case ID_PLAYLIST_SAVEPLAYLIST:
			on_event_Menu_MainPlayList_SaveAs();
			break;
		case ID_PLAYLIST_SAVE:
			on_event_Menu_MainPlayList_SavePlayList();
			break;
		}
		break;
	case WM_DROPFILES:
		on_event_DropFileToPlayList((HDROP)wParam);
		break;
	case WM_CLOSE:
		on_event_Menu_MainWindow_PlayList();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
HWND create_PlayList(HWND hWnd) {
	HWND TempHwnd;
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WinProcPlayList;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName	= TEXT("BP_PC_PL");
	wcex.lpszMenuName   = NULL;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	TempHwnd = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW,
		TEXT("BP_PC_PL"),
		TEXT("Play List"),
		WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_THICKFRAME | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, 0, 240, BP_MainWindowInfomation.WindowRect.bottom - BP_MainWindowInfomation.WindowRect.top, 
		hWnd, NULL , NULL, 0);
	return TempHwnd;
}
bool loadPlayList(TCHAR *) {
	return true;
}

bool ShowPlayList(bool JustSwitch, bool ShowHide) {
	if (JustSwitch) {
		return false;
	} else {
		if (ShowHide) {
			INT TempX = GetSystemMetrics(SM_CXSCREEN);
			if (BP_MainWindowInfomation.WindowRect.right + 240 > TempX) {
				MoveWindow(hWndPlayList, TempX -240, BP_MainWindowInfomation.WindowRect.top, 240, BP_MainWindowInfomation.WindowRect.bottom - BP_MainWindowInfomation.WindowRect.top, true);
			} else MoveWindow(hWndPlayList, BP_MainWindowInfomation.WindowRect.right, BP_MainWindowInfomation.WindowRect.top, 240, BP_MainWindowInfomation.WindowRect.bottom - BP_MainWindowInfomation.WindowRect.top, true);
			ShowWindow(hWndPlayList, SW_SHOW);
			return true;
		}
		else ShowWindow(hWndPlayList, SW_HIDE);
		return false;
	}
}

int playList_AddFile(TCHAR * NewFilePath, int index) {
	PlayListItem * PlayListItemInfo  = new PlayListItem();

	int Offset;
	Offset = getFileNameOffset(NewFilePath);
	lstrcpy(PlayListItemInfo->Title, NewFilePath + Offset);
	lstrcpy(PlayListItemInfo->Path, NewFilePath);
	lstrcpy(PlayListItemInfo->Atrist, TEXT(""));
	lstrcpy(PlayListItemInfo->Album, TEXT(""));
	PlayListItemInfo->Start = -1;
	PlayListItemInfo->End = 0;
	PlayListItemInfo->Ident = 0;

	return playList_AddFile(PlayListItemInfo, index);
}

int playList_AddFile(PlayListItem * PlayListItemInfo, int index) {
	LVITEM lvI;

	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvI.state = 0; 
	lvI.stateMask = 0; 
	lvI.iItem = index;
	lvI.iImage = index;
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM) PlayListItemInfo;
	lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISP message.
	return ListView_InsertItem(ListView, &lvI);
}

void playList_clear() {
	LVITEM lvItem;
	int Number = ListView_GetItemCount( ListView );
	for (Number--;Number>=0; Number--) {
		lvItem.iItem = Number;
		lvItem.mask   =   LVIF_PARAM;
		ListView_GetItem(ListView,&lvItem);
		delete ((PlayListItem *)(lvItem.lParam));
		ListView_DeleteItem(ListView, Number);
	}
}


void playList_deleteItem(int index) {
	LVITEM lvItem;
	lvItem.iItem = index;
	lvItem.mask = LVIF_PARAM;
	if (ListView_GetItem(ListView,&lvItem)) {
		delete ((PlayListItem *)(lvItem.lParam));
		ListView_DeleteItem(ListView, index);
	}
}

PlayListItem * playList_GetItemByIndex(int index) {
	LVITEM lvItem;
	lvItem.iItem = index;
	lvItem.mask = LVIF_PARAM;
	if (ListView_GetItem(ListView,&lvItem)) {
		return (PlayListItem *)(lvItem.lParam);
	} else {
		return NULL;
	}
}
TCHAR * playList_GetFileByIndex(int index) {
	PlayListItem * Temp  = playList_GetItemByIndex(index);
	if (NULL != Temp) {
		return Temp->Path;
	} else return NULL;
}

int playList_GetItemNumber() {
	return ListView_GetItemCount(ListView);
}

void playList_DeleteSelected(BOOL DiskAlso) {
	int NumberSelected =  ListView_GetSelectedCount(ListView);
	int Index;
	while (NumberSelected -- ) {
		Index = ListView_GetNextItem(ListView,-1,LVNI_SELECTED);
		PlayListItem * Temp  = playList_GetItemByIndex(Index);
		if (NULL != Temp) {
			if (DiskAlso) {
				DeleteFile(Temp->Path);
			}
			delete Temp;
		}
		ListView_DeleteItem(ListView,Index);
	}
}

//
BOOL playList_loadPlaylist(TCHAR * Path, BOOL changePath) {
	 playList_clear();
	if (changePath) lstrcpy(CurrentPlayListPath, Path);
	module_playlist_reader *  xxx = (module_playlist_reader *) new module_playlist_reader_bpls();
	if (xxx->loadFile(Path)) {
		xxx->loadToProc(on_action_AddFileItemtoPlayList);
		delete xxx;
		return TRUE;
	}
	return FALSE;
}
// when Path is null , it will save to current path
BOOL playList_savePlaylist(TCHAR * Path, BOOL changePath) {
	TCHAR * Temp;
	module_playlist_writer *  xxx = (module_playlist_writer *) new module_playlist_writer_bpls();
	if (Path != NULL) {
		if (changePath) lstrcpy(CurrentPlayListPath, Path);
		Temp = Path;
	} else {
		Temp = CurrentPlayListPath;
	}
	LVITEM lvItem;
	int Number = ListView_GetItemCount( ListView );
	for (Number--;Number>=0; Number--) {
		lvItem.iItem = Number;
		lvItem.mask   =   LVIF_PARAM;
		ListView_GetItem(ListView,&lvItem);
		xxx->add(-1, ((PlayListItem *)(lvItem.lParam)));
	}
	xxx->writeFile(Temp);
	delete xxx;
	return TRUE;
}