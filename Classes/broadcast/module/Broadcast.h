//
//  Broadcast.h
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#ifndef __PinLe__Broadcast__
#define __PinLe__Broadcast__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class Broadcast
{
public:
    enum TYPE
    {
        normal = 0,             //正常
        ui,                     //跳转界面
        link,                   //链接
    };
    Broadcast();
    ~Broadcast();
    string              id;             //广播ID
    string              title;          //标题
    string              content;        //广播内容文字
    string              linkstr;        //type != 0时 链接内容
    string              starttime;      //开始时间
    string              endtime;        //结束时间
    string              lastplay;       //上次播放时间
    Color3B             color3B;        //广播文字颜色
    int                 type;           //0：正常 1：跳转界面 2：链接
    int                 tag;            //跳转类型／界面内跳转根据这个字段来判断要跳到那个界面
    bool                used;           //是否播放过
    bool                isResponse;     //是否通知服务器已收到
    uint64_t            existtime;      //存在持续时间，从开始时间起(单位：秒)
    
    
public:
    bool                isValid();      //当前活动是否有效
    bool                isOnce();       //当前活动是否为1次性的
    
    Broadcast*          copy();
};

#endif /* defined(__PinLe__Broadcast__) */
