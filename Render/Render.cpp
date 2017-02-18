// Render.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Render.h"
#include "renderD3D.h"
#include "renderGDI.h"
#include <map>
std::map<CRender *,int> m_Rendermap;
void skpngZhuc();

#ifdef _DEBUG
#pragma comment (lib,"..\\debug\\libx86\\zlibstat.lib")
#pragma comment (lib,"..\\debug\\libx86\\pngd.lib") 
#pragma comment (lib,"..\\Debug\\libx86\\skiad.lib")
#else
#pragma comment (lib,"..\\Release\\libx86\\zlibstat.lib")
#pragma comment (lib,"..\\Release\\libx86\\png.lib")
#pragma comment (lib,"..\\Release\\libx86\\skia.lib")
#endif
#pragma comment (lib,"Windowscodecs.lib")
//#pragma comment (lib,"D3dx9.lib")
#pragma comment (lib,"Usp10.lib")
#pragma comment (lib,"Opengl32.lib")

extern "C"{
	void __declspec(dllexport) *CreateRender(HWND h,char *Oput){

		 bool xxxaa=true;
		 if(xxxaa){
		     skpngZhuc();
		     xxxaa=false;
		 }
		
		 CRender *m_pRender = new CRenderD3D();
		 *Oput=1;
		 if(!m_pRender->init(h))
		 {
            delete m_pRender;
			m_pRender =new CRenderGDI();
			m_pRender->init(h);
			*Oput=0;
			m_Rendermap.insert(std::pair<CRender *,int>(m_pRender,0));
		 }else{
            m_Rendermap.insert(std::pair<CRender *,int>(m_pRender,1));
		 }


		
		 return m_pRender;
	}

	char __declspec(dllexport) DelRender(void *p,char RenderType)
	{
          CRender *pIn=(CRender*)p;

		  std::map<CRender *,int>::iterator It= m_Rendermap.find(pIn);
		  if(It!=m_Rendermap.end())
		  {
			  if(It->second==1)
			  {
				  CRenderD3D *px=(CRenderD3D*)pIn;
				  delete px;
			  }else{
				  CRenderGDI *px=(CRenderGDI*)pIn;
				  delete px;
			  }
			  m_Rendermap.erase(It);
		  }
		 
		 delete pIn;
		 return 1;
	}
}
