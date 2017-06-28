#include "module_configurer.h"
#include "common_actions.h" 

INT_PTR CALLBACK cfg_theme_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	int CurrentIndex;
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE);

		IniDlgItemLang(hDlg, IDC_STATIC_LANG, 61021);
		IniDlgItemLang(hDlg, IDC_STATIC_SKIN, 61022);

		SendDlgItemMessage(hDlg, IDC_COMBO_LANG, CB_INSERTSTRING, -1, (LPARAM)TEXT("Default(English)"));
		
		{
			WIN32_FIND_DATA FindFileData;
			TCHAR TempFilePath[MAX_PATH];
			lstrcpy(TempFilePath, BP_PublicInfo.CurrentPath);
			lstrcat(TempFilePath, TEXT("\\lang\\*"));
			HANDLE hFind = INVALID_HANDLE_VALUE;
			hFind = FindFirstFile(TempFilePath, &FindFileData);
			while (INVALID_HANDLE_VALUE  !=  hFind) {
				if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
					int TempIndex = SendDlgItemMessage(hDlg, IDC_COMBO_LANG, CB_INSERTSTRING, -1, (LPARAM)FindFileData.cFileName);
					if (BP_Configer->Temp_ConfigReference.theme_langed) {
						if (lstrcmp(BP_Configer->Temp_ConfigReference.LangName, FindFileData.cFileName)==0) {
							CurrentIndex = TempIndex;
						}
					}
				}
				if (FindNextFile(hFind, &FindFileData)==NULL) break;
			}
			FindClose(hFind);
		}
		if (BP_Configer->Temp_ConfigReference.theme_langed) {
			SendDlgItemMessage(hDlg, IDC_COMBO_LANG, CB_SETCURSEL, CurrentIndex, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_COMBO_LANG, CB_SETCURSEL, 0, NULL);
		}

		SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_INSERTSTRING, -1, (LPARAM)TEXT("Default(BlueSquare)"));
		{
			WIN32_FIND_DATA FindFileData;
			TCHAR TempFilePath[MAX_PATH];
			lstrcpy(TempFilePath, BP_PublicInfo.CurrentPath);
			lstrcat(TempFilePath, TEXT("\\skin\\*"));
			HANDLE hFind = INVALID_HANDLE_VALUE;
			hFind = FindFirstFile(TempFilePath, &FindFileData);
			while (INVALID_HANDLE_VALUE  !=  hFind) {
				if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != TEXT('.')) {
					int TempIndex = SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_INSERTSTRING, -1, (LPARAM)FindFileData.cFileName);
					if (BP_Configer->Temp_ConfigReference.theme_skined) {
						if (lstrcmp(BP_Configer->Temp_ConfigReference.SkinName, FindFileData.cFileName)==0) {
							CurrentIndex = TempIndex;
						}
					}
				}
				if (FindNextFile(hFind, &FindFileData)==NULL) break;
			}
			FindClose(hFind);
		}
		if (BP_Configer->Temp_ConfigReference.theme_skined) {
			SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_SETCURSEL, CurrentIndex, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_SETCURSEL, 0, NULL);
		}


		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_COMBO_SKIN:
			{
				int Temp = SendDlgItemMessage(hDlg,IDC_COMBO_SKIN, CB_GETCURSEL, NULL, NULL);
				if (Temp == 0) BP_Configer->Temp_ConfigReference.theme_skined = false;
				else {
					BP_Configer->Temp_ConfigReference.theme_skined = true;
					SendDlgItemMessage(hDlg,IDC_COMBO_SKIN, CB_GETLBTEXT, Temp, (LPARAM) BP_Configer->Temp_ConfigReference.SkinName);
				}
			}
			break;
		case IDC_COMBO_LANG:
			{
				int Temp = SendDlgItemMessage(hDlg,IDC_COMBO_LANG, CB_GETCURSEL, NULL, NULL);
				if (Temp == 0) BP_Configer->Temp_ConfigReference.theme_langed = false;
				else {
					BP_Configer->Temp_ConfigReference.theme_langed = true;
					SendDlgItemMessage(hDlg,IDC_COMBO_LANG, CB_GETLBTEXT, Temp, (LPARAM) BP_Configer->Temp_ConfigReference.LangName);
				}
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK cfg_actions_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 

		IniDlgItemLang(hDlg, IDC_STATIC_HOTKEY, 61025);
		IniDlgItemLang(hDlg, IDC_STATIC_DOUBLECLICK, 61026);
		IniDlgItemLang(hDlg, IDC_STATIC_CENTREMOUSE, 61031);

		{
			TCHAR Temp[MAX_PATH];
			loadStringByID(61027, Temp, MAX_PATH);
			SendDlgItemMessage(hDlg, IDC_COMBO_DOUBLECLICKVEDIO, CB_INSERTSTRING, -1, (LPARAM)Temp);
			SendDlgItemMessage(hDlg, IDC_COMBO_CENTREMOUSE, CB_INSERTSTRING, -1, (LPARAM)Temp);
			loadStringByID(61028, Temp, MAX_PATH);
			SendDlgItemMessage(hDlg, IDC_COMBO_DOUBLECLICKVEDIO, CB_INSERTSTRING, -1, (LPARAM)Temp);
			SendDlgItemMessage(hDlg, IDC_COMBO_CENTREMOUSE, CB_INSERTSTRING, -1, (LPARAM)Temp);
		}
		SendDlgItemMessage(hDlg, IDC_COMBO_DOUBLECLICKVEDIO, CB_SETCURSEL, BP_Configer->Temp_ConfigReference.action_doubleClick, NULL);
		SendDlgItemMessage(hDlg, IDC_COMBO_CENTREMOUSE, CB_SETCURSEL, BP_Configer->Temp_ConfigReference.action_mouseCentre, NULL);
		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_COMBO_DOUBLECLICKVEDIO:
			BP_Configer->Temp_ConfigReference.action_doubleClick = SendDlgItemMessage(hDlg,IDC_COMBO_DOUBLECLICKVEDIO, CB_GETCURSEL, NULL, NULL);
			break;
		case IDC_COMBO_CENTREMOUSE:
			BP_Configer->Temp_ConfigReference.action_mouseCentre = SendDlgItemMessage(hDlg,IDC_COMBO_CENTREMOUSE, CB_GETCURSEL, NULL, NULL);
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK cfg_playlist_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 

		IniDlgItemLang(hDlg, IDC_STATIC_STARTSTATE, 61023);
		IniDlgItemLang(hDlg, IDC_CHECK_SHUFFER, 60112);
		IniDlgItemLang(hDlg, IDC_CHECK_REPEAT, 60111);
		IniDlgItemLang(hDlg, IDC_CHECK_SAVE_WHEN_CLOSE, 61030);
		


		if (BP_Configer->Temp_ConfigReference.pl_repeat) {
			SendDlgItemMessage(hDlg, IDC_CHECK_REPEAT, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_REPEAT, BM_SETCHECK, BST_UNCHECKED, NULL);
		}

		if (BP_Configer->Temp_ConfigReference.pl_shuffer) {
			SendDlgItemMessage(hDlg, IDC_CHECK_SHUFFER, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_SHUFFER, BM_SETCHECK, BST_UNCHECKED, NULL);
		}

		if (BP_Configer->Temp_ConfigReference.pl_saveWhenClose) {
			SendDlgItemMessage(hDlg, IDC_CHECK_SAVE_WHEN_CLOSE, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_SAVE_WHEN_CLOSE, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_REPEAT:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_REPEAT)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.pl_repeat = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.pl_repeat = false;
				break;
			}
			break;
		case IDC_CHECK_SHUFFER:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_SHUFFER)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.pl_shuffer = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.pl_shuffer = false;
				break;
			}
			break;
		case IDC_CHECK_SAVE_WHEN_CLOSE:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_SAVE_WHEN_CLOSE)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.pl_saveWhenClose = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.pl_saveWhenClose = false;
				break;
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK cfg_debug_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 

		IniDlgItemLang(hDlg, IDC_CHECK_DEBUGLOGFILE, 61029);

		if (BP_Configer->Temp_ConfigReference.debug_saveLogFile) {
			SendDlgItemMessage(hDlg, IDC_CHECK_DEBUGLOGFILE, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_DEBUGLOGFILE, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_DEBUGLOGFILE:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_DEBUGLOGFILE)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.debug_saveLogFile = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.debug_saveLogFile = false;
				break;
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK cfg_decode_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 

		IniDlgItemLang(hDlg, IDC_CHECK_USESYSTEMDECODE, 61024);

		if (BP_Configer->Temp_ConfigReference.decode_onlyUseSystemDecoder) {
			SendDlgItemMessage(hDlg, IDC_CHECK_USESYSTEMDECODE, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_USESYSTEMDECODE, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_USESYSTEMDECODE:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_USESYSTEMDECODE)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.decode_onlyUseSystemDecoder = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.decode_onlyUseSystemDecoder = false;
				break;
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}



INT_PTR CALLBACK cfg_general_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 

		IniDlgItemLang(hDlg, IDC_CHECK_KEEPACTIVE, 61020);

		if (BP_Configer->Temp_ConfigReference.general_preventStandby) {
			SendDlgItemMessage(hDlg, IDC_CHECK_KEEPACTIVE, BM_SETCHECK, BST_CHECKED, NULL);
		} else {
			SendDlgItemMessage(hDlg, IDC_CHECK_KEEPACTIVE, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		//
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_KEEPACTIVE:
			switch(IsDlgButtonChecked(hDlg, IDC_CHECK_KEEPACTIVE)) {
			case BST_CHECKED:
				BP_Configer->Temp_ConfigReference.general_preventStandby = true;
				break;
			case BST_UNCHECKED:
				BP_Configer->Temp_ConfigReference.general_preventStandby = false;
				break;
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ChildDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 
			BP_Configer->rectConfigDialog.left, BP_Configer->rectConfigDialog.top, 
        0, 0, SWP_NOSIZE); 
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_SAVE:
			break;
		}
		return (INT_PTR)TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}


module_configurer::module_configurer() {

	int i;
	HICON hicon;
	himl = ImageList_Create(16, 16, ILC_COLOR32, NUMBER_ITEMS, 20); 

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_GENERAL));
	ItemInfoS[0].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[0].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_GENERAL);
	ItemInfoS[0].StringID = 61001;
	ItemInfoS[0].CallBackProc = cfg_general_proc;

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_THEME));
	ItemInfoS[1].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[1].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_THEME);
	ItemInfoS[1].StringID = 61002;
	ItemInfoS[1].CallBackProc = cfg_theme_proc;

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_PLAYLIST));
	ItemInfoS[2].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[2].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_PLAYLIST);
	ItemInfoS[2].StringID = 61003;
	ItemInfoS[2].CallBackProc = cfg_playlist_proc;

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_DECODE));
	ItemInfoS[3].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[3].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_DECODE);
	ItemInfoS[3].StringID = 61006;
	ItemInfoS[3].CallBackProc = cfg_decode_proc;

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_KEY));
	ItemInfoS[4].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[4].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_ACTIONS);
	ItemInfoS[4].StringID = 61004;
	ItemInfoS[4].CallBackProc = cfg_actions_proc;

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CFG_DEBUG));
	ItemInfoS[5].iconID = ImageList_AddIcon(himl, hicon); 
	ItemInfoS[5].dlgTemplate = getDlgTemplateByID(IDD_DIALOG_CFG_DEBUG);
	ItemInfoS[5].StringID = 61005;
	ItemInfoS[5].CallBackProc = cfg_debug_proc;
	

}

ConfigerItemsInfo * module_configurer::getItemByID(int ID) {
	if (ID <0 || ID >= NUMBER_ITEMS) return NULL;
	return &ItemInfoS[ID];
}

int module_configurer::getItemsCount() {
	return NUMBER_ITEMS;
}

HIMAGELIST module_configurer::getImageList() {
	return himl;
}

module_configurer * BP_Configer;