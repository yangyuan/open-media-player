#pragma once
#include "common.h"

//Process Loop
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Config (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChildDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK OpenUrl(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

//Functions
bool solveKeyMessage(WPARAM KeyMessage);
bool initNotifyTray(HWND hWnd);

NOTIFYICONDATA NotifyTray;
