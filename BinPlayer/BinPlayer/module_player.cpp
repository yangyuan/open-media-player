#include "module_player.h"

module_player * BP_Player;


module_player::module_player(HWND Parent, LPTSTR szFile, LONGLONG NewStart, LONGLONG NewEnd) {
	core_ParentWindow = Parent;
	core_DecodeMode = 0;
	core_DecodeResult = 0;
	core_DirectShowPlayer = NULL;

	Start = NewStart;
	End = NewEnd;

	core_DirectShowPlayer = new DirectShowPlayer();
	if (core_DirectShowPlayer->LoadFilterAndTryRend(szFile, core_DecodeResult)) {
		core_DecodeMode = MODE_DIRECTSHOW;
		core_DirectShowPlayer->SetParentWindow(Parent);
	} else {
	}
}

module_player::~module_player() {
	if (core_DirectShowPlayer) delete core_DirectShowPlayer;
	return;
}
BOOL module_player::play_Play() {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->Play();
	}
	return FALSE;
}
BOOL module_player::play_Pause() {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->Pause();
	}
	return FALSE;
}
BOOL module_player::play_Stop() {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->Stop();
	}
	return FALSE;
}
BOOL module_player::control_GetVolume(DOUBLE & Volume) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->GetVolume(Volume);
	}
	return FALSE;
}
BOOL module_player::control_SetVolume(DOUBLE Volume) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->SetVolume(Volume);
	}
	return FALSE;
}
BOOL module_player::control_GetLength(LONGLONG & Length) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->GetLength(Length);
	}
	return FALSE;
}
BOOL module_player::control_GetPosition(LONGLONG & Position) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		LONGLONG StopPosition;
		return core_DirectShowPlayer->GetPosition(Position, StopPosition);
	}
	return FALSE;
}
BOOL module_player::control_GetStartPosition(LONGLONG & Position) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		Position = 0;
		return TRUE;
	}
	return FALSE;
}
BOOL module_player::control_GetEndPosition(LONGLONG & Position) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->GetLength(Position);
	}
	return FALSE;
}

BOOL module_player::control_SetPosition(LONGLONG Position) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->SetPosition(Position);
	}
	return FALSE;
}
BOOL module_player::control_SetStartPosition(LONGLONG) {
	return FALSE;
}
BOOL module_player::control_SetEndPosition(LONGLONG) {
	return FALSE;
}
BOOL module_player::control_GetOriginalVedioSize(LONG & w, LONG & h) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->GetOriginalVedioSize(w, h);
	}
	return FALSE;
}
BOOL module_player::control_SetVedioPosition(LONG x, LONG y, LONG w, LONG h) {
	if (MODE_DIRECTSHOW == core_DecodeMode) {
		return core_DirectShowPlayer->SetVedioPosition(x, y, w, h);
	}
	return FALSE;
}
BOOL module_player::control_GetParentWindow(HWND & ParentWindow) {
	if (NULL != core_ParentWindow) {
		ParentWindow = core_ParentWindow;
		return TRUE;
	} else {
		return FALSE;
	}
}
//public player and File information
BOOL module_player::player_Infomation(UINT ID, INT & Value) {
	switch (ID) {
		// Decode Result
		case 1: Value = core_DecodeResult;
			return TRUE;
		// Play State :0, stop; 1 pause; 2 running
		case 2: 
			if (MODE_DIRECTSHOW == core_DecodeMode) {
				Value = core_DirectShowPlayer->GetState();
				return TRUE;
			} else return FALSE;
		default:
			return FALSE;
	}

}
BOOL module_player::player_Data(UINT, VOID *) {
	return FALSE;
}


//************************ following are for directshow player *****************************

DirectShowPlayer::DirectShowPlayer() {
	// Get the interface for DirectShow's GraphBuilder
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&DirectShowGraphBuilder);
	// QueryInterface for DirectShow interfaces
	DirectShowGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&DirectShowMediaControl);
	DirectShowGraphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&DirectShowMediaEvent);
	DirectShowGraphBuilder->QueryInterface(IID_IMediaSeeking, (void **)&DirectShowMediaSeeking);
	DirectShowGraphBuilder->QueryInterface(IID_IMediaPosition, (void **)&DirectShowMediaPosition);
	// Query for video interfaces, which may not be relevant for audio files
	DirectShowGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&DirectShowVideoWindow);
	DirectShowGraphBuilder->QueryInterface(IID_IBasicVideo, (void **)&DirectShowBasicVideo);
	// Query for audio interfaces, which may not be relevant for video-only files
	DirectShowGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&DirectShowBasicAudio);
	DirectShowGraphBuilder->QueryInterface(IID_IFilterMapper, (void **)&DirectShowFilterMapper);
	DirectShowGraphBuilder->QueryInterface(IID_IFilterGraph, (void **)&DirectShowFilterGraph);
}

DirectShowPlayer::~DirectShowPlayer() {

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) { x->Release(); x = NULL; } }
#endif

	SAFE_RELEASE(DirectShowGraphBuilder);
	SAFE_RELEASE(DirectShowMediaControl);
	SAFE_RELEASE(DirectShowMediaEvent);
	SAFE_RELEASE(DirectShowMediaSeeking);
	SAFE_RELEASE(DirectShowMediaPosition);
	SAFE_RELEASE(DirectShowVideoWindow);
	SAFE_RELEASE(DirectShowBasicVideo);
	SAFE_RELEASE(DirectShowBasicAudio);
	SAFE_RELEASE(DirectShowFilterMapper);
	SAFE_RELEASE(DirectShowFilterGraph);

	//Clean Filters
	UINT Number = DirectShowFilters.size();
	for (UINT i =0; i<Number; i++) {
		delete DirectShowFilters.at(i);
	}
	DirectShowFilters.clear();
}

BOOL DirectShowPlayer::LoadFilterAndTryRend(LPTSTR szFile, INT & Result) {
	/************
	{
		TCHAR TempSCID[2][40] = {
			{TEXT("{CEA8DEFF-0AF7-4DB9-9A38-FB3C3AEFC0DE}")},
			{TEXT("{9736D831-9D6C-4E72-B6E7-560EF9181001}")}
		};
		DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("AviSplitter.ax"), 2, TempSCID);
		DirectShowFilters.push_back(TempFilter);
	}
	*************/



	if (!BP_ConfigReference.decode_onlyUseSystemDecoder) {
		{
			TCHAR TempSCID[3][40] = {
				{TEXT("{238D0F23-5DC9-45A6-9BE2-666160C324DD}")},
				{TEXT("{941A4793-A705-4312-8DFC-C11CA05F397E}")},
				{TEXT("{A4E38A50-618A-4B20-ABC6-551B5C661E9F}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("tsf.dll"), 3, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
		{
			TCHAR TempSCID[4][LENGTH_DIRECTSHOW_FILTER] = {
				{TEXT("{238D0F23-5DC9-45A7-9BE2-666160C324DD}")},
				{TEXT("{765035B3-5944-4A94-806B-20EE3415F26F}")},
				{TEXT("{941A4793-A705-4313-8DFC-C11CA05F397E}")},
				{TEXT("{E21BE468-5C18-43EC-B0CC-DB93A847D769}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("RealMediaSplitter.ax"), 4, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
		{
			TCHAR TempSCID[1][LENGTH_DIRECTSHOW_FILTER] = {
				{TEXT("{78766964-0000-0010-8000-00AA00389B71}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("divxdec.ax"), 1, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
		{
			TCHAR TempSCID[1][LENGTH_DIRECTSHOW_FILTER] = {
				{TEXT("{6F513D27-97C3-453C-87FE-B24AE50B1601}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("DivXDecH264.ax"), 1, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
		{
			TCHAR TempSCID[2][LENGTH_DIRECTSHOW_FILTER] = {
				{TEXT("{0A68C3B5-9164-4A54-AFAF-995B2FF0E0D4}")},
				{TEXT("{149D2E01-C32E-4939-80F6-C07B81015A7A}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("MatroskaSplitter.ax"), 2, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
		{
			TCHAR TempSCID[1][LENGTH_DIRECTSHOW_FILTER] = {
				{TEXT("{3D446B6F-71DE-4437-BE15-8CE47174340F}")}
			};
			DirectShowFilter * TempFilter = new DirectShowFilter(DirectShowGraphBuilder, TEXT("MpaDecFilter.ax"), 1, TempSCID);
			DirectShowFilters.push_back(TempFilter);
		}
	}

	HRESULT hr = DirectShowGraphBuilder->RenderFile(szFile, NULL);

	switch(hr) {
	case S_OK:
		Result = 1;
		break;
	case VFW_S_AUDIO_NOT_RENDERED:
		Result = 2;
		break;
	case VFW_S_VIDEO_NOT_RENDERED:
		Result = 3;
		break;
	case VFW_S_DUPLICATE_NAME:
	case VFW_S_PARTIAL_RENDER:
		Result = 4;
		break;
	default:
		Result = -1;
		break;
	}
	return TRUE;
}

BOOL DirectShowPlayer::SetParentWindow(HWND Parent) {
	DirectShowVideoWindow->put_Owner((OAHWND)Parent);
	DirectShowVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	DirectShowVideoWindow->put_MessageDrain((OAHWND)Parent);
	return TRUE;
}
BOOL DirectShowPlayer::GetOriginalVedioSize(LONG & w, LONG & h) {
	if ( DirectShowBasicVideo  == NULL ) return FALSE;
	HRESULT hr = DirectShowBasicVideo->GetVideoSize(&w, &h);
	if (hr != S_OK) return FALSE;
	else return TRUE;
}
BOOL DirectShowPlayer::SetVedioPosition(LONG x, LONG y, LONG w, LONG h) {
	if ( DirectShowVideoWindow  == NULL ) return FALSE;
	HRESULT hr = DirectShowVideoWindow->SetWindowPosition(x, y, w, h);
	if (hr != S_OK) return FALSE;
	else return TRUE;
}


BOOL DirectShowPlayer::Play() {
	if (DirectShowMediaControl != NULL) {
		if (S_OK == DirectShowMediaControl->Run()) return TRUE;
		else return FALSE;

	}
	return FALSE;
}

BOOL DirectShowPlayer::Pause() {
	if (DirectShowMediaControl != NULL) {
		DirectShowMediaControl->Pause();
		return TRUE;
	}
	return FALSE;
}

BOOL DirectShowPlayer::Stop() {
	if (DirectShowMediaControl != NULL) {
		DirectShowMediaControl->Stop();
		return TRUE;
	}
	return FALSE;
}

INT DirectShowPlayer::GetState() {
	if (DirectShowMediaControl != NULL) {
		OAFilterState pfs;
		DirectShowMediaControl->GetState(0,&pfs);
		if (pfs == State_Running) {
			return 2;
		} else if (pfs == State_Paused) {
			return 1;
		} else return 0;
	}
	return -1;
}

BOOL DirectShowPlayer::SetVolume(DOUBLE NewVolume) {
	if (NewVolume < 0 || NewVolume > 1) return FALSE;
	NewVolume = 1.0 - pow(NewVolume, 0.2);
	LONG TempVolume = NewVolume *10000;
	if (DirectShowBasicAudio != NULL) {
		DirectShowBasicAudio->put_Volume(-TempVolume);
		return TRUE;
	}
	return FALSE;
}
BOOL DirectShowPlayer::GetVolume(DOUBLE & Volume) {
	LONG Temp;
	if (DirectShowBasicAudio != NULL) {
		DirectShowBasicAudio->get_Volume(&Temp);
		Volume = Temp / 10000;
		Volume = pow(1.0 - Volume, 5.0);
		return TRUE;
	}
}

BOOL DirectShowPlayer::GetLength(LONGLONG & Length) {
	if (DirectShowMediaSeeking != NULL) {
		DirectShowMediaSeeking->GetDuration( &Length);
		return TRUE;
	}
	return FALSE;
}

BOOL DirectShowPlayer::GetPosition(LONGLONG & Position, LONGLONG & StopPosition) {
	if (DirectShowMediaSeeking != NULL) {
		DirectShowMediaSeeking->GetPositions(&Position,&StopPosition);
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL DirectShowPlayer::SetPosition(LONGLONG NewSeekPos) {
	if (DirectShowMediaSeeking != NULL) {
		DirectShowMediaSeeking->SetPositions(&NewSeekPos, AM_SEEKING_AbsolutePositioning , NULL, AM_SEEKING_NoPositioning);
		return TRUE;
	}
	return FALSE;
}


//************************ following are for directshow filters *****************************

DirectShowFilter::DirectShowFilter(IGraphBuilder * GraphBuilder, LPTSTR szFile, UINT Number, TCHAR CIDS[][LENGTH_DIRECTSHOW_FILTER]) {
	pGraphBuilder = GraphBuilder;
	HMODULE hMoudle = LoadLibrary(szFile);
	int FailedNumber = 0;
	for (int i=0; i< Number; i++) {
		if (!loadFilter(hMoudle, CIDS[i])) {
			FailedNumber ++;
		}
	}
	if (FailedNumber == 0) {
		Result = 1;
	} else if (FailedNumber == Number) {
		Result = -1;
	} else {
		Result = 0;
	}
}

DirectShowFilter::~DirectShowFilter() {
	if (hMoudle) FreeLibrary(hMoudle);
}

UINT DirectShowFilter::GetResult () {
	return Result;
}

BOOL DirectShowFilter::loadFilter(HMODULE hLib, TCHAR * CLSID_String) {
	typedef HRESULT (__stdcall * PDllGetClassObject)(REFCLSID rclsid,  REFIID riid, LPVOID * ppv );
	IBaseFilter * Filter = NULL; 
	IClassFactory * pCF = NULL;
	GUID CLSID_Temp;
	if (NULL == hLib) return FALSE;
	CLSIDFromString(CLSID_String ,&CLSID_Temp); 
	if(PDllGetClassObject p = (PDllGetClassObject)GetProcAddress(hLib, "DllGetClassObject")) {
		HRESULT hr;
		hr = p(CLSID_Temp, IID_IClassFactory, (void **)&pCF);
		if(!pCF) {
			return FALSE;
		}
		hr = pCF->CreateInstance(NULL, IID_IBaseFilter,(void**)&Filter);
		if(!Filter) return FALSE;
		pCF->Release();
		hr = pGraphBuilder->AddFilter(Filter, NULL);
		if(FAILED(hr)){
			return FALSE;
		}
	} else return FALSE;
	return TRUE;
}

BOOL DirectShowFilter::isFilterRegistered(CLSID inFilterId) {
	IBaseFilter *pFilter = NULL;
	HRESULT hr = CoCreateInstance(inFilterId,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void**)&pFilter);
	if(SUCCEEDED(hr)) {
		pFilter->Release();
		return TRUE;
	}
	return FALSE;  
}


BOOL DirectShowFilter::registerFilter (TCHAR * Name) {
	if(HMODULE h = LoadLibrary(Name))
	{
		typedef HRESULT (__stdcall * PDllRegisterServer)();
		if(PDllRegisterServer p = (PDllRegisterServer)GetProcAddress(h, "DllRegisterServer"))
		{
			p();
		}
		FreeLibrary(h);
	}
	return TRUE;
}

BOOL DirectShowFilter::unRegisterFilter (TCHAR * Name) {
	if(HMODULE h = LoadLibrary(Name))
	{
		typedef HRESULT (__stdcall * PDllRegisterServer)();
		if(PDllRegisterServer p = (PDllRegisterServer)GetProcAddress(h, "DllUnregisterServer"))
		{
			p();
		}
		FreeLibrary(h);
	}
	return TRUE;
}