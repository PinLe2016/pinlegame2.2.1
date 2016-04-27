#include "./CCPushService.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <platform/android/jni/JniHelper.h>

#include <cocos2d.h>

using namespace cocos2d;

#define SAFE_RELEASE(obj)   if(obj) { \
t.env->DeleteLocalRef(obj); \
}

namespace Cocos {
    
	static PushListener*	gListener = NULL;
	jstring string2jString(const char* pData);
	jobject cmap2jhashmap(JNIEnv* env, std::map<char*, char*> cmap);

    void PushService::setDebugMode(bool debug)
    {
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                             ,"com/cocos/CCPushHelper"
                             ,"setDebugMode"
                                ,"(Z)V"))
        {
            jboolean b = debug;
            t.env->CallStaticVoidMethod(t.classID, t.methodID, b);
            SAFE_RELEASE(t.classID);
        }
    }

    std::string PushService::getSDKVersion()
    {
        
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"getSDKVersion"
                                          ,"()Ljava/lang/String;"))
        {
            jstring version = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
            std::string ret = JniHelper::jstring2string(version);
            SAFE_RELEASE(t.classID);
            SAFE_RELEASE(version);
            
            return ret;
        }
        
        return "";
    }
    
    void PushService::setPushSwitchState(bool state)
    {
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                    ,"com/cocos/CCPushHelper"
                                    ,"setPushSwitchState"
                                    ,"(Z)V"))
        {
            jboolean b = state;
            t.env->CallStaticVoidMethod(t.classID, t.methodID, b);
            SAFE_RELEASE(t.classID);
        }
    }

    void PushService::setTags(const Tags& tags)
	{
        JniMethodInfo t;
        JniHelper::getMethodInfo(t
                                 , "java/util/ArrayList"
                                 , "<init>"
                                 , "()V");
        jobject list = t.env->NewObject(t.classID, t.methodID);
        SAFE_RELEASE(t.classID);
        
        JniHelper::getMethodInfo(t
                                 , "java/util/ArrayList"
                                 , "add"
                                 , "(Ljava/lang/Object;)Z");
       
        Tags::const_iterator it, itEnd = tags.end();
        for(it = tags.begin(); it != itEnd; ++it)
        {
            std::string str = *it;
			const char* pStr = str.c_str();
         
			jstring jStr = string2jString(pStr);
            log("PushService::setTags----------------1");
            t.env->CallObjectMethod(list, t.methodID,  jStr);
            SAFE_RELEASE(jStr);
        }
        
        SAFE_RELEASE(t.classID);

        if(JniHelper::getStaticMethodInfo(t
                                    ,"com/cocos/CCPushHelper"
                                    ,"setTags"
                                    , "(Ljava/util/List;)V"))
        {
            t.env->CallStaticVoidMethod(t.classID,t.methodID,list);
            SAFE_RELEASE(t.classID);
		}
	}

	void PushService::delTags(const Tags& tags)
	{
		JniMethodInfo t;
		JniHelper::getMethodInfo(t
			, "java/util/ArrayList"
			, "<init>"
			, "()V");
		jobject list = t.env->NewObject(t.classID, t.methodID);
		SAFE_RELEASE(t.classID);
		JniHelper::getMethodInfo(t
			, "java/util/ArrayList"
			, "add"
			, "(Ljava/lang/Object;)Z");

		Tags::const_iterator it, itEnd = tags.end();
		for(it = tags.begin(); it != itEnd; ++it)
		{
			std::string str = *it;
			const char* pStr = str.c_str();

			jstring jStr = string2jString(pStr);
			t.env->CallObjectMethod(list, t.methodID,  jStr);
			SAFE_RELEASE(jStr);
		}
		SAFE_RELEASE(t.classID);
		if(JniHelper::getStaticMethodInfo(t
			,"com/cocos/CCPushHelper"
			,"delTags"
			, "(Ljava/util/List;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID,t.methodID,list);
		}
		SAFE_RELEASE(t.classID);
	}

    void PushService::setStringTags(const char* str)
    {
        if(str == NULL)
        {
            return;
        }
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"setStringTags"
                                          ,"(Ljava/lang/String;)V"))
        {
            jstring strAccount = string2jString(str);
            t.env->CallStaticVoidMethod(t.classID, t.methodID, strAccount);
            SAFE_RELEASE(t.classID);
            SAFE_RELEASE(strAccount);
        }
    }
    
    
	void PushService::setAccount(const char* account)
	{
        if(account == NULL)
        {
            JniMethodInfo t;
            if(JniHelper::getStaticMethodInfo(t
                                              ,"com/cocos/CCPushHelper"
                                              ,"delAccount"
                                              ,"()V"))
            {
                t.env->CallStaticVoidMethod(t.classID,t.methodID);
                SAFE_RELEASE(t.classID);
            }
            
            return;
        }
        
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t
			,"com/cocos/CCPushHelper"
			,"setAccount"
			,"(Ljava/lang/String;)V"))
		{
			jstring strAccount = string2jString(account);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, strAccount);
			SAFE_RELEASE(t.classID);
            SAFE_RELEASE(strAccount);
		}
	}

	int PushService::setSilentTime(int startHour, int startMinute, int endHour, int endMinute)
	{
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"setSilentTime"
                                          ,"(IIII)I"))
        {
            jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID, startHour, startMinute, endHour, endMinute);
            SAFE_RELEASE(t.classID);
            
            return ret;
        }
        
        return -2;
	}

	int PushService::delSilentTime()
	{
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"delSilentTime"
                                          ,"()I"))
        {
            jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            SAFE_RELEASE(t.classID);
            
            return ret;
        }
        
        return -2;
	}
    
    int PushService::setLocalTimer(long time, const std::string& customParam, const std::string& notifyTitle, const std::string& notifyText)
	{
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"setLocalTimer"
                                          ,"(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I"))
        {
            jstring jparam = string2jString(customParam.c_str());
            jstring jTitle = string2jString(notifyTitle.c_str());
            jstring jText = string2jString(notifyText.c_str());
			jlong jTime = time;
			jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID, jTime, jparam, jTitle, jText);
            SAFE_RELEASE(t.classID);
            SAFE_RELEASE(jparam);
            SAFE_RELEASE(jTitle);
            SAFE_RELEASE(jText);
			return ret;
		}else{
			return -1;
		}
    }
    
    int PushService::cancelLocalTimer(long time, const std::string& customParam, const std::string& notifyTitle, const std::string& notifyText)
    {
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"cancelLocalTimer"
                                          ,"(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I"))
        {
            jstring jparam = string2jString(customParam.c_str());
            jstring jTitle = string2jString(notifyTitle.c_str());
            jstring jText = string2jString(notifyText.c_str());
            jlong jTime = time;
            jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID, jTime, jparam, jTitle, jText);
            SAFE_RELEASE(t.classID);
            SAFE_RELEASE(jparam);
            SAFE_RELEASE(jTitle);
            SAFE_RELEASE(jText);
			return ret;
		}else{
			return -1;
		}
    }
    
	int PushService::sendImMessage(const std::string& senderAccount, const std::string& receiverAccount, const std::string& content, const std::string& msgId) 
	{
		JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"sendImMessage"
                                          ,"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I"))
        {
            jstring jsender = string2jString(senderAccount.c_str());
            jstring jreceiver = string2jString(receiverAccount.c_str());
            jstring jcontent = string2jString(content.c_str());
			jstring jmsgid = string2jString(msgId.c_str());
            jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID, jsender, jreceiver, jcontent, jmsgid);
            SAFE_RELEASE(t.classID);
            SAFE_RELEASE(jsender);
            SAFE_RELEASE(jreceiver);
            SAFE_RELEASE(jcontent);
			SAFE_RELEASE(jmsgid);
			return ret;
		}else{
			return -1;
		}	
	}
	
    int PushService::cancelAllLocalTimer()
    {
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t
                                          ,"com/cocos/CCPushHelper"
                                          ,"cancelAllLocalTimer"
                                          ,"()I"))
        {
            jint ret = (jint)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            SAFE_RELEASE(t.classID);
			return ret;
        }else{
			return -1;
		}
    }

	void Analytics4PushService::setDebugMode(bool debug)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t
			, "com/cocos/CCPushHelper"
			, "setDebugModeForAnalytics"
			, "(Z)V"))
		{
			jboolean b = debug;
			t.env->CallStaticVoidMethod(t.classID, t.methodID, b);
			SAFE_RELEASE(t.classID);
		}
	}

	void Analytics4PushService::executeAnalytics(char* eventId)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t
			, "com/cocos/CCPushHelper"
			, "executeAnalytics"
			, "(Ljava/lang/String;)V"))
		{
			jstring strEventId = string2jString(eventId);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, strEventId);
			SAFE_RELEASE(t.classID);
			SAFE_RELEASE(strEventId);
		}
	}

	void Analytics4PushService::executeAnalytics(char* eventId, std::map<char*, char*> params)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t
			, "com/cocos/CCPushHelper"
			, "executeAnalytics"
			, "(Ljava/lang/String;Ljava/util/HashMap;)V")){

			jstring strEventId = string2jString(eventId);
			jobject objParamsMap = cmap2jhashmap(t.env, params);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, strEventId, objParamsMap);
			SAFE_RELEASE(t.classID);
			SAFE_RELEASE(strEventId);
			SAFE_RELEASE(objParamsMap);
		}
	}

	jobject cmap2jhashmap(JNIEnv* env, std::map<char*, char*> cmap){
		jclass jmapclass = env->FindClass("java/util/HashMap");
		jmethodID mid = env->GetMethodID(jmapclass, "<init>", "()V");
		jmethodID putmethod = env->GetMethodID(jmapclass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		jobject jmap = env->NewObject(jmapclass, mid);
		jclass strClass = env->FindClass("java/lang/String");
		jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
		jstring encoding = env->NewStringUTF("utf-8");

		std::map<char*, char*>::iterator it;
		for (it = cmap.begin(); it != cmap.end(); it++)
		{
			char* key = it->first;
			jbyteArray keybytes = env->NewByteArray(strlen(key));
			env->SetByteArrayRegion(keybytes, 0, strlen(key), (jbyte*)key);
			jstring jkey = (jstring)env->NewObject(strClass, ctorID, keybytes, encoding);
			char* value = it->second;
			jbyteArray valuebytes = env->NewByteArray(strlen(value));
			env->SetByteArrayRegion(valuebytes, 0, strlen(value), (jbyte*)value);
			jstring jvalue = (jstring)env->NewObject(strClass, ctorID, valuebytes, encoding);
			env->CallVoidMethod(jmap, putmethod, jkey, jvalue);
		}
		return jmap;
	}

    void PushService::registCallBack(PushListener* listener)
    {
        gListener = listener;
	}
	
	jstring string2jString(const char* pData)
	{
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t
			,"com/cocos/CCPushHelper"
			,"setDebugMode"
			,"(Z)V")) {
			if(NULL == pData) {
				return t.env->NewStringUTF("");
			}

			jclass strClass = t.env->FindClass("java/lang/String");
			jmethodID ctorID = t.env->GetMethodID(strClass, "<init>",
				"([BLjava/lang/String;)V");
			jbyteArray bytes = t.env->NewByteArray(strlen(pData));
			t.env->SetByteArrayRegion(bytes, 0, strlen(pData), (jbyte*) pData);
			jstring encoding = t.env->NewStringUTF("utf-8");
			jstring result = (jstring) t.env->NewObject(strClass, ctorID, bytes, encoding);
			t.env->DeleteLocalRef(encoding);
			t.env->DeleteLocalRef(bytes);
			t.env->DeleteLocalRef(strClass);
			SAFE_RELEASE(t.classID);
			return result;
		}
	}
    
}

extern "C"
{
    void Java_com_cocos_CCPushClientReceiver_nativePushSwitchState(JNIEnv*  env, jobject obj, jint retCode)
    {
        if(Cocos::gListener)
		{
            Cocos::gListener->onSetPushSwitchState(retCode);
		}
	}

	void Java_com_cocos_CCPushClientReceiver_nativeSetTags(JNIEnv*  env, jobject obj, jint retCode, jobject object)
	{
		if(Cocos::gListener)
		{
			Cocos::Tags tags;
			JniMethodInfo t;
			if ( JniHelper::getMethodInfo(t
				,"java/util/ArrayList"
				,"iterator"
				,"()Ljava/util/Iterator;")){
					jobject j_iteraror = t.env->CallObjectMethod(object,t.methodID);
					SAFE_RELEASE(t.classID);

					if(JniHelper::getMethodInfo(t
						,"java/util/Iterator"
						,"hasNext"
						,"()Z")){

							JniMethodInfo ret;
							if( JniHelper::getMethodInfo(ret
								,"java/util/Iterator"
								,"next"
								,"()Ljava/lang/Object;")){
									while(t.env->CallBooleanMethod(j_iteraror,t.methodID)){
										jstring string = (jstring)ret.env->CallObjectMethod(j_iteraror,ret.methodID);
                                        std::string tag = JniHelper::jstring2string(string);
                                        
										tags.push_back(tag);										
										SAFE_RELEASE(string);
									}
									SAFE_RELEASE(ret.classID);
							}
                        
                        SAFE_RELEASE(t.classID);
					}
			}
			Cocos::gListener->onSetTags(retCode, tags);
		}
	}

	void Java_com_cocos_CCPushClientReceiver_nativeDelTags(JNIEnv*  env, jobject obj, jint retCode, jobject object)
	{
		if(Cocos::gListener)
		{
			Cocos::Tags tags;
			JniMethodInfo t;
			if ( JniHelper::getMethodInfo(t
				,"java/util/ArrayList"
				,"iterator"
				,"()Ljava/util/Iterator;")){
					jobject j_iteraror = t.env->CallObjectMethod(object,t.methodID);
					SAFE_RELEASE(t.classID);

					if(JniHelper::getMethodInfo(t
						,"java/util/Iterator"
						,"hasNext"
						,"()Z")){

							JniMethodInfo ret;
							if( JniHelper::getMethodInfo(ret
								,"java/util/Iterator"
								,"next"
								,"()Ljava/lang/Object;" )){
									while(t.env->CallBooleanMethod(j_iteraror,t.methodID)){
										jstring string = (jstring)ret.env->CallObjectMethod(j_iteraror,ret.methodID);
                                        std::string tag = JniHelper::jstring2string(string);
                                        
										tags.push_back(tag);										
										SAFE_RELEASE(string);
									}
									SAFE_RELEASE(ret.classID);
							}
                        
                        SAFE_RELEASE(t.classID);
					}
			}
			Cocos::gListener->onDelTags(retCode, tags);
		}
	}

	void Java_com_cocos_CCPushClientReceiver_nativeSetAccount(JNIEnv*  env, jobject obj, jint retCode, jstring account)
	{
		if(Cocos::gListener)
		{
            std::string strAccount = JniHelper::jstring2string(account);
			Cocos::gListener->onSetAccount(retCode, strAccount);
		}
	}
    
    void Java_com_cocos_CCPushClientReceiver_nativeOnLocalTimer(JNIEnv* env, jobject obj, jstring param)
    {
        if(Cocos::gListener)
		{
            std::string strParam = JniHelper::jstring2string(param);
			Cocos::gListener->onLocalTimer(strParam);
		}
    }
    
    void Java_com_cocos_CCPushClientReceiver_nativeOnMessage(JNIEnv* env, jobject obj, jstring param)
    {
        if(Cocos::gListener)
		{
            std::string strParam = JniHelper::jstring2string(param);
			Cocos::gListener->onMessage(strParam);
		}
    }
	
	void Java_com_cocos_CCPushClientReceiver_nativeReceiveImMessage(JNIEnv* env, jobject obj, jstring sender, jstring content)
    {
        if(Cocos::gListener)
		{
            std::string strSender = JniHelper::jstring2string(sender);
			std::string strContent = JniHelper::jstring2string(content);
			Cocos::gListener->onReceiveImMessage(strSender, strContent);
		}
    }
	
	void Java_com_cocos_CCPushClientReceiver_nativeSendImMessage(JNIEnv* env, jobject obj, jstring msgId, jint retCode, jstring retMsg)
    {
        if(Cocos::gListener)
		{
            
			std::string strMsgId = JniHelper::jstring2string(msgId);
			std::string strRetMsg = JniHelper::jstring2string(retMsg);
			Cocos::gListener->onSendImMessage(strMsgId, retCode, strRetMsg);
		}
    }
	
}
#endif      //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)