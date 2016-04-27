//
//  TurntableSystem.cpp
//  PinLe
//
//  Created by 李辛 on 15/11/4.
//
//

#include "TurntableSystem.h"


TurntableSystem::TurntableSystem()
{
    nowModuleNum = 0;
    dianjicishu = 0;
    angleVectorNumber = 0;
    quanshunamber = 0;
    startAction = false;
    
}
TurntableSystem::~TurntableSystem()
{
    
}
bool TurntableSystem::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    
    LayerColor* ly = LayerColor::create(Color4B(255,100,255,255));
    addChild(ly);
    
    size = Director::getInstance()->getWinSize();
    
    Button* btn = Button::create("CSres/public/publicUI/button1_up.png");
    btn->setTag(2001);
    btn->setPosition(Vec2(size.width/2, size.height-200));
    btn->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::_eventTouch, this));
    addChild(btn);
    
    
    zhizhenSp = Sprite::create("CSres/public/publicUI/province.png");
    zhizhenSp->setAnchorPoint(Vec2(0, 0.5));
    zhizhenSp->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(zhizhenSp);

    return true;
}

void TurntableSystem::startRotate()
{
/////////////void TurntableSystem::performAction()///////////
//    updateTimeNumber = 0;
//    performAction();
//    setRotateAngleNumberVector((3 - nowModuleNum) * (360 / moduleNum) + addangle);
    
///////////void TurntableSystem::performAction1()//////////
    if(dianjicishu <= 9)
    {
        dianjicishu = dianjicishu * 2 + 1;
    }
    
    else
        dianjicishu++;
    
    if(!startAction)
    {
        performAction1();
        startAction = true;
    }
//
}


void TurntableSystem::performAction1()
{
    Sequence *seq = NULL;
    
    if(quanshunamber < dianjicishu)
    {
        if(quanshunamber < 5)
            quanshunamber++;
    }
    else
    {
        quanshunamber--;
    }
    
    
    if(dianjicishu != 0)
    {
        float ag1;
        ag1 = 180 + 180 * quanshunamber;
        GAMELOG("quanshunamber=%d", quanshunamber);
        angleFloat += ag1;
        
        seq = Sequence::create(
                               RotateBy::create(1, ag1),
                               CallFunc::create(CC_CALLBACK_0(TurntableSystem::performAction1, this)),
                               NULL);
        dianjicishu--;
        if(dianjicishu == 0)
        {
            setRotateAngleNumberVector((3 - nowModuleNum) * (360 / moduleNum) + (360 - (int)angleFloat % 360));
            
        }
    }
    else if(angleVectorNumber < angleVector.size())
    {
        seq = Sequence::create(
                               RotateBy::create(1, angleVector.at(angleVectorNumber)),
                               CallFunc::create([=](void){angleVectorNumber++;}),
                               CallFunc::create(CC_CALLBACK_0(TurntableSystem::performAction1, this)),
                               NULL);
        angleFloat += angleVector.at(angleVectorNumber);
    }
    
    if(seq)
    {
        zhizhenSp->runAction(seq);
    }
    else
    {
        zhizhenSp->stopAllActions();
        nowModuleNum = 3;
        startAction = false;
        angleVectorNumber = 0;
        angleFloat = 0;
        quanshunamber = 0;
    }
    
}

void TurntableSystem::performAction()
{
    Sequence *seq = NULL;
    
    if(updateTimeNumber < rotatenumber)
    {
        float ag1;
        if(updateTimeNumber == 0 || updateTimeNumber == rotatenumber - 1)
        {
            ag1 = 360;
        }
        else
        {
            ag1 = 720;
        }
        
        seq = Sequence::create(
                               RotateBy::create(1, ag1),
                               CallFunc::create(CC_CALLBACK_0(TurntableSystem::performAction, this)),
                               NULL);
    }
    else if(angleVectorNumber < angleVector.size())
    {
        seq = Sequence::create(
                               RotateBy::create(1, angleVector.at(updateTimeNumber - rotatenumber)),
                               CallFunc::create(CC_CALLBACK_0(TurntableSystem::performAction, this)),
                               
                               NULL);
        
    };
    
    updateTimeNumber++;
    if(seq)
    {
        zhizhenSp->runAction(seq);
    }
    else
    {
        zhizhenSp->stopAllActions();
        
        Button* btn = (Button*)this->getChildByTag(2001);
        btn->setEnabled(true);
        nowModuleNum = 3;
    }
    
}


void TurntableSystem::setRotateAngleNumberVector(float angleNumber)
{
    angleVector.clear();
    
    float averageNumber = angleNumber / updateSumTime;
    float angle1;
    GAMELOG("angleNumber = %f", angleNumber);
    
    for(int i = updateSumTime; i > 0; i--)
    {
        int tempNumber = i;
        angle1 = averageNumber * ((2 / (float)(updateSumTime + 1)) * (float)tempNumber);
        angleNumber -= angle1;
        angleVector.push_back(angle1);
    }

    if(angleNumber > 0)
    {
        angleVector.push_back(angleNumber);
    }
    
    GAMELOG("numberSize = %ld", angleVector.size());
    for(int i = 0; i < angleVector.size(); i++)
    {
        GAMELOG("number = %f", angleVector.at(i));
    }
    
}


void TurntableSystem::_eventTouch(Ref *pSender, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
    {
        return;
    }
    Button* widget = (Button*)pSender;
//    widget->setEnabled(false);
    int tag = widget->getTag();
    switch(tag)
    {
        case 2001:
        {
            startRotate();
        }break;
        default:break;
            
    }
}





/**
 
 * 积分抽奖算法
 *
 * @param list
 * @return

private int getGameWinningLevel(List<giftDetailModel> list) {
    
    // 中奖等级：未中奖
    int winningLevel = -1;
    
    if (list == null || list.size() <= 0) {
        return winningLevel;
    }
    
    // 中奖随机号
    int randomWinningNo = 0;
    int args[] = new int[list.size() * 2];
    int temp = (int) Math.round(Math.random() * 1000000000) % 1000000;
    int j = 0;
    
    for (int i = 0; i < list.size(); i++) {
        
        double tmpWinningPro = list.get(i).getWinningPro();
        
        if (j == 0) {
            args[j] = randomWinningNo;
        } else {
            args[j] = args[j - 1] + 1;
        }
        args[j + 1] = args[j] + (int) Math.round(tmpWinningPro * 10000) - 1;
        
        if (temp >= args[j] && temp <= args[j + 1]) {
            winningLevel = i + 1;
            return winningLevel;
        }
        j += 2;
    }
    return winningLevel;
}

 *****/

