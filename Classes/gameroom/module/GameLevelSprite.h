//
//  GameLevelSprite.h
//  PinLe
//
//  Created by hf on 15-2-14.
//
//

#ifndef __PinLe__GameLevelSprite__
#define __PinLe__GameLevelSprite__
#pragma once
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
#include "../../activity/module/Advertise.h"

#define debirsNum 20    //碎片的数量



class GameLevelSprite : public Advertise
{
public:
    GameLevelSprite();
    ~GameLevelSprite();
    unsigned int TypeIndex;     //游戏模式
    unsigned int CheckpointType;//关卡类型
    string sbireorder;         //图片打乱顺序
    int spireorder[debirsNum];    //碎片的数组
    int addGoldNum;     //完成拼图获得的奖励
};
#endif /* defined(__PinLe__GameLevelSprite__) */
