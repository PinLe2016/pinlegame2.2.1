//
//  HUDLayer.h
//  PinLe
//
//  Created by 李辛 on 15/10/22.
//
//

#ifndef __PinLe__HUDLayer__
#define __PinLe__HUDLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "../framework/config.h"
#include "../gameini/GameIni.h"
#include "../ThirdParty/C2DXShareSDK/ShareSDKHelper.h"

using namespace cocos2d;
using namespace cocos2d::ui;
class HUDLayer : public Layer, public IDataObserver
{
public:
    HUDLayer();
    ~HUDLayer();
    CREATE_FUNC(HUDLayer);
    virtual bool init();
    void eventTouch(Ref* pSender, Widget::TouchEventType type);
    virtual bool Notify(const string& name, Args& args);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
private:
    Sprite* sprite;
    bool canMove;
    Vec2 vSum;
    Vec2 vStartPosition;
};

#endif /* defined(__PinLe__HUDLayer__) */




