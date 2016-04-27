#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "test.h"
#include "gameini/GameIni.h"

using namespace cocos2d;
using namespace std;



extern "C"
{



std::string jstring2str(JNIEnv* env, jstring jstr)

{

    char*   rtn   =   NULL;

    jclass   clsstring   =   env->FindClass("java/lang/String");

    jstring   strencode   =   env->NewStringUTF("GB2312");

    jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");

    jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);

    jsize   alen   =   env->GetArrayLength(barr);

    jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);

    if(alen   >   0)

    {

        rtn   =   (char*)malloc(alen+1);

        memcpy(rtn,ba,alen);

        rtn[alen]=0;

    }

    env->ReleaseByteArrayElements(barr,ba,0);

    std::string stemp(rtn);

    free(rtn);

    return   stemp;

}

//void Java_org_cocos2dx_cpp_gamejnihelper_getdeviceInfo(JNIEnv*  env,
//		jobject thiz, jstring str)
//{
//
//
//	getdeviceInfo(jstring2str(env, str));
//}
//
//
//
//void Java_org_cocos2dx_cpp_gamejnihelper_getLocationInfo(JNIEnv*  env,
//		jobject thiz, jdouble x, jdouble y)
//{
//	getLocationInfo(x, y);
//}



}
