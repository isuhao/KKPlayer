#include "../ui.h"
#include "HistoryAdapter.h"
#include "../Tool/cchinesecode.h"
#include "../SqlOp/HistoryInfoMgr.h"
#include <algorithm>

extern SOUI::CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
namespace SOUI
{
	
	CHistoryAdapterFix::CHistoryAdapterFix()
	{
		    m_nSortOrder=0;
	        UpdateData();
	}
	CHistoryAdapterFix::~CHistoryAdapterFix()
	{
        ClearData();
		
	}
	int          CHistoryAdapterFix::getCount()
	{
		return m_slQue.size();
	}
	
	void         CHistoryAdapterFix::getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate)
	{
	    AV_Hos_Info * pAVPic=m_slQue.at(position);
		if(pItem->GetChildrenCount()==0)
		{
			pItem->InitFromXml(xmlTemplate);
		}

		SOUI::CAutoRefPtr<struct SOUI::IRenderFactory> pRenderFactory= GETRENDERFACTORY;  //  SOUI::SApplication::getSingleton().GetRenderFactory()
		SItemPanel * pSItme=(SItemPanel *)pItem;
		pSItme->SetUserData((ULONG_PTR)pAVPic);
		pSItme->GetEventSet()->subscribeEvent(EVT_ITEMPANEL_DBCLICK,Subscriber(&CHistoryAdapterFix::OnCbxSelChange,this));
		pSItme->GetEventSet()->subscribeEvent(EVT_ITEMPANEL_RCLICK,Subscriber(&CHistoryAdapterFix::OnRClick,this));
		SImageWnd  *pAV_img= pItem->FindChildByName2<SImageWnd>(L"AV_img");
		int picw=0,pich=0;
		std::map<int,IBitmap*>::iterator _It=m_BitMap.find(position);
		if(_It==m_BitMap.end())
		{
			BITMAPINFOHEADER header;
			header.biSize = sizeof(BITMAPINFOHEADER);
			int bpp=32;
			header.biWidth = pAVPic->width;
			header.biHeight = pAVPic->height*(-1);
			header.biBitCount = bpp;
			header.biCompression = 0;
			header.biSizeImage = 0;
			header.biClrImportant = 0;
			header.biClrUsed = 0;
			header.biXPelsPerMeter = 0;
			header.biYPelsPerMeter = 0;
			header.biPlanes = 1;


			//3 构造文件头
			BITMAPFILEHEADER bmpFileHeader;
			HANDLE hFile = NULL;
			DWORD dwTotalWriten = 0;
			DWORD dwWriten;

			bmpFileHeader.bfType = 0x4d42; //'BM';
			bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
			bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ pAVPic->width*pAVPic->height*bpp/8;


			int Totl=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+pAVPic->bufLen;
			unsigned char *Imgbufx=(unsigned char *)::malloc(Totl);
			unsigned char *bufx2=Imgbufx;
			memcpy(bufx2,&bmpFileHeader, sizeof(BITMAPFILEHEADER));

			bufx2+=sizeof(BITMAPFILEHEADER);

			memcpy(bufx2,&header, sizeof(BITMAPINFOHEADER));
			bufx2+=sizeof(BITMAPINFOHEADER);
			memcpy(bufx2,pAVPic->pBuffer, pAVPic->bufLen);
			IBitmap *pImg=NULL;

			pRenderFactory->CreateBitmap(&pImg);
			if(pImg!=NULL){
			    HRESULT  ll=0;
			    ll=pImg->LoadFromMemory(Imgbufx,Totl);
			}
			picw=pImg->Width();
			pich=pImg->Height();
			pAV_img->SetImage(pImg);
			m_BitMap.insert(std::pair<int,IBitmap*>(position,pImg));
			
			::free(Imgbufx);
		}else{
			 pAV_img->SetImage(_It->second);
			 picw=_It->second->Width();
			 pich=_It->second->Height();
		}
		if(pich<128)
		{
		     int hh=(128-pich)/2;
			 SStringT form;
			 form.Format(_T("2,%d,@128,@%d"),hh,pich);
			 pAV_img->SetAttribute(_T("pos"),form);
			 form.Format(_T("[5,%d"),hh);
			 pItem->FindChildByName(L"winAVInfo")->SetAttribute(_T("pos"),form);
		}else{
		     pAV_img->SetAttribute(_T("pos"),_T("2,2,@128,@128"));
			  pItem->FindChildByName(L"winAVInfo")->SetAttribute(_T("pos"),_T("[5,5"));
		}
		
        wchar_t strtmp[2048]=L"";
		std::wstring CurTimeStr;
		CChineseCode::charTowchar(pAVPic->url,strtmp,2048);
		CurTimeStr=strtmp;
		int index=CurTimeStr.find_last_of(L"\\");
		if(index<0)
			index=CurTimeStr.find_last_of(L"//");
		if(index>=0)
		{
			 index++;
             CurTimeStr=CurTimeStr.substr(index,CurTimeStr.length()-index);
			 index=CurTimeStr.find_last_of(L".");
             CurTimeStr=CurTimeStr.substr(0,index);
		}
		

		SStatic *pTxt = pItem->FindChildByName2<SStatic>(L"AV_name");
		pTxt->GetUserData();
		pTxt->SetWindowText(CurTimeStr.c_str());



		SStatic *pAV_CurTime= pItem->FindChildByName2<SStatic>(L"AV_CurTime");
		

		int h=(pAVPic->CurTime/(60*60));
		int m=(pAVPic->CurTime%(60*60))/(60);
		int s=((pAVPic->CurTime%(60*60))%(60));
		
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
		pAV_CurTime->SetWindowText(CurTimeStr.c_str());


		h=pAVPic->TotalTime/(60*60);
		m=(pAVPic->TotalTime%(60*60))/60;
		s=((pAVPic->TotalTime%(60*60))%60);


		if(h<10)
		{
			swprintf_s(strtmp,L"0%d:",h);
			CurTimeStr=strtmp;
		}
		else{
			swprintf_s(strtmp,L"%d:",m);
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

		SStatic *pAV_Time= pItem->FindChildByName2<SStatic>(L"AV_Time");
		pAV_Time->SetWindowText(CurTimeStr.c_str());
	}

	bool         CHistoryAdapterFix::OnRClick(EventArgs *pEvt)
	{

		SItemPanel * pSItme=(SItemPanel *)pEvt->sender;
		if(pSItme)
		{
			CRect rt=	pSItme->GetClientRect();
			AV_Hos_Info * pAVPic =(AV_Hos_Info *)pSItme->GetUserData();
			g_stravhisurl=pAVPic->url;

			POINT point;
			GetCursorPos(&point);
			int xPos=point.x;
			int yPos=point.y;

			/*GetWindowRect(pSItme->GetHostHwnd(),&rt);
			xPos+=rt.left;
			yPos+=rt.top;*/
			SOUI::SMenuEx me;
			BOOL xx=me.LoadMenu(_T("SMENUEX:avhismenuex"));
			me.TrackPopupMenu(0,xPos,yPos,pSItme->GetHostHwnd());
		}
	   return true;
	}
	bool         CHistoryAdapterFix::OnCbxSelChange(EventArgs *pEvt)
	{
		SItemPanel * pSItme=(SItemPanel *)pEvt->sender;
		AV_Hos_Info * pAVPic =(AV_Hos_Info *)pSItme->GetUserData();

		::SendMessageA(pSItme->GetHostHwnd(),WM_UI_AVLIST_OPEN,(WPARAM)pAVPic->url,0);
	    return true;
	}
	static bool SortByM1( const AV_Hos_Info *v1, const AV_Hos_Info *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
	{  
		return v1->id < v2->id;//升序排列  
	}  
    static bool SortByM2( const AV_Hos_Info *v1, const AV_Hos_Info *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
	{  
		return v1->id > v2->id;//升序排列  
	}  
	void         CHistoryAdapterFix::UpdateData()
	{
		         ClearData();
		         CHistoryInfoMgr* InfoMgr=CHistoryInfoMgr::GetInance();
		         InfoMgr->GetAVHistoryInfo(m_slQue);
				 if(m_nSortOrder==0)
				    std::sort(m_slQue.begin(),m_slQue.end(),SortByM1);
				 else
					std::sort(m_slQue.begin(),m_slQue.end(),SortByM2);
				 notifyDataSetChanged();
				 
	}
	void         CHistoryAdapterFix::SortOrder(int i)
	{
	m_nSortOrder=i;
	}
	void         CHistoryAdapterFix::ClearData()
	{
		m_BitMap.clear();
	    for(int i=0;i!=m_slQue.size();i++)
		{
			AV_Hos_Info *Item=m_slQue.at(i);
			free(Item->url);
			free(Item->pBuffer);
			free(Item);
		}
		m_slQue.clear();
	}
}