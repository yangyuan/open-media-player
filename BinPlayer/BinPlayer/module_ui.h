#pragma once
#include "common.h"
#include "control_trackbar.h"
#include "module_player.h"

//
bool moveAllWindows(RECT *RectWindows);
void CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime);
LRESULT CALLBACK WinProcControls(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcRainBow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool createUIWindows(HWND hWnd);
void noticeToUIIsPlaying(bool IsPlaying);
void PainBackGround(PAINTSTRUCT *Painter, PaintInfoBox * PaintInfo, RECT * ClientRect);
BOOL IsFilterRegistered(CLSID   inFilterId) ;

bool on_RainBowPaint(HDC hdc);
bool moveControls(RECT *RectWindows);
bool reDrawButtonByHand();
bool reNewButton(HWND hWnd, int state);
LRESULT CALLBACK WndProcButtons(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


