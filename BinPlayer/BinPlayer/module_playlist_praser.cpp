#include "module_playlist_praser.h"

module_playlist_reader_bpls::module_playlist_reader_bpls(void) {

	pXMLDom = NULL;

	HRESULT hr;
	//Initialize COM
	CoInitialize(NULL);

	//Initialize DOM
	hr = CoCreateInstance(__uuidof(DOMDocument30),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void**)&pXMLDom);
	if (FAILED(hr)) return;
	pXMLDom->put_async(VARIANT_FALSE);
	pXMLDom->put_validateOnParse(VARIANT_FALSE);
	pXMLDom->put_resolveExternals(VARIANT_FALSE);
}

module_playlist_reader_bpls::~module_playlist_reader_bpls(void) {
	pXMLDom->Release();
	CoUninitialize();
}


BOOL module_playlist_reader_bpls::loadFile(TCHAR * Path) {
	VARIANT_BOOL status;
	VARIANT var;
	//prepare a string to load XML
	VariantInit(&var);
	V_BSTR(&var) = SysAllocString(Path);
	V_VT(&var) = VT_BSTR;

	//Load XML
	pXMLDom->load(var, &status);
	SysFreeString(V_BSTR(&var));
	if (status!=VARIANT_TRUE) {
		return 0;
	}
}

BOOL doActionsWithNode (IXMLDOMNode *pNode, BOOL AddFile(PlayListItem * bpls)) {
	if (NULL == pNode) return FALSE;
	BSTR bstr = NULL;
	IXMLDOMNamedNodeMap * pNodeMap; 
	IXMLDOMNode * pNodeTemp;
	PlayListItem * bplsItem;
	pNode->get_attributes(&pNodeMap);
	if (NULL == pNodeMap) return FALSE;
	// read attributes
	bplsItem = new PlayListItem();
	pNodeMap->getNamedItem(TEXT("t"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		lstrcpy(bplsItem->Title, bstr);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	pNodeMap->getNamedItem(TEXT("a"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		lstrcpy(bplsItem->Atrist, bstr);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	pNodeMap->getNamedItem(TEXT("l"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		lstrcpy(bplsItem->Album, bstr);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	pNodeMap->getNamedItem(TEXT("s"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		lstrcpy(bplsItem->Path, bstr);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	pNodeMap->getNamedItem(TEXT("b"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		char TempChars[MAX_PATH];
		WideCharToMultiByte(CP_ACP , NULL, bstr,-1,TempChars, MAX_PATH ,NULL ,FALSE);
		sscanf(TempChars, "%ld" ,&bplsItem->Start);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	pNodeMap->getNamedItem(TEXT("e"), &pNodeTemp);
	if (NULL != pNodeTemp) {
		pNodeTemp->get_text(&bstr);
		char TempChars[MAX_PATH];
		WideCharToMultiByte(CP_ACP , NULL, bstr,-1,TempChars, MAX_PATH ,NULL ,FALSE);
		sscanf(TempChars, "%ld" ,&bplsItem->End);
		SysFreeString(bstr);
		pNodeTemp->Release();
	}
	AddFile(bplsItem);
	// freeMap
	pNodeMap->Release();
	return TRUE;
}

BOOL module_playlist_reader_bpls::loadToProc(BOOL AddFile(PlayListItem * bpls)) {
	IXMLDOMNode *pNode=NULL;
	IXMLDOMNodeList *pNodes=NULL;
	
	long length;
	pXMLDom->selectNodes(TEXT("//BPF[@t|@a|@l|@s|@b|@e]"), &pNodes);
	if (NULL != pNodes) {
		pNodes->get_length(&length);
		for (long i=0; i<length; i++) {
			pNodes->get_item(i, &pNode);
			if (NULL != pNode) {
				doActionsWithNode(pNode, AddFile);
				pNode->Release();
			}
		}
		pNodes->Release();
	}
	return TRUE;
}
INT module_playlist_reader_bpls::getNumber() {
	IXMLDOMNode *pNode=NULL;
	IXMLDOMNodeList *pNodes=NULL;

	long length;
	pXMLDom->selectNodes(TEXT("//BPF[@t|@a|@l|@s|@b|@e]"), &pNodes);
	if (NULL != pNodes) {
		pNodes->get_length(&length);
		pNodes->Release();
		return length;
	}
	return -1;
}

BOOL setNodeAttribute (IXMLDOMElement * pRoot, TCHAR * AttributeName, TCHAR * AttributeValue) {
	BSTR bstr = SysAllocString(AttributeName);
	VARIANT var;
	VariantInit(&var);
	V_BSTR(&var) = SysAllocString(AttributeValue);
	V_VT(&var) = VT_BSTR;

	pRoot->setAttribute(bstr, var);
	SysFreeString(bstr);
	SysFreeString(V_BSTR(&var));
	return TRUE;
}

BOOL createElement (IXMLDOMDocument *pXMLDom, IXMLDOMElement ** ppRoot,  TCHAR * ElementName) {
	BSTR bstr = SysAllocString(ElementName);
	pXMLDom->createElement(bstr, ppRoot);
	SysFreeString(bstr);
	return TRUE;
}

BOOL BreakLine(IXMLDOMDocument *pXMLDom, IXMLDOMElement * pRoot) {
	HRESULT hr;
	BSTR bstrWs;
	IXMLDOMText *pws=NULL;
	IXMLDOMNode *pBuf=NULL;
	bstrWs = SysAllocString(TEXT("\r\n"));
	pXMLDom->createTextNode(bstrWs,&pws);
	pRoot->appendChild(pws,&pBuf);

	pws->Release();
	pBuf->Release();
	SysFreeString(bstrWs);
	pBuf=NULL;
	return TRUE;
}


module_playlist_writer_bpls::module_playlist_writer_bpls() {
	pXMLDom = NULL;

	BSTR bstr;
	HRESULT hr;
	//Initialize COM
	CoInitialize(NULL);

	//Initialize DOM
	hr = CoCreateInstance(__uuidof(DOMDocument30),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void**)&pXMLDom);
	if (FAILED(hr)) return;
	pXMLDom->put_async(VARIANT_FALSE);
	pXMLDom->put_validateOnParse(VARIANT_FALSE);
	pXMLDom->put_resolveExternals(VARIANT_FALSE);

	
	IXMLDOMNode *pNode=NULL;
	createElement(pXMLDom, &pRoot, TEXT("BPLS"));
	setNodeAttribute(pRoot, TEXT("version"), TEXT("1.0"));
	setNodeAttribute(pRoot, TEXT("name"), TEXT("Playlist"));
	pXMLDom->appendChild(pRoot, &pNode);
	BreakLine(pXMLDom, pRoot);
	pNode->Release();
}

module_playlist_writer_bpls::~module_playlist_writer_bpls() {
	pXMLDom->Release();
	CoUninitialize();
}

BOOL module_playlist_writer_bpls::writeFile(TCHAR * Path) {

	VARIANT_BOOL status;
	VARIANT var;
	//prepare a string to load XML
	VariantInit(&var);
	V_BSTR(&var) = SysAllocString(Path);
	V_VT(&var) = VT_BSTR;

	//Load XML
	pXMLDom->save(var);
	SysFreeString(V_BSTR(&var));
	return TRUE;
}

BOOL module_playlist_writer_bpls::add(INT Index, PlayListItem * bpls) {
	//following is for test;
	IXMLDOMNode *pNode=NULL;
	IXMLDOMElement *pElement;
	createElement(pXMLDom, &pElement, TEXT("BPF"));
	setNodeAttribute(pElement, TEXT("t"), bpls->Title);
	setNodeAttribute(pElement, TEXT("a"), bpls->Atrist);
	setNodeAttribute(pElement, TEXT("l"), bpls->Album);
	setNodeAttribute(pElement, TEXT("s"), bpls->Path);
	TCHAR Buffer[MAX_PATH];
	wsprintf(Buffer, TEXT("%ld\0"), bpls->Start);
	setNodeAttribute(pElement, TEXT("b"), Buffer);
	wsprintf(Buffer, TEXT("%ld\0"), bpls->End);
	setNodeAttribute(pElement, TEXT("e"), Buffer);
	pRoot->appendChild(pElement, &pNode);
	BreakLine(pXMLDom, pRoot);
	pNode->Release();
	return TRUE;
}
INT module_playlist_writer_bpls::getNumber() {
	return -1;
}