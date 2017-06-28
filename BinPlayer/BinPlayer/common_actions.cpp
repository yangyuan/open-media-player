#include "common_actions.h" 

TCHAR DebugFilePath[MAX_PATH];
bool DebugFilePathInied = false;

void initedDebugFilePath() {
	lstrcpy(DebugFilePath, BP_PublicInfo.CurrentPath);
	lstrcat(DebugFilePath, TEXT("\\events.log"));
	DebugFilePathInied = true;
}

void debugMsg(TCHAR * Info, int offset) {
	if (!BP_ConfigReference.debug_saveLogFile) return;
	if (! DebugFilePathInied) initedDebugFilePath();
	SYSTEMTIME DebugTime;
	GetLocalTime(&DebugTime);
	TCHAR buf[24];
	wsprintf (buf, TEXT("%02d:%02d:%02d.%03d\0"), DebugTime.wHour, DebugTime.wMinute, DebugTime.wSecond, DebugTime.wMilliseconds+offset);

	WritePrivateProfileString(TEXT("debug"), buf, Info,  DebugFilePath);
}
void debugMsg(int Info, int offset) {
	if (!BP_ConfigReference.debug_saveLogFile) return;
	 TCHAR buf[100];
  //formats and stores a series of characters and values in a buffer
	 wsprintf (buf, TEXT("Got Error ID: x%x\0"), Info);
	 debugMsg(buf, offset);
}

bool loadPNGFromResource (Gdiplus::Image ** pImg, UINT ResourceID, HINSTANCE hInstance) {
	HRSRC hRsrc =   FindResource (hInst, MAKEINTRESOURCE(ResourceID), TEXT("PNG"));
	//Not Found
	if (!hRsrc) return false;
	// load resource into memory
	DWORD len = SizeofResource(hInstance,   hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInstance, hRsrc);
	//Load failed
	if (!lpRsrc) return false;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream*   pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

	*pImg=Gdiplus::Image::FromStream(pstm);

	GlobalUnlock(m_hMem);
	pstm-> Release();
	FreeResource(lpRsrc); 

	return true;
}

bool calculateRECTBySkinDOCK(RECT * rect, RECT * ParentRect, SkinDOCK * SkinDock) {
	if (NULL == rect || NULL == ParentRect || NULL == SkinDock) return false; 

	if (SkinDock->dock.left && SkinDock->dock.right) {
		rect->left = ParentRect->left + SkinDock->rect.left;
		rect->right = ParentRect->right - SkinDock->rect.right;
	} else if (SkinDock->dock.left && !SkinDock->dock.right) {
		rect->left = ParentRect->left + SkinDock->rect.left;
		rect->right = rect->left + SkinDock->rect.right;
	} else if (!SkinDock->dock.left && SkinDock->dock.right) {
		rect->right = ParentRect->right - SkinDock->rect.left;
		rect->left = rect->right - SkinDock->rect.right;
	} else if (!SkinDock->dock.left && !SkinDock->dock.right) {
		rect->left = (ParentRect->right - ParentRect->left- SkinDock->rect.right)/2 + SkinDock->rect.left;
		rect->right = rect->left + SkinDock->rect.right;
	}

	if (SkinDock->dock.top && SkinDock->dock.bottom) {
		rect->top = ParentRect->top + SkinDock->rect.top;
		rect->bottom = ParentRect->bottom - SkinDock->rect.bottom;
	} else if (SkinDock->dock.top && !SkinDock->dock.bottom) {
		rect->top = ParentRect->top + SkinDock->rect.top;
		rect->bottom = rect->top + SkinDock->rect.bottom;
	} else if (!SkinDock->dock.top && SkinDock->dock.bottom) {
		rect->bottom = ParentRect->bottom - SkinDock->rect.top;
		rect->top = rect->bottom - SkinDock->rect.bottom;
	} else if (!SkinDock->dock.top && !SkinDock->dock.bottom) {
		rect->top = (ParentRect->bottom - ParentRect->top- SkinDock->rect.bottom)/2 + SkinDock->rect.top;
		rect->bottom = rect->top + SkinDock->rect.bottom;
	}

	return true;
}

bool openOpenFileDialog(TCHAR * OpendFile, TCHAR * Title, TCHAR * Filter, HWND hWnd, UINT Index) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	*OpendFile = TEXT('\0');

    // Fill in standard structure fields
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hWnd;
    ofn.lpstrFilter       = Filter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = Index;
    ofn.lpstrFile         = OpendFile;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = Title;
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir   = NULL;

    // Create the standard file open dialog and return its result
    return GetOpenFileName((LPOPENFILENAME)&ofn);
}

bool openSaveFileDialog(TCHAR * SaveFile, TCHAR * Title, TCHAR * Filter, HWND hWnd, UINT Index) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	*SaveFile = TEXT('\0');

	// Fill in standard structure fields
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = hWnd;
	ofn.lpstrFilter       = Filter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = Index;
	ofn.lpstrFile         = SaveFile;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrTitle        = Title;
	ofn.lpstrFileTitle    = NULL;
	ofn.lpstrDefExt       = TEXT("*\0");
	ofn.Flags             = OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir   = NULL;

	// Create the standard file open dialog and return its result
	return GetSaveFileName((LPOPENFILENAME)&ofn);
}
void initialSkinDOCK(SkinDOCK* Skin) {
	Skin->dock.left = false;
	Skin->dock.right = false;
	Skin->dock.top = false;
	Skin->dock.bottom = false;
	Skin->rect.left = 0;
	Skin->rect.right = 0;
	Skin->rect.top = 0;
	Skin->rect.bottom = 0;
}

bool loadStringByID(unsigned int ID, TCHAR * lpBuffer,int nBufferMax) {
	int Temp = LoadString(BP_PublicInfo.LanguageInst, ID, lpBuffer, nBufferMax);
	if (0 == Temp) {
		lstrcpy(lpBuffer, TEXT(""));
		return false;
	}
	return true;
}

void IniDlgItemLang(HWND dlg, UINT ItemID, UINT StringID, bool Title){
	static TCHAR Buffer[MAX_PATH];
	loadStringByID(StringID, Buffer, MAX_PATH);
	if (Title) {
		SetWindowText(dlg, Buffer);
	} else {
		SendDlgItemMessage(dlg, ItemID, WM_SETTEXT, NULL, (LPARAM)Buffer);
	}
}

TCHAR ConfigPath[MAX_PATH];
static bool ConfigPathinited = false;

void initedConfigPath() {
	lstrcpy(ConfigPath, BP_PublicInfo.CurrentPath);
	lstrcat(ConfigPath, TEXT("\\config.ini"));
	ConfigPathinited = true;
}

bool getConfigInt(TCHAR * Class, TCHAR * ConfigName, int * Value) {
	if (!ConfigPathinited) initedConfigPath();
	* Value = GetPrivateProfileInt(Class, ConfigName, 0xFFFFFFFF, ConfigPath);
	if (*Value == 0xFFFFFFFF) return false;
	return true;
}

bool setConfigInt(TCHAR * Class, TCHAR * ConfigName, int Value) {
	if (!ConfigPathinited) initedConfigPath();
	static TCHAR ss[MAX_PATH];
	wsprintf(ss, TEXT("%d"), Value);
	WritePrivateProfileString(Class, ConfigName, ss, ConfigPath);
	return true;
}

bool getConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value, int MaxSize) {
	if (!ConfigPathinited) initedConfigPath();
	int Temp = GetPrivateProfileString(Class, ConfigName, NULL, Value, MaxSize ,  ConfigPath);
	if (0 != Temp) return true;
	else return false;
}

bool setConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value) {
	if (!ConfigPathinited) initedConfigPath();
	WritePrivateProfileString(Class, ConfigName, Value,  ConfigPath);
	return true;
}

bool getConfigStruct(TCHAR * Class, TCHAR * ConfigName, void * Value, int Size) {
	if (!ConfigPathinited) initedConfigPath();
	int Temp = GetPrivateProfileStruct(Class, ConfigName,  Value, Size,  ConfigPath);
	if (0 != Temp) return true;
	else return false;
}

TCHAR SkinPath[MAX_PATH];
static bool SkinPathinited = false;

bool getSkinImage(Gdiplus::Image ** Image, TCHAR * ImageName ) {
	if (!SkinPathinited) {
		lstrcpy(SkinPath, BP_PublicInfo.CurrentPath);
		lstrcat(SkinPath, TEXT("\\skin\\"));
		lstrcat(SkinPath, BP_ConfigReference.SkinName);
		lstrcat(SkinPath, TEXT("\\"));
		SkinPathinited = true;
	}
	TCHAR FileName[MAX_PATH];
	lstrcpy(FileName, SkinPath);
	lstrcat(FileName, ImageName);
	if (NULL != *Image) delete *Image;
	*Image =  new Gdiplus::Image(FileName);
	if (NULL != *Image) return true;
	else return false;
}

TCHAR SkinConfigPath[MAX_PATH];
static bool SkinConfiginited = false;

void InitialSkinConfig() {
	lstrcpy(SkinConfigPath, BP_PublicInfo.CurrentPath);
	lstrcat(SkinConfigPath, TEXT("\\skin\\"));
	lstrcat(SkinConfigPath, BP_ConfigReference.SkinName);
	lstrcat(SkinConfigPath, TEXT("\\config.ini"));
	ConfigPathinited = true;
}

bool getSkinConfigInt(TCHAR * Class, TCHAR * ConfigName, int * Value) {
	if (!SkinConfiginited) InitialSkinConfig();
	* Value = GetPrivateProfileInt(Class, ConfigName, 0, SkinConfigPath);
	return true;
}

bool getSkinConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value, int MaxSize) {
	if (!SkinConfiginited) InitialSkinConfig();
	int Temp = GetPrivateProfileString(Class, ConfigName, NULL, Value, MaxSize ,  SkinConfigPath);
	if (0 != Temp) return true;
	else return false;
}

bool getSkinConfigStruct(TCHAR * Class, TCHAR * ConfigName, void * Value, int Size) {
	if (!SkinConfiginited) InitialSkinConfig();
	int Temp = GetPrivateProfileStruct(Class, ConfigName,  Value, Size,  SkinConfigPath);
	if (0 != Temp) return true;
	else return false;
}

void findRecurFiles(TCHAR * CurrentFilePath, void (__cdecl *CallBackFindFile)(TCHAR *), bool Recursive) {
	WIN32_FIND_DATA FindFileData;
	TCHAR TempFilePath[MAX_PATH];
	TCHAR TempFilePathNext[MAX_PATH];
	lstrcpy(TempFilePath, CurrentFilePath);
	lstrcat(TempFilePath, TEXT("\\"));
	lstrcat(TempFilePath, TEXT("*"));
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(TempFilePath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return;

	do {
		if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
			lstrcpy(TempFilePathNext, CurrentFilePath);
			lstrcat(TempFilePathNext, TEXT("\\"));
			lstrcat(TempFilePathNext, FindFileData.cFileName);
			CallBackFindFile(TempFilePathNext);
		}  else if (FindFileData.cFileName[0] != TEXT('.') && Recursive) {
			lstrcpy(TempFilePathNext, CurrentFilePath);
			lstrcat(TempFilePathNext, TEXT("\\"));
			lstrcat(TempFilePathNext, FindFileData.cFileName);
			findRecurFiles(TempFilePathNext, CallBackFindFile);
		}
	} while (FindNextFile(hFind, &FindFileData)!= 0);
	FindClose(hFind);
}

bool AddItemToTreeView (HWND hwndTV, LPWSTR lpszItem, int ImageId, LPARAM lparam) { 
    TVITEM tvi; 
    TVINSERTSTRUCT tvins; 
    HTREEITEM hti; 

    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE; 

    // Set the text of the item. 
    tvi.pszText = lpszItem; 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 

    // Assume the item is not a parent item, so give it a 
    // document image. 
    tvi.iImage = ImageId;
	tvi.iSelectedImage = ImageId;

    // Save the heading level in the item's application-defined 
    // data area. 
    tvi.lParam = lparam; 
    tvins.item = tvi; 
    tvins.hInsertAfter = TVI_LAST; 


    tvins.hParent = TVI_ROOT; 

    // Add the item to the tree-view control. 
    if (NULL == TreeView_InsertItem(hwndTV,&tvins)) return false;
	else return true;
}

DLGTEMPLATE * WINAPI getDlgTemplateByID(UINT DLG_ID) {
    HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(DLG_ID), RT_DIALOG); 
    HGLOBAL hglb = LoadResource(hInst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb); 
} 

void resetSystemIdleTime () {
	SetThreadExecutionState(ES_DISPLAY_REQUIRED);
	SetThreadExecutionState(ES_SYSTEM_REQUIRED);
}

void topTheWindow (HWND hwnd, BOOL Top) {
	if (Top) {
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE);
	} else {
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE);
	}
}