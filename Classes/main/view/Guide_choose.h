//
//  Guide_choose.h
//  PinLe
//
//  Created by 李辛 on 15/12/1.
//
//

#ifndef __PinLe__Guide_choose__
#define __PinLe__Guide_choose__

#include <stdio.h>
#include "cocos-ext.h"
#include "../../gameini/GameIni.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class GuideChoose : public LayerColor
{
public:
    GuideChoose();
    ~GuideChoose();
    CREATE_FUNC(GuideChoose);
    virtual bool init();
    
    
    void    event_call(Ref* psender, Widget::TouchEventType type);
    void    updateTime(float ft);
    void    startGuide();
private:
    Sprite* guideSprite1;
    Sprite* guideSprite2;
    Sprite* guideSprite3;
    Sprite* guideHand;
    void    initSprite();
    float   vecY;
    float   shouY;
};


#endif /* defined(__PinLe__Guide_choose__) */
