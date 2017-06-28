#pragma once
#include <list>
#include "common.h"
#include "events.h"

LRESULT CALLBACK WinProcPlayList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND create_PlayList(HWND hWnd);
bool loadPlayList(TCHAR *);

//true return when is trun to show, false for hide
bool ShowPlayList(bool JustSwitch, bool ShowHide);

//-1 for failed ,else for success and the current index of the new item
int playList_AddFile(TCHAR * NewFile, int index);
int playList_AddFile(PlayListItem * PlayListItemInfo, int index);
void playList_clear();
void playList_deleteItem(int index);
int playList_GetItemNumber();
void playList_DeleteSelected(BOOL DiskAlso);
//
BOOL playList_loadPlaylist(TCHAR * Path, BOOL changePath);
// when Path is null , it will save to current path
BOOL playList_savePlaylist(TCHAR * Path, BOOL changePath);

//null if failed
PlayListItem * playList_GetItemByIndex(int index);
TCHAR * playList_GetFileByIndex(int index);

TCHAR CurrentPlayListPath[];