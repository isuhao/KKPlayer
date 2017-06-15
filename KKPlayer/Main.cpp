// MangageTrade.cpp : main source file for MangageTrade.exe
//

#include "stdafx.h"

#include "resource.h"
#include "MainFrm.h"
#include <GdiPlus.h>
#include <Gdiplusinit.h>
#include "MainPage/MainDlg.h"
#include "MainPage/SUIVideo.h"
#include "MainPage/KKWkeWebkit.h"
#include "control/kkmclv.h"22
#include "Control/kkseek.h"
//#include "DownManage/AVDownManage.h"
#include "Dir/Dir.hpp"
#include "Tool/CFileMgr.h"
#include "Tool/cchinesecode.h"
#include "Tool/FileRelation.h"
#include "Tool/WinDir.h"

#include "SqlOp/HistoryInfoMgr.h"
#include "../KKPlayerCore/KKPlugin.h"
#include "../KKPlayerCore/KKPlayer.h"
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Kernel32.lib")


void DeclareDumpFile();
CAppModule _Module;


SOUI::CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
HWND G_Parent=NULL;
CMainFrame *pWnd;
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	int nRet = theLoop.Run();
    _Module.RemoveMessageLoop();
	
	return nRet;
}

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
SOUI::CMainDlg *m_pDlgMain=NULL;

//F:\ProgramTool\OpenPro\KKPlayer\KKPlayer>uiresbuilder.exe -iuires/uires.idx -puires -rres/KK_res.rc2
std::basic_string<char> GetModulePathA();
void skpngZhuc();



CreateRender pfnCreateRender = NULL;
DelRender pfnDelRender=NULL;
SOUI::SApplication *theApp=NULL;
HINSTANCE GhInstance;
void RelationIco()
{
    CFileRelation FileRelation;
	CWinDir windir;
	std::wstring Localpath=windir.GetModulePath();
	std::wstring pathIco=Localpath+L"KKIcons.dll,"; 
	std::wstring Propath=Localpath+_T("\\KKplayer.exe");
	
	
	bool bReIcon=FileRelation.CheckFileRelation(L".3gp",L"KKPlayer.3gp");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-101"; 
       
		FileRelation.RegisterFileRelation(_T(".3gp"),(LPTSTR)Propath.c_str(),_T("KKPlayer.3gp"),(LPTSTR)pathIco.c_str(),_T(".3gp"));
	}

	bReIcon=FileRelation.CheckFileRelation(L".avi",L"KKPlayer.avi");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-102"; 
       
		FileRelation.RegisterFileRelation(_T(".avi"),(LPTSTR)Propath.c_str(),_T("KKPlayer.avi"),(LPTSTR)pathIco.c_str(),_T(".avi"));
	}
	bReIcon=FileRelation.CheckFileRelation(L".bt",L"KKPlayer.bt");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-103"; 
       
		FileRelation.RegisterFileRelation(_T(".bt"),(LPTSTR)Propath.c_str(),_T("KKPlayer.bt"),(LPTSTR)pathIco.c_str(),_T(".bt"));
	}

    bReIcon=FileRelation.CheckFileRelation(L".flv",L"KKPlayer.flv");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-104"; 
       
		FileRelation.RegisterFileRelation(_T(".flv"),(LPTSTR)Propath.c_str(),_T("KKPlayer.flv"),(LPTSTR)pathIco.c_str(),_T(".flv"));
	}
	bReIcon=FileRelation.CheckFileRelation(L".mkv",L"KKPlayer.mkv");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-105"; 
       
		FileRelation.RegisterFileRelation(_T(".mkv"),(LPTSTR)Propath.c_str(),_T("KKPlayer.mkv"),(LPTSTR)pathIco.c_str(),_T(".mkv"));
	}
	bReIcon=FileRelation.CheckFileRelation(L".mov",L"KKPlayer.mov");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-106"; 
       
		FileRelation.RegisterFileRelation(_T(".mov"),(LPTSTR)Propath.c_str(),_T("KKPlayer.mov"),(LPTSTR)pathIco.c_str(),_T(".mov"));
	}

	bReIcon=FileRelation.CheckFileRelation(L".mp4",L"KKPlayer.mp4");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-107"; 
       
		FileRelation.RegisterFileRelation(_T(".mp4"),(LPTSTR)Propath.c_str(),_T("KKPlayer.mp4"),(LPTSTR)pathIco.c_str(),_T(".mp4"));
	}
	
	bReIcon=FileRelation.CheckFileRelation(L".mov",L"KKPlayer.mpg");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-108"; 
       
		FileRelation.RegisterFileRelation(_T(".mpg"),(LPTSTR)Propath.c_str(),_T("KKPlayer.mpg"),(LPTSTR)pathIco.c_str(),_T(".mpg"));
	}

	bReIcon=FileRelation.CheckFileRelation(L".rm",L"KKPlayer.rm");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-109"; 
       
		FileRelation.RegisterFileRelation(_T(".rm"),(LPTSTR)Propath.c_str(),_T("KKPlayer.rm"),(LPTSTR)pathIco.c_str(),_T(".rm"));
	}

	bReIcon=FileRelation.CheckFileRelation(L".rmvb",L"KKPlayer.rmvb");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-110"; 
       
		FileRelation.RegisterFileRelation(_T(".rmvb"),(LPTSTR)Propath.c_str(),_T("KKPlayer.rmvb"),(LPTSTR)pathIco.c_str(),_T(".rmvb"));
	}

	bReIcon=FileRelation.CheckFileRelation(L".wma",L"KKPlayer.wma");
	if(!bReIcon){
		pathIco=Localpath+L"KKIcons.dll,-111"; 
		FileRelation.RegisterFileRelation(_T(".wma"),(LPTSTR)Propath.c_str(),_T("KKPlayer.wma"),(LPTSTR)pathIco.c_str(),_T(".wma"));
	}
	
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

	LPWSTR *szArglist = NULL;   
	int nArgs = 0;   
	std::wstring  urlpath;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);   
	if( NULL != szArglist)   
	{   
         for(int i=1;i<nArgs;i++)
		 {
		     urlpath+=*(szArglist+i);
			 if(i!=nArgs-1)
			   urlpath+=L" ";
		 }
	} 
	LocalFree(szArglist);

    DeclareDumpFile();
    
    GhInstance=hInstance;
	FARPROC spdpia = GetProcAddress(GetModuleHandle(TEXT("user32")), "SetProcessDPIAware");  
	if(spdpia!=NULL)
	{
	   spdpia();
	}

	CWinDir windir;
	std::wstring Propath=windir.GetUserAppDir();
	Propath+=L"\\kkplayer\\";
	windir.CreateDir(Propath.c_str());
	Propath+=L"DB";
	std::string pp;
	CChineseCode::UnicodeToUTF8((wchar_t*)Propath.c_str(),pp);/**/

	RelationIco();
	
	CHistoryInfoMgr *hismgr=CHistoryInfoMgr::GetInance();
	hismgr->SetPath(pp.c_str());
	hismgr->InitDb();
	hismgr->GetH264Codec();
	hismgr->GetH265Codec();
	
	HMODULE hRender = LoadLibraryA("Render.dll");
	if(hRender){
          pfnCreateRender = (CreateRender)GetProcAddress(hRender, "CreateRender");
		  pfnDelRender = (DelRender)GetProcAddress(hRender, "DelRender");
	}
	//装载
	std::string strPath= GetModulePathA();
	strPath+="\\Plugin";

	std::list<std::string> DllPathInfoList;
	dir::listFiles(DllPathInfoList,strPath,"dll");
	std::list<std::string>::iterator It=DllPathInfoList.begin();
	int Lenxx=sizeof( __KKPluginInfo);
	//
	for (;It!=DllPathInfoList.end();++It)
	{

		//char ptl[32];
		///******创建一个插件******/
		//fCreateKKPlugin CreKKP;
		///**********删除一个插件**************/
		//fDeleteKKPlugin DelKKp;
		///***********下载文件*****************/
		//fKKDownAVFile   KKDownAVFile;
		///************停止下载*****************/
		//fKKStopDownAVFile KKStopDownAVFile;
		//fKKDownAVFileSpeedInfo KKDownAVFileSpeed;
		//fFree KKFree;

		HMODULE	hdll= LoadLibraryA((*It).c_str());
		fCreateKKPlugin        pfn = (fCreateKKPlugin)GetProcAddress(hdll, "CreateKKPlugin");
		fGetPtlHeader          pfGetPtl=(fGetPtlHeader)GetProcAddress(hdll, "GetPtlHeader");
		fDeleteKKPlugin        pDel=(fDeleteKKPlugin)GetProcAddress(hdll, "DeleteKKPlugin");
        fKKDownAVFile          pKKDownAVFile=(fKKDownAVFile)GetProcAddress(hdll, "KKDownAVFile");
		fKKPauseDownAVFile     pKKPauseDownAVFile=(fKKPauseDownAVFile)GetProcAddress(hdll, "KKPauseDownAVFile");
		fFree                  pKKFree=(fFree)GetProcAddress(hdll, "KKFree");
        fKKDownAVFileSpeedInfo pKKDownAVFileSpeedInfo=(fKKDownAVFileSpeedInfo)GetProcAddress(hdll, "KKDownAVFileSpeedInfo");
        fKKUrlParser           pKKUrlParser=(fKKUrlParser)GetProcAddress(hdll, "KKUrlParser");
		if(pfn!=NULL&&pfGetPtl!=NULL&& pDel!=NULL)
		{
			
				KKPluginInfo Info;
				pfGetPtl(Info.ptl,32);
				Info.CreKKP= pfn;
				Info.DelKKp=pDel;
                Info.Handle=hdll;
				Info.KKDownAVFile=pKKDownAVFile;
				Info.KKPauseDownAVFile=pKKPauseDownAVFile;
				Info.KKFree=pKKFree;
				Info.KKDownAVFileSpeedInfo=pKKDownAVFileSpeedInfo;
				Info.KKUrlParser=pKKUrlParser;
				KKPlayer::AddKKPluginInfo(Info);
			
		}else{
			FreeLibrary(hdll);
		}

		int i=0;
		i++;
	}

	DllPathInfoList.clear();
	int ll=sizeof(ULONG_PTR);
	HRESULT hRes = ::CoInitialize(NULL);
    G_Parent =(HWND)_wtoi(lpstrCmdLine);
	wchar_t abcd[100]=L"";
	wsprintf(abcd,L"%d",G_Parent);
	ATLASSERT(SUCCEEDED(hRes));
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	//HWND h=CreateMessageWindow(hInstance);
	
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();
	

	using namespace SOUI;
	SComMgr * pComMgr = new SComMgr;

	SOUI::CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
	
	pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
    
	
	pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
	pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
	
	::SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED |ES_CONTINUOUS);
	theApp=new SApplication(pRenderFactory,hInstance);
    theApp->RegisterWndFactory(TplSWindowFactory<CSuiVideo>());
    theApp->RegisterWndFactory(TplSWindowFactory<CKKmclv>());
    theApp->RegisterWndFactory(TplSWindowFactory<SAVSeekBar>());
	
	 KKWkeLoader wkeLoader;
	 if(wkeLoader.Init(_T("wke.dll")))
	 {
	   theApp->RegisterWndFactory(TplSWindowFactory<KKWkeWebkit>());
	 }

	

	HMODULE hui=LoadLibrary(_T("kkui.dll"));
	if(hui){
		CAutoRefPtr<IResProvider>   pResProvider;
		CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
		BOOL ret=pResProvider->Init((WPARAM)hui,0);
		theApp->AddResProvider(pResProvider);
	}else{
       assert(0);
	}
	
	CAutoRefPtr<ITranslatorMgr> trans;
	pComMgr->CreateTranslator((IObjRef**)&trans);
	if(trans)
	{
	        theApp->SetTranslator(trans);
            pugi::xml_document xmlLang;
            if(theApp->LoadXmlDocment(xmlLang,_T("lang_cn"),_T("translator"))){
                CAutoRefPtr<ITranslator> langCN;
                trans->CreateTranslator(&langCN);
                langCN->Load(&xmlLang.child(L"language"),1);//1=LD_XML
                trans->InstallTranslator(langCN);
            }
	}


	//加载系统资源
	HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
	if(hSysResource){
		CAutoRefPtr<IResProvider> sysSesProvider;
		CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
		sysSesProvider->Init((WPARAM)hSysResource,0);
		theApp->LoadSystemNamedResource(sysSesProvider);
	}

	//CAVDownManage Dow;
	//Dow.Start();
	std::wstring path=windir.GetModulePath();
	
	SOUI::SStringT str;
	SOUI::CMainDlg dlgMain;
	m_pDlgMain=&dlgMain;
	dlgMain.Create(NULL,WS_POPUP|WS_MINIMIZEBOX | WS_MAXIMIZEBOX&~WS_CAPTION,0,0,0,0,0);
	dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow(dlgMain.m_hWnd);
	dlgMain.ShowWindow(SW_SHOWNORMAL);

	if(urlpath.length()>3)
	{
	    char localurl[1024]="";
	    CChineseCode::wcharTochar(urlpath.c_str(), localurl,1024);
		dlgMain.WinTabShow(1);
	    dlgMain.OpenMedia(localurl);
	}
	//int nRet =  Run(lpstrCmdLine, nCmdShow);
	int 	nRet = theApp->Run(dlgMain.m_hWnd);

 
	_Module.Term();
	::CoUninitialize();

	::SetThreadExecutionState(ES_CONTINUOUS);
	return nRet;
}
