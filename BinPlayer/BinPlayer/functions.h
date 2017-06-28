#pragma once
#include "common.h"

// get the extension of a file
//Return Value 
// the offset of the Extension or MAX_PATH for failed;
//Remarks
// Path remains no change
unsigned int getFileExtensionOffset(TCHAR * Path);

// whether the file has the extension of a playlist
bool seemsPlayLits(TCHAR * Path);

bool CheckMenuItemByBool(HMENU hmenu, UINT uIDCheckItem, bool Check);

// turn Time into string ex. 00:00, 03:10, 01:23:54, 99:59:59, N/A
//Paramenter
//  Buffer: sizeof Buffer should be no lesser than 9
//  Type: 0 for ms
//Return Value 
//  true if success , false failed
bool makeStringByTime(TCHAR * Buffer, LONGLONG Time ,UINT Type );
unsigned int getFileNameOffset(TCHAR * Path);
