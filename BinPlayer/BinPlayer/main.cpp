#include "main.h"
#include "module_player.h"
#include "module_ui.h"
#include "module_configurer.h"
#include "common_actions.h" 


#include "events.h"

//Global Varialitys for all
HWND hWndWindowMain;//Handle of Main Window
HWND hWndTrackbarSeeking;//
HWND hWndTrackbarVolume;//
HWND hWndRainbow;//
HWND hWndTime;//
HWND hWndFileInfo;//
HWND hWndControls;//

HINSTANCE hInst;//Instance Handle
//
TCHAR szTitle[25] = TEXT("BinPlayer");
TCHAR WindowClassMain[10] = TEXT("BinPlayer");


//This RECT Include the default full Size Calculated By client size
RECT RectWindowsDefault;

//The Entry Of The Process
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	//Ignore UNREFERENCED PARAMETER Info
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//Initialize Common Controls

	hInst = hInstance; // Store instance handle in our global variable
	on_action_InitBeforeCreation();
	//Register Class For Main Window
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= CreateSolidBrush(RGB(0, 0, 0));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU_MAIN);
	wcex.lpszClassName	= WindowClassMain;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);
	// Perform application initialization:
	hWndWindowMain = CreateWindowEx(WS_EX_ACCEPTFILES,
		WindowClassMain,
		szTitle, 
		WS_OVERLAPPEDWINDOW  |  WS_CLIPCHILDREN ,
		CW_USEDEFAULT, CW_USEDEFAULT, SkinConfig.MiniWidth, SkinConfig.MiniHeight,
		NULL, NULL, hInstance, NULL);
	if (!hWndWindowMain) {
		return FALSE;
	}

	on_action_InitInsideCreation();

//	SetLayeredWindowAttributes(hWndWindowMain,  NULL, BP_PublicInfo.alpha, LWA_ALPHA );
	//Calculate RectWindowsDefault 
	RECT RectWindowsTemp;
	GetClientRect(hWndWindowMain, &RectWindowsTemp);
	GetWindowRect(hWndWindowMain, &RectWindowsDefault);
	RectWindowsDefault.right = 2*RectWindowsDefault.right - 2*RectWindowsDefault.left-RectWindowsTemp.right;
	RectWindowsDefault.bottom = 2*RectWindowsDefault.bottom - 2*RectWindowsDefault.top-RectWindowsTemp.bottom;
	MoveWindow(hWndWindowMain, RectWindowsDefault.left, RectWindowsDefault.top, RectWindowsDefault.right, RectWindowsDefault.bottom, TRUE);




	//Show Main Window
	ShowWindow(hWndWindowMain, nCmdShow);
	UpdateWindow(hWndWindowMain);

	on_action_InitAfterCreation();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message) {
	case WM_CREATE:
		createUIWindows(hWnd);
		initNotifyTray(hWnd);
		break;
	case WM_NOTIFYICON:
		switch (LOWORD(lParam)) {
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
			on_event_TrayIcon_Click();
			break;
		case WM_RBUTTONUP:
			on_event_TrayIcon_RightClick();
		break;
		}
		break;
	case WM_SYSCOMMAND:
		switch(wParam) {
		case SC_MINIMIZE:
			on_event_MainWindow_Minilize();
			break;
		default:
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		SetFocus(hWnd);
		// Parse the menu selections:
		switch (wmId) {
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_HELP_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hWnd, About);
			break;
		case ID_TOOLS_OPTIONS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hWnd, Config);
			break;
		case ID_FILE_OPENFILE:
			on_event_Menu_MainWindow_OpenFile();
			break;
		case ID_FILE_OPENURL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_OPENURL), hWnd, OpenUrl);
			break;
		case ID_PLAY_RESUMEPAUSE:
			on_event_Menu_MainWindow_ResumePause();
			break;
		case ID_PLAY_STOP:
			on_action_ChangePlayState(0);
			break;
		case ID_HELP_CHECKFORUPDATES:
			on_event_Menu_MainWindow_CheckUpdate();
			break;
		case ID_PLAY_REPEAT:
			BP_PublicInfo.PlayRepeat = !BP_PublicInfo.PlayRepeat;
			CheckMenuItemByBool(hMenuMain, ID_PLAY_REPEAT, BP_PublicInfo.PlayRepeat);
			break;
		case ID_PLAY_SHUFFLE:
			BP_PublicInfo.PlayShuffle = !BP_PublicInfo.PlayShuffle;
			CheckMenuItemByBool(hMenuMain, ID_PLAY_SHUFFLE, BP_PublicInfo.PlayShuffle);
			break;
		case ID_VIEW_ONTOP:
			BP_PublicInfo.IsOnTop = !BP_PublicInfo.IsOnTop;
			CheckMenuItemByBool(hMenuMain, ID_VIEW_ONTOP, BP_PublicInfo.IsOnTop);
			topTheWindow(hWndWindowMain, BP_PublicInfo.IsOnTop);
			break;
		case ID_VIEW_PLAYLIST:
			on_event_Menu_MainWindow_PlayList();
			break;
		case ID_PLAY_NEXT:
			on_event_Menu_MainWindow_Play_Next();
			break;
		case ID_PLAY_BACK:
			on_event_Menu_MainWindow_Play_Back();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DROPFILES:
		on_event_DropFileToMainWindow((HDROP)wParam);

		break;
	case WM_SIZE:
		if (SIZE_MINIMIZED == wParam) {
			break;
		}
		GetWindowRect(hWndWindowMain, &BP_MainWindowInfomation.WindowRect);
		GetClientRect(hWndWindowMain, &BP_MainWindowInfomation.WindowClientRect);
		moveAllWindows(&BP_MainWindowInfomation.WindowClientRect);
		if (SIZE_MAXIMIZED == wParam) on_action_AujustVedioWindow(true, false);
		else on_action_AujustVedioWindow(false, false);
		break;
	case WM_MOVE:
		GetWindowRect(hWndWindowMain, &BP_MainWindowInfomation.WindowRect);
		break;
	case WM_LBUTTONDOWN:
		BP_MainWindowInfomation.MovingIsActive = true;
		BP_MainWindowInfomation.MovingPosX = GET_X_LPARAM(lParam);
		BP_MainWindowInfomation.MovingPosY = GET_Y_LPARAM(lParam);
		SetCapture(hWnd);
		break;
	case WM_LBUTTONUP:
		BP_MainWindowInfomation.MovingIsActive = false;
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if ( !BP_MainWindowInfomation.MovingIsActive || wParam!=MK_LBUTTON || (BP_PublicInfo.ScreenMode != 0)) return 0;
		GetWindowRect(hWndWindowMain, &BP_MainWindowInfomation.WindowRect);
		MoveWindow(hWnd, 
			BP_MainWindowInfomation.WindowRect.left - BP_MainWindowInfomation.MovingPosX + GET_X_LPARAM(lParam),
			BP_MainWindowInfomation.WindowRect.top - BP_MainWindowInfomation.MovingPosY + GET_Y_LPARAM(lParam), 
			BP_MainWindowInfomation.WindowRect.right - BP_MainWindowInfomation.WindowRect.left, 
			BP_MainWindowInfomation.WindowRect.bottom - BP_MainWindowInfomation.WindowRect.top,
			true);
		break;
	case WM_LBUTTONDBLCLK:
		on_event_Mouse_Double();
		break;
	case WM_MBUTTONUP:
		on_event_Mouse_Middle();
		break;
	case WM_KEYDOWN:
		solveKeyMessage(wParam);
		break;
	case WM_GETMINMAXINFO:
		((MINMAXINFO *)lParam)->ptMinTrackSize.x = RectWindowsDefault.right;
		((MINMAXINFO *)lParam)->ptMinTrackSize.y = RectWindowsDefault.bottom;
		break;
	case WM_CLOSE:
		on_action_InitBeforeClosing();
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		
		//IniDlgItemLang(hDlg, NULL, 61000, true);
		SendDlgItemMessage(hDlg, IDC_STATIC_VERSION, WM_SETTEXT, NULL, (LPARAM)FILE_VERSION);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK OpenUrl(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDOK) {
			TCHAR TempFilePath[MAX_PATH];
			GetDlgItemText(hDlg, IDC_EDIT_OPENURL_URL, TempFilePath, MAX_PATH);
			on_action_AddFiletoPlayList(TempFilePath, true);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}

	return (INT_PTR)FALSE;
}

bool solveKeyMessage(WPARAM KeyMessage) {
	switch (KeyMessage){ 
	case VK_LEFT: 
		on_event_Key_LEFT();
		break; 
	case VK_RIGHT: 
		on_event_Key_RIGHT();
		break; 
	case VK_UP: 
		on_event_Key_UP();
		break; 
	case VK_DOWN: 
		on_event_Key_DOWN();
		break; 
	case VK_SPACE:
		on_event_Key_SPACE();
		break;
	case VK_RETURN:
		on_event_Key_RETURN();
		break;
	default: 
		break; 
	}
	return true;
}


bool initNotifyTray(HWND hWnd) {
		NotifyTray.cbSize = sizeof(NOTIFYICONDATA);
		NotifyTray.hWnd = hWnd;
		NotifyTray.uFlags = NIF_INFO |NIF_MESSAGE | NIF_ICON | NIF_TIP; 
		NotifyTray.uID = 21;
		NotifyTray.uCallbackMessage = WM_NOTIFYICON;
		NotifyTray.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_TRAY));
		NotifyTray.dwInfoFlags = NIIF_INFO;
		NotifyTray.uVersion = NOTIFYICON_VERSION;
		NotifyTray.uTimeout=1000;
		NotifyTray.dwState=NIS_SHAREDICON;
		NotifyTray.dwStateMask=0;
		NotifyTray.dwInfoFlags=NIIF_INFO;
		return true;
}

VOID WINAPI OnSelChanged(HWND hwndDlg, ConfigerItemsInfo * Old, ConfigerItemsInfo *New)  { 
    if ( Old != NULL) DestroyWindow(Old->hwnd); 
	New->hwnd = CreateDialogIndirect(hInst, New->dlgTemplate, hwndDlg, New->CallBackProc); 
}


VOID WINAPI OnTabbedDialogInit(HWND hwndDlg) { 
	//get TreeView Cotrol
	HWND hwndTemp = GetDlgItem(hwndDlg,IDC_TREE_CONFIG);
	//set new height
	TreeView_SetItemHeight(hwndTemp, 22);

	// get Configer class
	if (NULL == BP_Configer) BP_Configer = new module_configurer();
	BP_Configer->Temp_ConfigReference = BP_ConfigReference;

	// BP_Configer->getImageList() return the imagelist initied
	TreeView_SetImageList(hwndTemp, BP_Configer->getImageList(), TVSIL_NORMAL); 
	// number of Items
	int Temp = BP_Configer->getItemsCount();
	TCHAR TempString[MAX_PATH];
	for (int i=0; i<Temp; i++) {
		ConfigerItemsInfo * cfgitem;
		// Get Item config info
		cfgitem = BP_Configer->getItemByID(i);
		// load string from resources
		loadStringByID(cfgitem->StringID, TempString, MAX_PATH);
		AddItemToTreeView(hwndTemp, TempString, cfgitem->iconID, (LPARAM)cfgitem); 
	}

	hwndTemp = GetDlgItem(hwndDlg,IDC_SUBDLG_RECT);
	GetWindowRect(hwndTemp, &BP_Configer->rectConfigDialog);
	BP_Configer->rectConfigDialog.right -= BP_Configer->rectConfigDialog.left;
	BP_Configer->rectConfigDialog.bottom -= BP_Configer->rectConfigDialog.top;
	ScreenToClient(hwndDlg, (LPPOINT)&BP_Configer->rectConfigDialog);
	
}


INT_PTR CALLBACK Config (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	LPNMTREEVIEW pnmtv;
	switch (message) {
	case WM_INITDIALOG:
		OnTabbedDialogInit(hDlg);

		IniDlgItemLang(hDlg, NULL, 61000, true);
		IniDlgItemLang(hDlg, IDC_BUTTON_SAVE, 60402);
		IniDlgItemLang(hDlg, IDC_BUTTON_EXIT, 60404);
		IniDlgItemLang(hDlg, IDC_BUTTON_APPLY, 60403);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_SAVE:
			on_action_SaveConfigFile(&BP_Configer->Temp_ConfigReference);
			break;
		case IDC_BUTTON_EXIT:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDC_BUTTON_APPLY:
			BP_ConfigReference = BP_Configer->Temp_ConfigReference;
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TVN_BEGINDRAG:
		case TVN_BEGINRDRAG:
			TreeView_SelectItem(((NMTREEVIEW *)lParam)->hdr.hwndFrom, ((NMTREEVIEW *)lParam)->itemNew.hItem);
			break;
		case TVN_SELCHANGED:
			OnSelChanged(hDlg, (ConfigerItemsInfo *)((LPNMTREEVIEW)lParam)->itemOld.lParam, (ConfigerItemsInfo *)((LPNMTREEVIEW)lParam)->itemNew.lParam);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}

