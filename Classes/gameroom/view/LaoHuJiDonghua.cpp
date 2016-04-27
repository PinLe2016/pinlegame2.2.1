//
//  LaoHuJiDonghua.cpp
//  PinLe
//
//  Created by 李辛 on 15/11/18.
//
//

#include "LaoHuJiDonghua.h"
LaoHuJiDonghua::LaoHuJiDonghua()
{
    nowSlowDown = false;
    nowSlowDown1 = false;
    startSpeed = 1;
    maxSpeed = 45;
    addSpeedDistance = 6;
    updateDistance = 0.1;
}
LaoHuJiDonghua::~LaoHuJiDonghua()
{
    
}
bool LaoHuJiDonghua::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    
    setSwallowsTouches(false);
 //   setDate("CSres/public/publicUI/number0-9", "item_", 10, Vec2(300, 500));
    
    
//    Button* button = Button::create("CSres/Activities/ActivitiesUI/1activity_back_up.png");
//    button->setPosition(Vec2(500,1000));
//    button->setTag(1101);
//    button->addTouchEventListener(CC_CALLBACK_2(LaoHuJiDonghua::eventCallBack, this));
//    addChild(button);
//    
//    Button* button1 = Button::create("CSres/Activities/ActivitiesUI/1activity_back_up.png");
//    button1->setPosition(Vec2(200,1000));
//    button1->setTag(1102);
//    button1->addTouchEventListener(CC_CALLBACK_2(LaoHuJiDonghua::eventCallBack, this));
//    addChild(button1);
    
    return true;
}



void  LaoHuJiDonghua::setDate(const string& spritFileName, const string& spriteItemName, const int& itemNumber, const Vec2& vec, const string& backSpriteName)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(StringUtils::format("%s.plist", spritFileName.c_str()), StringUtils::format("%s.png", spritFileName.c_str()));
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName(StringUtils::format("%s0.png", spriteItemName.c_str()));
    
    clipnode = ClippingNode::create();
    clipnode->setContentSize(Size(tempSprite->getContentSize().width, tempSprite->getContentSize().height));
    clipnode->setAnchorPoint(Vec2(0.5,0.5));
    clipnode->setPosition(vec);
    addChild(clipnode);
    this->setContentSize(Size(clipnode->getContentSize().width, clipnode->getContentSize().height));
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clipnode->getContentSize().width, 0);
    rectangle[2] = Vec2(clipnode->getContentSize().width, clipnode->getContentSize().height);
    rectangle[3] = Vec2(0, clipnode->getContentSize().height);
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clipnode->setStencil(stencil);
    
    
    if(!backSpriteName.empty())
    {
        Sprite* backsp = Sprite::createWithSpriteFrameName(backSpriteName);
        backsp->setAnchorPoint(Vec2(0.5, 0.5));
        backsp->setPosition(clipnode->getContentSize().width / 2, clipnode->getContentSize().height / 2);
        clipnode->addChild(backsp);
    }
    
    addSpriteData(spriteItemName, itemNumber);
    
}

void LaoHuJiDonghua::addSpriteData(const string& itemSpriteName, const int& itemNumber)
{
    for(int i = 0; i < itemNumber; i++)
    {
        Sprite* sprite = Sprite::createWithSpriteFrameName(StringUtils::format("%s%d.png", itemSpriteName.c_str(), i));
        sprite->setAnchorPoint(Vec2(0.5, 0.5));
        sprite->setTag(i);
        sprite->setPosition(Vec2(clipnode->getContentSize().width / 2, clipnode->getContentSize().height / 2 - (clipnode->getContentSize().height * i)));
        clipnode->addChild(sprite);
        itemVer.push_back(sprite);
    }
    
}



void LaoHuJiDonghua::scheduleUpdate(float ft)
{
//    moveNum++;
//    GAMELOG("##########%d", moveNum);
    
    for(vector<Sprite*>::iterator a = itemVer.begin(); a != itemVer.end(); a++)
    {
        Sprite* lab = (Sprite*)(*a);
//        lab->runAction(MoveBy::create(updateDistance, Vec2(0, startSpeed)));
        lab->setPositionY(lab->getPositionY() + startSpeed);
        
        float y = lab->getPositionY();
        Sprite *la = itemVer.at(itemVer.size() - 1);
        if(y > clipnode->getContentSize().height / 2 + la->getContentSize().height * 2)
        {
            lab->stopAllActions();
            lab->setPositionY(la->getPositionY() - clipnode->getContentSize().height);
            itemVer.erase(a);
            itemVer.push_back(lab);
            
            if(nowSlowDown)
            {
                int num = lab->getTag();
                if(num == stopNumber)
                {
                    nowSlowDown1 = true;
                    slowStepNumber = 0;
                    setSlowStepVer(clipnode->getContentSize().height / 2 - (lab->getPositionY()) - startSpeed, 100);
                    nowSlowDown = false;
                }
            }
        }
    }
    
    if(nowSlowDown1)
    {
        startSpeed = slowStepVer.at(slowStepNumber);
        slowStepNumber++;
        if(slowStepNumber == slowStepVer.size())
        {
            nowSlowDown1 = false;
            unschedule(schedule_selector(LaoHuJiDonghua::scheduleUpdate));
            IMessageManager::getInstance()->postNotification("POS_LIGER");
        }
    }
    else
    {
        if(startSpeed < maxSpeed)
        {
            startSpeed += addSpeedDistance;
        }
        else
        {
            startSpeed = maxSpeed;
        }
    }
}


void LaoHuJiDonghua::startGo()
{
    schedule(schedule_selector(LaoHuJiDonghua::scheduleUpdate));
}

void LaoHuJiDonghua::stopGo(int num)
{
    stopNumber = num;
    nowSlowDown = true;
}


void  LaoHuJiDonghua::setStartSpeed(float ft)
{
    if(ft < maxSpeed)   
    {
        startSpeed = ft;
    }
    else
    {
        return;
    }
}

void LaoHuJiDonghua::setMaxtSpeed(float ft)
{
    if(ft > 0)
        maxSpeed = ft;
}
//通过label测试／／／／／／／／／／／／


void LaoHuJiDonghua::labelText()
{
    Button* button = Button::create("CSres/Activities/ActivitiesUI/1activity_back_up.png");
    button->setPosition(Vec2(500,1000));
    button->setTag(1101);
    button->addTouchEventListener(CC_CALLBACK_2(LaoHuJiDonghua::eventCallBack, this));
    addChild(button);
    
    Button* button1 = Button::create("CSres/Activities/ActivitiesUI/1activity_back_up.png");
    button1->setPosition(Vec2(200,1000));
    button1->setTag(1102);
    button1->addTouchEventListener(CC_CALLBACK_2(LaoHuJiDonghua::eventCallBack, this));
    addChild(button1);
    
    
    clipnode = ClippingNode::create();
    clipnode->setContentSize(Size(108,93));
    clipnode->setAnchorPoint(Vec2(0.5,0.5));
    clipnode->setPosition(Vec2(300,500));
    addChild(clipnode);
    
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clipnode->getContentSize().width, 0);
    rectangle[2] = Vec2(clipnode->getContentSize().width, clipnode->getContentSize().height);
    rectangle[3] = Vec2(0, clipnode->getContentSize().height);
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clipnode->setStencil(stencil);
    
    
    
    
    setNumberVector();
    
    this->schedule(schedule_selector(LaoHuJiDonghua::updateTime), updateDistance);
}

void LaoHuJiDonghua::updateTime(float ft)
{
    
    for(vector<Label*>::iterator a = numberVer.begin(); a != numberVer.end(); a++)
    {
        Label* lab = (Label*)(*a);
        lab->runAction(MoveBy::create(updateDistance, Vec2(0, startSpeed)));
        
        float y = lab->getPositionY();
        Label *la = numberVer.at(numberVer.size() - 1);
        if(y > clipnode->getContentSize().height / 2 + la->getContentSize().height * 2)
        {
            lab->stopAllActions();
            lab->setPositionY(la->getPositionY() - la->getContentSize().height);
            numberVer.erase(a);
            numberVer.push_back(lab);
            
            if(nowSlowDown)
            {
                string str = lab->getString();
                int num = atoi(str.c_str());
                if(num == stopNumber)
                {
                    nowSlowDown1 = true;
                    slowStepNumber = 0;
                    setSlowStepVer(clipnode->getContentSize().height / 2 - (lab->getPositionY()) - startSpeed, 30);
                    nowSlowDown = false;
                }
            }
        }
    }

    if(nowSlowDown1)
    {
        startSpeed = slowStepVer.at(slowStepNumber);
        slowStepNumber++;
        if(slowStepNumber == slowStepVer.size())
        {
            nowSlowDown1 = false;
            unschedule(schedule_selector(LaoHuJiDonghua::updateTime));
        }
    }
    else
    {
        if(startSpeed < maxSpeed)
        {
            startSpeed += addSpeedDistance;
        }
    }    
}

void LaoHuJiDonghua::setNumberVector()
{
    float fl;
    for(int i = 0; i < 10; i++)
    {
        Label* label = Label::create();
        label->setString(StringUtils::format("%d", i));
        label->setSystemFontSize(50);
        label->setAnchorPoint(Vec2(0.5,0.5));
        fl = label->getContentSize().height;
        label->setPosition(Vec2(clipnode->getContentSize().width / 2, clipnode->getContentSize().height / 2 - (label->getContentSize().height * i)));
        clipnode->addChild(label);
        numberVer.push_back(label);
    }
}

void LaoHuJiDonghua::setSlowStepVer(float num, int cont)
{
    slowStepVer.clear();
    float fa;
    float numDistance = num / (float)cont;
    for(int i = cont; i > 0; i--)
    {
        int  tempNumber = i;
        fa = numDistance * ((2 / (float)(cont + 1)) * (float)tempNumber);
//        GAMELOG("i = %d, fa = %f", i , fa);
        slowStepVer.push_back(fa);
        num -= fa;
    }
    
    GAMELOG("slowStepVer.size = %ld", slowStepVer.size());
    if(num > 0)
    {
        slowStepVer.push_back(num);
    }
}


void LaoHuJiDonghua::eventCallBack(Ref *ref, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Widget* widget = (Widget*)ref;
    int tag = widget->getTag();
    switch(tag)
    {
        case 1101:
        {
            srand((unsigned)time(NULL));
            stopNumber = rand() % 10;
            GAMELOG("stopNumber = %d", stopNumber);
            stopGo(stopNumber);
            
//            nowSlowDown = true;
        }break;
        case 1102:
        {
//            this->schedule(schedule_selector(LaoHuJiDonghua::updateTime), updateDistance);
            startGo();
            startSpeed = 0;
        }break;
        default:
            break;
    }
}

