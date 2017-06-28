#pragma once
#include "common.h"
bool loadPNGFromResource (Gdiplus::Image ** pImg, UINT ResourceID, HINSTANCE hInstance);
bool calculateRECTBySkinDOCK(RECT * rect, RECT * ParentRect, SkinDOCK * SkinDock);
void debugMsg(TCHAR * Info, int offset = 0);
void debugMsg(int Info, int offset = 0);

bool openOpenFileDialog(TCHAR * OpendFile, TCHAR * Title, TCHAR * Filter, HWND hWnd, UINT Index = 1);
bool openSaveFileDialog(TCHAR * SaveFile, TCHAR * Title, TCHAR * Filter, HWND hWnd, UINT Index = 1);
void initialSkinDOCK(SkinDOCK* Skin);

bool loadStringByID(unsigned int ID, TCHAR * lpBuffer,int nBufferMax);
void IniDlgItemLang(HWND dlg, UINT ItemID, UINT StringID, bool Title = false);

bool getConfigInt(TCHAR * Class, TCHAR * ConfigName, int * Value);
bool getConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value, int MaxSize);
bool getConfigStruct(TCHAR * Class, TCHAR * ConfigName, void * Value, int Size);

bool setConfigInt(TCHAR * Class, TCHAR * ConfigName, int Value);
bool setConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value);


bool getSkinImage(Gdiplus::Image ** Image, TCHAR * ImageName);
bool getSkinConfigInt(TCHAR * Class, TCHAR * ConfigName, int * Value);
bool getSkinConfigString(TCHAR * Class, TCHAR * ConfigName, TCHAR * Value, int MaxSize);
bool getSkinConfigStruct(TCHAR * Class, TCHAR * ConfigName, void * Value, int Size);

// find all files in CurrentFilePath, and call CallBackFindFile when find any file not dir
// Recurisive
// the input must be a dir
void findRecurFiles(TCHAR * CurrentFilePath, void (__cdecl *CallBackFindFile)(TCHAR *), bool Recursive = true);
bool AddItemToTreeView (HWND hwndTV, LPWSTR lpszItem, int ImageId, LPARAM lparam);

// used when need a global DlgTemplate
DLGTEMPLATE * WINAPI getDlgTemplateByID(UINT DLG_ID);

void resetSystemIdleTime ();

void topTheWindow (HWND hwnd, BOOL Top);

