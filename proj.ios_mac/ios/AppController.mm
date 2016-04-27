/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"

#import <ShareSDK/ShareSDK.h>
#import <TencentOpenAPI/QQApiInterface.h>
#import <TencentOpenAPI/TencentOAuth.h>

#import "../../Classes/ThirdParty/CocosPush/CocosPush.h"
#import "../../Classes/ThirdParty/CocosPush/ios/CCPush.h"
#import "../../Classes/ThirdParty/C2DXShareSDK/ios/ShareSDK/Extend/WeChatSDK/WXApi.h"
#import "../../Classes/ThirdParty/C2DXShareSDK/ios/ShareSDK/Extend/QQConnectSDK/TencentOpenAPI.framework/Headers/QQApiInterface.h"

#import "../../Classes/gameini/GameIni.h"

#import "UMessage.h"
@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

#define UMSYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define _IPHONE80_ 80000


// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    [_viewController addSkipBackupAttributeToPath:docDir];
    
    
    // Set RootViewController to window
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        if ([[UIDevice currentDevice].systemVersion floatValue] > 4.0)
        {
            [window setRootViewController:_viewController];
        }
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //=============================【CocosPush】===================================================
#ifdef GAME_TEST
    //设置log开关
    Cocos::PushService::setDebugMode(true);
    //设置是否为开发环境
    Cocos::PushService::setDevPlatForm(true);
#endif
    //初始化推送SDK
    Cocos::PushService::initSDK(CocosPush_AppID, CocosPush_ChannelId, launchOptions);
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        UIUserNotificationType types =
        UIUserNotificationTypeBadge|
        UIUserNotificationTypeSound|
        UIUserNotificationTypeAlert;
        UIUserNotificationSettings *settings = [UIUserNotificationSettings
                                                settingsForTypes:types categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    }
    else
    {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
         UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge| UIRemoteNotificationTypeSound];
    }
    //=============================【ShareSDK】===================================================
    //导入微信类型
    [ShareSDK importWeChatClass:[WXApi class]];
    //导入QQ类型
//    [ShareSDK importQQClass:[QQApiInterface class] tencentOAuthCls:[TencentOAuth class]];
    
    //======================================================================================
    //===================友盟推送=============
//    ym_push:launchOptions;
    
    [UMessage startWithAppkey:@"56d56f58e0f55a6d58001a2f" launchOptions:launchOptions];
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= _IPHONE80_
    if(UMSYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0"))
    {
        //register remoteNotification types （iOS 8.0及其以上版本）
        UIMutableUserNotificationAction *action1 = [[UIMutableUserNotificationAction alloc] init];
        action1.identifier = @"action1_identifier";
        action1.title=@"Accept";
        action1.activationMode = UIUserNotificationActivationModeForeground;//当点击的时候启动程序
        
        UIMutableUserNotificationAction *action2 = [[UIMutableUserNotificationAction alloc] init];  //第二按钮
        action2.identifier = @"action2_identifier";
        action2.title=@"Reject";
        action2.activationMode = UIUserNotificationActivationModeBackground;//当点击的时候不启动程序，在后台处理
        action2.authenticationRequired = YES;//需要解锁才能处理，如果action.activationMode = UIUserNotificationActivationModeForeground;则这个属性被忽略；
        action2.destructive = YES;
        
        UIMutableUserNotificationCategory *categorys = [[UIMutableUserNotificationCategory alloc] init];
        categorys.identifier = @"category1";//这组动作的唯一标示
        [categorys setActions:@[action1,action2] forContext:(UIUserNotificationActionContextDefault)];
        
        UIUserNotificationSettings *userSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge|UIUserNotificationTypeSound|UIUserNotificationTypeAlert
                                                                                     categories:[NSSet setWithObject:categorys]];
        [UMessage registerRemoteNotificationAndUserNotificationSettings:userSettings];
        
    } else{
        //register remoteNotification types (iOS 8.0以下)
        [UMessage registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge
         |UIRemoteNotificationTypeSound
         |UIRemoteNotificationTypeAlert];
    }
#else
    
    //register remoteNotification types (iOS 8.0以下)
    [UMessage registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge
     |UIRemoteNotificationTypeSound
     |UIRemoteNotificationTypeAlert];
    
#endif
    //for log
    [UMessage setLogEnabled:YES];
    
    //========================================

    app->run();
    
    return YES;
}


-(string) Bmk_Locations
{
//    return (string)_locService.userLocation.heading;
}

//注册推送功能
- (void)registerForRemoteNOtification
{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        UIUserNotificationType types =
        UIUserNotificationTypeBadge|
        UIUserNotificationTypeSound|
        UIUserNotificationTypeAlert;
        //配置交互式通知内容，只有iOS8系统以上可用
        //        NSSet *categotiesSet = [NSSet setWithObject:[self createNotificationCategory]];
        //此方法第二个参数即为交互推送的内容，如果不使用设置为nil即可
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:types
                                                                                 categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    } else {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
         UIRemoteNotificationTypeAlert|
         UIRemoteNotificationTypeBadge|
         UIRemoteNotificationTypeSound];
    }
}


//iOS8需要加入以下方法完成推送功能的注册
- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings {
    [[UIApplication sharedApplication] registerForRemoteNotifications];
}


//成功收到device token的代码，必须调用CCPush的注册registDeviceToken接口来向服务器
//提交device token，并确认收到正确的回调（如1.11和1.12中说明），否则在没有上传device
//token的前提下条用设置或删除标签，设置或删除账号会返回10004错误码（参数错误）。
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [UMessage registerDeviceToken:deviceToken];
    Cocos::PushService::registDeviceToken(deviceToken);
    
    NSLog(@"tototootoo %@",[[[[deviceToken description] stringByReplacingOccurrencesOfString: @"<" withString: @""]
                  stringByReplacingOccurrencesOfString: @">" withString: @""]
                 stringByReplacingOccurrencesOfString: @" " withString: @""]);
    
}

//如果向APNS注册通知功能失败会调用以下接口，请查找原因（网络问题，应用在设置里是否打
//开推送功能等），并重新调用1.7中的注册推送部分的代码来重新请求
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"2222222222%@", error);
}

//接收远程通知(<iOS7)
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
    // 处理推送消息
    Cocos::PushService::handleRemoteNotification(userInfo);
    [UMessage didReceiveRemoteNotification:userInfo];
}


//background models下接收远程推送（>= iOS7）
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler
{
    // 处理推送消息
    Cocos::PushService::handleRemoteNotification(userInfo);
    completionHandler(UIBackgroundFetchResultNoData);
}

//接收本地通知并处理
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    Cocos::PushService::handleLocalNotification(notification);
}


//实现CCPushCallBackDelegate协议
//在AppDelegate中实现CCPushCallBackDelegate协议，用于请求（如注册deviceToken、设置标签、别名等）接收回调，代码如下
- (void)onMethod:(CocosPushMethod)methodType response:(NSDictionary*)data;
{
    switch (methodType)
    {
        case PushRequestMethod_RegDeviceToken:
        { }    break;
        case PushRequestMethod_SetAccount:
        { }    break;
        case PushRequestMethod_SetTag:
        { }    break;
        case PushRequestMethod_DelTag:
        { }    break;
        case PushRequestMethod_SwitchState:
        { }    break;
        case PushRequestMethod_SendImMsg:
        { }    break;
        default:
            break;
    }
}

//UINotification的回调方式
//SDK同时提供UINotification的回调方式，可根据情况选择实现CCPushCallBackDelegate协议还是通知方式，通知代码如下
- (void)onMethodWithResponse:(NSNotification*)aNotification;
{
    NSInteger methodType = [((NSNumber*)[aNotification.userInfo objectForKey:@"request_type"]) integerValue];
    NSDictionary *aResponse = [aNotification.userInfo objectForKey:@"request_info"];
    
    switch (methodType)
    {
        case PushRequestMethod_RegDeviceToken:
        { }    break;
        case PushRequestMethod_SetAccount:
        { }    break;
        case PushRequestMethod_SetTag:
        { }    break;
        case PushRequestMethod_DelTag:
        { }    break;
        case PushRequestMethod_SwitchState:
        { }    break;
        case PushRequestMethod_SendImMsg:
        { }    break;
        default:
            break;
    }
}




- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}





//=======================【微信】==============================
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    return [ShareSDK handleOpenURL:url sourceApplication:nil annotation:nil wxDelegate:self];
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    return [ShareSDK handleOpenURL:url sourceApplication:sourceApplication annotation:annotation wxDelegate:self];
}

-(void) onReq:(BaseReq*)req
{
}

-(void) onResp:(BaseResp*)resp
{
    if([resp isKindOfClass:[SendMessageToWXResp class]])
    {
        //        NSString *strTitle = [NSString stringWithFormat:@"发送媒体消息结果"];
        //        NSString *strMsg = [NSString stringWithFormat:@"errcode:%d", resp.errCode];
        //
        //        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        //        [alert show];
        //        [alert release];
        
        //        WeChatHelper::getInstance()->onResp(resp.errCode);
    }
}


//============================================================

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
