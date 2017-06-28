#pragma once
#include <commctrl.h> 
#pragma comment (linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment (lib,"comctl32.lib")
#include "resource.h"
struct DOCK {
	// if left and right; rect.left and rect.right used
	// if left not right; rect.left used and rect.right used as width;
	// the same to if right not left (rect.right as width)
	// if not left and not right; rect.left used as offset, rect.right used as width;
	// and the same to top and bottom
	bool left;
	bool top;
	bool right;
	bool bottom;
} dock;
RECT rect;

void calculate(HWND hDlg);
void Anticalculate(HWND hDlg);
HWND DialogComboH;
HWND DialogComboV;