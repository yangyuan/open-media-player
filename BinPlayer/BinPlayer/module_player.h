#pragma once
#include "common.h"
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>
#include <math.h>
#include <vector>

#pragma comment (lib,"strmiids.lib")
#pragma comment (lib,"quartz.lib")

#define MODE_DIRECTSHOW 1
#define LENGTH_DIRECTSHOW_FILTER 40

/* class of DirectShow Filter;
*
* when created, it will load Filter File and Initrd Filter Factory
* when deleted, it will unload Filter File
*/
class DirectShowFilter {
public:
	DirectShowFilter(IGraphBuilder * GraphBuilder, LPTSTR szFile ,UINT Number, TCHAR CIDS[][LENGTH_DIRECTSHOW_FILTER]);
	~DirectShowFilter();
	BOOL isFilterRegistered(CLSID inFilterId);
	BOOL loadFilter(HMODULE hLib, TCHAR * CLSID_String);

	BOOL unRegisterFilter (TCHAR * Name);
	BOOL registerFilter (TCHAR * Name);
	UINT GetResult ();
private:
	UINT Result; // 1 all success, 0 partly success, -1 all failed
	HMODULE hMoudle;
	IGraphBuilder * pGraphBuilder;
};

class DirectShowPlayer {
public:
	DirectShowPlayer();
	~DirectShowPlayer();
	//public play actions
	BOOL Play();
	BOOL Pause();
	BOOL Stop();
	INT GetState();

	//public play control
	BOOL GetVolume(DOUBLE &);
	BOOL SetVolume(DOUBLE);

	BOOL GetLength(LONGLONG &);
	BOOL GetPosition(LONGLONG &, LONGLONG &);
	BOOL SetPosition(LONGLONG);
	BOOL SetParentWindow(HWND Parent);
	BOOL GetOriginalVedioSize(LONG & w, LONG & h);
	BOOL SetVedioPosition(LONG x, LONG y, LONG w, LONG h);
	BOOL LoadFilterAndTryRend(LPTSTR szFile, INT & Result);
	//	BOOL RendFile(LPTSTR szFile);
private:
	std::vector<DirectShowFilter *> DirectShowFilters;

	IGraphBuilder *DirectShowGraphBuilder;
	IMediaControl *DirectShowMediaControl;
	IMediaEventEx *DirectShowMediaEvent;
	IVideoWindow *DirectShowVideoWindow;
	IBasicAudio *DirectShowBasicAudio;
	IBasicVideo *DirectShowBasicVideo;
	IMediaSeeking *DirectShowMediaSeeking;
	IMediaPosition *DirectShowMediaPosition;
	IVideoFrameStep *DirectShowVideoFrameStep;
	IFileSourceFilter *DirectShowFileSourceFilter;
	IFilterMapper *DirectShowFilterMapper;
	IFilterGraph *DirectShowFilterGraph;
};


/* this class is main class for play media
* this is the seperated class for play media
*
* there are initialization of GDIPlus and DirectShow, you can remove them if you have done it.
* to improve performance there is no check for paramenters, so be careful
*/

class module_player {
public:
	// the player must include the FileInfomation
	module_player(HWND Parent, LPTSTR szFile, LONGLONG Start, LONGLONG End);
	~module_player();

	//public play actions
	BOOL play_Play();
	BOOL play_Pause();
	BOOL play_Stop();

	//public play control
	BOOL control_GetVolume(DOUBLE &);
	BOOL control_SetVolume(DOUBLE);

	BOOL control_GetLength(LONGLONG &);

	BOOL control_GetPosition(LONGLONG &);
	BOOL control_GetStartPosition(LONGLONG &);
	BOOL control_GetEndPosition(LONGLONG &);

	BOOL control_SetPosition(LONGLONG);
	BOOL control_SetStartPosition(LONGLONG);
	BOOL control_SetEndPosition(LONGLONG);

	BOOL control_GetOriginalVedioSize(LONG & w, LONG & h);
	BOOL control_SetVedioPosition(LONG x, LONG y, LONG w, LONG h);
	BOOL control_GetParentWindow(HWND & ParentWindow);

	//public player and File information
	BOOL player_Infomation(UINT, INT &);
	BOOL player_Data(UINT, VOID *);
private:
	//common used
	HWND core_ParentWindow;
	/* 0 for not used, MODE_DIRECTSHOW for DirectShow, -1 for Total error */
	INT core_DecodeMode;
	/* 0 for not used, 1 for full success, 2, for no audio, 3 for no vedio, 4 for partly but has audio and vedio, -1 cannnot run */
	INT core_DecodeResult;
	/* DirectShow Player */
	DirectShowPlayer * core_DirectShowPlayer;
	//FileInfomation;
	LONGLONG Start;
	LONGLONG End;

};

extern module_player * BP_Player;