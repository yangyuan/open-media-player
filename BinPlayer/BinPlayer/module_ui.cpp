#include "module_ui.h"
#include "control_trackbar.h"
#include "common_actions.h" 
#include "module_playlist.h"

RECT RectRainBow;
RECT RectControl;
RECT RectClientRainBow;
RECT RectClientControl;
WNDPROC DefaultButtonProc;

bool createUIWindows(HWND hWnd) {
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProcRainBow;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= CreateSolidBrush(RGB(0, 0, 0));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("BP_PC_R");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= NULL;
	wcex.lpfnWndProc	= WinProcControls;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= CreateSolidBrush(SkinConfig.BackGroundColor);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("BP_PC_C");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	hWndRainbow = CreateWindowEx(NULL ,
		TEXT("BP_PC_R"),
		TEXT(""), 
		WS_CHILD | WS_VISIBLE|  WS_CLIPCHILDREN ,
		0 , 0, 0, 0,
		hWnd, NULL, hInst, NULL);
	hWndControls = CreateWindowEx(NULL ,
		TEXT("BP_PC_C"),
		TEXT(""), 
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
		0, 0, 0, 0,
		hWnd, NULL, hInst, NULL);

	SetTimer(hWnd, 1, 100 , TimerProc);
	SetTimer(hWnd, 2, 500 , TimerProc);
	SetTimer(hWnd, 3, 45000 , TimerProc);
	return TRUE;
}

bool moveAllWindows(RECT *RectWindows) {
	HDWP temp = BeginDeferWindowPos(2);

	calculateRECTBySkinDOCK(&SkinConfig.RainBow.rectExact, &BP_MainWindowInfomation.WindowClientRect, &SkinConfig.RainBow);
	calculateRECTBySkinDOCK(&SkinConfig.Controls.rectExact, &BP_MainWindowInfomation.WindowClientRect, &SkinConfig.Controls);

	DeferWindowPos(temp, hWndRainbow, NULL,
		SkinConfig.RainBow.rectExact.left, 
		SkinConfig.RainBow.rectExact.top, 
		SkinConfig.RainBow.rectExact.right - SkinConfig.RainBow.rectExact.left, 
		SkinConfig.RainBow.rectExact.bottom - SkinConfig.RainBow.rectExact.top,
		SWP_NOZORDER);
	DeferWindowPos(temp, hWndControls, NULL,
		SkinConfig.Controls.rectExact.left, 
		SkinConfig.Controls.rectExact.top, 
		SkinConfig.Controls.rectExact.right - SkinConfig.Controls.rectExact.left, 
		SkinConfig.Controls.rectExact.bottom - SkinConfig.Controls.rectExact.top,
		SWP_NOZORDER);
	EndDeferWindowPos(temp);
	return TRUE;
}

void calculateRECTAndDeferWindowPos( RECT * ParentRect, SkinDOCK * SkinDock, HDWP temp, HWND hWnd) {
	calculateRECTBySkinDOCK(&SkinDock->rectExact, ParentRect, SkinDock);
	DeferWindowPos(temp, hWnd, NULL,
	SkinDock->rectExact.left, 
	SkinDock->rectExact.top, 
	SkinDock->rectExact.right - SkinDock->rectExact.left, 
	SkinDock->rectExact.bottom -SkinDock->rectExact.top,
	SWP_NOZORDER);
}

bool moveControls(RECT *RectWindows) {
	HDWP temp = BeginDeferWindowPos(6);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Seeking, temp, hWndTrackbarSeeking);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Volume, temp, hWndTrackbarVolume);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Button_Play, temp, hButton_Play);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Button_Stop, temp, hButton_Stop);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Button_Back, temp, hButton_Back);
	calculateRECTAndDeferWindowPos(&RectClientControl, &SkinConfig.Button_Next, temp, hButton_Next);
	EndDeferWindowPos(temp);
	return TRUE;
}

void CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime) {
	switch (nTimerid) {
    case 1:
		on_event_Time_100ms();
		break;
	case 2:
		on_event_Time_500ms();
		break;
	case 3:
		on_event_Time_45s();
		break;
	}
}



//Notice UI that I'm Playing (used by PlayerClass);
void noticeToUIIsPlaying(bool IsPlaying) {
	BP_PublicInfo.IsPlaying = IsPlaying;
	if (!IsPlaying) NotifyTray.hIcon = hIcon_Tray;
	if (BP_PublicInfo.IsInTray) Shell_NotifyIcon(NIM_MODIFY,&NotifyTray);
	reDrawButtonByHand();
}

LRESULT CALLBACK WndProcRainBow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
//		PainBackGround(&ps, &SkinConfig.paintInfoRainBow, &RectClientRainBow);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		GetWindowRect(hWnd, &RectRainBow);
		GetClientRect(hWnd, &RectClientRainBow);
		break;
	case WM_LBUTTONDOWN:
		MapWindowPoints(hWnd, hWndWindowMain, (LPPOINT)(&lParam), 2 );
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONUP:
			PostMessage(hWndWindowMain, message,wParam, MAKELPARAM(GET_X_LPARAM(lParam) + SkinConfig.RainBow.rectExact.left, GET_Y_LPARAM(lParam) + SkinConfig.RainBow.rectExact.top));
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool createControls(HWND hWnd) {
	hWndPlayList = create_PlayList(hWnd);
	Create_Control_TrackBarSeeking(hWnd);
	Create_Control_TrackBarVolume(hWnd);
	hButton_Play = CreateWindowEx(NULL, 
		WC_BUTTON, TEXT(""), 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		0, 0, 0, 0,
		hWnd, (HMENU)ID_PLAY_RESUMEPAUSE, hInst, 0);
	hButton_Stop = CreateWindowEx(NULL, 
		WC_BUTTON, TEXT(""), 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		0, 0, 0, 0,
		hWnd, (HMENU)ID_PLAY_STOP, hInst, 0);
	hButton_Back = CreateWindowEx(NULL, 
		WC_BUTTON, TEXT(""), 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		0, 0, 0, 0,
		hWnd, (HMENU)ID_PLAY_BACK, hInst, 0);
	hButton_Next = CreateWindowEx(NULL, 
		WC_BUTTON, TEXT(""), 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		0, 0, 0, 0,
		hWnd, (HMENU)ID_PLAY_NEXT, hInst, 0);
	long dwValue = GetClassLong(hButton_Play, GCL_STYLE);
	SetClassLong(hButton_Play, GCL_STYLE,  dwValue & ~CS_DBLCLKS);
	SetClassLong(hButton_Stop, GCL_STYLE,  dwValue & ~CS_DBLCLKS);
	SetClassLong(hButton_Back, GCL_STYLE,  dwValue & ~CS_DBLCLKS);
	SetClassLong(hButton_Next, GCL_STYLE,  dwValue & ~CS_DBLCLKS);
	SkinConfig.PaintInfoButton_PlayPause.hwnd = hButton_Play;
	SkinConfig.PaintInfoButton_Stop.hwnd = hButton_Stop;
	SkinConfig.PaintInfoButton_Next.hwnd = hButton_Next;
	SkinConfig.PaintInfoButton_Back.hwnd = hButton_Back;
	
	DefaultButtonProc = (WNDPROC) SetWindowLong(hButton_Play, GWL_WNDPROC, (LONG)WndProcButtons);
	SetWindowLong(hButton_Stop, GWL_WNDPROC, (LONG)WndProcButtons);
	SetWindowLong(hButton_Back, GWL_WNDPROC, (LONG)WndProcButtons);
	SetWindowLong(hButton_Next, GWL_WNDPROC, (LONG)WndProcButtons);
	return true;
}

void PainBackGround(PAINTSTRUCT *Painter, PaintInfoBox * PaintInfo, RECT * ClientRect) {
	Gdiplus::Graphics graphics(Painter->hdc);
	if (NULL != PaintInfo->Buffer) {
		delete PaintInfo->Buffer;
	}
	PaintInfo->Buffer = new Gdiplus::Bitmap(ClientRect->right, ClientRect->bottom);
	Gdiplus::Graphics graphicsBuffer(PaintInfo->Buffer);
	Gdiplus::Rect Temp(0, 0, ClientRect->right, ClientRect->bottom);



	graphicsBuffer.Clear(Gdiplus::Color::Color(GetRValue(SkinConfig.BackGroundColor), GetGValue(SkinConfig.BackGroundColor), GetBValue(SkinConfig.BackGroundColor)));

	graphicsBuffer.DrawImage(PaintInfo->pImgBG, Temp,
		0, 0, 
		PaintInfo->pImgBG->GetWidth() -1, PaintInfo->pImgBG->GetHeight(), Gdiplus::UnitPixel);

	Temp.Width = PaintInfo->pImgLeft->GetWidth();
	Temp.Height = PaintInfo->pImgLeft->GetHeight();
	graphicsBuffer.DrawImage(PaintInfo->pImgLeft, Temp,
		0, 0, 
		Temp.Width, Temp.Height, Gdiplus::UnitPixel);

	Temp.Width = PaintInfo->pImgRight->GetWidth();
	Temp.X = ClientRect->right-Temp.Width;
	Temp.Height = PaintInfo->pImgRight->GetHeight();
	graphicsBuffer.DrawImage(PaintInfo->pImgRight, Temp,
		0, 0, 
		Temp.Width, Temp.Height, Gdiplus::UnitPixel);

	graphics.DrawImage(PaintInfo->Buffer, 0,  0, ClientRect->right, ClientRect->bottom);
}


void ButtonPaint( PaintInfoButton * PaintInfo,  RECT * RectInParent, Gdiplus::Bitmap * BufferParent) {
	HDC hdc = GetDC(PaintInfo->hwnd);
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Rect TempRect(0, 0, RectInParent->right - RectInParent->left, RectInParent->bottom - RectInParent->top);
	Gdiplus::Bitmap Buffer(TempRect.Width, TempRect.Height);
	Gdiplus::Graphics graphicsBuffer(&Buffer);
	graphicsBuffer.DrawImage(BufferParent, TempRect,
	RectInParent->left, RectInParent->top,
	TempRect.Width, TempRect.Height, Gdiplus::UnitPixel);

	graphicsBuffer.DrawImage(PaintInfo->pImgBG, 
		0, 0, 
		PaintInfo->State * TempRect.Width, 0,
		TempRect.Width * 4, TempRect.Height, Gdiplus::UnitPixel);
	graphics.DrawImage(&Buffer, 0,  0, TempRect.Width, TempRect.Height);
	ReleaseDC(PaintInfo->hwnd, hdc);
}

struct TrackInfo {
	bool initirded;
	bool Tracking;
	TRACKMOUSEEVENT tme;
	bool over;
	bool pressing;
} TrackInfos[4];


int getIDByHwnd(HWND hWnd) {
	if (hWnd == hButton_Play) return 0;
	else if (hWnd == hButton_Stop) return 1;
	else if (hWnd == hButton_Back) return 2;
	else if (hWnd == hButton_Next) return 3;
	return -1;
}
void NewTrack(HWND hwnd) {
	int i = getIDByHwnd(hwnd);
	if (-1 == i) return;
	if (!TrackInfos[i].initirded) {
		TrackInfos[i].tme.cbSize = sizeof(TRACKMOUSEEVENT);
		TrackInfos[i].tme.hwndTrack = hwnd;
		TrackInfos[i].tme.dwFlags = TME_LEAVE | TME_HOVER;
		TrackInfos[i].tme.dwHoverTime = 20;
		TrackInfos[i].initirded = true;
	}
	if (TrackInfos[i].Tracking != true) {
		TrackInfos[i].Tracking = true;
		int temp = TrackMouseEvent (&TrackInfos[i].tme);
		temp = 5;
	}
}
void EndTrack(HWND hwnd) {
	int i = getIDByHwnd(hwnd);
	if (-1 == i) return;
	TrackInfos[i].Tracking = false;
}

void SetButtonOver(HWND hwnd, bool over) {
	int i = getIDByHwnd(hwnd);
	if (-1 == i) return;
	TrackInfos[i].over = over;
}

void SetButtonPress(HWND hwnd, bool pressing) {
	int i = getIDByHwnd(hwnd);
	if (-1 == i) return;
	TrackInfos[i].pressing = pressing;
}


LRESULT CALLBACK WinProcControls(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static DRAWITEMSTRUCT* pdis;
	static NMBCHOTITEM* nbhi;
	int TempStatue;
	PAINTSTRUCT ps;
	switch (message) {
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_CREATE:
		createControls(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		ps.hdc = GetDC(hWnd);
		PainBackGround(&ps, &SkinConfig.paintInfoControls, &RectClientControl);
		ReleaseDC(hWnd, ps.hdc);
		SendMessage(hWndTrackbarVolume, WM_PAINT, 0, 0);
		ButtonPaint(&SkinConfig.PaintInfoButton_PlayPause, &SkinConfig.Button_Play.rectExact,  SkinConfig.paintInfoControls.Buffer);
		ButtonPaint(&SkinConfig.PaintInfoButton_Stop, &SkinConfig.Button_Stop.rectExact,  SkinConfig.paintInfoControls.Buffer);
		ButtonPaint(&SkinConfig.PaintInfoButton_Back, &SkinConfig.Button_Back.rectExact,  SkinConfig.paintInfoControls.Buffer);
		ButtonPaint(&SkinConfig.PaintInfoButton_Next, &SkinConfig.Button_Next.rectExact,  SkinConfig.paintInfoControls.Buffer);
		break;
	case WM_SIZE:
		GetWindowRect(hWnd, &RectControl);
		GetClientRect(hWnd, &RectClientControl);
		moveControls(&RectClientControl);
		break;
	case WM_MOUSEHOVER:
		break;
	case WM_COMMAND:
	case WM_KEYDOWN:
		SendMessage(hWndWindowMain,  message,   wParam,   lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool reDrawButtonByHand() {
	if (BP_PublicInfo.IsPlaying) SkinConfig.PaintInfoButton_PlayPause.pImgBG = SkinConfig.pImgButton_Pause;
	else SkinConfig.PaintInfoButton_PlayPause.pImgBG = SkinConfig.pImgButton_Play;
	ButtonPaint(&SkinConfig.PaintInfoButton_PlayPause, &SkinConfig.Button_Play.rectExact,  SkinConfig.paintInfoControls.Buffer);
	return true;
}

// state : 0, normal , 1 over ,2 pressed; -1 for find by yourself
bool reNewButton(HWND hWnd, int state) {
	if (state == -1) {
		int i = getIDByHwnd(hWnd);
		if (TrackInfos[i].pressing) state = 2;
		else if (TrackInfos[i].over) state = 1;
		else state = 0;
	}
	if (hWnd == hButton_Play) {
		SkinConfig.PaintInfoButton_PlayPause.State = state;
		if (BP_PublicInfo.IsPlaying) SkinConfig.PaintInfoButton_PlayPause.pImgBG = SkinConfig.pImgButton_Pause;
		else SkinConfig.PaintInfoButton_PlayPause.pImgBG = SkinConfig.pImgButton_Play;
		ButtonPaint(&SkinConfig.PaintInfoButton_PlayPause, &SkinConfig.Button_Play.rectExact,  SkinConfig.paintInfoControls.Buffer);
	}
	else if (hWnd == hButton_Stop) {
			SkinConfig.PaintInfoButton_Stop.State = state;
			ButtonPaint(&SkinConfig.PaintInfoButton_Stop, &SkinConfig.Button_Stop.rectExact,  SkinConfig.paintInfoControls.Buffer);

	}
	else if (hWnd == hButton_Back) {
			SkinConfig.PaintInfoButton_Back.State = state;
			ButtonPaint(&SkinConfig.PaintInfoButton_Back, &SkinConfig.Button_Back.rectExact,  SkinConfig.paintInfoControls.Buffer);
	}
	else if (hWnd == hButton_Next) {
			SkinConfig.PaintInfoButton_Next.State = state;
			ButtonPaint(&SkinConfig.PaintInfoButton_Next, &SkinConfig.Button_Next.rectExact,  SkinConfig.paintInfoControls.Buffer);
	}
	return true;
}


LRESULT CALLBACK WndProcButtons(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_LBUTTONUP:
		SetButtonPress(hWnd, false);
		SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetMenu(hWnd),hWnd), NULL);
		reNewButton(hWnd, -1);
		break;
	case WM_MOUSEMOVE:
		SetButtonOver(hWnd, true);
		reNewButton(hWnd, -1);
		NewTrack(hWnd);
		break;
	case WM_MOUSELEAVE:
		SetButtonOver(hWnd, false);
		reNewButton(hWnd, 0);
		EndTrack(hWnd);
		break;
	case WM_MOUSEHOVER:
		EndTrack(hWnd);
		break;
	case WM_LBUTTONDOWN:
		SetButtonPress(hWnd, true);
		reNewButton(hWnd, 2);
		break;
	default:
		break;
	}
	return CallWindowProc(DefaultButtonProc, hWnd, message, wParam, lParam);
}