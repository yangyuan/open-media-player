#include "events.h"

void on_event_DropFileToMainWindow(HDROP hDrop) {
	on_action_DropFileAndAdd(hDrop, true);
	on_action_PlayPlayList(0);
	SetFocus(hWndWindowMain);
}
void on_event_DropFileToPlayList(HDROP hDrop) {
	on_action_DropFileAndAdd(hDrop, false);
}
void on_event_PlayListItem_DoubleClick(int index) {
	on_action_PlayPlayList(index);
}
void on_event_MainWindow_Minilize() {
	on_action_HideAllWindows();
	BP_PublicInfo.IsInTray = true;
	Shell_NotifyIcon(NIM_ADD, &NotifyTray);
}

void on_event_Menu_MainWindow_PlayList() {
	BP_PublicInfo.PlayList = IsWindowVisible(hWndPlayList);
	if (BP_PublicInfo.PlayList) on_action_HidePlayList();
	else on_action_ShowPlayList();
}

void on_event_TrayIcon_Click() {
	on_action_ShowMainWindow();
	on_action_HideTrayIcon();
}

void on_event_TrayIcon_RightClick () {
	POINT MousePoint;
	GetCursorPos(&MousePoint);
//	TrackPopupMenu(GetSubMenu(hMenuMain, 2), TPM_RIGHTALIGN, MousePoint.x, MousePoint.y, 0, hWndWindowMain, NULL);
	TrackPopupMenu(GetSubMenu(hMenuTray, 0), TPM_RIGHTALIGN, MousePoint.x, MousePoint.y, 0, hWndWindowMain, NULL);
}

void on_event_Menu_MainWindow_CheckUpdate() {
	on_action_CheckUpdate();
}

void on_event_PlayFinished() {
	on_action_ChangePlayState(0);
	if (BP_PublicInfo.PlayShuffle) on_action_PlayShuffle();
	else on_action_PlayNext(BP_PublicInfo.PlayRepeat);
}

void on_event_Menu_MainWindow_Play_Next() {
	if (BP_PublicInfo.PlayShuffle) on_action_PlayShuffle();
	else on_action_PlayNext(BP_PublicInfo.PlayRepeat);
}
void on_event_Menu_MainWindow_Play_Back() {
	if (BP_PublicInfo.PlayShuffle) on_action_PlayShuffle();
	else on_action_PlayBack(BP_PublicInfo.PlayRepeat);
}
void on_event_Menu_MainWindow_OpenFile() {
	on_action_OpenMediaFileAndAddtoPlayList(true);
}

void on_event_Menu_MainWindow_ResumePause() {
	on_action_PlayPause();
}

void on_event_Menu_MainPlayList_AddFile() {
	on_action_OpenMediaFileAndAddtoPlayList(false);
}
void on_event_Menu_MainPlayList_AddFileFromPlayList() {
	on_action_OpenPlayListFileAndAddtoPlayList();
}
void on_event_Menu_MainPlayList_LoadPlayList() {
	on_action_LoadPlayList(FALSE, FALSE, TRUE);
}
void on_event_Menu_MainPlayList_SaveAs() {
	on_action_SavePlayList(FALSE, TRUE, FALSE);
}
void on_event_Menu_MainPlayList_SavePlayList() {
	on_action_SavePlayList(FALSE, FALSE, FALSE);
}


void on_event_Mouse_Middle() {
	if (BP_ConfigReference.action_mouseCentre) {
		on_action_PlayPause();
	} else {
		on_action_SwitchFullScreen(true, false);
	}
}
void on_event_Mouse_Double() {
	if (BP_ConfigReference.action_doubleClick) {
		on_action_PlayPause();
	} else {
		on_action_SwitchFullScreen(true, false);
	}
}


void on_event_Key_LEFT() {
	on_action_SetSeekPosition(false, -100000000);
}
void on_event_Key_RIGHT() {
	on_action_SetSeekPosition(false, 100000000);
}
void on_event_Key_UP() {
	on_action_SetVolume (false, 0.05);
}
void on_event_Key_DOWN() {
	on_action_SetVolume (false, -0.05);
}
void on_event_Key_RETURN() {
	on_action_SwitchFullScreen(true, false);
}
void on_event_Key_SPACE() {
	on_action_PlayPause();
}

//Time
void on_event_Time_100ms() {
	on_action_CorrectSeekPositionToUI();
	on_action_FullScreenCursorActions();
}
void on_event_Time_500ms() {
	if (BP_PublicInfo.IsInTray && BP_PublicInfo.IsPlaying) {
		if (hIcon_Tray_V == NotifyTray.hIcon) NotifyTray.hIcon = hIcon_Tray;
		else NotifyTray.hIcon = hIcon_Tray_V;
		Shell_NotifyIcon(NIM_MODIFY,&NotifyTray);
	}
}
void on_event_Time_45s() {
	if (BP_PublicInfo.IsPlaying && BP_ConfigReference.general_preventStandby) resetSystemIdleTime();
}

void on_event_Volume_NewPosition(double NewPosition) {
	on_action_SetVolume(true, NewPosition);
}
void on_event_Seeking_NewPosition(double NewPosition) {
	on_action_SetSeekPosition(NewPosition);
}