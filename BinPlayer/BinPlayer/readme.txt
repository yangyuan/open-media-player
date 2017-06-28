every module only include 3 header

common.h
events.h
actions.h


******** Multi Languange solution **********


******** Unsolved Problem ********
LoadString 
loadStringByID not well used


ofn.lpstrTitle = TEXT("Open Media File...\0");
TCHAR szTitle[LENGTH_FILE_MAX] = TEXT("BinPlayer Alpha Version");
TCHAR WindowClassMain[LENGTH_FILE_MAX] = TEXT("BinPlayer");


******** GDI Plus ********
//Gdiplus::FontFamily  fontFamily(L"Tahoma");
//Gdiplus::Font        font(&fontFamily, 11, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 102, 153, 204));
//WCHAR string2[] = L"BinPlayer Alpha Version ";
//graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
//graphics.DrawString(string2, -1, &font, Gdiplus::PointF(10.0f, 10.0f), &solidBrush);

hdc = GetDC(hWnd);
Gdiplus::Graphics graphics(hdc);
Gdiplus::FontFamily  fontFamily(L"Tahoma");
Gdiplus::Font        font(&fontFamily, 11, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 102, 153, 204));
WCHAR       string2[] = L"Font Paint  中文";
//graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
graphics.DrawString(string2, -1, &font, Gdiplus::PointF(10.0f, 10.0f), &solidBrush);
ReleaseDC (hdc);



   1. // DsDemoDlg.cpp : implementation file   
   2. //   
   3.    
   4. #include "stdafx.h"   
   5. #include "DsDemo.h"   
   6. #include "DsDemoDlg.h"   
   7.    
   8.    
   9. #ifdef _DEBUG   
  10. #define new DEBUG_NEW   
  11. #undef THIS_FILE   
  12. static char THIS_FILE[] = __FILE__;   
  13. #endif   
  14.    
  15. /////////////////////////////////////////////////////////////////////////////   
  16. // CDsDemoDlg dialog   
  17.    
  18. CDsDemoDlg::CDsDemoDlg(CWnd* pParent /*=NULL*/)   
  19.     : CDialog(CDsDemoDlg::IDD, pParent)   
  20. {   
  21.     //{{AFX_DATA_INIT(CDsDemoDlg)   
  22.         // NOTE: the ClassWizard will add member initialization here   
  23.     //}}AFX_DATA_INIT   
  24.     // Note that LoadIcon does not require a subsequent DestroyIcon in Win32   
  25.     m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);   
  26. }   
  27.    
  28. void CDsDemoDlg::DoDataExchange(CDataExchange* pDX)   
  29. {   
  30.     CDialog::DoDataExchange(pDX);   
  31.     //{{AFX_DATA_MAP(CDsDemoDlg)   
  32.         // NOTE: the ClassWizard will add DDX and DDV calls here   
  33.     //}}AFX_DATA_MAP   
  34. }   
  35.    
  36. BEGIN_MESSAGE_MAP(CDsDemoDlg, CDialog)   
  37.     //{{AFX_MSG_MAP(CDsDemoDlg)   
  38.     ON_WM_PAINT()   
  39.     ON_WM_QUERYDRAGICON()   
  40.     //}}AFX_MSG_MAP   
  41. END_MESSAGE_MAP()   
  42.    
  43. /////////////////////////////////////////////////////////////////////////////   
  44. // CDsDemoDlg message handlers   
  45.    
  46. BOOL CDsDemoDlg::OnInitDialog()   
  47. {   
  48.     CDialog::OnInitDialog();   
  49.    
  50.     // Set the icon for this dialog.  The framework does this automatically   
  51.     //  when the application's main window is not a dialog   
  52.     SetIcon(m_hIcon, TRUE);         // Set big icon   
  53.     SetIcon(m_hIcon, FALSE);        // Set small icon   
  54.        
  55.     // TODO: Add extra initialization here   
  56.        
  57.     return TRUE;  // return TRUE  unless you set the focus to a control   
  58. }   
  59.    
  60. // If you add a minimize button to your dialog, you will need the code below   
  61. //  to draw the icon.  For MFC applications using the document/view model,   
  62. //  this is automatically done for you by the framework.   
  63.    
  64. void CDsDemoDlg::OnPaint()    
  65. {   
  66.     if (IsIconic())   
  67.     {   
  68.         CPaintDC dc(this); // device context for painting   
  69.    
  70.         SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);   
  71.    
  72.         // Center icon in client rectangle   
  73.         int cxIcon = GetSystemMetrics(SM_CXICON);   
  74.         int cyIcon = GetSystemMetrics(SM_CYICON);   
  75.         CRect rect;   
  76.         GetClientRect(&rect);   
  77.         int x = (rect.Width() - cxIcon + 1) / 2;   
  78.         int y = (rect.Height() - cyIcon + 1) / 2;   
  79.    
  80.         // Draw the icon   
  81.         dc.DrawIcon(x, y, m_hIcon);   
  82.     }   
  83.     else   
  84.     {   
  85.         CDialog::OnPaint();   
  86.     }   
  87. }   
  88.    
  89. // The system calls this to obtain the cursor to display while the user drags   
  90. //  the minimized window.   
  91. HCURSOR CDsDemoDlg::OnQueryDragIcon()   
  92. {   
  93.     return (HCURSOR) m_hIcon;   
  94. }   
  95.    
  96.    
  97. /////////////////////////////////////////////////////////////////////////   
  98. // 枚举Filter上的Pin   
  99. IPin * CDsDemoDlg::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)   
 100. {   
 101.     BOOL       bFound = FALSE;   
 102.     IEnumPins  *pEnum;   
 103.     IPin       *pPin;   
 104.    
 105.     HRESULT hr = pFilter->EnumPins(&pEnum);   
 106.     if (FAILED(hr))   
 107.     {   
 108.         return NULL;   
 109.     }   
 110.     while(pEnum->Next(1, &pPin, 0) == S_OK)   
 111.     {   
 112.         PIN_DIRECTION PinDirThis;   
 113.         pPin->QueryDirection(&PinDirThis);   
 114.         if (bFound = (PinDir == PinDirThis))   
 115.             break;   
 116.         pPin->Release();   
 117.     }   
 118.     pEnum->Release();   
 119.     return (bFound ? pPin : NULL);    
 120. }   
 121.    
 122. // 枚举Pin上的媒体类型   
 123. BOOL CDsDemoDlg::GetMediaType(IPin * inPin)   
 124. {   
 125.     IEnumMediaTypes * pMTEnum = NULL;    
 126.     AM_MEDIA_TYPE *   pAMType = NULL;   
 127.     if (SUCCEEDED(inPin->EnumMediaTypes(&pMTEnum)))   
 128.     {   
 129.         ASSERT(pMTEnum);   
 130.         while (pMTEnum->Next(1, &pAMType, 0) == S_OK)   
 131.         {   
 132.             // 对取得的媒体类型进行处理   
 133.             // ...   
 134.    
 135.             DeleteMediaType(pAMType);   
 136.         }   
 137.         pMTEnum->Release();   
 138.         return TRUE;   
 139.     }   
 140.     return FALSE;   
 141. }   
 142.    
 143. // 判断某个Filter是否已经注册   
 144. BOOL CDsDemoDlg::IsFilterRegistered(CLSID inFilterId)   
 145. {   
 146.     IBaseFilter * pFilter = NULL;   
 147.     if (SUCCEEDED(CoCreateInstance(inFilterId, NULL, CLSCTX_INPROC_SERVER,   
 148.         IID_IBaseFilter, (void **)&pFilter)))   
 149.     {   
 150.         pFilter->Release();   
 151.         return TRUE;   
 152.     }   
 153.     return FALSE;   
 154. }   
 155.    
 156. // 在程序中注册某个Filter文件   
 157. BOOL CDsDemoDlg::RegisterFilter(const char * inFilterAx)   
 158. {   
 159.     typedef (WINAPI * REGISTER_FUNC) (void);   
 160.     REGISTER_FUNC   MyFunc = NULL;   
 161.    
 162.     HMODULE hModule = ::LoadLibrary(inFilterAx);   
 163.     if (hModule)   
 164.     {   
 165.         MyFunc = (REGISTER_FUNC) GetProcAddress(hModule, "DllRegisterServer");   
 166.         BOOL pass = (MyFunc != NULL);   
 167.         if (pass)   
 168.         {   
 169.             MyFunc();   
 170.         }   
 171.         ::FreeLibrary(hModule);   
 172.         return pass;   
 173.     }   
 174.    
 175.     return FALSE;   
 176. }   
 177.    
 178. // 修改Filter的Merit值   
 179. BOOL CDsDemoDlg::SetFilterMerit(const char * inClsid, DWORD inMerit)   
 180. {   
 181.     typedef struct   
 182.     {   
 183.         DWORD   dwVersion;    // 版本号   
 184.         DWORD   dwMerit;      // Merit值   
 185.         DWORD   dwPinCount;   // Pin的数量   
 186.         DWORD   dwReserved;   // 保留   
 187.     } FILTER_HEADER;   
 188.    
 189.     const char *  cRegistryEntry = "CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance\\";   
 190.     const long    cMaxLength = 1024 * 16;   
 191.     BYTE          filterData[cMaxLength];   
 192.     DWORD         actualLength = 0;   
 193.    
 194.     // 生成Filter信息注册部分的注册表入口   
 195.     char   szEntry[1000];   
 196.     strcpy(szEntry, cRegistryEntry);   
 197.     strcat(szEntry, inClsid);   
 198.        
 199.     HKEY hKey   = NULL;   
 200.     LONG result = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, szEntry, 0, KEY_ALL_ACCESS, &hKey);   
 201.     BOOL pass = (result == ERROR_SUCCESS);   
 202.     if (pass)   
 203.     {   
 204.         // 读取FilterData的值   
 205.         actualLength = actualLength;   
 206.         result = ::RegQueryValueEx(hKey, "FilterData", NULL, NULL, filterData, &actualLength);   
 207.         pass   = (result == ERROR_SUCCESS);   
 208.     }   
 209.     if (pass)   
 210.     {   
 211.         // 修改FiterData中Merit部分，然后写回到注册表   
 212.         FILTER_HEADER * filterHeader = (FILTER_HEADER *) filterData;   
 213.         filterHeader->dwMerit = inMerit;   
 214.         result = ::RegSetValueEx(hKey, "FilterData", NULL, REG_BINARY, filterData, actualLength);   
 215.         pass   = (result == ERROR_SUCCESS);   
 216.     }   
 217.     if (hKey)   
 218.     {   
 219.         ::RegCloseKey(hKey);   
 220.     }   
 221.     return pass;   
 222. }   
 
 
	loadPNGFromResource (&pImgButton_Pause, IDB_PNG_BUTTON_PAUSE, hInst);
	loadPNGFromResource (&pImgButton_Play, IDB_PNG_BUTTON_PLAY, hInst);
	loadPNGFromResource (&pImgButton_Stop, IDB_PNG_BUTTON_STOP, hInst);
	loadPNGFromResource (&pImgButton_Back, IDB_PNG_BUTTON_BACK, hInst);
	loadPNGFromResource (&pImgButton_Next, IDB_PNG_BUTTON_NEXT, hInst);