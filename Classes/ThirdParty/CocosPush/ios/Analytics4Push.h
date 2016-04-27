//
//  Analytics4Push.h
//  CocoPushSDK
//
//  Created by 李发禹 on 14-11-10.
//  Copyright (c) 2014年 gaochao. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Analytics4Push : NSObject
/**
 * 设置是否是调试模式
 * @param isDebugMode
 */
+ (void) setDebugMode:(BOOL)isDebugMode;

/**
 * 执行统计
 * @param eventId 自定义事件ID
 */
+ (void) executeAnalytics:(NSString *)eventId;

/**
 * 执行统计
 * @param eventId 自定义事件ID
 * @param params  用户自定义参数
 */
+ (void) executeAnalytics:(NSString *)eventId Params:(NSDictionary *)params;

@end
