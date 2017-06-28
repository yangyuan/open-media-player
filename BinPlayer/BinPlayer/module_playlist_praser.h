#pragma once
#include "common.h"


class module_playlist_reader
{
public:
	virtual BOOL loadFile(TCHAR * Path) = 0;
	virtual BOOL loadToProc(BOOL AddFileProc(PlayListItem * bpls)) = 0;
	virtual INT getNumber() = 0;
};

class module_playlist_writer
{
public:
	virtual BOOL writeFile(TCHAR * Path) = 0;
	// Index -1 end;
	virtual BOOL add(INT Index, PlayListItem * bpls) = 0;
	virtual INT getNumber() = 0;
};

class module_playlist_reader_bpls : module_playlist_reader
{
public:
	module_playlist_reader_bpls(void);
	~module_playlist_reader_bpls(void);
	BOOL loadFile(TCHAR * Path);
	BOOL loadToProc(BOOL AddFile(PlayListItem * bpls));
	INT getNumber();
private:
	IXMLDOMDocument *pXMLDom;
};



class module_playlist_writer_bpls : module_playlist_writer
{
public:
	module_playlist_writer_bpls(void);
	~module_playlist_writer_bpls(void);
	BOOL writeFile(TCHAR * Path);
	BOOL add(INT Index, PlayListItem * bpls);
	INT getNumber();
private:
	IXMLDOMDocument *pXMLDom;
	IXMLDOMElement *pRoot;
};

