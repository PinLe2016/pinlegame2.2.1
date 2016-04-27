//
//  Guide_choose.cpp
//  PinLe
//
//  Created by 李辛 on 15/12/1.
//
//

#include "Guide_choose.h"
GuideChoose::GuideChoose()
{
    vecY = 600;
    shouY = 350;
}
GuideChoose::~GuideChoose()
{
    
}

bool GuideChoose::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    
    LayerColor* layer = LayerColor::create(Color4B(0,0,0,200));
    addChild(layer);
    
    this->setSwallowsTouches(true);
    this->setTouchEnabled(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    guideSprite1 = Sprite::create("CSres/Main/MainUI/main_jxb.png");
    guideSprite1->setPosition(Vec2(375.5-180, vecY));
    guideSprite1->setLocalZOrder(10);
    addChild(guideSprite1);
    
    guideSprite2 = Sprite::create("CSres/Main/MainUI/main_go.png");
    guideSprite2->setPosition(Vec2(375.5, vecY));
    addChild(guideSprite2);
    guideSprite2->setLocalZOrder(15);
    
    guideSprite3 = Sprite::create("CSres/Main/MainUI/main_plb.png");
    guideSprite3->setPosition(Vec2(375.5+180, vecY));
    addChild(guideSprite3);
    guideSprite3->setLocalZOrder(13);
    
    guideHand = Sprite::create("CSres/Main/MainUI/shou.png");
    guideHand->setPosition(Vec2(375.5+180, shouY));
    addChild(guideHand);
    guideHand->setLocalZOrder(20);
    
    
    Button* button = Button::create("CSres/public/publicUI/public_X_up.png");
    button->setTag(1001);
    button->addTouchEventListener(CC_CALLBACK_2(GuideChoose::event_call, this));
    addChild(button);
    button->setPosition(Vec2(680,1250));
    
    
    return true;
}

void GuideChoose::updateTime(float ft)
{
    guideSprite1->setPosition(guideSprite1->getPosition() + Vec2(10,0));
    guideSprite2->setPosition(guideSprite2->getPosition() + Vec2(-7, 0));
    guideSprite3->setPosition(guideSprite3->getPosition() + Vec2(-5,0));
    if(guideSprite3->getPositionX() < (375.5+90))
    {
        guideSprite3->setLocalZOrder(15);
        guideSprite2->setLocalZOrder(13);
    }
    
    if(guideSprite3->getPositionX() < 375.5)
    {
        this->unschedule(schedule_selector(GuideChoose::updateTime));
        initSprite();
    }
    
    
}

void GuideChoose::startGuide()
{
    schedule(schedule_selector(GuideChoose::updateTime));
    
    guideHand->runAction(Spawn::create(
                   Sequence::create(ScaleTo::create(1.2,0.5),ScaleTo::create(0.8,0.5), NULL),
                   Sequence::create(MoveTo::create(2, Vec2(375.5-180, shouY)),
                                    NULL),
                                       NULL));
}

void GuideChoose::initSprite()
{
    guideHand->setPosition(Vec2(375.5+180, shouY));
    guideSprite1->setPosition(Vec2(375.5-180, vecY));
    guideSprite2->setPosition(Vec2(375.5, vecY));
    guideSprite3->setPosition(Vec2(375.5+180, vecY));
    guideSprite2->setLocalZOrder(15);
    guideSprite3->setLocalZOrder(13);
    
    this->runAction(Sequence::create(
                                     DelayTime::create(1),
                                     CallFunc::create([=](){startGuide();}),
                                     NULL));
}

void GuideChoose::event_call(Ref* psender, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
    {
        return;
    }
    
    Widget* widget = (Widget*)psender;
    int tag = widget->getTag();
    switch(tag)
    {
        case 1001:
        {
            IMessageManager::getInstance()->postNotification(GUIDE_MAIN_CHOOSE_GAME);
        }break;
        case 1002:
        {
            
        }break;
        default:
            break;
    }
}