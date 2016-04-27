//
//  CCPushService.cpp
//  HelloLua
//
//  Created by 孙永刚 on 14-7-7.
//
//

#include "CCPushService.h"
#import "CCPush.h"
#import "Analytics4Push.h"
#import "objc/runtime.h"
#pragma mark - Convert

//ARC与非ARC模式下C指针与OC对象互转（数据结构相同前提下）
#if __has_feature(objc_arc)
#define C_BRIDGE_OC(abc) ((__bridge id) abc)
#define OC_BRIDGE_C(abc) ((__bridge void*) abc)
#define SAFE_ARC_RELEASE(abc) (abc)
#define SAFE_ARC_AUTORELEASE(abc) (abc)
#else
#define C_BRIDGE_OC(abc) ((id) abc)
#define OC_BRIDGE_C(abc) ((void*) abc)
#define SAFE_ARC_RELEASE(abc) ([(abc) release])
#define SAFE_ARC_AUTORELEASE(abc) ([(abc) autorelease])
#endif

#define CC_NOTIFICATION_ID @"cc_notification_id"
#define CC_NOTIFICATION_P @"cc_notification_param"

NSArray *listToNSArray(const Cocos::Tags *tags)
{
    if (tags == NULL) {
        return nil;
    }
    
    if (tags->empty()) {
        return [NSArray array];
    }
    
    std::list<std::string>::const_iterator it;
    NSMutableArray *tempArray = [[NSMutableArray alloc] initWithCapacity:0];
    for (it = tags->begin(); it != tags->end(); ++it) {
        NSString *str = [NSString stringWithCString:(*it).c_str()
                                           encoding:NSUTF8StringEncoding];
        [tempArray addObject:str];
    }
    SAFE_ARC_AUTORELEASE(tempArray);
    return tempArray;
}

void NSArrayToList(NSArray *tags_source, Cocos::Tags &tags_target)
{
    if ([tags_source count]) {
        for (NSString *tagStr in tags_source) {
            tags_target.push_back([tagStr UTF8String]);
        }
    }
}

#pragma mark - Object-C

@interface CCPushCallBackController : NSObject <CCPushCallBackDelegate>

@property (nonatomic, assign) Cocos::PushListener *callbackListener;

+ (CCPushCallBackController *)shareInstance;

@end


@implementation CCPushCallBackController

static CCPushCallBackController *OCCallBackController = nil;

+ (CCPushCallBackController *)shareInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        OCCallBackController = [[CCPushCallBackController alloc] init];
    });
    
    return OCCallBackController;
}

- (void)onMethod:(CocosPushMethod)methodType response:(NSDictionary*)aResponse;
{
   
    if (self.callbackListener == NULL) {
        return;
    }
    
    int retCode = [[aResponse objectForKey:PushRequestRetCodeKey] integerValue];
    
    switch (methodType)
    {
        case PushRequestMethod_RegDeviceToken:
        {
            self.callbackListener->onRegistDeviceToken(retCode);
        }
            break;
        case PushRequestMethod_SetAccount:
        {
            char *account = NULL;
            NSDictionary *failedRet = [aResponse objectForKey:PushRequestFailedParmaKey];
            if (failedRet) {
                NSString *accountStr = [failedRet objectForKey:PushRequestSetAccountKey];
                account = (char *)[accountStr UTF8String];
            }
            
            std::string str;
            if (account) {
                str = account;
            }

            self.callbackListener->onSetAccount(retCode, str);
        }
            break;
        case PushRequestMethod_SetTag:
        {
            Cocos::Tags failedList;
            NSDictionary *failedRet = [aResponse objectForKey:PushRequestFailedParmaKey];
            if (failedRet) {
                NSArray *setTags = [failedRet objectForKey:PushRequsetSetTagsKey];
                NSArrayToList(setTags, failedList);
            }
            self.callbackListener->onSetTags(retCode, failedList);
        }
            break;
        case PushRequestMethod_DelTag:
        {
            Cocos::Tags failedList;
            NSDictionary *failedRet = [aResponse objectForKey:PushRequestFailedParmaKey];
            if (failedRet) {
                NSArray *setTags = [failedRet objectForKey:PushRequestDeleteTagsKey];
                NSArrayToList(setTags, failedList);
            }
            self.callbackListener->onDelTags(retCode, failedList);
        }
            break;
        case PushRequestMethod_SwitchState:
        {
            self.callbackListener->onSetPushSwitchState(retCode);
        }
            break;
            
        case PushRequestMethod_SendImMsg:
        {
            char * retmsg = (char *)[[aResponse objectForKey:PushRequestRetMsgKey] UTF8String];
            self.callbackListener->onSendImMsg(retCode,retmsg);
            
        }
            break;
            
        default:
            break;
    }
}

@end


#pragma mark - C++

namespace Cocos
{
    void PushService::setDebugMode(bool debug)
    {
        [CCPush setDebugMode:debug];
    }
    
    void PushService::setDevPlatForm(bool isDev){
        [CCPush setDevPlatForm:isDev];
    }
    
    void PushService::registCallBack(Cocos::PushListener *listener)
    {
        if (listener != NULL) {
            [CCPushCallBackController shareInstance].callbackListener = listener;
            
            [CCPush setDelegate:[CCPushCallBackController shareInstance]];
        }
    }
    
    std::string PushService::getSDKVersion(void)
    {
        NSString *verStr = [CCPush SDKVersion];
        return [verStr UTF8String];
    }
    
    void PushService::initSDK(void *launchOptions)
    {
        NSDictionary *lanchInfo = C_BRIDGE_OC(launchOptions);
        
        [CCPush initPush:lanchInfo];
        
    }
    
    void PushService::initSDK(const std::string& appId, const std::string& channelId, void *launchOptions)
    {
        NSString *appidStr= [NSString stringWithUTF8String:appId.c_str()];
        NSString *channedIdStr = [NSString stringWithUTF8String:channelId.c_str()];
        
        [CCPush initPushWithAppId:appidStr channelId:channedIdStr launchOptions:C_BRIDGE_OC(launchOptions)];
        

    }
    
    void PushService::registDeviceToken(void *deviceToken)
    {
        [CCPush registDeviceToken:C_BRIDGE_OC(deviceToken)];
    }
    
    void PushService::sendImMsg(const std::string &sendAccount, const std::string &receiverAccount, const std::string &content){
        NSString *sendAccountStr= [NSString stringWithUTF8String:sendAccount.c_str()];
        NSString *receiverAccountStr = [NSString stringWithUTF8String:receiverAccount.c_str()];
        NSString *contentStr = [NSString stringWithUTF8String:content.c_str()];
        
        [CCPush sendImMsg:sendAccountStr receiver:receiverAccountStr msgContent:contentStr];
    }
    
    
    
    void PushService::handleRemoteNotification(void *userInfo)
    {
        NSDictionary *dic = C_BRIDGE_OC(userInfo);
        [CCPush handleNotification:dic];
        
        NSData *JsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
        NSString *JsonString = [[NSString alloc] initWithData:JsonData encoding:NSUTF8StringEncoding];
        SAFE_ARC_AUTORELEASE(JsonString);
        
        if ([CCPushCallBackController shareInstance].callbackListener) {
            [CCPushCallBackController shareInstance].callbackListener->onMessage([JsonString UTF8String]);
        }
    }
    
    void PushService::handleLocalNotification(void *userInfo)
    {
        UILocalNotification *notify = C_BRIDGE_OC(userInfo);
        
        NSString *param = [notify.userInfo objectForKey:CC_NOTIFICATION_P];
        
        if ([CCPushCallBackController shareInstance].callbackListener) {
            [CCPushCallBackController shareInstance].callbackListener->onLocalTimer([param UTF8String]);
        }
    }
    
    void PushService::setPushSwitchState(bool on)
    {
        [CCPush setPushSwitchState:on];
    }
    
    void PushService::setTags(const Tags &tags)
    {
        NSArray *temp = listToNSArray(&tags);
        [CCPush setTags:temp];
    }
    
    void PushService::delTags(const Tags &tags)
    {
        NSArray *temp = listToNSArray(&tags);
        [CCPush deleteTags:temp];
    }
    
    void PushService::setAccount(const char *account)
    {
        NSString *accountStr = account==NULL? @"":[NSString stringWithUTF8String:account];
        
        [CCPush setAccount:accountStr];
    }
    
    void* PushService::setLocalTimer(long time, const std::string &customParam, const std::string& notifyText, const std::string& timerId)
    {
        NSDate *fireDate = [NSDate dateWithTimeIntervalSince1970:time];

        NSDictionary *userInfo = @{CC_NOTIFICATION_ID: [NSString stringWithUTF8String:timerId.c_str()],
                                   CC_NOTIFICATION_P:[NSString stringWithUTF8String:customParam.c_str()]};
        
        __block UILocalNotification *notification = nil;
        [CCPush localNotificationWithFireDate:fireDate
                                    alertBody:[NSString stringWithUTF8String:notifyText.c_str()]
                                     userInfo:userInfo
                                       config:^(UILocalNotification *notify) {
                                           notification = notify;
                                       }];
        
        return OC_BRIDGE_C(notification);
    }
    
    /**
     * 通过设置的id值取消本地通知
     */
    void PushService::cancelLocalTimer(const std::string &timerId)
    {
        [CCPush cancelLocalNotificationWithKey:CC_NOTIFICATION_ID value:[NSString stringWithUTF8String:timerId.c_str()]];
    }
    
    /**
     * 通过本地通知实例取消本地通知
     */
    void PushService::cancelLocalTimer(void *timer)
    {
        if (timer != NULL) {
            UILocalNotification *notification = C_BRIDGE_OC(timer);
            [CCPush cancelLocalNotification:notification];
        }
    }
    
    void PushService::cancelAllLocalTimer()
    {
        [CCPush cancelAllLocalNotifications];
    }
    
#pragma mark 统计
    
    void Analytics4PushService::setDebugMode(bool isDebugModel){
        [Analytics4Push setDebugMode:isDebugModel];
    }
    
    void Analytics4PushService::executeAnalytics(const std::string &eventId){
        NSString *eventIdStr = [NSString stringWithUTF8String:eventId.c_str()];
        [Analytics4Push executeAnalytics:eventIdStr];
    }
    
    void Analytics4PushService::executeAnalytics(const std::string &eventId, void *params){
        NSString *eventIdStr = [NSString stringWithUTF8String:eventId.c_str()];
        NSDictionary *eventParams = C_BRIDGE_OC(params);
        [Analytics4Push executeAnalytics:eventIdStr Params:eventParams];
        
    }
}

