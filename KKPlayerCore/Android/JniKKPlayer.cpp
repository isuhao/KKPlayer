#include "JniKKPlayer.h"
#include <jni.h>
#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include "AndPlayerStateNotifyMgr.h"
#include "AndKKPlayerUI.h"
#include <map>
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

///状态通知管理器
CAndPlayerStateNotifyMgr  PlayerStateNotifyMgr;
void* ConvertThread(void *param)
{

}

///播放器状态通知
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SetPlayerNotify(JNIEnv *env, jobject instance,jint obj,jobject notify)
{
	
	PlayerStateNotifyMgr.SetPlayerNotify(obj,notify);
}
/***********初始一个KKUI**********/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_IniKK(JNIEnv *pEv, jobject p, jint Render)
{
    CAndKKPlayerUI *pKKUI = new CAndKKPlayerUI(Render);
    int Ret=(int)pKKUI;
    return Ret;
}

JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_IniGl(JNIEnv *env, jobject instance, jint player)
{
    CAndKKPlayerUI *pKKUI = (CAndKKPlayerUI *)player;
    jint ret=pKKUI->Init();
	pKKUI->SetSurfaceTexture(env);
	return ret;
}

JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SetDecoderMethod(JNIEnv *env, jobject instance, jint obj, jint method)
{
	CAndKKPlayerUI *pKKUI = (CAndKKPlayerUI *)obj;
	if(pKKUI!=0){
		pKKUI->SetDecoderMethod(method);
	}
}
JNIEXPORT jobject JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GetSurfaceTexture(JNIEnv * env, jobject instance, jint obj)
{
	CAndKKPlayerUI *pKKUI = (CAndKKPlayerUI *)obj;
	if(pKKUI!=0){
		return pKKUI->GetSurfaceTexture();
	}
	return 0;
}
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_OnSurfaceTextureFrameAailable(JNIEnv * env, jobject instance, jint obj)
{
	CAndKKPlayerUI *pKKUI = (CAndKKPlayerUI *)obj;
	if(pKKUI!=0){
		pKKUI->OnSurfaceTextureFrameAailable();
	}
}
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SetKeepRatio(JNIEnv *env, jobject instance, jint obj, jint KeepRatio)
{
	CAndKKPlayerUI *pKKUI = (CAndKKPlayerUI *)obj;
    pKKUI->SetKeepRatio(KeepRatio);
}

JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_OnSize(JNIEnv *env, jobject instance, jint obj,jint w, jint h) {
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *)obj;
    return pKKUI->OnSize(w,h);
}

JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GlRender(JNIEnv *env, jobject instance, jint obj) {
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *)obj;
    pKKUI->renderFrame(0);
    return 1;
}


/*JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SurfaceRender(JNIEnv * env, jobject instance, jint obj, jobject surface)
{
	CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *)obj;
	
	LOGE("ANativeWindow_fromSurface ");
	ANativeWindow * mANativeWindow = ANativeWindow_fromSurface(env, surface);
	LOGE("Surface %d ",mANativeWindow);
	if(mANativeWindow !=NULL){
	
	pKKUI->renderFrame(mANativeWindow);
	ANativeWindow_release(mANativeWindow);
	LOGE("ANativeWindow_release ");
	}
    return 1;
}*/

JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_DelKK(JNIEnv *env, jobject instance,jint obj)
{
    if(obj!=0){
       CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
       delete pKKUI;
       LOGE("DelKK\n");
    }
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKOpenMedia(JNIEnv *env, jobject instance, jstring str_,jint obj)
{
    //char str[] ="rtmp://121.42.14.63/live/livestream";
    const char *str=env->GetStringUTFChars(str_, 0);

    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    int l= pKKUI->OpenMedia((char*)str);
    env->ReleaseStringUTFChars(str_, str);
    return l;
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKIsNeedReConnect(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    if(pKKUI->GetNeedReconnect())
        return 1;
	}
    return 0;
}
JNIEXPORT jstring JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GetMediaInfo(JNIEnv *env, jobject instance, jint obj) {
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    MEDIA_INFO info=pKKUI->GetMediaInfo();
    char s[256]="";
    sprintf(s,"%d;%d",info.CurTime,info.TotalTime);
    return env->NewStringUTF(s);
}
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_Pause(JNIEnv *env, jobject instance, jint obj) {
	if(obj!=0){
       CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
       pKKUI->Pause();
	}
}
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_Seek(JNIEnv *env, jobject instance, jint obj, jint value)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    pKKUI->Seek(value);
	}
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKCloseMedia(JNIEnv *env,jobject instance, jint obj)
{
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    pKKUI->CloseMedia();
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetPlayerState(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    return  pKKUI->GetPlayerState();
	}
	return -1;
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKIsReady(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    return  pKKUI->GetIsReady();
	}
	return -1;
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetRealtime(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    return  pKKUI->GetRealtime();
	}
	return -1;
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetRealtimeDelay(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    return  pKKUI->GetRealtimeDelay();
	}
	return -1;
}
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKForceFlushQue(JNIEnv *env, jobject instance,jint obj)
{
	if(obj!=0){
        CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
        pKKUI->ForceFlushQue();
	}
	
}

JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKSetMinRealtimeDelay(JNIEnv *env, jobject instance,jint obj, jint value)
{
	if(obj!=0){
    CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
    pKKUI->SetMinRealtimeDelay(value);
	}
	return -1;
}
jstring cToJstringutf(JNIEnv* env, const char* pat) {  
    jclass strClass = env->FindClass("java/lang/String");  
    jmethodID ctorID = env->GetMethodID(strClass, "<init>",  "([BLjava/lang/String;)V");  
    jbyteArray bytes = env->NewByteArray( strlen(pat));  
    env->SetByteArrayRegion( bytes, 0, strlen(pat), (jbyte*) pat);  
    jstring encoding = env->NewStringUTF("utf-8");  
    return (jstring)env->NewObject( strClass, ctorID, bytes, encoding);  
}
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GetkkMediaInfo(JNIEnv *env, jobject instance,jint obj, jobject jInfo)
{
	int ret=0;
	CAndKKPlayerUI *pKKUI=(CAndKKPlayerUI *) obj;
	if(pKKUI&&obj){
        MEDIA_INFO info=pKKUI->GetMediaInfo();
        jclass Infocls = env->GetObjectClass(jInfo); 
        jfieldID jfsAVRes = env->GetFieldID(Infocls, "AVRes", "Ljava/lang/String;");
        info.AVRes[1023]=0;		
		if(strlen(info.AVRes)>0)
		env->SetObjectField(jInfo, jfsAVRes, env->NewStringUTF(info.AVRes));
      
	    
	    ///视频解码器名称
	    jfieldID jfVideoCodecname = env->GetFieldID(Infocls, "VideoCodecname", "Ljava/lang/String;");
        if(strlen(info.videoinfo.codecname)>0)		
		env->SetObjectField(jInfo, jfVideoCodecname, cToJstringutf(env,info.videoinfo.codecname));
	
	    ///平均码率
	    jfieldID jfVideoBitrate = env->GetFieldID(Infocls, "VideoBitrate", "I");		
        env->SetIntField(jInfo, jfVideoBitrate, info.videoinfo.bitrate);		
		
        ///平均帧率
	    jfieldID jfVideoFramerate = env->GetFieldID(Infocls, "VideoFramerate", "I");		
        env->SetIntField(jInfo, jfVideoFramerate, info.videoinfo.framerate);	
   
   
        ///音频解码器名称
        jfieldID jfAudioCodecname = env->GetFieldID(Infocls, "AudioCodecname", "Ljava/lang/String;");
        if(strlen(info.audioinfo.codecname)>0)		
		env->SetObjectField(jInfo, jfAudioCodecname, cToJstringutf(env,info.audioinfo.codecname));
	    
		///音频采样率
	    jfieldID jfAudioSample_rate = env->GetFieldID(Infocls, "AudioSample_rate", "I");		
        env->SetIntField(jInfo, jfAudioSample_rate, info.audioinfo.sample_rate);	
       
	    ///声 道 数
	    jfieldID jfAudioChannels = env->GetFieldID(Infocls, "AudioChannels", "I");		
        env->SetIntField(jInfo, jfAudioChannels, info.audioinfo.channels);	
        
		
		///文件大小
        jfieldID jfiFileSize = env->GetFieldID(Infocls, "FileSize", "I");		
        env->SetIntField(jInfo, jfiFileSize, info.FileSize);		
		
		///当前播放时间
		jfieldID jfiCurTime = env->GetFieldID(Infocls, "CurTime", "I");	
		env->SetIntField(jInfo, jfiCurTime, info.CurTime);
		
		///总时间
		jfieldID jfiTotalTime = env->GetFieldID(Infocls, "TotalTime", "I");	
		env->SetIntField(jInfo, jfiTotalTime, info.TotalTime); 
		
		///序列号
		jfieldID jfiSerial = env->GetFieldID(Infocls, "Serial", "I");	
		env->SetIntField(jInfo, jfiSerial, info.serial);
		
		///序列号1
		jfieldID jfiSerial1 = env->GetFieldID(Infocls, "Serial1", "I");	
		env->SetIntField(jInfo, jfiSerial1, info.serial1);
		
		jfieldID jfiOpen = env->GetFieldID(Infocls, "Open", "I");	
		env->SetIntField(jInfo, jfiOpen, info.Open);
		  
		jfieldID jfiKKState = env->GetFieldID(Infocls, "KKState", "I");	
		env->SetIntField(jInfo, jfiKKState, info.KKState);
		
		jfieldID jfiSegId = env->GetFieldID(Infocls, "SegId", "I");	
		env->SetIntField(jInfo, jfiSegId, info.SegId);
		//return 1;
		jfieldID jfsSpeedInfo= env->GetFieldID(Infocls, "SpeedInfo", "Ljava/lang/String;");	
		
		info.SpeedInfo[1023]=0;
	    if(strlen(info.SpeedInfo)>0)	
		    env->SetObjectField(jInfo, jfsSpeedInfo, env->NewStringUTF(info.SpeedInfo));
		if(info.serial1!=-1)
			ret=1;
		return ret;
    }
	return 0;
}

///Java虚拟机变量
JavaVM *        kk_Gjava_vm=NULL;
pthread_mutex_t kk_Gvm_lock = PTHREAD_MUTEX_INITIALIZER;
extern "C"{
int av_jni_set_java_vm(void *vm, void *log_ctx);
}
/**
* JNI 库模块加载事件处理接口。可以做一些函数的加载
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * java_vm, void * reserved)
{
	kk_Gjava_vm=java_vm;
	av_jni_set_java_vm(java_vm, NULL);
	
	PlayerStateNotifyMgr.Start();
	return JNI_VERSION_1_6;
}
/**
* JNI 库模块卸载事件处理接口。
*/
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * java_vm, void * reserved)
{
	kk_Gjava_vm=0;
}


///线程附加到vm中
void* kk_jni_attach_env()
{
    int ret = 0;
    JNIEnv *env = NULL;

	PlayerStateNotifyMgr.Stop();
    pthread_mutex_lock(&kk_Gvm_lock);
   
    JavaVM* java_vm=kk_Gjava_vm;
    if (!java_vm) {
         LOGE("No Java virtual machine has been registered\n");
         return NULL;
    }

    ret = java_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    switch(ret)
	{
		case JNI_EDETACHED:
			if (java_vm->AttachCurrentThread(&env, NULL) != 0) {
				LOGE("Failed to attach the JNI environment to the current thread\n");
				env = NULL;
			}
			break;
		case JNI_OK:
			break;
		case JNI_EVERSION:
			LOGE("The specified JNI version is not supported\n");
			break;
		default:
			LOGE("Failed to get the JNI environment attached to this thread \n");
			break;
    }

	if(env!=NULL){
		LOGE("attached to this thread Ok \n");
	}
	 pthread_mutex_unlock(&kk_Gvm_lock);
    return env;
}
int kk_jni_detach_env()
{
	pthread_mutex_lock(&kk_Gvm_lock);
	JavaVM* java_vm=kk_Gjava_vm;
    if (java_vm == NULL) {
        LOGE("No Java virtual machine has been registered\n");
		pthread_mutex_unlock(&kk_Gvm_lock);
        return -1;
    }

    int ret=java_vm->DetachCurrentThread();
	pthread_mutex_unlock(&kk_Gvm_lock);
	LOGE("detached to this thread Ok \n");
	return ret;
}
