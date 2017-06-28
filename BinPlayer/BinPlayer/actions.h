#pragma once
#include "common.h"
#include "events.h"
#include "common_actions.h"

#include "module_player.h"
#include "module_playlist.h"
#include "module_ui.h"
#include "module_playlist_praser.h"
#include "module_configurer.h"

//windows control
void on_action_HideAllWindows();
void on_action_ShowPlayList();
void on_action_HidePlayList();
void on_action_ShowMainWindow();
void on_action_HideMainWindow();
void on_action_ShowTrayIcon();
void on_action_HideTrayIcon();

//play and playList Control
void on_action_DropFileAndAdd(HDROP hDrop, bool clearBeforeAction);
void on_action_PlayPlayList(int index);
void on_action_PlayNext(bool repeat);
void on_action_PlayBack(bool repeat);
void on_action_PlayShuffle();
void on_action_OpenMediaFileAndAddtoPlayList(bool AutoStart);
void on_action_OpenPlayListFileAndAddtoPlayList();
void on_action_AddFiletoPlayList(TCHAR * Path, bool AutoStart);
BOOL on_action_AddFileItemtoPlayList(PlayListItem * PlayListItemInfo);
// Clean is available only when Dafault is TRUE
BOOL on_action_LoadPlayList(BOOL Default, BOOL Clean, BOOL ChangePath);
// SaveAs is available only when Dafault is FALSE
BOOL on_action_SavePlayList(BOOL Dafault, BOOL SaveAs, BOOL ChangePath);
BOOL on_action_LoadDefaultPlayList();
BOOL on_action_SaveDefaultPlayList();

//Length of Buffer souble more than 24
void on_action_CalculateTimeString(double Position, TCHAR * Buffer);

//for debug 
void on_action_CheckUpdate();

//language control
void on_action_ChangeMenuLang(HINSTANCE);


//following cannot be called by UI
void on_action_playMediaFile(TCHAR * Path);

//Initial
void on_action_InitBeforeCreation();
// after create but not shown
void on_action_InitInsideCreation();
void on_action_InitAfterCreation();
void on_action_InitBeforeClosing();
//initial Config
void on_action_LoadConfig();
//Load Skin
bool on_action_LoadSkin();
void on_action_LoadDefaultSkin();
//Load Menu ; this function called after several main function was maded
void on_action_InitMenus();


void on_action_FullScreenCursorActions();


//Config used actions

void on_action_SaveConfigFile(ConfigReference * ConfigInfo);
void on_action_LoadConfigFile(ConfigReference * ConfigInfo);


//player Control
void on_action_SetVolume (bool Absolute, double NewVolume);
void on_action_SetSeekPosition(bool Absolute, LONGLONG NewPosition);
void on_action_SetSeekPosition(double NewPosition);
void on_action_ChangePlayState(int State);//0 stop, 1 pasue, 2 play 
void on_action_PlayPause();
void on_action_AujustVedioWindow(bool VideoRatioDirection, bool VideoSizeDirection, BOOL AbsoluteCentre = FALSE);
void on_action_SwitchFullScreen(bool JustSwitch, bool ToFullScreen);
void on_action_CorrectSeekPositionToUI();
void on_action_CaptionToCliboard();