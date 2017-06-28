#pragma once
extern HWND hWndWindowMain;//Handle of Main Window
extern HINSTANCE hInst;//Instance Handle
extern HWND hWndPlayList;//Handle of Plsylist ListBox
extern HWND hWndTrackbarSeeking;//
extern HWND hWndTrackbarVolume;//
extern HWND hWndRainbow;//
extern HWND hWndTime;//
extern HWND hWndFileInfo;//
extern HWND hWndControls;//

extern HMENU hMenuMain;
extern HMENU hMenuTray;
extern HMENU hMenuPlayListPop;

extern RECT RectWindowsDefault;


extern HWND hButton_Play;
extern HWND hButton_Stop;
extern HWND hButton_Back;
extern HWND hButton_Next;

extern HICON hIcon_False;
extern HICON hIcon_True;




struct publicInfo {
	TCHAR CurrentPath[MAX_PATH];
	int alpha;



	HINSTANCE LanguageInst;
	bool PlayRepeat;//repeat?
	bool PlayShuffle;
	bool PlayList;//repeat?
	bool IsInTray;
	bool IsPlaying;
	bool IsOnTop;

	// 0 normal ; 1 full screen; 2 Switching
	UINT ScreenMode;
};
extern publicInfo BP_PublicInfo;

// this struct is used for discript the location of a item on ui

struct DOCK {
	// if left and right; rect.left and rect.right used
	// if left not right; rect.left used and rect.right used as width;
	// the same to if right not left (rect.right as width)
	// if not left and not right; rect.left used as offset, rect.right used as width;
	// and the same to top and bottom
	bool left;
	bool top;
	bool right;
	bool bottom;
};

struct SkinDOCK {
	DOCK dock;
	RECT rect;
	RECT rectExact;
};

struct PaintInfoBox {
	Gdiplus::Image * pImgBG;
	Gdiplus::Image * pImgLeft;
	Gdiplus::Image * pImgRight;
	Gdiplus::Bitmap * Buffer;
};

struct PaintInfoTrackBar {
	long SkinOffsetX;
	long ThumbWidth;
	Gdiplus::Image * pImgSeek;
	Gdiplus::Image * pImgSeekAv;
	Gdiplus::Image * pImgThumb;
};

struct PaintInfoButton {
	HWND hwnd;
	Gdiplus::Image * pImgBG;
	int State;//0 normal, 1 pressed, 2 fly, 3 not available
};

struct SkinConfiguration {
	long MiniHeight;
	long MiniWidth;
	SkinDOCK RainBow;
	SkinDOCK Controls;
	SkinDOCK Seeking;
	SkinDOCK Volume;

	SkinDOCK Button_Play;
	SkinDOCK Button_Stop;
	SkinDOCK Button_Next;
	SkinDOCK Button_Back;

	PaintInfoButton PaintInfoButton_PlayPause;
	PaintInfoButton PaintInfoButton_Stop;
	PaintInfoButton PaintInfoButton_Next;
	PaintInfoButton PaintInfoButton_Back;

	Gdiplus::Image * pImgButton_Play;
	Gdiplus::Image * pImgButton_Pause;
	Gdiplus::Image * pImgButton_Stop;
	Gdiplus::Image * pImgButton_Back;
	Gdiplus::Image * pImgButton_Next;

	COLORREF BackGroundColor;

	PaintInfoBox paintInfoControls;
};

extern SkinConfiguration SkinConfig;

struct CurrentFileInfomation {
	int currentFileIndexOfPlayList;
};

extern CurrentFileInfomation BP_CurrentInfomation;

struct MainWindowInfomation {
	//Changed only when resize
	RECT WindowRect;
	RECT WindowClientRect;
	//For Moving Infomation
	bool MovingIsActive;
	int MovingPosX; 
	int MovingPosY;
};

extern MainWindowInfomation BP_MainWindowInfomation;

// This struct is for store the infmation of two Trackbar
// BP_tris is for Seeking Position
// BP_trie is for Volume Control
struct ClassInfoTrackbar {
	RECT WindowRect;
	RECT ThumbRect;
	double Position;
	double PositionAv;
	bool IsActive;
	PaintInfoTrackBar PaintInfo;
	//Fillowing are Temp;
	long TempLength;//for WM_LBUTTONDOWN
	HWND ToolTip;
};
extern ClassInfoTrackbar BP_tris, BP_trie;

extern HICON hIcon_Tray;
extern HICON hIcon_Tray_V;
extern NOTIFYICONDATA NotifyTray;

struct PlayListItem {
    TCHAR Title[MAX_PATH];
	TCHAR Atrist[MAX_PATH];
	TCHAR Album[MAX_PATH];
	TCHAR Path[MAX_PATH];
	LONGLONG Start;
	LONGLONG End;
	DWORD Ident;
};

struct ConfigReference {
	bool general_preventStandby;
	bool ui_alwaysShowTray;
	bool decode_onlyUseSystemDecoder;
	bool debug_saveLogFile;
	bool theme_skined;
	bool theme_langed;

	bool pl_shuffer;
	bool pl_repeat;
	bool pl_saveWhenClose;

	//0 fuul screen, 1 palypuase
	int action_doubleClick;
	int action_mouseCentre;

	TCHAR SkinName[COMMON_LENGTH];
	TCHAR LangName[COMMON_LENGTH];
};

extern ConfigReference BP_ConfigReference;


// this struct is used by Configure Dialog Tree View;

typedef INT_PTR (CALLBACK * CALLBACK_PROC )(HWND, UINT, WPARAM, LPARAM);

struct ConfigerItemsInfo {
	CALLBACK_PROC CallBackProc;
	UINT iconID;
	UINT StringID;
	HWND hwnd;
	DLGTEMPLATE * dlgTemplate;
};





