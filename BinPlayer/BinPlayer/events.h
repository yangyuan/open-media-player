#pragma once
#include "common.h"
#include "events.h"
#include "actions.h"
#include "functions.h"
// Called when Drag File to the main Window
/*
if themed to be 'a' playlist use it as playList and start playing;
  if analisys failed try to play it. 
else clean the playlist
  and add it to the playlist and play the playlist
if is a dirctionary all multi file
  add all to playlist
*/
void on_event_DropFileToMainWindow(HDROP hDrop);

// Called when Drag File to the PlayList Window
/*
simial to on_event_DropFileToMainWindow, but not clear file before
*/
void on_event_DropFileToPlayList(HDROP hDrop);

//when double click a playlist item
// only avaliable when double click a real item
void on_event_PlayListItem_DoubleClick(int index);

//when MainWindow is Minilized
void on_event_MainWindow_Minilize();

//TrayIcon left clicked
void on_event_TrayIcon_Click();
void on_event_TrayIcon_RightClick();

//Menu Events
void on_event_Menu_MainWindow_PlayList();
void on_event_Menu_MainWindow_CheckUpdate();
void on_event_Menu_MainWindow_Play_Next();
void on_event_Menu_MainWindow_Play_Back();
void on_event_Menu_MainWindow_OpenFile();
void on_event_Menu_MainWindow_ResumePause();
void on_event_Menu_MainPlayList_AddFile();
void on_event_Menu_MainPlayList_AddFileFromPlayList();
void on_event_Menu_MainPlayList_LoadPlayList();
void on_event_Menu_MainPlayList_SaveAs();
void on_event_Menu_MainPlayList_SavePlayList();
//happened when current play is finished
void on_event_PlayFinished();

//key Mouse Events
void on_event_Mouse_Middle();
void on_event_Mouse_Double();

void on_event_Key_LEFT();
void on_event_Key_RIGHT();
void on_event_Key_UP();
void on_event_Key_DOWN();
void on_event_Key_RETURN();
void on_event_Key_SPACE();

//Time
void on_event_Time_100ms();
void on_event_Time_500ms();
void on_event_Time_45s();

void on_event_Volume_NewPosition(double NewPosition);
void on_event_Seeking_NewPosition(double NewPosition);