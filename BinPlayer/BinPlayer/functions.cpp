#include "functions.h"

unsigned int getFileExtensionOffset(TCHAR * Path) {
	HRESULT hr;
	UINT FilePathLen = 0;
	if (NULL == Path) return MAX_PATH;
	hr = StringCchLength(Path, MAX_PATH , &FilePathLen);
	if (S_OK != hr) return MAX_PATH;
	for (UINT i = 1; i <FilePathLen; i++) {
		if (TEXT('.') == Path[FilePathLen-i-1]) {
			if (S_OK != hr) return MAX_PATH;
			else return FilePathLen-i;
		}
	}
	return MAX_PATH;
}

unsigned int getFileNameOffset(TCHAR * Path) {
	HRESULT hr;
	UINT FilePathLen = 0;
	if (NULL == Path) return MAX_PATH;
	hr = StringCchLength(Path, MAX_PATH , &FilePathLen);
	if (S_OK != hr) return MAX_PATH;
	for (UINT i = 1; i <FilePathLen; i++) {
		if (TEXT('\\') == Path[FilePathLen-i-1]  ||  TEXT('/') == Path[FilePathLen-i-1]) {
			if (S_OK != hr) return MAX_PATH;
			else return FilePathLen-i;
		}
	}
	return MAX_PATH;
}

bool seemsPlayLits(TCHAR * Path) {
	if (NULL == Path) return false;
	unsigned int ExtenOffset = getFileExtensionOffset(Path);
	if (MAX_PATH == ExtenOffset) return false;
	if (lstrcmpi(Path + ExtenOffset, TEXT("txt")) == 0 ) return true;
	return false;
}


bool CheckMenuItemByBool(HMENU hmenu, UINT uIDCheckItem, bool Check ) {
	if (Check) CheckMenuItem(hmenu, uIDCheckItem, MF_BYCOMMAND|MF_CHECKED);
	else CheckMenuItem(hmenu, uIDCheckItem, MF_BYCOMMAND|MF_UNCHECKED);
	return true;
}

bool makeStringByTime(TCHAR * Buffer, LONGLONG Time ,UINT Type ) {
	if (NULL == Buffer || Time <0) return false;
	switch (Type) {
	case 0:
		Time/=1000;
		break;
	case 1:
		Time/=10000000;
		break;
	default:
		return false;
	}
	int TempSeconds = Time%60;
	int TempMinutes = Time/60;
	int TempHours = TempMinutes/60;
	TempMinutes = TempMinutes%60;
	if (0 == TempHours)	wsprintf(Buffer, TEXT("%02d:%02d\0"), TempMinutes, TempSeconds);
	else if (TempHours < 100) wsprintf(Buffer, TEXT("%02d:%02d:%02d\0"), TempHours, TempMinutes, TempSeconds);
	else wsprintf(Buffer, TEXT("N/A\0"));
	return true;
}

