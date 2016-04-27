//
//  CCPushService.h
//  CocosPush
//
//  Created by Ray on 14-11-10.
//  Copyright (c) 2014ƒÍ CocoaChina. All rights reserved.
//  SDK Version 1.1.0

#ifndef __CC_PUSH_SERVICE__
#define __CC_PUSH_SERVICE__

#include <cocos2d.h>
#include <string>
#include <list>
#include <map>

namespace Cocos
{
	typedef std::list<std::string>	Tags;
	class PushListener;

	class PushService
	{
	public:

		/************************************************************************/
		/* 接口                                                                  */
		/************************************************************************/

		/**
		 * 设置调试模式
		 */
		static void setDebugMode(bool debug);

		/**
		 * 获取SDK版本号
		 */
		static std::string getSDKVersion(void);

		/**
		 * 开启/关闭推送服务
		 */
		static void setPushSwitchState(bool on);

		/**
		 * 设置/清除标签
		 */
		static void setTags(const Tags& tags);
		static void delTags(const Tags& tags);
        static void setStringTags(const char* str);

		/**
		 * 设置/清除帐号
		 */
		static void setAccount(const char* account);
        


		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		/** Android API **/
        
        /**
         * 创建本地通知
         */
        static int setLocalTimer(long time, const std::string& customParam, const std::string& notifyTitle, const std::string& notifyText);
        
        /**
         * 清除本地通知
         */
        static int cancelLocalTimer(long time, const std::string& customParam, const std::string& notifyTitle, const std::string& notifyText);
		
		/**
		 * 设置/清除本地通知
		 */
		static int cancelAllLocalTimer();

		/**
		 * 设置/清除静默时间
         * @param startHour 静音时段的开始时间 - 小时 （24小时制，范围：0~23 ）
         * @param startMinute 静音时段的开始时间 - 分钟（范围：0~59 ）
         * @param endHour 静音时段的结束时间 - 小时 （24小时制，范围：0~23 ）
         * @param endMinute 静音时段的结束时间 - 分钟（范围：0~59 ）
         * @return retCode 0表示设置成功；非0表示设置失败。
		 */
		static int setSilentTime(int startHour, int startMinute, int endHour, int endMinute);
		static int delSilentTime();

		/**
		 * 发送IM消息
		 * @param sendAccount		发送者账号
		 * @param receiverAccount	接受者账号
		 * @param content			文本内容
		 * @param msgId				消息标识
		 */
		static int sendImMessage(const std::string& senderAccount, const std::string& receiverAccount, const std::string& content, const std::string& msgId);
		#endif

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		/** iOS API **/
        
        /**
         *  初始化
         */
        static void initSDK(void *launchOptions);
        
        /**
         *  初始化,传入app id和渠道号
         */
        static void initSDK(const std::string& appId,
                            const std::string& channelId,
                            void *launchOptions);

        /**
         * 注册device token
         */
        static void registDeviceToken(void *deviceToken);
        
        /**
         * 处理收到的远程通知
         */
        static void handleRemoteNotification(void *userInfo);
        
        /**
         * 处理收到的本地通知
         */
        static void handleLocalNotification(void *userInfo);
        
        /**
         * 创建本地通知（iOS创建方法）
         */
        static void* setLocalTimer(long time,
                                   const std::string& customParam,
                                   const std::string& notifyText,
                                   const std::string& timerId);

        /**
         * 通过设置的id值取消本地通知
         */
        static void cancelLocalTimer(const std::string& timerId);

        /**
         * 通过本地通知实例取消本地通知
         */
        static void cancelLocalTimer(void *timer);

		/**
		 * 设置/清除本地通知
		 */
		static void cancelAllLocalTimer();
        
		#endif
		
		/************************************************************************/
		/* 回调                                                                  */
		/************************************************************************/
		static void registCallBack(PushListener* listener);
	};

	class Analytics4PushService
	{
	public:

		/**
		*  设置调试模式开关
		*
		*  @param isDebugModel 调试模式开关
		*/
		static void setDebugMode(bool isDebugMode);
		
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		/**
		*  统计
		*
		*  @param eventId 统计事件的id
		*/
		static void executeAnalytics(const std::string& eventId);

		/**
		*  统计
		*
		*  @param eventId 统计事件的id
		*  @param params  操作参数 类型为字典
		*/
		static void executeAnalytics(const std::string& eventId, void *params);
		#endif


		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		/**
		*  执行统计：携带自定义参数
		*
		*  @param eventId 自定义事件的id
		*/
		static void executeAnalytics(char* eventId);
		/**
		*  执行统计：携带自定义参数
		*/
		static void executeAnalytics(char* eventId, std::map<char*, char*> params);
		#endif
		
	};

	class PushListener
	{
	public:

		/**
		 * 设置标签
		 */
		virtual void onSetTags(int retCode, const Tags& failTags){};
		virtual void onDelTags(int retCode, const Tags& failTags){};

		/**
		 * 开启推送服务
		 */
		virtual void onSetPushSwitchState(int retCode){};

		/**
		 * 设置帐号
		 */
		virtual void onSetAccount(int retCode, const std::string& account){};

        /**
		 * 本地通知
		 */
		virtual void onLocalTimer(const std::string& param){};
        
        /**
		 * 透传消息
		 */
		virtual void onMessage(const std::string& customContent) = 0;
		
		
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		/** iOS API **/
        
        /**
         * 注册device token
         */
        virtual void onRegistDeviceToken(int retCode){};
        
        #endif
        
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		/** Android Api **/
        /**
		 * 接收到IM消息
		 */
		virtual void onReceiveImMessage(const std::string& sender, const std::string& content){};
		
		/**
		 * 发送IM消息
		 */
		virtual void onSendImMessage(const std::string& msgId, int retCode, const std::string& retMsg){};
		#endif
	};
}

#endif