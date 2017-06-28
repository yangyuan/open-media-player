#pragma once
#define COMMON_LENGTH 20
#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <objbase.h>
#include <msxml2.h>
#include <strsafe.h>
#include "resource.h"
#include "global.h"

#include <commctrl.h> 
#pragma comment (linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment (lib,"comctl32.lib")
#pragma comment (lib,"gdiplus.lib")  

#define MASSAGE_MEDIA_EVENT 50001
#define WM_NOTIFYICON 50002

#define FILE_VERSION TEXT("BinPlayer 1.0.0.16")
#define VERSION_CHECK TEXT("http://www.bin32.com/check?id=1&ver=16")



//the Files can be opened
#define FILE_OPEN_FILTER_TEXT \
	TEXT("All Media Files\0*.wav; *.wma; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd; *.asf; *.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v; *.wmv; *.mid; *.midi; *.rmi; *.rm; *.rmvb; *.mkv\0")\
	TEXT("All PlayList Files\0*.bpls\0")\
    TEXT("All Files\0*.*;\0\0")

#define VOLUMN_DEFAULT 100
#define SEEKING_POSITION_DEFAULT 0


