#include "control_trackbar.h"
#include "events.h"

LRESULT CALLBACK TrackBarProcSeeking(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TrackBarProcVolume(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//NewTempX =  X- BP_tris.ThumbHalfWidth
void updateThumbPosition(ClassInfoTrackbar * BPtri, HWND hWnd, long NewTempX, bool UpdateValue) {
	if (NULL != hWnd )	InvalidateRect(hWnd, &BPtri->ThumbRect, FALSE);
		if (NewTempX < 0) NewTempX = 0;
		else if (NewTempX > BPtri->TempLength) NewTempX = BPtri->TempLength;
		BPtri->ThumbRect.left = NewTempX;
		BPtri->ThumbRect.right = NewTempX + BPtri->PaintInfo.ThumbWidth;
		if (UpdateValue) BPtri->Position = NewTempX/(double)BPtri->TempLength;
	if (NULL != hWnd )	InvalidateRect(hWnd, &BPtri->ThumbRect, FALSE);
}

void Create_Control_TrackBarSeeking(HWND hWnd) {
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= TrackBarProcSeeking;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU_MAIN);
	wcex.lpszClassName	= TEXT("BP_PC_S");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	hWndTrackbarSeeking = CreateWindowEx(NULL,
		TEXT("BP_PC_S"),
		TEXT(""),
		WS_CHILD|WS_VISIBLE,
		0, 0, 0, 0, 
		hWnd, (HMENU)IDOK, NULL,0); 

	BP_tris.ThumbRect.top = 0;
	BP_tris.ThumbRect.left = 0;
	BP_tris.ThumbRect.right = BP_tris.PaintInfo.pImgThumb->GetWidth();
	BP_tris.ThumbRect.bottom = 0;
	BP_tris.Position = 0;
	BP_tris.IsActive = false;

}

HWND CreateTrackingToolTip(TOOLINFO * g_toolItem,  HWND hDlg, WCHAR* pText) {
    // Create a ToolTip.
	pText[0] = 0;
	HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hDlg, NULL, hInst,NULL);

    // Set up tool information.
    // In this case, the "tool" is the entire parent window.
	g_toolItem->cbSize = sizeof(TOOLINFO);
    g_toolItem->uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    g_toolItem->hwnd = hDlg;
    g_toolItem->hinst = hInst;
    g_toolItem->lpszText = pText;
    g_toolItem->uId = (UINT_PTR)hDlg;
    GetClientRect (hDlg, &g_toolItem->rect);

    // Associate the ToolTip with the tool window.
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) g_toolItem);	
    return hwndTT;
}

void Create_Control_TrackBarVolume(HWND hWnd) {
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= TrackBarProcVolume;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU_MAIN);
	wcex.lpszClassName	= TEXT("BP_PC_V");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	BP_trie.ThumbRect.top = 0;
	BP_trie.ThumbRect.left = 0;
	BP_trie.ThumbRect.right = BP_trie.PaintInfo.pImgThumb->GetWidth();;
	BP_trie.ThumbRect.bottom = 0;
	BP_trie.Position = 0;
	BP_trie.IsActive = false;


	hWndTrackbarVolume = CreateWindowEx(NULL,
		TEXT("BP_PC_V"),
		TEXT(""),
		WS_CHILD|WS_VISIBLE,
		0, 0, 0, 0, 
		hWnd, (HMENU)IDOK, NULL,0);

// ****************************************************
	setVolumenToUI(1);
	on_event_Volume_NewPosition(1);
	BP_trie.Position = 1;
}


void TrackBarPaint(PAINTSTRUCT *Painter, ClassInfoTrackbar * ClassInfo, RECT * RectInParent, Gdiplus::Bitmap * BufferParent) {


	

	Gdiplus::Graphics graphics(Painter->hdc);
	Gdiplus::Image * imageBG = ClassInfo->PaintInfo.pImgSeek;
	Gdiplus::Image * imageTB = ClassInfo->PaintInfo.pImgThumb;
	Gdiplus::Image * imageBGA = ClassInfo->PaintInfo.pImgSeekAv;

	Gdiplus::Bitmap Buffer(ClassInfo->WindowRect.right, ClassInfo->WindowRect.bottom);
	Gdiplus::Graphics graphicsBuffer(&Buffer);


	int TempX1 = ClassInfo->PaintInfo.SkinOffsetX;
	int TempX2 = ClassInfo->WindowRect.right - TempX1;
	int TempX3 = TempX2 - TempX1;
	int TempX4 = imageBG->GetWidth() - TempX1;
	int TempX5 = TempX4 - TempX1;
	int TempY1 = ClassInfo->WindowRect.bottom;

	Gdiplus::Rect TempRect1(0, 0, TempX1, TempY1);
	Gdiplus::Rect TempRect2(TempX1, 0, TempX3, TempY1);
	Gdiplus::Rect TempRect3(TempX2, 0, TempX1, TempY1);
	Gdiplus::Rect TempRect4(0, 0, RectInParent->right - RectInParent->left, RectInParent->bottom - RectInParent->top);

	graphicsBuffer.DrawImage(BufferParent, TempRect4,
		RectInParent->left, RectInParent->top,
		TempRect4.Width, TempRect4.Height, Gdiplus::UnitPixel);

	graphicsBuffer.DrawImage(imageBG, TempRect1,
		0, 0, 
		TempX1, TempY1, Gdiplus::UnitPixel);
	graphicsBuffer.DrawImage(imageBG, TempRect2,
		TempX1, 0,
		TempX5, TempY1, Gdiplus::UnitPixel);
	graphicsBuffer.DrawImage(imageBG, TempRect3,
		TempX4, 0, 
		TempX1, TempY1, Gdiplus::UnitPixel);
	graphicsBuffer.DrawImage(imageTB, ClassInfo->ThumbRect.left,  ClassInfo->ThumbRect.top, imageTB->GetWidth(), imageTB->GetHeight());


	graphics.DrawImage(&Buffer, 0,  0, Buffer.GetWidth(), Buffer.GetHeight());



	return;
}

LRESULT CALLBACK TrackBarProcSeeking(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static TOOLINFO ToolInfo;
	static bool IsToolInfoTrack = false;
	static POINT pt;
	static TCHAR ToolWords[24];
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) {
	case WM_CREATE:
		//BP_tris.ToolTip = CreateTrackingToolTip(& ToolInfo, hWnd, ToolWords);
		break;
	case WM_ERASEBKGND :
		break; 
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TrackBarPaint(&ps, &BP_tris, &SkinConfig.Seeking.rectExact, SkinConfig.paintInfoControls.Buffer);
		EndPaint(hWnd, &ps);
		DeleteDC (hdc);
		break; 
	case WM_SIZE:
		GetClientRect(hWnd, &BP_tris.WindowRect);
		BP_tris.ThumbRect.bottom = BP_tris.WindowRect.bottom;
		BP_tris.TempLength = BP_tris.WindowRect.right-BP_tris.PaintInfo.ThumbWidth;
		updateThumbPosition(&BP_tris, hWndTrackbarSeeking, BP_tris.Position * BP_tris.TempLength, false);
		break;
	case WM_LBUTTONUP:
		BP_tris.IsActive = FALSE;
		ReleaseCapture();
		//SendMessage(BP_tris.ToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&ToolInfo);
		//IsToolInfoTrack = false;
		break;
	case WM_MOUSEMOVE:
		if ( !BP_tris.IsActive || wParam!=MK_LBUTTON) return 0;
	case WM_LBUTTONDOWN:
		BP_tris.IsActive = TRUE;
		SetCapture(hWnd);
		updateThumbPosition(&BP_tris, hWnd, GET_X_LPARAM(lParam) - BP_tris.PaintInfo.ThumbWidth/2, true);
		on_event_Seeking_NewPosition(BP_tris.Position);


		//pt.x = BP_tris.ThumbRect.left;
		//pt.y = BP_tris.ThumbRect.top - 18;
		//on_action_CalculateTimeString(BP_tris.Position, ToolWords);
		//SendMessage(BP_tris.ToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ToolInfo);
		//ClientToScreen(hWnd, &pt);
		//SendMessage(BP_tris.ToolTip, TTM_TRACKPOSITION,  0, (LPARAM)MAKELONG(pt.x, pt.y));
		//if (!IsToolInfoTrack) {
		//	SendMessage(BP_tris.ToolTip, TTM_TRACKACTIVATE,  (WPARAM)TRUE, (LPARAM)&ToolInfo);
		//	IsToolInfoTrack = true;
		//}


		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


double getPositionFromUI() {
	return BP_tris.Position;
}
bool setPositionToUI(double NewPosition) {
	BP_tris.Position = NewPosition;
	updateThumbPosition(&BP_tris, hWndTrackbarSeeking, NewPosition*BP_tris.TempLength, true);
	return true;
}
double getVolumeFromUI() {
	return BP_trie.Position;
}
bool setVolumenToUI(double NewVolume) {
	BP_trie.Position = NewVolume;
	updateThumbPosition(&BP_trie, hWndTrackbarVolume, NewVolume*BP_trie.TempLength, true);
	return true;
}
LRESULT CALLBACK TrackBarProcVolume(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	static TOOLINFO ToolInfo;
	static bool IsToolInfoTrack = false;
	static POINT pt;
	static TCHAR ToolWords[4];
	HDC hdc;
	switch (message) {
	case WM_CREATE:
		//BP_trie.ToolTip = CreateTrackingToolTip(& ToolInfo, hWnd, ToolWords);
		break;
	case WM_ERASEBKGND :
		break; 
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		ps.hdc = GetDC(hWnd);
		TrackBarPaint(&ps, &BP_trie, &SkinConfig.Volume.rectExact, SkinConfig.paintInfoControls.Buffer);
		ReleaseDC (hWnd, hdc);
		break; 
	case WM_SIZE:
		GetClientRect(hWnd, &BP_trie.WindowRect);
		BP_trie.ThumbRect.bottom = BP_trie.WindowRect.bottom;
		BP_trie.TempLength = BP_trie.WindowRect.right-BP_trie.PaintInfo.ThumbWidth;
		updateThumbPosition(&BP_trie, hWndTrackbarVolume, BP_trie.Position * BP_trie.TempLength, false);
		break;
	case WM_LBUTTONUP:
		BP_trie.IsActive = FALSE;
		//SendMessage(BP_trie.ToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&ToolInfo);
		//IsToolInfoTrack = false;
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if ( !BP_trie.IsActive || wParam!=MK_LBUTTON) return 0;
	case WM_LBUTTONDOWN:
		BP_trie.IsActive = TRUE;
		SetCapture(hWnd);
		updateThumbPosition(&BP_trie, hWnd, GET_X_LPARAM(lParam) - BP_trie.PaintInfo.ThumbWidth/2, true);
		on_event_Volume_NewPosition(BP_trie.Position);

		//pt.x = BP_trie.ThumbRect.left;
		//pt.y = BP_trie.ThumbRect.top - 18;
		//wsprintf(ToolInfo.lpszText, TEXT("%d\0"), (int)(BP_trie.Position * 100));
		//SendMessage(BP_trie.ToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ToolInfo);
		//ClientToScreen(hWnd, &pt);
		//SendMessage(BP_trie.ToolTip, TTM_TRACKPOSITION,  0, (LPARAM)MAKELONG(pt.x, pt.y));
		//if (!IsToolInfoTrack) {
		//	SendMessage(BP_trie.ToolTip, TTM_TRACKACTIVATE,  (WPARAM)TRUE, (LPARAM)&ToolInfo);
		//	IsToolInfoTrack = true;
		//}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}