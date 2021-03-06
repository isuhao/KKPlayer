#include "StdAfx.h"
#include "KKSound.h"
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"dsound.lib")
#define MAX_AUDIO_BUF 4  
#define BUFFERNOTIFYSIZE 1024*4//16384//1024*16*8
DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];  
HANDLE m_event[MAX_AUDIO_BUF];  
int sample_rate=44100;  //PCM sample rate  
int channels=2;         //PCM channel number  
int bits_per_sample=16; //bits per sample  
CKKSound::CKKSound(void)
{   
	m_ReadAudioH=0;
	m_ReadAddr=0;
}
int CKKSound::InitAudio()
{
	m_ReadAudioH=0;
	m_ReadAddr=0;
	IsClose=true;
	lpDirectSound=NULL;
	res=WAIT_OBJECT_0;  
	m_pFun=NULL;
	m_UserData=NULL;
	m_pDSBuffer8=NULL; //used to manage sound buffers.  
	ppDSBuffer=NULL;
	m_pDSNotify=NULL;
	buf=NULL;  
	buf_len=0;  
	m_Stop=true;
	
	offset=BUFFERNOTIFYSIZE;  
	HRESULT hr = DirectSoundCreate8(NULL, &lpDirectSound, NULL);
	if(FAILED(hr))
	{
		return -1;
	}
	return 0;
}
CKKSound::~CKKSound(void)
{
	::CloseHandle(m_ReadAudioH);
}
void CKKSound::SetUserData(void* UserData)
{
	m_UserData=UserData;
}
void CKKSound::SetWindowHAND(int hwnd)
{

	if(lpDirectSound==NULL)
	{
		//::MessageBox( hwnd,L"DirectSoundCreate8创建错误,请检查音频设配",L"提示",MB_ICONHAND);
		::MessageBox((HWND) hwnd,L"系统检测到电脑未开启音频设备，为确保系统正常，建议您开启音频设备",L"提示",MB_ICONINFORMATION);
		return;
	}

	HRESULT hr = lpDirectSound->SetCooperativeLevel((HWND)hwnd,DSSCL_PRIORITY);
	if(FAILED(hr))
	{
		//error
	}
	int sample_rate=44100;  //PCM sample rate  
	int channels=2;         //PCM channel number  
	int bits_per_sample=16; //bits per sample  
	DSBUFFERDESC dsbd;  
	memset(&dsbd,0,sizeof(dsbd));  
	dsbd.dwSize=sizeof(dsbd); 
	dsbd.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLVOLUME;  
	dsbd.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE;   
	//WAVE Header  
	dsbd.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));  
	dsbd.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM;     
	/* format type */  
	(dsbd.lpwfxFormat)->nChannels=channels;            
	/* number of channels (i.e. mono, stereo...) */  
	(dsbd.lpwfxFormat)->nSamplesPerSec=sample_rate;       
	/* sample rate */  
	(dsbd.lpwfxFormat)->nAvgBytesPerSec=sample_rate*(bits_per_sample/8)*channels;   
	/* for buffer estimation */  
	(dsbd.lpwfxFormat)->nBlockAlign=(bits_per_sample/8)*channels;          
	/* block size of data */  
	(dsbd.lpwfxFormat)->wBitsPerSample=bits_per_sample;       
	/* number of bits per sample of mono data */  
	(dsbd.lpwfxFormat)->cbSize=0;  

	hr=lpDirectSound->CreateSoundBuffer(&dsbd,&ppDSBuffer,NULL);
	if(FAILED(hr))
	{
		//error
	}

	if( FAILED(ppDSBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&m_pDSBuffer8)))
	{  
		//error
	}  
	//Get IDirectSoundNotify8  
	if(FAILED(ppDSBuffer->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify)))
	{  
		//error
	}  
	for(int i =0;i<MAX_AUDIO_BUF;i++)
	{  
		m_pDSPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;  
		m_event[i]=::CreateEvent(NULL,TRUE,false,NULL);   /********收到*********/
		m_pDSPosNotify[i].hEventNotify=m_event[i];  
	} 

	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_pDSPosNotify);  
	m_pDSNotify->Release();  

	IsClose=false;
	 
  // m_ReadAudioH=(HANDLE)_beginthreadex(NULL, NULL, KKAudiothread, (LPVOID)this, 0,&m_ReadAddr);
}
void CKKSound::SetVolume(long value)
{
	if(m_pDSBuffer8!=NULL)
	 m_pDSBuffer8->SetVolume(value);
	
}
long CKKSound::GetVolume()
{
   long ll=0;
  HRESULT hr = m_pDSBuffer8->GetVolume(&ll);
   return ll;
}
 
void CKKSound::ReadAudio()
{
	
	if(!IsClose&&lpDirectSound!=NULL)
	{
		if((res >=WAIT_OBJECT_0)&&(res <=WAIT_OBJECT_0+MAX_AUDIO_BUF))
		{ 
			m_pDSBuffer8->Lock(offset,BUFFERNOTIFYSIZE,&buf,&buf_len,NULL,NULL,0);
			if(buf!=NULL){
				memset(buf,0,buf_len);
				if(m_pFun!=NULL)
				{
					
					m_pFun(m_UserData,(char*)buf,buf_len);
				}
			}
			m_pDSBuffer8->Unlock(buf,buf_len,NULL,0); 

			offset+=buf_len;  
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);  
			//重置信号
			ResetEvent(m_event[res]); 
		}
		m_Lock.Lock();
		bool ll=m_Stop;
		m_Lock.Unlock();
		if(m_Stop)
		{
		  res=WaitForMultipleObjects (MAX_AUDIO_BUF, m_event, FALSE, INFINITE);  
		}/**/
	}
}
void CKKSound::CloseAudio()
{
	m_Lock.Lock();
	IsClose=true;
	m_Lock.Unlock();
	for(int i=0;i< MAX_AUDIO_BUF ;i++)
	{
	  CloseHandle(m_event[i]);
	}
	if(m_pDSBuffer8!=NULL)
	{
		m_pDSBuffer8->Release();
		m_pDSBuffer8=NULL;
	}
	if(lpDirectSound!=NULL)
	{
		lpDirectSound->Release();
		lpDirectSound=NULL;
	}
    

}
void CKKSound::SetAudioCallBack(pfun fun)
{
	m_pFun=fun;
}

void CKKSound::Start()
{
	if(m_pDSBuffer8!=NULL)
	{
		m_pDSBuffer8->SetCurrentPosition(0);  
		m_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
		m_Lock.Lock();
		m_Stop=true;
		m_Lock.Unlock();
	}
}
void CKKSound::Stop()
{
	if(m_pDSBuffer8!=NULL)
	{
		m_Lock.Lock();
		m_Stop=false;
		m_Lock.Unlock();
		m_pDSBuffer8->Stop();
		for (int i=0;i<MAX_AUDIO_BUF;i++)
		{
			SetEvent(m_event[i]);
		}
	}
}
unsigned __stdcall CKKSound::KKAudiothread(LPVOID lpParameter)
{
	/*CKKSound* p=(CKKSound* )lpParameter;
	p->ReadAudio();*/
    return 1;
}