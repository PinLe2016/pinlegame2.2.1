//
//  IUILayer.h
//  framework
//
//  Created by Kennedy on 15/5/26.
//
//

#ifndef __framework__IUILayer__
#define __framework__IUILayer__

#include "cocos2d.h"
#include "../framework/config.h"

using namespace cocos2d;

#define Notify_Compare(str)  if(name.compare(str) == 0)

class IUILayer : public LayerColor, public IDataObserver
{
public:
    IUILayer();
    ~IUILayer();
    virtual bool init();
    
    virtual bool Notify(const string& name, Args& args);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    bool            isVauled(TextField* tf);
    
    void            attachNetworkMes();
    void            detachNetworkMes();
    
    void            popNetConnectingView();
    bool            getis_switch();
    void            closeNetConnectingView();
    
    void            playerUpgrade();
    ClippingNode*   openGuideLayer(float radius, const Vec2 &position, float delay = 0.f);
    void            closeGuideLayer();
    
public:
    bool    isGuide;
    ClippingNode*   guideLayer;
private:
    Msg_Layer*  network;
};








#endif /* defined(__framework__IUILayer__) */
