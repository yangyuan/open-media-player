// BinPlayer_SkinMaker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BinPlayer_SkinMaker.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING] = TEXT("BinPlay_SM");			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	InitCommonControls();
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BINPLAYER_SKINMAKER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_BINPLAYER_SKINMAKER));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, NULL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   return TRUE;
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
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SKINDOCK), hWnd, About);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
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
	switch (message)
	{
	case WM_INITDIALOG:
		DialogComboH = GetDlgItem(hDlg, IDC_COMBO_H);
		SendMessage(DialogComboH, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("水平居中"));
		SendMessage(DialogComboH, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("左距固定"));
		SendMessage(DialogComboH, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("右距固定"));
		SendMessage(DialogComboH, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("左右固定"));
		DialogComboV = GetDlgItem(hDlg, IDC_COMBO_V);
		SendMessage(DialogComboV, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("竖直居中"));
		SendMessage(DialogComboV, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("上距固定"));
		SendMessage(DialogComboV, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("下距固定"));
		SendMessage(DialogComboV, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("上下固定"));
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			calculate(hDlg);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK2) {
			Anticalculate(hDlg);
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

void MsgNotFinish(HWND hDlg) {
	MessageBox(hDlg, TEXT("请检查是否填写完毕且正确"), TEXT("注意"), MB_OK);
}

void calculate(HWND hDlg) {



	int Temp;
	Temp = SendMessage( DialogComboH, (UINT) CB_GETCURSEL, 0, 0 ); 
	switch(Temp) {
	case 0:
		dock.left = false;
		dock.right = false;
		break;
	case 1:
		dock.left = true;
		dock.right = false;
		break;
	case 2:
		dock.left = false;
		dock.right = true;
		break;
	case 3:
		dock.left = true;
		dock.right = true;
		break;
	default:
		MsgNotFinish(hDlg);
		return;
	}
	Temp = SendMessage( DialogComboV, (UINT) CB_GETCURSEL, 0, 0 ); 
	switch(Temp) {
	case 0:
		dock.top = false;
		dock.bottom = false;
		break;
	case 1:
		dock.top = true;
		dock.bottom = false;
		break;
	case 2:
		dock.top = false;
		dock.bottom = true;
		break;
	case 3:
		dock.top = true;
		dock.bottom = true;
		break;
	default:
		MsgNotFinish(hDlg);
		return;
	}
	BOOL lpTranslated;
	rect.left = GetDlgItemInt( hDlg, IDC_EDIT_H1, &lpTranslated, TRUE);
	if (!lpTranslated) {
		MsgNotFinish(hDlg);
		return;
	}
	rect.right = GetDlgItemInt( hDlg, IDC_EDIT_H2, &lpTranslated, TRUE);
	if (!lpTranslated) {
		MsgNotFinish(hDlg);
		return;
	}
	rect.top = GetDlgItemInt( hDlg, IDC_EDIT_V1, &lpTranslated, TRUE);
	if (!lpTranslated) {
		MsgNotFinish(hDlg);
		return;
	}
	rect.bottom = GetDlgItemInt( hDlg, IDC_EDIT_V2, &lpTranslated, TRUE);
	if (!lpTranslated) {
		MsgNotFinish(hDlg);
		return;
	}

	TCHAR ConfigPath[16];
	TCHAR Classname[12];
	lstrcpy(ConfigPath, TEXT("SkinMaker.ini"));
	lstrcpy(Classname, TEXT("BPSM_TEMP"));
	WritePrivateProfileStruct(Classname, TEXT("DOCK"), &dock, sizeof(DOCK),  ConfigPath);
	WritePrivateProfileStruct(Classname, TEXT("RECT"), &rect, sizeof(RECT),  ConfigPath);

	TCHAR Buffer[64];
	GetPrivateProfileString(Classname, TEXT("DOCK"), NULL, Buffer, 64,  ConfigPath);
	SetDlgItemText(hDlg, IDC_EDIT_DOCK, Buffer);
	GetPrivateProfileString(Classname, TEXT("RECT"), NULL, Buffer, 64,  ConfigPath);
	SetDlgItemText(hDlg, IDC_EDIT_RECT, Buffer);
	return;
}

void MsgCannotRead(HWND hDlg) {
	MessageBox(hDlg, TEXT("数据无法分析,请检查是否填写正确"), TEXT("注意"), MB_OK);
}

void Anticalculate(HWND hDlg) {

	TCHAR ConfigPath[16];
	TCHAR Classname[12];

	lstrcpy(ConfigPath, TEXT("SkinMaker.ini"));
	lstrcpy(Classname, TEXT("BPSM_TEMP"));

	TCHAR Buffer[64];
	if(!GetDlgItemText(hDlg, IDC_EDIT_DOCK, Buffer, 64)) {
		MsgCannotRead(hDlg);
		return;
	}
	WritePrivateProfileString(Classname, TEXT("DOCK"), Buffer, ConfigPath);

	if(!GetDlgItemText(hDlg, IDC_EDIT_RECT, Buffer, 64)) {
		MsgCannotRead(hDlg);
		return;
	}
	WritePrivateProfileString(Classname, TEXT("RECT"), Buffer,  ConfigPath);




	if(!GetPrivateProfileStruct(Classname, TEXT("DOCK"), &dock, sizeof(DOCK),  ConfigPath)) {
		MsgCannotRead(hDlg);
		return;
	}
	if(!GetPrivateProfileStruct(Classname, TEXT("RECT"), &rect, sizeof(RECT),  ConfigPath)) {
		MsgCannotRead(hDlg);
		return;
	}



	int Temp;
	Temp = 0;
	if (dock.right) Temp+=2;
	if (dock.left) Temp+=1;

	SendMessage(DialogComboH, (UINT) CB_SETCURSEL, (WPARAM)Temp, 0 ); 

	Temp = 0;
	if (dock.bottom) Temp+=2;
	if (dock.top) Temp+=1;

	SendMessage( DialogComboV, (UINT) CB_SETCURSEL, (WPARAM)Temp, 0 ); 

	SetDlgItemInt( hDlg, IDC_EDIT_H1, rect.left, TRUE);
	SetDlgItemInt( hDlg, IDC_EDIT_H2, rect.right, TRUE);
	SetDlgItemInt( hDlg, IDC_EDIT_V1, rect.top, TRUE);
	SetDlgItemInt( hDlg, IDC_EDIT_V2, rect.bottom, TRUE);

	return;
}