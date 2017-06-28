#pragma once
#include "common.h"
#include "module_ui.h"
#include "module_player.h"


void Create_Control_TrackBarSeeking(HWND hWnd );
void Create_Control_TrackBarVolume(HWND hWnd);

double getPositionFromUI();
bool setPositionToUI(double);
double getVolumeFromUI();
bool setVolumenToUI(double);