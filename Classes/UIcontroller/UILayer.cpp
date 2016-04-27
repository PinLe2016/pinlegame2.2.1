//
//  IUILayer.cpp
//  framework
//
//  Created by Kennedy on 15/5/26.
//
//

#include "IUILayer.h"

//#include "../config.h"
#include "../UIcontroller/UIcontroller.h"
#include "../gameini/GameIni.h"

static bool is_switch=false;
IUILayer::IUILayer()
{
//    GAMELOG("-------IUILayer::IUILayer---------");
    network = NULL;
    guideLayer = nullptr;
    setKeyboardEnabled(YES);
    setTouchEnabled(YES);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    isGuide = NO;
}

IUILayer::~IUILayer()
{
    closeNetConnectingView();   
    
//    GAMELOG("-------IUILayer::~IUILayer---------");
}


bool IUILayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    return true;
}

void IUILayer::attachNetworkMes()
{
    IMessageManager::getInstance()->attach(this, NETWORK_SENDING);
    IMessageManager::getInstance()->attach(this, NETWORK_TIMEOUT);
    IMessageManager::getInstance()->attach(this, NETWORK_CALLBACK);
    IMessageManager::getInstance()->attach(this, NETWORK_ERROR_RESPONSE);
//    IMessageManager::getInstance()->attach(this, NETWORK_ERROR_JSON);
    IMessageManager::getInstance()->attach(this, NETWORK_ERROR_SERVICE);
    
    
    IMessageManager::getInstance()->attach(this, MESSAGEBOX_ERROR);
    IMessageManager::getInstance()->attach(this, MESSAGEBOX_OK);
    IMessageManager::getInstance()->attach(this, MESSAGEBOX_CANCEL);
    
    IMessageManager::getInstance()->attach(this, SYSTEM_ENTERBACKGROUND);
    IMessageManager::getInstance()->attach(this, SYSTEM_ENTERFOREGROUND);
    IMessageManager::getInstance()->attach(this, SYSTEM_KEYBACK);

}

void IUILayer::detachNetworkMes()
{
    IMessageManager::getInstance()->detach(this, NETWORK_SENDING);
    IMessageManager::getInstance()->detach(this, NETWORK_TIMEOUT);
    IMessageManager::getInstance()->detach(this, NETWORK_CALLBACK);
    IMessageManager::getInstance()->detach(this, NETWORK_ERROR_RESPONSE);
//    IMessageManager::getInstance()->detach(this, NETWORK_ERROR_JSON);
    IMessageManager::getInstance()->detach(this, NETWORK_ERROR_SERVICE);
    
    IMessageManager::getInstance()->detach(this, MESSAGEBOX_ERROR);
    IMessageManager::getInstance()->detach(this, MESSAGEBOX_OK);
    IMessageManager::getInstance()->detach(this, MESSAGEBOX_CANCEL);
    
    IMessageManager::getInstance()->detach(this, SYSTEM_ENTERBACKGROUND);
    IMessageManager::getInstance()->detach(this, SYSTEM_ENTERFOREGROUND);
    IMessageManager::getInstance()->detach(this, SYSTEM_KEYBACK);
}


bool   IUILayer::getis_switch(){
    return is_switch;
}
void IUILayer::popNetConnectingView()
{
    if(! network )
    {
        string filepath = "CSres/public/loading.csb";
        network = Msg_Layer::createEx(Color4B(0, 0, 0, 0));
        Node* node = CSLoader::createNode(filepath);
        auto loadAction = CSLoader::createTimeline(filepath);
        loadAction->gotoFrameAndPlay(0, true);
        node->runAction(loadAction);
        network->_widgetLayer->addChild(node);
        is_switch=true;
    }
}

void IUILayer::playerUpgrade()
{
    UIMessage::createMesBox("等级提升");
}


void IUILayer::closeNetConnectingView()
{
    if(network)
    {
        network->close();
        network = NULL;
    }
}


ClippingNode*   IUILayer::openGuideLayer(float radius, const Vec2& position, float delay)
{
    auto clip = ClippingNode::create();
    clip->setInverted(true);
    clip->setAlphaThreshold(1.0f);
    addChild(clip, 500);
    
    auto back = LayerColor::create(Color4B(0, 0, 0, 255*0.8));
    clip->addChild(back);
    
    auto front = DrawNode::create();
    
    //    front->drawCircle(Vec2::ZERO, 80, CC_DEGREES_TO_RADIANS(90), 50, false, Color4F::RED);
    front->drawSolidCircle(Vec2::ZERO, radius, CC_DEGREES_TO_RADIANS(90), 50, Color4F::WHITE);
    
    front->setPosition(position);
    float scale = 1.2;
//    front->runAction(Sequence::create(ScaleTo::create(0.8, 1.5), ScaleTo::create(0.8, 1), NULL));
    front->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, scale), ScaleTo::create(0.8, 1), NULL)));
    clip->setStencil(front);
    
    
    
    
    if(delay > 0)
    {
        clip->setVisible(NO);
        clip->runAction(Sequence::create(DelayTime::create(delay), Show::create(), NULL));
    }

    guideLayer = clip;
    isGuide = YES;
    return clip;
}

void IUILayer::closeGuideLayer()
{
    guideLayer->removeFromParentAndCleanup(YES);
    guideLayer = nullptr;
    isGuide = NO;
}



bool IUILayer::isVauled(TextField* tf)
{
    if(!tf || tf->getString().empty())
        return false;
    
    return (tf->getPlaceHolder().compare(tf->getString()) != 0);
}


bool IUILayer::Notify(const string& name, Args& args)
{
    Notify_Compare(NETWORK_SENDING)
    {
        popNetConnectingView();
        return true;
    }
    Notify_Compare(NETWORK_CALLBACK)    
    {
        closeNetConnectingView();
        return true;
    }
    Notify_Compare(NETWORK_TIMEOUT)
    {
        UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_timeout), args, 1);
        return true;
    }
    Notify_Compare(NETWORK_ERROR_RESPONSE)
    {
        UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_net), args, 1);
        return true;
    }
//    Notify_Compare(MES_PLAYER_UPGRADE)
//    {
////        UIMessage::createMesBox("等级提升");
//        return true;
//    }
    
//    Notify_Compare(NETWORK_ERROR_JSON)
//    {
//        UIMessage::createMesBox("服务端返回json解析错误");
//        return true;
//    }
    
    return false;
}

void IUILayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode != EventKeyboard::KeyCode::KEY_BACK)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    IMessageManager::getInstance()->postNotification(SYSTEM_KEYBACK);
}







//==========================================================================================



