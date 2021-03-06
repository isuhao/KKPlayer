// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include <dwmapi.h>
#include <queue>
#include <map>
#include <ShellAPI.h>
#include <helper/SAdapterBase.h>
#include "SuiVideo.h"


#include "KKWkeWebkit.h"

#include "AVInfo.h"
#include "AVOpenUrl.h"
#include "json/json.h"

#include <control/SCmnCtrl.h>
#include <control/SSliderBar.h>
#include <control/SListView.h>

#include "../Control/kkseek.h"
#include "../Tool/cchinesecode.h"
#include "../Model/HistoryAdapter.h"
#include "AvCodecSetting.h"
#include "AvStreamSetting.h"
#include "AvRendererSetting.h"

int OpenLocalFile(HWND hWnd,std::wstring &path); 
SOUI::CMainDlg* G_pCMainDlg=NULL;
namespace SOUI
{
    BOOL ModifyStyleEx2(HWND m_hWnd,DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) throw()
	{
		ATLASSERT(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

   

	CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_AVMiniBottom(this), m_AVMiniTittle(this),m_bMini(0)
	{
		m_bLayoutInited=FALSE;
		m_PlayerState=0;
		m_lastSeekTime=0;
		m_MenuId=0;
		m_bFullScreen=false;
		m_bScreenNormal=true;
		m_showAV_List=0;
        m_bMini=0;
		m_nDownSpeed=0;
		 G_pCMainDlg=this;
		//KKWkeWebkit *pKKWkeWebkit= new KKWkeWebkit();
	
	} 

	CMainDlg::~CMainDlg()
	{
		
	}

	void CMainDlg::ForceNoRound()
	{
		
		CRect rcWindow, rcClient;
		CRgn rgnWindow, rgnMinus, rgnAdd;

		::GetWindowRect(m_hWnd,&rcWindow);
		::GetClientRect(m_hWnd,&rcClient);
		//::ClientToScreen(m_hWnd,rcClient);

		rcClient.OffsetRect(- rcWindow.TopLeft());
		
		::SetWindowRgn(m_hWnd,rgnWindow,1);
	}
	int CMainDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
	{
		SetMsgHandled(FALSE);
		m_AVMiniTittle.Create(this->m_hWnd,WS_CHILD|WS_POPUP|WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW,0,0,150,22);
		ModifyStyleEx2(m_AVMiniTittle.m_hWnd,WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
		
		m_AVMiniBottom.Create(this->m_hWnd,WS_CHILD|WS_POPUP|WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW,0,0,150,22);
		ModifyStyleEx2(m_AVMiniBottom.m_hWnd,WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);

		/*m_AVMaxBottom.Create(this->m_hWnd,WS_CHILD|WS_POPUP|WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW,0,0,150,22);
		ModifyStyleEx2(m_AVMaxBottom.m_hWnd,WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);*/
		return 0;
	}
	BOOL CMainDlg::OnInitDialog( HWND hWnd, LPARAM lParam )
	{
		m_bLayoutInited=TRUE;
        SetMsgHandled(FALSE);
		
		showAV_List();

		SWindow* split_row=FindChildByName(L"split_row");
		if(split_row)
		   split_row->SetAttribute(L"pos",L"1,[0,-1,]-0");
		return 0;
	}
	
	
  
	char * GetFormtDBySpeed(char *strabce,int FileSize);
	
	
	void CMainDlg::OnClose()
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(av)
		   av->Close();
		PostMessage(WM_QUIT,0,0);
	}
	void CMainDlg::OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
	}
	void CMainDlg::OnRestore()
	{

		SendMessage(WM_SYSCOMMAND,SC_RESTORE);
		m_bFullScreen=false;
	}
	void CMainDlg::OnMinimize()
	{
		ShowMiniUI(false);
		SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
		m_bFullScreen=false;
	}
    void CMainDlg::OnRButtonUp(UINT nFlags, CPoint point)
	{
		  SetMsgHandled(FALSE);
           CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		   CRect rt=av->GetClientRect();

		   SListView *pLstViewFix = FindChildByName2<SListView>("AV_list");
		   CRect rt2=pLstViewFix->GetClientRect();
		   if(rt.PtInRect(point)||rt2.PtInRect(point))
		   {
			   int i=0;
			   i++;
		   }
	}
	void CMainDlg::OnAvSetting()
	{
		
		SWindow* pWin=FindChildByName(L"btn_More");
		if(pWin)
		{
			CRect rt=	pWin->GetClientRect();
			int xPos=rt.left;
			int yPos=rt.top+rt.bottom-rt.top;

			rt=GetWindowRect();
			xPos+=rt.left;
			
			yPos+=rt.top;
			SOUI::SMenuEx me;
			BOOL xx=me.LoadMenu(_T("SMENUEX:avsettingmenuex"));
			me.TrackPopupMenu(0,xPos,yPos,m_hWnd);
		}
	}


	LRESULT CMainDlg::OnUIMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	    SetMsgHandled(TRUE);

	    //全屏显示
		if(GetFullScreen()){

			RECT BottomRt;
			::GetWindowRect(m_hWnd,&BottomRt);
			
			RECT TittleRt={0,0,BottomRt.right,40};
			
			POINT point;
			GetCursorPos(&point);

			BottomRt.top=BottomRt.bottom-40;
			if(::PtInRect(&BottomRt, point)||::PtInRect(&TittleRt, point))
			{
			    ShowMiniUI(true);
			}else{
			    ShowMiniUI(false);
			}
			
		}else if(!GetScreenModel())//迷你模式
		{
			int ll=0x8000&GetAsyncKeyState(VK_LBUTTON);
			if(!ll)
			{
				ShowMiniUI(true);
			}else
			{
				ShowMiniUI(false);
			}
		}

		return 1;
	}
    LRESULT CMainDlg::OnUIRbuttonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	    SetMsgHandled(TRUE);

		int xPos = GET_X_LPARAM(lParam); 
	    int yPos = GET_Y_LPARAM(lParam);
		RECT rt;
		::GetWindowRect(m_hWnd,&rt);
		xPos+=rt.left;
		yPos+=rt.top;

		SOUI::SMenuEx me;
		BOOL xx=me.LoadMenu(_T("SMENUEX:avmenuex"));


		if(!GetFullScreen())
		{
			 SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(3);
			 if(GetFullScreen())
			 pItem->SetAttribute(_T("check"),_T("1"));
			 pItem->SetAttribute(_T("enable"),_T("1"));
			 pItem->SetAttribute(_T("colorText"),_T("#000000"));
		}else {
			SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(3);
			pItem->SetAttribute(_T("check"),_T("1"));
			pItem->SetAttribute(_T("enable"),_T("1"));
			pItem->SetAttribute(_T("colorText"),_T("#000000"));
		}

		if(GetScreenModel())
		{
			{
				SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(4);
				pItem->SetAttribute(_T("check"),_T("1"));
			}
			{
				SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(5);
				pItem->SetAttribute(_T("check"),_T("0"));
			}  
		}else{
			{
				SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(4);
				pItem->SetAttribute(_T("check"),_T("0"));
			}
			{
				SOUI:: SWindow *pItem=(SOUI:: SWindow *) me.GetMenuItem(5);
				pItem->SetAttribute(_T("check"),_T("1"));
			}  
		}
		me.TrackPopupMenu(0,xPos,yPos,m_hWnd);

		return 1;
	}

	LRESULT CMainDlg::OnUILbuttonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SetMsgHandled(TRUE);
		if(!GetScreenModel())
		{
		   ShowMiniUI(false);
		}/**/
		return 1;
	}
	LRESULT CMainDlg::OnSetAvTilte(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	     SetMsgHandled(TRUE);

		 wchar_t* title=(wchar_t*)wParam;

		 FindChildByName("AVNameTitle")->SetWindowText(title);/**/
		 m_AVMiniTittle.FindChildByName("AVNameTitle")->SetWindowText(title);/**/
		 return 1;
	}
	LRESULT CMainDlg::OnAvlistOpen(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	     SetMsgHandled(TRUE);

		 char *url=(char *)wParam;
		 if(url)
		 this->OpenMedia(url);
		 return 1;
	}
	void CMainDlg::OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);
      
			if(!m_bLayoutInited)
			return;
		if(nType==SIZE_MAXIMIZED)
		{
			FindChildByName(L"btn_restore")->SetVisible(TRUE);
			FindChildByName(L"btn_max")->SetVisible(FALSE);
		}else if(nType==SIZE_RESTORED)
		{
			FindChildByName(L"btn_restore")->SetVisible(FALSE);
			FindChildByName(L"btn_max")->SetVisible(TRUE);
		}
		ShowMiniUI(false);
        ForceNoRound();
	}
	void CMainDlg::OnBtnMsgBox()
	{
		SMessageBox(NULL,_T("this is a message box"),_T("haha"),MB_OK|MB_ICONEXCLAMATION);
		SMessageBox(NULL,_T("this message box includes two buttons"),_T("haha"),MB_YESNO|MB_ICONQUESTION);
		SMessageBox(NULL,_T("this message box includes three buttons"),NULL,MB_ABORTRETRYIGNORE);
	}
	void CMainDlg::OnShowWindow( BOOL bShow, UINT nStatus )
	{
		if(bShow)
		{
 			//AnimateHostWindow(200,AW_CENTER);
		}
	}

	
	void CMainDlg::OnFolder()
	{
		
		std::wstring Path;
		if(OpenLocalFile(m_hWnd,Path))
		{  
			m_CumrTime=0;
			SStatic* AV_CurTimeTxt=( SStatic  *)FindChildByName(L"AV_CurTimeTxt");
			AV_CurTimeTxt->SetWindowText(L"00:00:00/00:00:00");

			SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
			SeekBar->SetRange(0,10000);
			SeekBar->SetValue(0);

			char path[1024]="";
			CChineseCode::wcharTochar(Path.c_str(),path,1024);
			OpenMedia(path);
		} /* */
	}
	void CMainDlg::OnFileList()
	{
		if(m_showAV_List==0)
		{
           m_showAV_List=1;
		}else{
           m_showAV_List=0;
		}
		showAV_List();
	}


    void CMainDlg::OnAVPlay()
	{
		SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(m_PlayerState==1){
			m_AVMiniBottom.SetPlayState(true);
            pPlay->SetAttribute(L"skin",L"_skin.PLAY",TRUE);
			av->SetPlayStat(1);
			pPlay->UpdateWindow();
			m_PlayerState=2;

		}else if(m_PlayerState==2){
			    m_AVMiniBottom.SetPlayState(false);
                pPlay->SetAttribute(L"skin",L"_skin.Pause",TRUE);
				av->SetPlayStat(1);
				m_PlayerState=1;
				pPlay->UpdateWindow();
		}
     
	}
	void CMainDlg::OnMuteAudio()
	{
	   SWindow* pWin= FindChildByName(L"btn_audio");
	    SWindow* pWin2= m_AVMiniBottom.FindChildByName(L"btn_audio");
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(pWin)
		{
			int lx=pWin->GetUserData();
			if(lx==0){
			   m_nMuteVol=av->GetVolume();
			   if( m_nMuteVol>100)
				    m_nMuteVol=100;
			   pWin->SetAttribute(L"skin",L"_skin.AUDIO3",TRUE);
			   pWin->SetUserData(1);
			   this->SetVolume(0,true);
			   m_AVMiniBottom.SetVolume(0);
			   pWin2->SetAttribute(L"skin",L"_skin.AUDIO3",TRUE);
			}else{
			   pWin->SetAttribute(L"skin",L"_skin.AUDIO2",TRUE);
			   pWin2->SetAttribute(L"skin",L"_skin.AUDIO2",TRUE);
			    this->SetVolume( m_nMuteVol,true);
					m_AVMiniBottom.SetVolume(m_nMuteVol);
			   pWin->SetUserData(0);
			}
		}
	}
    void CMainDlg::OnTimer(char cTimerID)
	{
		//return;
        if(m_PlayerState!=0&&cTimerID==1){
            UpdateAVSeek();
		}else if(cTimerID==2){
			if(!GetScreenModel()|| GetFullScreen()){
				POINT pt;
				::GetCursorPos(&pt); 

				RECT rt2;
				::GetWindowRect(m_AVMiniTittle.m_hWnd,&rt2);

				RECT rt3;
				::GetWindowRect(m_AVMiniBottom.m_hWnd,&rt3);
				if(!::PtInRect(&rt2,pt)&&!::PtInRect(&rt3,pt)){
                    ShowMiniUI(false);
				}	
			}
		}
	}
	bool CMainDlg::OnSliderVideo(EventArgs *pEvt)
	{
		EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
		SASSERT(pEvt2);
		SAVSeekBar  * pSlider = sobj_cast<SAVSeekBar>(pEvt->sender);
		SASSERT(pSlider);


		//SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"slider_video");
		if(m_PlayerState!=0)
		{
			m_SeekTimer=0;
			m_lastSeekTime=m_CumrTime;
			int vol=pEvt2->nPos-m_CumrTime;
			m_CumrTime+=vol;
			CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
			av->AvSeek(pEvt2->nPos);
		}/**/
		return true;
	}
	void CMainDlg::SetVolume(int vol,bool ctl)
	{
	
		if(ctl){
		     SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"AvAudio");
			 VolBar->SetValue(vol);
		}
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		av->SetVolume(vol);
	}
	bool CMainDlg::OnSliderAudio(EventArgs *pEvt)
	{
		EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
		SASSERT(pEvt2);
		SSliderBar * pSlider = sobj_cast<SSliderBar>(pEvt->sender);
		SASSERT(pSlider);

       
		SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"AvAudio");
		
		long vol=pEvt2->nPos;
		
		SetVolume(vol);
		m_AVMiniBottom.SetVolume(vol);
		return true;
	}
	
	int  CMainDlg::OpenMedia(char *path,const char* avname,const char* cmd)
	{
		 this->KillTimer(1);

		
		
		m_nDownSpeed=0;
		SStatic* AV_CurTimeTxt=( SStatic  *)FindChildByName(L"AV_CurTimeTxt");
		AV_CurTimeTxt->SetWindowText(L"00:00:00/00:00:00");

		SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");

		m_urlStr=path;
		//SuiVideo
		int ret=av->OpenMedia(path,avname,cmd);
		m_nDownSpeed=ret;
          this->FindChildByName(L"AV_SpeedTxt")->SetVisible(FALSE,1);
		
		if(ret==-1)
		{
			//MessageBox(m_hWnd, L"文件打开错误", 0, MB_OK); 
			FindChildByName("AVNameTitle")->SetWindowText(L"");
			m_PlayerState=0;
			SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
			pPlay->SetAttribute(L"skin",L"_skin.PLAY",TRUE);
            m_AVMiniBottom.SetPlayState(true);
			pPlay->Invalidate();
			FindChildByName(L"WinCenterOpenAV")->SetVisible(1,1);
		}else
		{

			// SeekBar->SetRange(0,1);
			m_PlayerState=1;

			SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
			pPlay->SetAttribute(L"skin",L"_skin.Pause",TRUE);
			m_AVMiniBottom.SetPlayState(false);
			pPlay->Invalidate();
			this->SetTimer(1,500);
			
			SetSeekControlValue(0,0);
			m_CumrTime=0;
			FindChildByName(L"WinCenterOpenAV")->SetVisible(FALSE,1);
		}
		  
		return ret;
	}


	int  CMainDlg::CloseMedia()
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
	    av->SetAVVisible(false);
		FindChildByName(L"WinCenterOpenAV")->SetVisible(1,1);
		av->Close();
		m_urlStr="";
		m_PlayerState=0;
		return 1;
   }
	int  CMainDlg::DownMedia(char *KKVURL)
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		int ret=av->DownMedia(KKVURL);
		return 0;
	}
  
	void CMainDlg::OnDecelerate()
	{
         CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		 av->OnDecelerate();
	}
	void CMainDlg::OnAccelerate()
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		av->OnAccelerate();
	}
	//DUI菜单响应函数
	void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
	{
		if(uNotifyCode==0)
		{
			if(nID == 1){
			   OnFolder();
			}else if(nID==2){
				    CAVOpenUrl OpenUrlDlg(this);
				    OpenUrlDlg.DoModel(0,0,m_hWnd);
			}else if(nID==3){
				FullScreen();
				 m_bMini=0;
			}else if(nID==4){
			    m_bMini=0;
				ScreenNormal();
			}else if(nID==5){
				ScreenMini();
				//ForceNoRound();
			}else if(nID==6){
				CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
				MEDIA_INFO info;
				memset(&info,0,sizeof(info));
				//;
				av->GetMeadiaInfo(info);
				info.AvFile=m_urlStr.c_str();
				CAVInfo infodlg(info);
                infodlg.DoModel(0,0,m_hWnd);
			}else if(nID==7){
				
			}else if(nID==8){
				ShowAVDownManageDlg();
			}else if(nID==100){ 
				     ///codec setting
			       CAvCodecSetting avv;
				   avv.DoModel(0,0,m_hWnd);
			}else if(nID==101){ 
				   ///stream setting
			       CAvStreamSetting avs;
				   avs.DoModel(0,0,m_hWnd);
			}else if(nID==102){
				   ///画面渲染设置
			       CAvRendererSetting avr;
				   avr.DoModel(0,0,m_hWnd);
			}else if(nID==110){
			      ///播放
				this->OpenMedia((char*)g_stravhisurl.c_str());
			}else if(nID==111){
				 ///删除
				 CHistoryInfoMgr *InfoMgr=CHistoryInfoMgr::GetInance();
				 InfoMgr->DelAVinfo(g_stravhisurl.c_str());
				 SListView *pLstViewFix = FindChildByName2<SListView>("AV_list");
			    if(pLstViewFix){
					ILvAdapter *pAdapter =pLstViewFix->GetAdapter();
					if(pAdapter){
							((CHistoryAdapterFix*)pAdapter)->UpdateData();
					}
				}
			     
			}else if(nID==113){
				///升序
				SListView *pLstViewFix = FindChildByName2<SListView>("AV_list");
			    if(pLstViewFix){
					ILvAdapter *pAdapter =pLstViewFix->GetAdapter();
					if(pAdapter){
						    ((CHistoryAdapterFix*)pAdapter)->SortOrder(0);
							((CHistoryAdapterFix*)pAdapter)->UpdateData();
					}
				}
			     
			}else if(nID==114){
			       ///降序
					SListView *pLstViewFix = FindChildByName2<SListView>("AV_list");
					if(pLstViewFix){
						ILvAdapter *pAdapter =pLstViewFix->GetAdapter();
						if(pAdapter){
							    ((CHistoryAdapterFix*)pAdapter)->SortOrder(1);
								((CHistoryAdapterFix*)pAdapter)->UpdateData();
						}
					}
			}else if(nID==115){
			      ///打开文件位置
				std::string temp="/select, ";
			    temp+=g_stravhisurl;
			    ShellExecuteA(NULL,NULL,"explorer",temp.c_str(),NULL,SW_SHOW); 
			}
		}
	}

	
	void CMainDlg::ShowMiniUI(bool show)
	{
	    if(show)
		{
			
			if(!::IsWindowVisible (m_AVMiniTittle.m_hWnd))
			{
				RECT rt;
				::GetWindowRect(m_hWnd,&rt);

				RECT rt2;
				::GetWindowRect(m_AVMiniTittle.m_hWnd,&rt2);
				if(rt2.left!=rt.left)
				{
                       ::SetWindowPos(m_AVMiniTittle.m_hWnd,0,rt.left+1,rt.top+1,rt.right-rt.left-1,29,SWP_NOZORDER);

					   int hei=48;
					   ::SetWindowPos(m_AVMiniBottom.m_hWnd,0,rt.left+1,rt.bottom-hei,rt.right-rt.left-2,hei,SWP_NOZORDER);
				}
				


				::ShowWindow(m_AVMiniTittle.m_hWnd,SW_SHOW);
				::ShowWindow(m_AVMiniBottom.m_hWnd,SW_SHOW);
				SWindow* pWin= m_AVMiniBottom.FindChildByName(L"AVPlayBtn");
				if(pWin!=NULL){
				    if(m_bFullScreen)
				        pWin->SetAttribute(L"pos",L"|0, 5,@30,@30");
                    else
					    pWin->SetAttribute(L"pos",L"5, 5,@30,@30");
					///重新计算位置
					pWin->GetParent()->UpdateChildrenPosition();	
				}
				BOOL XXX=this->SetTimer(2,3000);
				XXX++;
			}
		}else{
			 
			if(::IsWindowVisible(m_AVMiniTittle.m_hWnd))
			{
				 ::SetWindowPos(m_AVMiniTittle.m_hWnd,0,1,1,0,0,SWP_NOZORDER);
				 ::SetWindowPos(m_AVMiniBottom.m_hWnd,0,1,1,0,0,SWP_NOZORDER);
				 ::ShowWindow(m_AVMiniTittle.m_hWnd,SW_HIDE);
				 ::ShowWindow(m_AVMiniBottom.m_hWnd,SW_HIDE);
				 this->KillTimer(2);
			}
		}
	}

	//全屏
	bool CMainDlg::GetFullScreen()
	{
		 return m_bFullScreen;
	}
	
	///返回为true 普通模式，返回为FALSE ，mini模式
	bool  CMainDlg::GetScreenModel()
	{
		return m_bScreenNormal;
	}
	void CMainDlg:: OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(nChar==27){
		         if(m_bFullScreen)
		            FullScreen();
		}else if(nChar==VK_UP){
			long vol=av->GetVolume()+10;
			av->SetVolume(vol);
			if(vol<100)
            SetVolume(vol,true);
		}else if(nChar==VK_DOWN){
		    long vol=av->GetVolume()-10;
			if(vol<0)
				vol=0;
			if(vol<100)
			av->SetVolume(vol);
			SetVolume(vol,true);
		}else{
			nRepCnt =nRepCnt|nRepCnt<<16;
		    HWND h=av->GetPlayerHwnd();
			::PostMessage(h,WM_KEYDOWN,nChar,nRepCnt);
		}
		SetMsgHandled(FALSE);
	 }
	void CMainDlg::SetSeekControlValue(int Value,int CacheValue)
	{
              SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
			  SeekBar->SetValue(Value);
			  SeekBar->SetCacheValue(CacheValue);


			  SeekBar=(SAVSeekBar *)m_AVMiniBottom.FindChildByName(L"slider_video_Seek");
			  SeekBar->SetValue(Value);
			  SeekBar->SetCacheValue(CacheValue);
	}
    void CMainDlg::SetSeekCacheControlValue(int CacheValue)
	{
		SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
		SeekBar->SetCacheValue(CacheValue);


		SeekBar=(SAVSeekBar *)m_AVMiniBottom.FindChildByName(L"slider_video_Seek");
		SeekBar->SetCacheValue(CacheValue);
	}
	void CMainDlg::SetSeekControlRange(int Value)
	{
		SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
		SeekBar->SetRange(0,Value);

		SeekBar=(SAVSeekBar *)m_AVMiniBottom.FindChildByName(L"slider_video_Seek");
		SeekBar->SetRange(0,Value);
	}
	void CMainDlg::OnShowWinPlay()
	{
           WinTabShow(1);
	}
	void CMainDlg::OnShowWinRes()
	{
          WinTabShow(2);
	}
    void CMainDlg::OnWebResSearch()
	{
        
	}
	
	void CMainDlg::WinTabShow(int Tab)
	{
            if(Tab==1)
			{
				FindChildByName(L"WinAVPlayer")->SetVisible(TRUE,TRUE);
				
				FindChildByName(L"WinWebRes")->SetVisible(FALSE,TRUE);
				
				FindChildByName(L"TopPlay_Opt_Line")->SetVisible(TRUE,TRUE);
                FindChildByName(L"TopRes_Opt_Line")->SetVisible(FALSE,TRUE);

				FindChildByName(L"btn_TopPlay_Opt")->SetAttribute(L"colorText",L"#46C832");
                FindChildByName(L"btn_TopRes_Opt")->SetAttribute(L"colorText",L"#999999");

				CSuiVideo* pSV=FindChildByName2<CSuiVideo>(L"AVMainWnd");
				if(pSV&&m_PlayerState==1){
					 pSV->SetAVVisible(TRUE);
			     }
			}else if(Tab==2){
				 CSuiVideo* pSV=FindChildByName2<CSuiVideo>(L"AVMainWnd");
				 if(pSV)
					 pSV->SetAVVisible(FALSE);
				FindChildByName(L"TopPlay_Opt_Line")->SetVisible(FALSE,TRUE);
                FindChildByName(L"TopRes_Opt_Line")->SetVisible(TRUE,TRUE);

				FindChildByName(L"WinAVPlayer")->SetVisible(FALSE,TRUE);
				FindChildByName(L"WinWebRes")->SetVisible(TRUE,TRUE);

				FindChildByName(L"btn_TopPlay_Opt")->SetAttribute(L"colorText",L"#999999");
				FindChildByName(L"btn_TopRes_Opt")->SetAttribute(L"colorText",L"#46C832");
				SWindow *pWebkitRes=FindChildByName(L"webkitRes");
				if(pWebkitRes)
				 pWebkitRes->SetFocus();
              
			}
	}
    void CMainDlg::ShowAVDownManageDlg()
	{
		CAVTransferDlg dlg(this);
	    dlg.DoModel(0,0,m_hWnd);
	}
	BOOL CMainDlg::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
	{
		SetMsgHandled(FALSE);
	    return 0;
	}
	void CMainDlg::UpdateDownSpeed(const char* jsonstr)
	{
		
	   wchar_t strtmp[100]=L"";
		Json::Reader jsonreader;
		Json::Value jsonValue;
		if(jsonreader.parse(jsonstr,jsonValue)){
			SWindow *ctl=this->FindChildByName(L"AV_SpeedTxt");
			if(!ctl->IsVisible())
		        ctl->SetVisible(TRUE,1);
			if(jsonValue["speed"].isInt())
			{
				int speed=jsonValue["speed"].asInt();
				if(speed<1024)
			       swprintf_s(strtmp, L"%d b/s",speed);
				else
                    swprintf_s(strtmp,L"%d kb/s",speed/1000);
			   ctl->SetWindowText(strtmp);
			}
		}
	}
    void CMainDlg::UpdateAVSeek()
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		MEDIA_INFO info;
		memset(&info,0,sizeof(info));
		if(!av->GetMeadiaInfo(info)){
			UpdateDownSpeed(info.SpeedInfo);
			return ;
		}
        
		SAVSeekBar *SeekBar=(SAVSeekBar *)FindChildByName(L"slider_video_Seek");
         wchar_t strtmp[100]=L"";
		std::wstring CurTimeStr;

		int h=(info.CurTime/(60*60));
		int m=(info.CurTime%(60*60))/(60);
		int s=((info.CurTime%(60*60))%(60));
		
		if(h<10){
			swprintf_s(strtmp,L"0%d:",h);
			CurTimeStr=strtmp;
		}
		else{
			swprintf_s(strtmp,L"%d:",h);
			CurTimeStr=strtmp;
		}
		if(m<10){
			swprintf_s(strtmp,L"0%d:",m);
			CurTimeStr+=strtmp;
		}
		else{				  
			swprintf_s(strtmp,L"%d:",m);
			CurTimeStr+=strtmp;
		}

		if(s<10){
			swprintf_s(strtmp,L"0%d",s);
			CurTimeStr+=strtmp;
		}
		else{
			swprintf_s(strtmp,L"%d",s);
			CurTimeStr+=strtmp;
		}
		CurTimeStr+=L" / ";




		int Min=0;
		int Max=0;
		SeekBar->GetRange(&Min,&Max);
		static std::wstring hh=L"00:00:00";
		if(info.TotalTime>0){
			SetSeekControlRange(info.TotalTime);
			h=info.TotalTime/(60*60);
			m=(info.TotalTime%(60*60))/60;
			s=((info.TotalTime%(60*60))%60);


			if(h<10)
			{
				swprintf_s(strtmp,L"0%d:",h);
				hh=strtmp;
			}
			else{
				swprintf_s(strtmp,L"%d:",m);
				hh=strtmp;
			}
			if(m<10){
				swprintf_s(strtmp,L"0%d:",m);
				hh+=strtmp;
			}
			else{
				swprintf_s(strtmp,L"%d:",m);
				hh+=strtmp;
			}
			if(s<10){
				swprintf_s(strtmp,L"0%d",s);
				hh+=strtmp;
			}
			else{
				swprintf_s(strtmp,L"%d",s);
				hh+=strtmp;
			}
		}

		/*if(info.CurTime>m_CumrTime&&info.serial==av->PktSerial()){
			if(m_CumrTime-m_lastSeekTime<=0&&info.CurTime-m_CumrTime<10)
			{
				SetSeekControlValue(info.CurTime,info.CacheInfo.MaxTime);
				m_CumrTime=info.CurTime;
			}else if(m_CumrTime-m_lastSeekTime>0)
			{
				SetSeekControlValue(info.CurTime,info.CacheInfo.MaxTime);
				m_CumrTime=info.CurTime;
			}

		}*/
		if(info.serial==av->PktSerial()){
				SetSeekControlValue(info.CurTime,info.CacheInfo.MaxTime);
				m_CumrTime=info.CurTime;
		}else{
              SetSeekCacheControlValue(info.CacheInfo.MaxTime);
		}
		m_serial=info.serial;
		m_SeekTimer++;
		CurTimeStr+=hh;
		
	    this->FindChildByName(L"AV_CurTimeTxt")->SetWindowText(CurTimeStr.c_str());
		m_AVMiniBottom.FindChildByName(L"AV_CurTimeTxt")->SetWindowText(CurTimeStr.c_str());
		UpdateDownSpeed(info.SpeedInfo);
        static int tx=0;
		
		if(tx>10){
		   av->SaveSnapshoot();
		   //tx=0;
		}else{
			tx++;
		}
	}

	void CMainDlg::FullScreen()
	{
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(!m_bFullScreen)
		{
              OnMaximize();
			  FindChildByName(L"AV_Tittle")->SetVisible(FALSE);
			  FindChildByName(L"SeekWin")->SetVisible(FALSE);
			  FindChildByName(L"ButtonToolbar")->SetVisible(FALSE);
			  FindChildByName(L"AV_list_pane")->SetVisible(FALSE);
			 
			  FindChildByName(L"split_row")->SetAttribute(L"pos",L"0,0,-0,-0");
			  m_bFullScreen=true;

			  int  cx   =   GetSystemMetrics(   SM_CXSCREEN   );   
			  int  cy   =   GetSystemMetrics(   SM_CYSCREEN   );

			  //LONG style = GetWindowLong(m_hWnd,GWL_STYLE);//获得窗口风格   
			  //style =style &~WS_CAPTION & ~WS_SIZEBOX;//窗口全屏显示且不可改变大小   
			  //SetWindowLong(m_hWnd,GWL_STYLE,style); //设置窗口风格   
			  ::SetWindowPos(m_hWnd,0,-1,-1,cx+2,cy+2,SWP_SHOWWINDOW);
			  av->FullScreen();
			  ::SetFocus(m_hWnd);
		}else
		{
			OnRestore();
			if(m_bScreenNormal)
			    AV_Title();
			av->FullScreen();
			m_bFullScreen=false;
		}
	}
	
	void CMainDlg::ScreenNormal()
	{
		   OnRestore();
		   AV_Title();
		   ShowMiniUI(false);
		   m_bScreenNormal=true;
	}
	void CMainDlg::ScreenMini()
	{
		OnRestore();
		FindChildByName(L"AV_Tittle")->SetVisible(FALSE);
		FindChildByName(L"SeekWin")->SetVisible(FALSE);
		FindChildByName(L"ButtonToolbar")->SetVisible(FALSE);
		FindChildByName(L"AV_list_pane")->SetVisible(FALSE);
       // FindChildByName(L"AV_CurTimeTxt")->SetVisible(FALSE);


		FindChildByName(L"SeekWin")->SetAttribute(_T("pos"),_T("0,0,@0,@0"));
        FindChildByName(L"ButtonToolbar")->SetAttribute(_T("pos"),_T("0,0,@0,@0"));
		FindChildByName(L"AV_Tittle")->SetAttribute(_T("pos"),_T("0,0,@0,@0"));
       // FindChildByName(L"AV_CurTimeTxt")->SetAttribute(_T("pos"),_T("0,0,@0,@0"));
			FindChildByName(L"WinAVPlayer")->SetAttribute(L"pos",L"0,0,-0,-0");
		FindChildByName(L"split_row")->SetAttribute(L"pos",L"1,1,-1,-1");
		m_bScreenNormal=false;
		m_bMini=1;
	}
	
	void CMainDlg::showAV_List()
	{
		//return;
		if(m_showAV_List==1)
		{
			//行高固定的列表
			SListView *pLstViewFix = FindChildByName2<SListView>("AV_list");
		//	pLstViewFix->get
			if(pLstViewFix)
			{
				//CSuiVideo* pAV=(CSuiVideo* )FindChildByName(L"AVMainWnd");
				ILvAdapter *pAdapter =pLstViewFix->GetAdapter();
				if(!pAdapter)
				{
					pAdapter=new CHistoryAdapterFix();
			        pLstViewFix->SetAdapter(pAdapter);
				    pAdapter->Release();
				}else{
					((CHistoryAdapterFix*)pAdapter)->UpdateData();
				}
			
			}
			SWindow* AV_list_pane=FindChildByName(L"AV_list_pane");
            if(AV_list_pane)
			{
				AV_list_pane->SetAttribute(L"pos",L"[0,0,200],-0");
			    AV_list_pane->SetVisible(TRUE,TRUE);
			}

			AV_list_pane=FindChildByName(L"AV_Video_pane");
		    if(AV_list_pane)
			AV_list_pane->SetAttribute(L"pos",L"0,[0,]0,-0");
		}else{
			SWindow* AV_list_pane=FindChildByName(L"AV_list_pane");
			if(AV_list_pane)
			{
			   FindChildByName(L"AV_list_pane")->SetAttribute(L"pos",L"0,0,0,0");
			   FindChildByName(L"AV_list_pane")->SetVisible(FALSE,TRUE);
			}
			AV_list_pane=FindChildByName(L"AV_Video_pane");
		    if(AV_list_pane)
			AV_list_pane->SetAttribute(L"pos",L"0,[0,-0,]-0");
			
		}
	}	
	void CMainDlg::AV_Title()
	{
		FindChildByName(L"AV_Tittle")->SetVisible(true);
		FindChildByName(L"SeekWin")->SetVisible(true);
		FindChildByName(L"ButtonToolbar")->SetVisible(true);
		showAV_List();

		FindChildByName(L"SeekWin")->SetAttribute(_T("pos"),_T("0,]-10,-0,]-0"));
		FindChildByName(L"ButtonToolbar")->SetAttribute(_T("pos"),_T("0,-50,-0,-0"));
		FindChildByName(L"AV_Tittle")->SetAttribute(_T("pos"),_T("0,0,-0,50"));
		
		FindChildByName(L"WinAVPlayer")->SetAttribute(L"pos",L"0,50,-0,-0");
		FindChildByName(L"split_row")->SetAttribute(L"pos",L"1,[0,-1,]-0");
	}	
}