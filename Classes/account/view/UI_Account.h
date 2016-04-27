//
//  UI_Account.h
//  PinLe
//
//  Created by Kennedy on 15/7/27.
//
//

#ifndef __PinLe__UI_Account__
#define __PinLe__UI_Account__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"
using namespace cocos2d::experimental::ui;

class UI_Account : public IUILayer
{
public:
    UI_Account();
    ~UI_Account();
    WebView *    _webview;
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Account);
    
    //    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    
    virtual void onEnter();
//    virtual void onExit();
    
public:
    void        eventCallback(Ref * pSender, Widget::TouchEventType type);
    Node*       rootNode;
    IPlayerData* _player;
    
    ui::ScrollView* sv_grade;
};

#endif /* defined(__PinLe__UI_Account__) */
