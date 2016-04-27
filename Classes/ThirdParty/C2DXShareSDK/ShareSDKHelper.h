//
//  ShareSDKHelper.h
//  PinLe
//
//  Created by Kennedy on 15/9/2.
//
//

#ifndef __PinLe__ShareSDKHelper__
#define __PinLe__ShareSDKHelper__


#include "cocos2d.h"


#if   (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/SdkPublic/C2DXShareSDK.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "Android/SdkPublic/C2DXShareSDK.h"
#endif

#if   (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #define SHARESDK_APPID          "a1bb2a4cf752"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define SHARESDK_APPID          "a1bb805aec94"
#endif


using namespace std;


//----------------------------------【微信】-------------------------------------------
#define OPEN_WECHAT_APPID                   "wx9389b5e4d6e62685"
#define OPEN_WECHAT_APPSECRET               "d99360c9e13c53207f20f786a6902587"
//----------------------------------【QQ】--------------------------------------------
#define OPEN_QQ_APPID                       "1104665654"
#define OPEN_QQ_APPKEY                      "tTALUgQjTy1dlLR1"
//----------------------------------【新浪微博】---------------------------------------
#define OPEN_SINA_APPID                     "4234430023"
#define OPEN_SINA_APPSECRET                 "cf71f74bcd8a48c58be6836aeb5cd55a"



//==================================[message define]============================
#define MES_SHARESDK_SUCCEED                "MES_SHARESDK_SUCCEED"
#define MES_SHARESDK_FAILED                 "MES_SHARESDK_FAILED"
#define MES_SHARESDK_START                  "MES_SHARESDK_START"
#define MES_SHARESDK_CANCELED               "MES_SHARESDK_CANCELED"



class ShareSDKHelper
{
public:
    ShareSDKHelper();
    ~ShareSDKHelper();
    static  ShareSDKHelper *getInstance();
    static  void            destroyInstance();
    
public:
    void    initSDK(const string& AppID);
    void    initPlatformConfig();
    void    initSDKAndPlatformConfig(const string& AppID);
    
public:
    void    sendImage(const string& title, const string& description, const string& imagePath);
    void    sendLink(const string& title, const string& description, const string& webpageUrl, const string& imagePaht);
    
};




#endif /* defined(__PinLe__ShareSDKHelper__) */
