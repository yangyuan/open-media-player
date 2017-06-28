#include "actions.h"

void on_action_DropFileAndAdd(HDROP hDrop, bool clearBeforeAction) {
	TCHAR FileBuffer[MAX_PATH];
	unsigned int i;
	unsigned int NumberOfDrags;
	NumberOfDrags = DragQueryFile(hDrop, 0xFFFFFFFF, FileBuffer, MAX_PATH);
	if (clearBeforeAction) playList_clear();
	if (1 == NumberOfDrags) {
		DragQueryFile(hDrop, 0, FileBuffer, MAX_PATH);
		if (seemsPlayLits(FileBuffer)) {
	//		if (LoadPlayList()) return;
		}
		on_action_AddFiletoPlayList(FileBuffer, false);
	} else {
		for (i = 0; i<NumberOfDrags; i++) {
			DragQueryFile(hDrop, i, FileBuffer, MAX_PATH);
			on_action_AddFiletoPlayList(FileBuffer, false);
		}
	}
}

void on_action_HideAllWindows() {
	ShowWindow(hWndWindowMain,SW_HIDE);
	ShowPlayList(false, false);
	CheckMenuItemByBool(hMenuMain, ID_VIEW_PLAYLIST, false);
}

void on_action_ShowPlayList() {
	ShowPlayList(false, true);
	CheckMenuItemByBool(hMenuMain, ID_VIEW_PLAYLIST, true);
}
void on_action_HidePlayList() {
	ShowPlayList(false, false);
	CheckMenuItemByBool(hMenuMain, ID_VIEW_PLAYLIST, false);
}
void on_action_ShowMainWindow() {
	ShowWindow(hWndWindowMain,SW_SHOW);
	SetForegroundWindow(hWndWindowMain);
}
void on_action_HideMainWindow() {
	ShowWindow(hWndWindowMain,SW_HIDE);
}
void on_action_ShowTrayIcon() {
}
void on_action_HideTrayIcon() {
	BP_PublicInfo.IsInTray = false;
	Shell_NotifyIcon(NIM_DELETE, &NotifyTray);
}

void on_action_CheckUpdate() {
	ShellExecute(NULL, NULL, VERSION_CHECK, NULL, NULL, SW_SHOWNORMAL);
}

void on_action_PlayPlayList(int index) {
	TCHAR * TempPath = playList_GetFileByIndex(index);
	if (NULL != TempPath) {
		BP_CurrentInfomation.currentFileIndexOfPlayList = index;
		on_action_playMediaFile(TempPath);
	}
}

void on_action_PlayNext(bool repeat) {
	TCHAR * TempPath = playList_GetFileByIndex(BP_CurrentInfomation.currentFileIndexOfPlayList+1);
	if (NULL != TempPath) {
		BP_CurrentInfomation.currentFileIndexOfPlayList ++ ;
		on_action_playMediaFile(TempPath);
		return;
	}
	if (!repeat) return;
	TempPath = playList_GetFileByIndex(0);
	if (NULL != TempPath) {
		BP_CurrentInfomation.currentFileIndexOfPlayList  = 0 ;
		on_action_playMediaFile(TempPath);
		return;
	}
	return;
}
void on_action_PlayBack(bool repeat) {
	TCHAR * TempPath;
	if (BP_CurrentInfomation.currentFileIndexOfPlayList > 0) {
		TempPath = playList_GetFileByIndex(BP_CurrentInfomation.currentFileIndexOfPlayList-1);
		if (NULL != TempPath) {
			BP_CurrentInfomation.currentFileIndexOfPlayList -- ;
			on_action_playMediaFile(TempPath);
			return;
		}
	} else if (repeat) {
		int Number = playList_GetItemNumber();
		TempPath = playList_GetFileByIndex(Number-1);
		if (NULL != TempPath) {
			BP_CurrentInfomation.currentFileIndexOfPlayList  = Number-1 ;
			on_action_playMediaFile(TempPath);
			return;
		}
	}
	return;
}

void on_action_PlayShuffle() {
	int Number = playList_GetItemNumber();
	if (Number == 0) return;
	DWORD Temp =  GetTickCount();
	Number = Temp%Number;
	TCHAR * TempPath;
	TempPath = playList_GetFileByIndex(Number);
	if (NULL != TempPath) {
		BP_CurrentInfomation.currentFileIndexOfPlayList  = Number ;
		on_action_playMediaFile(TempPath);
		return;
	}
}


void on_action_OpenMediaFileAndAddtoPlayList(bool AutoStart) {
	TCHAR NewFileName[MAX_PATH];
	TCHAR BufferLang[MAX_PATH];
	LoadString(BP_PublicInfo.LanguageInst, IDS_STRING60410 ,BufferLang, MAX_PATH);
	if (openOpenFileDialog(NewFileName, BufferLang, FILE_OPEN_FILTER_TEXT, hWndWindowMain)) {
		on_action_AddFiletoPlayList(NewFileName, AutoStart);
	}
}

BOOL on_action_AddFileItemtoPlayList(PlayListItem * PlayListItemInfo) {
	playList_AddFile(PlayListItemInfo, 0x7FFFFFFF);
	return TRUE;
}

BOOL on_action_LoadPlayList(BOOL Default, BOOL Clean, BOOL ChangePath) {
	TCHAR TempPath[MAX_PATH];
	if (Default) {
		lstrcpy(TempPath, BP_PublicInfo.CurrentPath);
		lstrcat(TempPath, TEXT("\\binplayer.bpls"));
		if (Clean) DeleteFile(TempPath);
	} else {
		TCHAR BufferLang[MAX_PATH];
		LoadString(BP_PublicInfo.LanguageInst, IDS_STRING60410 ,BufferLang, MAX_PATH);
		if (!openSaveFileDialog(TempPath, BufferLang, FILE_OPEN_FILTER_TEXT, hWndWindowMain, 2)) {
			return FALSE;
		}
	}
	playList_loadPlaylist(TempPath, ChangePath);
}
BOOL on_action_SavePlayList(BOOL Dafault, BOOL SaveAs, BOOL ChangePath) {
	TCHAR TempPath[MAX_PATH];
	if (Dafault) {
		lstrcpy(TempPath, BP_PublicInfo.CurrentPath);
		lstrcat(TempPath, TEXT("\\binplayer.bpls"));
		playList_savePlaylist(TempPath, ChangePath);
	} else if (SaveAs) {
		TCHAR BufferLang[MAX_PATH];
		LoadString(BP_PublicInfo.LanguageInst, 60412 ,BufferLang, MAX_PATH);
		if (openSaveFileDialog(TempPath, BufferLang, FILE_OPEN_FILTER_TEXT, hWndWindowMain, 2)) {
			playList_savePlaylist(TempPath, ChangePath);
		} else {
			return FALSE;
		}
	} else {
		playList_savePlaylist(NULL, FALSE);
	}
	return TRUE;
}

BOOL on_action_LoadDefaultPlayList() {
	if (BP_ConfigReference.pl_saveWhenClose) on_action_LoadPlayList(TRUE, FALSE ,TRUE);
	else on_action_LoadPlayList(TRUE, TRUE ,TRUE);
	return TRUE;
}
BOOL on_action_SaveDefaultPlayList() {
	if (BP_ConfigReference.pl_saveWhenClose) on_action_SavePlayList(TRUE, FALSE, TRUE);
	return TRUE;
}

void on_action_OpenPlayListFileAndAddtoPlayList() {
	TCHAR NewFileName[MAX_PATH];
	TCHAR BufferLang[MAX_PATH];
	LoadString(BP_PublicInfo.LanguageInst, IDS_STRING60410 ,BufferLang, MAX_PATH);
	if (openOpenFileDialog(NewFileName, BufferLang, FILE_OPEN_FILTER_TEXT, hWndWindowMain, 2)) {
		module_playlist_reader *  xxx = (module_playlist_reader *) new module_playlist_reader_bpls();
		if (xxx->loadFile(NewFileName)) {
			xxx->loadToProc(on_action_AddFileItemtoPlayList);
			delete xxx;
		}
	}
}

void TryAddFile(TCHAR * Path) {
	on_action_AddFiletoPlayList(Path, false);
}

void on_action_AddFiletoPlayList(TCHAR * Path, bool AutoStart) {
	
	DWORD  FileAttributes = GetFileAttributes(Path);
	if ( INVALID_FILE_ATTRIBUTES == FileAttributes) return;
	else if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
		findRecurFiles(Path , TryAddFile);
	}
	else {
		int Temp = playList_AddFile(Path, 0x7FFFFFFF);
		if (AutoStart && -1!=Temp) {
			on_action_PlayPlayList(Temp);
		}
	}
	return;
}

void on_action_playMediaFile(TCHAR * Path) {
	if (BP_Player!=NULL) delete BP_Player;
	BP_Player = new module_player(hWndRainbow, Path, -1, -1);
	INT RendState;
	BP_Player->player_Infomation(1, RendState);
	if (RendState<0) {
		on_action_PlayNext(false);
		debugMsg(Path);
		debugMsg(TEXT("Rend Failed"), 1);
	}
	on_action_PlayPause();
	on_action_AujustVedioWindow(true, true);
}

//player Control
void on_action_SetVolume(bool Absolute, double NewVolume) {
	if (BP_Player!=NULL) {
		DOUBLE Volume;
		if (BP_Player->control_GetVolume(Volume));
		if (!Absolute) NewVolume += Volume;
		BP_Player->control_SetVolume(NewVolume);
		setVolumenToUI(NewVolume);
	}
}

void on_action_SetSeekPosition(bool Absolute, LONGLONG NewPosition) {
	if (BP_Player!=NULL) {
		LONGLONG Position;
		if (BP_Player->control_GetPosition(Position));
		if (!Absolute) NewPosition += Position;
		BP_Player->control_SetPosition(NewPosition);
	}
}

void on_action_SetSeekPosition(double NewPosition) {
	if (NewPosition == 1) return;
	if (BP_Player!=NULL) {
		LONGLONG FileLength;
		BP_Player->control_GetLength(FileLength);
		FileLength *= NewPosition;
		BP_Player->control_SetPosition(FileLength);
	}
}

void on_action_ChangePlayState(int State) {//0 stop, 1 pasue, 2 play
	if (BP_Player!=NULL) {
		INT RendState;
		BP_Player->player_Infomation(1, RendState);
		if (RendState <0) return;
		switch (State) {
		case 0:
			BP_Player->play_Stop();
			BP_Player->control_SetPosition(0);
			noticeToUIIsPlaying(false);
			break;
		case 1:
			BP_Player->play_Pause();
			noticeToUIIsPlaying(false);
			break;
		case 2:
			BP_Player->play_Play();
			noticeToUIIsPlaying(true);
			break;
		default:
			break;
		}
	}
}

void on_action_PlayPause() {
	if (BP_Player!=NULL) {
		INT PlayState;
		BP_Player->player_Infomation(2, PlayState);
		if (PlayState == 2) {
			on_action_ChangePlayState(1);
		} else {
			on_action_ChangePlayState(2);
		}
	} else {
		on_event_Menu_MainWindow_OpenFile();
	}
}
void on_action_AujustVedioWindow(bool VideoRatioDirection, bool VideoSizeDirection, BOOL AbsoluteCentre) {
	if (BP_Player!=NULL) {
		LONG TempX;
		LONG TempY;
		if (!BP_Player->control_GetOriginalVedioSize(TempX, TempY)) return;
		HWND ParentWindow;
		if (!BP_Player->control_GetParentWindow(ParentWindow)) return;

		RECT grc;
		RECT Main;
		if (VideoSizeDirection) {
			GetWindowRect(ParentWindow, &grc);
			GetWindowRect(hWndWindowMain, &Main);
			if (BP_PublicInfo.ScreenMode == 0) {
				MoveWindow(hWndWindowMain, Main.left, Main.top, (Main.right - Main.left) + (TempX - (grc.right - grc.left)) , (Main.bottom - Main.top) + (TempY - (grc.bottom - grc.top)), TRUE);
				GetClientRect(ParentWindow, &grc);
				BP_Player->control_SetVedioPosition(0, 0, grc.right, grc.bottom);
			} else {
				BP_Player->control_SetVedioPosition((Main.right-TempX)/2 , (Main.bottom-TempY)/2, TempX, TempY);
			}
		} else if (VideoRatioDirection) {
			if (AbsoluteCentre) GetClientRect(hWndWindowMain, &grc);
			else GetClientRect(ParentWindow, &grc);
			double TempRait1, TempRait2;
			TempRait1 = grc.right/(double)grc.bottom;
			TempRait2 = TempX/(double)TempY;
			if (TempRait1> TempRait2) {
				int TempWidth = grc.bottom*TempRait2;
				BP_Player->control_SetVedioPosition(( grc.right- TempWidth)/2, 0, TempWidth, grc.bottom);
			} else {
				if (TempRait2 == 0) return;
				int TempHeight = grc.right/TempRait2;
				BP_Player->control_SetVedioPosition(0, ( grc.bottom- TempHeight)/2, grc.right, TempHeight);
			}
		} else {
			GetClientRect(ParentWindow, &grc);
			BP_Player->control_SetVedioPosition(0, 0, grc.right, grc.bottom);
		}
		return;
	}
	return;
}

void on_action_SwitchFullScreen(bool JustSwitch, bool ToFullScreen) {
	static long style;
	if (BP_PublicInfo.ScreenMode == 0) {
		BP_PublicInfo.ScreenMode = 2;
		ShowWindow(hWndWindowMain, SW_HIDE);
		SetMenu(hWndWindowMain, NULL);
		ShowWindow(hWndControls, SW_HIDE);
		style = GetWindowLong(hWndWindowMain,GWL_STYLE);
		style &= ~(WS_DLGFRAME | WS_THICKFRAME);
		SetWindowLong(hWndWindowMain,GWL_STYLE, style);
		
		ShowWindow(hWndWindowMain, SW_SHOWMAXIMIZED);
		RECT Temp;
		GetWindowRect(hWndWindowMain, &Temp);
		on_action_AujustVedioWindow(true, false, true);
		ShowWindow(hWndWindowMain, SW_SHOW);
		BP_PublicInfo.ScreenMode = 1;
	} else {
		BP_PublicInfo.ScreenMode = 2;
		ShowWindow(hWndWindowMain, SW_HIDE);
		style = GetWindowLong(hWndWindowMain,GWL_STYLE);
		style |= WS_DLGFRAME | WS_THICKFRAME;
		SetWindowLong(hWndWindowMain,GWL_STYLE, style);
		SetMenu(hWndWindowMain, hMenuMain);
		ShowWindow(hWndWindowMain,  SW_RESTORE);
		ShowWindow(hWndControls, SW_SHOW);
		while(ShowCursor(true)<0) {};
		//*************88
		on_action_AujustVedioWindow(true, false);
		ShowWindow(hWndWindowMain, SW_SHOW);
		BP_PublicInfo.ScreenMode = 0;
	}
}
void on_action_CorrectSeekPositionToUI() {
	LONGLONG FileLength;
	if (BP_Player!=NULL) {
		BP_Player->control_GetLength(FileLength);
		LONGLONG CurrentTime;
		BP_Player->control_GetPosition(CurrentTime);
		double Temp = CurrentTime;
		Temp /= FileLength;
		if ( Temp == 1){
			Temp = 0;
			on_event_PlayFinished();
		}
		setPositionToUI(Temp);
	}
}

void on_action_CaptionToCliboard() {

	BSTR Buffer;
	if (BP_Player!=NULL) {
		if(false) {
			if (OpenClipboard(hWndWindowMain)) {

				//EmptyClipboard();
				//SetClipboardData(
			}
		}
	}
}


void on_action_CalculateTimeString(double Position, TCHAR * Buffer) {
	static bool Maded = false;
	static TCHAR AllLength[9];
	static LONGLONG FileLength = 0;
	static LONGLONG CurrentLength = 0;
	if (BP_Player!=NULL) {
		if (!Maded) {
			BP_Player->control_GetLength(FileLength);
			makeStringByTime(AllLength, FileLength , 1);
			Maded = true;
		}
		CurrentLength = FileLength * Position;
		makeStringByTime(Buffer, CurrentLength , 1);
		lstrcat(Buffer, TEXT("/"));
		lstrcat(Buffer,AllLength);
	} else {
		Maded = false;
		lstrcpy(Buffer, TEXT("N/A"));
	}
}


//language control

HINSTANCE LangInstance;
HMENU CurrentMenu;
void ChangeMenuItemText(unsigned int MenuID, unsigned int LangID, bool ByPos) {
	static TCHAR BufferLang[MAX_PATH];
	static int TempLength = 0;
	TempLength = LoadString(LangInstance, LangID ,BufferLang, MAX_PATH);
	if (TempLength == 0) return;
	if (ByPos) ModifyMenu(CurrentMenu, MenuID, MF_STRING | MF_BYPOSITION, MenuID, BufferLang);
	else ModifyMenu(CurrentMenu, MenuID, MF_STRING, MenuID, BufferLang);
}

void on_action_ChangeMenuLang(HINSTANCE Instance) {
	
	if (NULL == Instance) return;
	LangInstance = Instance;
	CurrentMenu = hMenuMain;

	ChangeMenuItemText(0, 60100, true);
	ChangeMenuItemText(1, 60101, true);
	ChangeMenuItemText(2, 60102, true);
	ChangeMenuItemText(3, 60103, true);
	ChangeMenuItemText(4, 60104, true);
	
	ChangeMenuItemText(ID_FILE_OPENFILE, 60105, false);
	ChangeMenuItemText(ID_FILE_OPENURL, 60106, false);
	ChangeMenuItemText(ID_FILE_EXIT, 60107, false);
	ChangeMenuItemText(ID_VIEW_PLAYLIST, 60108, false);
	ChangeMenuItemText(ID_VIEW_ONTOP, 60118, false);
	ChangeMenuItemText(ID_PLAY_RESUMEPAUSE, 60109, false);
	ChangeMenuItemText(ID_PLAY_STOP, 60110, false);
	ChangeMenuItemText(ID_PLAY_REPEAT, 60111, false);
	ChangeMenuItemText(ID_PLAY_SHUFFLE, 60112, false);
	ChangeMenuItemText(ID_PLAY_NEXT, 60113, false);
	ChangeMenuItemText(ID_PLAY_BACK, 60114, false);
	ChangeMenuItemText(ID_TOOLS_OPTIONS, 60115, false);
	ChangeMenuItemText(ID_HELP_ABOUT, 60116, false);
	ChangeMenuItemText(ID_HELP_CHECKFORUPDATES, 60117, false);

	CurrentMenu = hMenuPlayListPop;

	ChangeMenuItemText(ID_PLAYLIST_ADDFILE, 60140, false);
	ChangeMenuItemText(1, 60141, true);
	ChangeMenuItemText(ID_ADD_FROMPLAYLIST, 60142, false);
	ChangeMenuItemText(ID_PLAYLIST_CLEARALL, 60143, false);
	ChangeMenuItemText(ID_PLAYLIST_REMOVEREDUNDANCY, 60144, false);
	ChangeMenuItemText(7, 60145, true);
	ChangeMenuItemText(ID_PLAYLIST_LOADPLAYLIST, 60146, false);
	ChangeMenuItemText(ID_PLAYLIST_SAVE, 60147, false);
	ChangeMenuItemText(ID_PLAYLIST_SAVEPLAYLIST, 60148, false);
	ChangeMenuItemText(3, 60149, true);
	ChangeMenuItemText(ID_PLAYLIST_DELETEFROMDISK, 60151, false);
	ChangeMenuItemText(ID_PLAYLIST_DELETEFROMLIST, 60150, false);
	ChangeMenuItemText(ID_PLAYLISTWITHFILESELECTED_PROPERTY, 60152, false);
	
	DrawMenuBar(hWndWindowMain);
}

void on_action_InitBeforeCreation() {
	InitCommonControls();
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	// Initialize COM


	int Temp;
	TCHAR TempDirPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, BP_PublicInfo.CurrentPath);
	lstrcpy(TempDirPath, BP_PublicInfo.CurrentPath);
	lstrcat(TempDirPath, TEXT("\\codec\\"));
	
	Temp = SetDllDirectory(TempDirPath);

	on_action_LoadConfig();
	BP_PublicInfo.IsInTray = false;
	BP_CurrentInfomation.currentFileIndexOfPlayList = -1;

	BP_PublicInfo.ScreenMode = 0;


	//******************************************
//	 SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,0,NULL,0);  
	//hIcon_False = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_COMM_FALSE));
	//hIcon_True = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_COMM_TRUE));


}

void on_action_InitInsideCreation() {
	hMenuMain = GetMenu(hWndWindowMain);

	hMenuPlayListPop = GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_PLAYLIST)), 0);

	if (BP_PublicInfo.LanguageInst != hInst) on_action_ChangeMenuLang(BP_PublicInfo.LanguageInst);
	hMenuTray = CreatePopupMenu();
	MENUITEMINFO MenuInfo;
	ZeroMemory(&MenuInfo, sizeof(MENUITEMINFO));
	MenuInfo.cbSize = sizeof(MENUITEMINFO);
	MenuInfo.fMask = MIIM_SUBMENU;
	MenuInfo.hSubMenu = GetSubMenu(hMenuMain,2);
	InsertMenuItem(hMenuTray, 0, false, &MenuInfo);
	MenuInfo.hSubMenu = GetSubMenu(hMenuMain,0);
	InsertMenuItem(hMenuTray, 0, false, &MenuInfo);

	//
	on_action_LoadDefaultPlayList();
}

void on_action_InitMenus() {

	CheckMenuItemByBool(hMenuMain, ID_PLAY_REPEAT, BP_PublicInfo.PlayRepeat);
	CheckMenuItemByBool(hMenuMain, ID_PLAY_SHUFFLE, BP_PublicInfo.PlayShuffle);
	CheckMenuItemByBool(hMenuMain, ID_VIEW_ONTOP, BP_PublicInfo.IsOnTop);
	topTheWindow(hWndWindowMain, BP_PublicInfo.IsOnTop);
}

void on_action_InitAfterCreation() {

	// initial menu
	on_action_InitMenus();


	TCHAR BufferLang[MAX_PATH];
	int TempLength = 0;
	TempLength = LoadString(BP_PublicInfo.LanguageInst, IDS_STRING60000 ,BufferLang, MAX_PATH);
	if (TempLength != 0) {
		SetWindowText(hWndWindowMain, BufferLang);
	}
	TempLength = LoadString(BP_PublicInfo.LanguageInst, IDS_STRING60411 ,BufferLang, MAX_PATH);
	if (TempLength != 0) {
		SetWindowText(hWndPlayList, BufferLang);
	}



	// initial tray icon
	hIcon_Tray = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_TRAY));
	hIcon_Tray_V = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_TRAY_V));

	// initial seeking bar
	setPositionToUI(0);

	// initial botton state
	noticeToUIIsPlaying(false);

//************************
	


}

void on_action_InitBeforeClosing() {
	on_action_SaveDefaultPlayList();
}

#define CommonLength 16
//initial Config
void on_action_LoadConfig() {
	TCHAR TempClassName[CommonLength];
	lstrcpy(TempClassName, TEXT("BINPLAYER"));


	on_action_LoadConfigFile(&BP_ConfigReference);


	int TempValue;
	if (!on_action_LoadSkin()) {
		debugMsg(TEXT("SKIN Load Failed"));
		on_action_LoadDefaultSkin();
	}
	BP_trie.PaintInfo.ThumbWidth = BP_trie.PaintInfo.pImgThumb->GetWidth();
	BP_tris.PaintInfo.ThumbWidth = BP_tris.PaintInfo.pImgThumb->GetWidth();
	SkinConfig.PaintInfoButton_PlayPause.pImgBG = SkinConfig.pImgButton_Play;
	SkinConfig.PaintInfoButton_Stop.pImgBG = SkinConfig.pImgButton_Stop;
	SkinConfig.PaintInfoButton_Next.pImgBG = SkinConfig.pImgButton_Next;
	SkinConfig.PaintInfoButton_Back.pImgBG = SkinConfig.pImgButton_Back;
	SkinConfig.PaintInfoButton_PlayPause.State = 0;
	SkinConfig.PaintInfoButton_Stop.State = 0;
	SkinConfig.PaintInfoButton_Next.State = 0;
	SkinConfig.PaintInfoButton_Back.State = 0;

	//if (!getConfigInt(TempClassName, TEXT("alpha"), &BP_PublicInfo.alpha)) BP_PublicInfo.alpha = 255;
	//if (BP_PublicInfo.alpha<10 || BP_PublicInfo.alpha>255) BP_PublicInfo.alpha = 10;
	//	
	
	if(1 == BP_ConfigReference.theme_langed) {
		TCHAR TempLanguagePath[MAX_PATH];
		lstrcpy(TempLanguagePath, BP_PublicInfo.CurrentPath);
		lstrcat(TempLanguagePath, TEXT("\\lang\\"));
		lstrcat(TempLanguagePath, BP_ConfigReference.LangName);
		BP_PublicInfo.LanguageInst = LoadLibrary(TempLanguagePath);
		if (NULL == BP_PublicInfo.LanguageInst) {
			debugMsg(TEXT("Language Load Failed"));
			BP_PublicInfo.LanguageInst = hInst;
		}
	} else BP_PublicInfo.LanguageInst = hInst;


	BP_PublicInfo.PlayRepeat = BP_ConfigReference.pl_repeat;
	BP_PublicInfo.PlayShuffle = BP_ConfigReference.pl_shuffer;
	BP_PublicInfo.PlayList = false;
	BP_PublicInfo.IsOnTop = false;
}

#ifndef SAFE_GET
#define SAFE_GET(x) { if (!x) return false; }
#endif
//Load Skin
bool on_action_LoadSkin() {

	if (!BP_ConfigReference.theme_skined)  {
		on_action_LoadDefaultSkin();
		return true;
	}

	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("RainBow"), &SkinConfig.RainBow.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("RainBow"), &SkinConfig.RainBow.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Controls"), &SkinConfig.Controls.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Controls"), &SkinConfig.Controls.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Volume"), &SkinConfig.Volume.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Volume"), &SkinConfig.Volume.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Seeking"), &SkinConfig.Seeking.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Seeking"), &SkinConfig.Seeking.rect, sizeof(RECT)))

	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Button_Play"), &SkinConfig.Button_Play.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Button_Play"), &SkinConfig.Button_Play.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Button_Stop"), &SkinConfig.Button_Stop.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Button_Stop"), &SkinConfig.Button_Stop.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Button_Next"), &SkinConfig.Button_Next.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Button_Next"), &SkinConfig.Button_Next.rect, sizeof(RECT)))
	SAFE_GET(getSkinConfigStruct(TEXT("DOCK"), TEXT("Button_Back"), &SkinConfig.Button_Back.dock, sizeof(DOCK)))
	SAFE_GET(getSkinConfigStruct(TEXT("RECT"), TEXT("Button_Back"), &SkinConfig.Button_Back.rect, sizeof(RECT)))

	SAFE_GET(getSkinImage(&BP_tris.PaintInfo.pImgSeek, TEXT("trackbar_seek.png")))
	SAFE_GET(getSkinImage(&BP_tris.PaintInfo.pImgSeekAv, TEXT("trackbar_seek_av.png")))
	SAFE_GET(getSkinImage(&BP_tris.PaintInfo.pImgThumb, TEXT("trackbar_seek_thumb.png")))

	SAFE_GET(getSkinImage(&BP_trie.PaintInfo.pImgSeek, TEXT("trackbar_volume.png")))
	SAFE_GET(getSkinImage(&BP_trie.PaintInfo.pImgSeekAv, TEXT("trackbar_volume_av.png")))
	SAFE_GET(getSkinImage(&BP_trie.PaintInfo.pImgThumb, TEXT("trackbar_volume_thumb.png")))

	SAFE_GET(getSkinImage(&SkinConfig.paintInfoControls.pImgBG, TEXT("controlbar.png")))
	SAFE_GET(getSkinImage(&SkinConfig.paintInfoControls.pImgLeft, TEXT("controlbar_left.png")))
	SAFE_GET(getSkinImage(&SkinConfig.paintInfoControls.pImgRight, TEXT("controlbar_right.png")))


	SAFE_GET(getSkinImage(&SkinConfig.pImgButton_Play, TEXT("button_play.png")))
	SAFE_GET(getSkinImage(&SkinConfig.pImgButton_Pause, TEXT("button_pause.png")))
	SAFE_GET(getSkinImage(&SkinConfig.pImgButton_Stop, TEXT("button_stop.png")))
	SAFE_GET(getSkinImage(&SkinConfig.pImgButton_Back, TEXT("button_back.png")))
	SAFE_GET(getSkinImage(&SkinConfig.pImgButton_Next, TEXT("button_next.png")))

	int Temp;
	SAFE_GET(getSkinConfigInt(TEXT("OFFSET"), TEXT("Volume"), &Temp))
	BP_trie.PaintInfo.SkinOffsetX = Temp;
	SAFE_GET(getSkinConfigInt(TEXT("OFFSET"), TEXT("Seeking"), &Temp))
	BP_tris.PaintInfo.SkinOffsetX = Temp;

	int R, B, G;
	SAFE_GET(getSkinConfigInt(TEXT("SKIN"), TEXT("MiniHeight"), &Temp))
	SkinConfig.MiniHeight = Temp;
	SAFE_GET(getSkinConfigInt(TEXT("SKIN"), TEXT("MiniWidth"), &Temp))
	SkinConfig.MiniWidth = Temp;
	SAFE_GET(getSkinConfigInt(TEXT("SKIN"), TEXT("bgR"), &R))
	SAFE_GET(getSkinConfigInt(TEXT("SKIN"), TEXT("bgG"), &G))
	SAFE_GET(getSkinConfigInt(TEXT("SKIN"), TEXT("bgB"), &B))
	SkinConfig.BackGroundColor = RGB(R,G,B);
	return true;
}
void on_action_LoadDefaultSkin() {
	initialSkinDOCK(&SkinConfig.RainBow);
	initialSkinDOCK(&SkinConfig.Controls);
	initialSkinDOCK(&SkinConfig.Volume);
	initialSkinDOCK(&SkinConfig.Seeking);
	SkinConfig.RainBow.dock.left = 1;
	SkinConfig.RainBow.dock.right = 1;
	SkinConfig.RainBow.dock.top = 1;
	SkinConfig.RainBow.dock.bottom = 1;
	SkinConfig.RainBow.rect.left = 0;
	SkinConfig.RainBow.rect.right = 0;
	SkinConfig.RainBow.rect.top = 0;
	SkinConfig.RainBow.rect.bottom = 42;
	SkinConfig.Controls.dock.bottom = true;
	SkinConfig.Controls.dock.left = true;
	SkinConfig.Controls.dock.right = true;
	SkinConfig.Controls.rect.top = 0;
	SkinConfig.Controls.rect.bottom = 42;
	SkinConfig.Volume.dock.top = true;
	SkinConfig.Volume.dock.right = true;
	SkinConfig.Volume.rect.left = 4;
	SkinConfig.Volume.rect.right = 60;
	SkinConfig.Volume.rect.bottom = 12;
	SkinConfig.Seeking.dock.top = true;
	SkinConfig.Seeking.dock.left = true;
	SkinConfig.Seeking.dock.right = true;
	SkinConfig.Seeking.rect.left = 4;
	SkinConfig.Seeking.rect.right = 70;
	SkinConfig.Seeking.rect.bottom = 12;



	initialSkinDOCK(&SkinConfig.Button_Play);
	initialSkinDOCK(&SkinConfig.Button_Stop);
	initialSkinDOCK(&SkinConfig.Button_Next);
	initialSkinDOCK(&SkinConfig.Button_Back);
	SkinConfig.Button_Play.dock.top = true;
	SkinConfig.Button_Play.rect.left = -45;
	SkinConfig.Button_Play.rect.right = 26;
	SkinConfig.Button_Play.rect.top = 12;
	SkinConfig.Button_Play.rect.bottom = 26;
	SkinConfig.Button_Stop.dock.top = true;
	SkinConfig.Button_Stop.rect.left = -15;
	SkinConfig.Button_Stop.rect.right = 26;
	SkinConfig.Button_Stop.rect.top = 12;
	SkinConfig.Button_Stop.rect.bottom = 26;
	SkinConfig.Button_Back.dock.top = true;
	SkinConfig.Button_Back.rect.left = 15;
	SkinConfig.Button_Back.rect.right = 26;
	SkinConfig.Button_Back.rect.top = 12;
	SkinConfig.Button_Back.rect.bottom = 26;
	SkinConfig.Button_Next.dock.top = true;
	SkinConfig.Button_Next.rect.left = 45;
	SkinConfig.Button_Next.rect.right = 26;
	SkinConfig.Button_Next.rect.top = 12;
	SkinConfig.Button_Next.rect.bottom = 26;

	loadPNGFromResource(&BP_tris.PaintInfo.pImgSeek, IDB_PNG_SEEKING, hInst);
	loadPNGFromResource(&BP_tris.PaintInfo.pImgSeekAv, IDB_PNG_SEEKING_AV, hInst);
	loadPNGFromResource(&BP_tris.PaintInfo.pImgThumb, IDB_PNG_SEEKING_THUMB, hInst);
	BP_tris.PaintInfo.SkinOffsetX = 10;

	loadPNGFromResource(&BP_trie.PaintInfo.pImgSeek, IDB_PNG_VOLUME, hInst);
	loadPNGFromResource(&BP_trie.PaintInfo.pImgSeekAv, IDB_PNG_VOLUME_AV, hInst);
	loadPNGFromResource(&BP_trie.PaintInfo.pImgThumb, IDB_PNG_VOLUME_THUMB, hInst);
	BP_trie.PaintInfo.SkinOffsetX = 10;

	loadPNGFromResource(&SkinConfig.paintInfoControls.pImgBG, IDB_PNG_CONTROL, hInst);
	loadPNGFromResource(&SkinConfig.paintInfoControls.pImgLeft, IDB_PNG_CONTROL_LEFT, hInst);
	loadPNGFromResource(&SkinConfig.paintInfoControls.pImgRight, IDB_PNG_CONTROL_RIGHT, hInst);

	loadPNGFromResource(&SkinConfig.pImgButton_Play, IDB_PNG_BUTTON_PLAY, hInst);
	loadPNGFromResource(&SkinConfig.pImgButton_Pause, IDB_PNG_BUTTON_PAUSE, hInst);
	loadPNGFromResource(&SkinConfig.pImgButton_Stop, IDB_PNG_BUTTON_STOP, hInst);
	loadPNGFromResource(&SkinConfig.pImgButton_Back, IDB_PNG_BUTTON_BACK, hInst);
	loadPNGFromResource(&SkinConfig.pImgButton_Next, IDB_PNG_BUTTON_NEXT, hInst);

	SkinConfig.MiniHeight = 120;
	SkinConfig.MiniWidth = 240;
	SkinConfig.BackGroundColor = RGB(198,198,198);
}

void on_action_FullScreenCursorActions() {
	if (BP_PublicInfo.ScreenMode == 1) {
		static POINT cursorLast;
		static POINT cursor;
		static int count = 0;
		static bool show = true;
		static bool showtool = false;
		GetCursorPos(&cursor);
		if (PtInRect(&SkinConfig.Controls.rectExact, cursor)) {
			if (showtool == false) {
				ShowWindow(hWndControls, SW_SHOW);
				SetWindowPos(hWndControls, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				showtool = true;
			}
		} else if (showtool == true ) {
			ShowWindow(hWndControls, SW_HIDE);
			showtool = false;
		}
		if (cursor.x == cursorLast.x && cursorLast.y == cursor.y) {
			count ++ ;
			if (count>5 && show == true) {
				if(ShowCursor(false)<0)	show = false;
			}
		} else {
			cursorLast.x = cursor.x ;
			cursorLast.y = cursor.y;
			count = 0 ;
			if (show == false) {
				if(ShowCursor(true)>=0) show = true;
			}
		}
	}
}


void on_action_SaveConfigFile(ConfigReference * ConfigInfo) {
	TCHAR TempClassName[CommonLength];
	lstrcpy(TempClassName, TEXT("BINPLAYER"));

	int TempValue;
	TempValue = ConfigInfo->theme_skined;
	setConfigInt(TempClassName, TEXT("skined"), TempValue);

	TempValue = ConfigInfo->theme_langed;
	setConfigInt(TempClassName, TEXT("langed"), TempValue);
	
	TempValue = ConfigInfo->general_preventStandby;
	setConfigInt(TempClassName, TEXT("active"), TempValue);
	
	TempValue = ConfigInfo->ui_alwaysShowTray;
	setConfigInt(TempClassName, TEXT("tray"), TempValue);

	TempValue = ConfigInfo->decode_onlyUseSystemDecoder;
	setConfigInt(TempClassName, TEXT("sysdec"), TempValue);

	TempValue = ConfigInfo->debug_saveLogFile;
	setConfigInt(TempClassName, TEXT("log"), TempValue);

	TempValue = ConfigInfo->pl_repeat;
	setConfigInt(TempClassName, TEXT("repeat"), TempValue);

	TempValue = ConfigInfo->pl_shuffer;
	setConfigInt(TempClassName, TEXT("shuffer"), TempValue);

	TempValue = ConfigInfo->pl_saveWhenClose;
	setConfigInt(TempClassName, TEXT("savepl"), TempValue);

	TempValue = ConfigInfo->action_doubleClick;
	setConfigInt(TempClassName, TEXT("dobcli"), TempValue);

	TempValue = ConfigInfo->action_mouseCentre;
	setConfigInt(TempClassName, TEXT("muscen"), TempValue);

	setConfigString(TempClassName, TEXT("skin"), ConfigInfo->SkinName);

	setConfigString(TempClassName, TEXT("lang"), ConfigInfo->LangName);
	
}
void on_action_LoadConfigFile(ConfigReference * ConfigInfo) {
	TCHAR TempClassName[CommonLength];
	lstrcpy(TempClassName, TEXT("BINPLAYER"));

	int TempValue;
	getConfigInt(TempClassName, TEXT("skined"), &TempValue);
	ConfigInfo->theme_skined = TempValue;

	getConfigInt(TempClassName, TEXT("langed"), &TempValue);
	ConfigInfo->theme_langed = TempValue;

	if (!getConfigString(TempClassName, TEXT("skin"), ConfigInfo->SkinName, COMMON_LENGTH)) ConfigInfo->theme_skined = false;

	if (!getConfigString(TempClassName, TEXT("lang"), ConfigInfo->LangName, COMMON_LENGTH)) ConfigInfo->theme_langed = false;

	getConfigInt(TempClassName, TEXT("active"), &TempValue);
	ConfigInfo->general_preventStandby = TempValue;

	getConfigInt(TempClassName, TEXT("tray"), &TempValue);
	ConfigInfo->ui_alwaysShowTray = TempValue;

	getConfigInt(TempClassName, TEXT("sysdec"), &TempValue);
	ConfigInfo->decode_onlyUseSystemDecoder = TempValue;

	getConfigInt(TempClassName, TEXT("log"), &TempValue);
	ConfigInfo->debug_saveLogFile = TempValue;

	getConfigInt(TempClassName, TEXT("repeat"), &TempValue);
	ConfigInfo->pl_repeat = TempValue;

	getConfigInt(TempClassName, TEXT("shuffer"), &TempValue);
	ConfigInfo->pl_shuffer = TempValue;

	getConfigInt(TempClassName, TEXT("savepl"), &TempValue);
	ConfigInfo->pl_saveWhenClose = TempValue;

	getConfigInt(TempClassName, TEXT("dobcli"), &TempValue);
	ConfigInfo->action_doubleClick = TempValue;

	getConfigInt(TempClassName, TEXT("muscen"), &TempValue);
	ConfigInfo->action_mouseCentre = TempValue;
}