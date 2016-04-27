//
//  CCPush.h
//  CocosPush
//
//  Created by 孙永刚 on 14-4-4.
//  Copyright (c) 2014年 CocoaChina. All rights reserved.
//  SDK Version 1.2.2


#import <Foundation/Foundation.h>

/****状态码说明****/
typedef NS_ENUM(NSInteger, BPushErrorCode) {
    PushErrorCode_Success = 0,
    PushErrorCode_ParameterError = 10004,      // 参数错误
    PushErrorCode_InternalParameter = 10005,      // 服务端内部错误
    PushErrorCode_NetworkInvalible = 10006,       // 网络连接错误
};


/****通知回调****/
/**
  *  返回信息存于NSNotification的userInfo字段，包括内容有：
  *  @"request_type"：值为NSNumber，对应CocosPushMethod类型
  *  @"request_info"：值为NSDictionary，为返回内容
  *  分别对应- (void)onMethod:(CocosPushMethod)methodType response:(NSDictionary*)aResponse;中的methodType与aResponse
  */
extern NSString * const PushRequestNotification;    //请求回调通知

typedef NS_ENUM(NSInteger, CocosPushMethod) {
    PushRequestMethod_RegDeviceToken = 1,
    PushRequestMethod_SetAccount,
    PushRequestMethod_SetTag,
    PushRequestMethod_DelTag,
    PushRequestMethod_SwitchState,
    PushRequestMethod_SendImMsg
};

/****代理回调****/
@protocol CCPushCallBackDelegate <NSObject>

/**
 *  回调
 *
 *  @param methodType 请求方法类型
 *  @param aResponse  返回结果字典,包含: ret_code/ret_msg 等
 *  @return  none
 */
- (void)onMethod:(CocosPushMethod)methodType response:(NSDictionary*)aResponse;

@end


@interface CCPush : NSObject

/**
 *  SDK版本号
 *
 *  @return  版本号字符串
 */
+ (NSString *)SDKVersion;

/**
 *  设置调试模式
 *
 *  @param debug  YES or NO
 *  @return  none
 */
+ (void)setDebugMode:(BOOL)debug;

/**
 *  设置SDK使用环境是否为开发环境,默认为生产环境
 *
 *  @param   isDev YES:开发环境 NO:生产环境
 *  @return  none
 */
+ (void)setDevPlatForm:(BOOL)isDev;

/**
 *  初始化SDK
 *
 *  @param   launchOptions
 *  @return  none
 */
+ (void)initPush:(NSDictionary *)launchOptions;


/**
 *  初始化SDK
 *
 *  @param appId         push平台创建应用时生成的appid
 *  @param channelId     渠道号
 *  @param launchOptions      app启动信息
 *  @return  none
 */
+ (void)initPushWithAppId:(NSString *)appId channelId:(NSString *)channelId launchOptions:(NSDictionary *)launchOptions;


/**
 *   注册device token
 *
 *  @param deviceToken        通过AppDelegate的didRegisterForRemoteNotificationsWithDeviceToken的回调获取
 *  @return  none
 */
+ (void)registDeviceToken:(NSData *)deviceToken;


/**
 *  设置delegate
 *
 *  @param   delegate    设置代理
 *  @return  none
 */
+ (void)setDelegate:(id<CCPushCallBackDelegate>) delegate;

/**
 *  设置推送开关
 *
 *  @param   state  开关状态
 *  @return  none
 */
+ (void)setPushSwitchState:(BOOL)state;

/**
 *  设置标签
 *
 *  @param   tags 标签
 *  @return  none
 */
+ (void)setTags:(NSArray *)tags;

/**
 *  删除标签
 *
 *  @param   tags  标签
 *  @return  none
 */
+ (void)deleteTags:(NSArray *)tags;

/**
 *  设置帐号
 *
 *  @param   account   帐号，可设置为nil
 *  @return  none
 */
+ (void)setAccount:(NSString *)account;

/**
 *  发送im消息
 *
 *  @param sender   发送者
 *  @param receiver    接收者
 *  @param content  消息内容
 *  @return  none
 */
+ (void)sendImMsg:(NSString *)sender receiver:(NSString *)receiver msgContent:(NSString *)content;

/**
 *  在didReceiveRemoteNotification中调用，用于推送反馈
 *
 *  @param  userInfo
 *  @return none
 */
+ (void)handleNotification:(NSDictionary *)userInfo;

/**
 *  设置本地通知
 *
 *  @param aFireDate     通知时间
 *  @param aAlertBody    通知内容
 *  @param aUserInfo     扩展字典
 *  @param aNotification      block块,对生成的UILocalNotification作进一步设置（角标，按钮显示，反复提醒等），如果不需要设置为nil即可
 *  @return  none
 */
+ (void)localNotificationWithFireDate:(NSDate *)aFireDate alertBody:(NSString *)aAlertBody userInfo:(NSDictionary *)aUserInfo config:(void(^)(UILocalNotification *notify))aNotification;

/**
 *  通过userinfo中设置的键值来取消本地通知
 *
 *  @param aKey    需要取消的通知的key
 *  @param aValue    key对应的值
 *  @return  none
 */
+ (void)cancelLocalNotificationWithKey:(NSString *)aKey value:(NSString *)aValue;

/**
 *  取消本地通知
 *
 *  @param notification    需要取消的通知
 *  @return none
 */
+ (void)cancelLocalNotification:(UILocalNotification *)notification;

/**
 *  取消所有本地通知
 *
 *  @return none
 */
+ (void)cancelAllLocalNotifications;

@end


/****返回结果的键****/
#define PushRequestRetCodeKey       @"ret_code"       //返回状态码，NSNumber类型，内容为：BPushErrorCode定义值
#define PushRequestRetMsgKey        @"ret_msg"        //状态信息，值为NSString类型
#define PushRequestFailedParmaKey   @"failed"         //失败内容，值为NSDictionary类型
#define PushRequestSwitchStateKey   @"switch_state"   //push服务打开和关闭，值为NSString类型：on打开，off关闭
#define PushRequestSetAccountKey    @"set_alias"      //设置的帐号，值为NSString类型
#define PushRequsetSetTagsKey       @"set_tags"        //设置的标签，值为NSArray类型
#define PushRequestDeleteTagsKey    @"del_tags"       //删除标签，值为NSArray类型


/***********************推送消息数据结构说明***********************/
/****官方字段****/
#define PUSH_APS          @"aps"                  //官方字段
#define PUSH_ALERT        @"alert"                //消息内容
#define PUSH_SOUND        @"sound"                //提示音文件名，默认为default
#define PUSH_BADGE        @"badge"                //应用角标
#define PUSH_CA           @"content-available"    //后台运行接收消息

/****自定义字段****/
#define PUSH_PUSH         @"push"                 //收到的消息
#define PUSH_APPMSG       @"appmsg"               //透传消息体
#define PUSH_MSGID        @"msgid"                //消息id
#define PUSH_THIRD        @"third"                //字典，内容为用户自定义键值对

/****im消息字段****/
#define PUSH_IM                 @"im"                 //收到的IM消息
#define PUSH_IM_MSG_CONTENT     @"msg_content"        //消息内容
#define PUSH_IM_MSG_TYPE        @"msg_type"           //消息id
#define PUSH_IM_RECEIVER_TARGET @"receiver_target"    //数组(消息接受者列表)
#define PUSH_IM_RECEIVER_TYPE   @"receiver_type"      //消息接受者类型
#define PUSH_IM_RESERVE         @"reserve"            //消息保留字段
#define PUSH_IM_SENDER          @"sender"             //消息发送者

