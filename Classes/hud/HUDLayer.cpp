//
//  HUDLayer.cpp
//  PinLe
//
//  Created by 李辛 on 15/10/22.
//
//

#include "HUDLayer.h"


HUDLayer::HUDLayer()
{
    IMessageManager::getInstance()->attach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
    canMove = false;
    vSum = Vec2(0,0);
}
HUDLayer::~HUDLayer()
{
    IMessageManager::getInstance()->detach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
}


bool HUDLayer::init()
{
    

    Node* node = CSLoader::createNode("res/CSres/public/HUDlayer.csb");
    addChild(node);
    
    
    Button* btn = (Button*)node->getChildByTag(782);
    btn->addTouchEventListener(CC_CALLBACK_2(HUDLayer::eventTouch, this));
    
    
    setSwallowsTouches(false);
    setTouchEnabled(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    log("HUDLayer::init");
    
    return true;
}

void HUDLayer::eventTouch(Ref* pSender, Widget::TouchEventType type)
{
    
    Button* bbb = (Button*)pSender;
    
    if(type == Widget::TouchEventType::BEGAN)
    {
        Vec2 vStart = bbb->getTouchBeganPosition();
        vStartPosition = bbb->getPosition();
        vSum = vStartPosition - vStart;
        log("HUDLayer::eventTouch BEGAN");
    }
    
    if(type == Widget::TouchEventType::MOVED)
    {
        bbb->setPosition(bbb->getTouchMovePosition() + vSum);
        Vec2 vPosition = bbb->getPosition();
        if(fabsf((vPosition-vStartPosition).x) > 5 || fabsf((vPosition-vStartPosition).y) > 5)
        {
            canMove = true;
        }
    }
    
    if(type == Widget::TouchEventType::ENDED)
    {
        if(canMove)
        {
            
        }
        else
        {
            GameIni::getInstance()->screenShoot();      //截屏发送分享流程    不可删除 IMAGE_SCREENSHOOT_FILENAME
            setSwallowsTouches(true);
        }
        canMove = false;
        vSum = Vec2(0,0);
        
    }
}



bool HUDLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
void HUDLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}
void HUDLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    setSwallowsTouches(false);
}



bool HUDLayer::Notify(const string& name, Args& args)
{
    if(name.empty())
    {
        return true;
    }
    if(name.compare(MES_GAMEINI_SCREENSHOOT_SUCCEED) == 0)
    {
        ShareSDKHelper::getInstance()->sendImage(GameIni::getInstance()->shareSdkInfo.title,
                                                 GameIni::getInstance()->shareSdkInfo.description,
                                                 args.content);
        return true;
    }
    
    return true;
}