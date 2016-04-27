//
//  UI_Logo.h
//  PinLe
//
//  Created by Kennedy on 15/6/11.
//
//

#ifndef __PinLe__UI_Logo__
#define __PinLe__UI_Logo__


#include "../../framework/config.h"
#include "../../gameini/GameIni.h"

using namespace cocostudio::timeline;

enum VERSIONBACK
{
    no_back = 0,
    error_infor,
    true_infor,
};


class UI_Logo : public IUILayer
{
public:
    UI_Logo();
    ~UI_Logo();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Logo);
    
    //    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    
public:
    IPlayerData*    _player;
    Sprite*         pinle;
    void            actionBack();
    void            checkVersion(float dt);
    void            http_GetGameUrl();
    void            http_backGameUrl(string str);
    void            checkVersionOver(bool b = false);
    void            updateCheckUrlBack(float ft);
    Args            _args;
    int             getgameurltype;
    int             error_time;
    bool            diango;
    
    
////////////引导页
    __Array* yemianDian;
    Sprite* DangQianDian;
    Sprite* temppos;
    void  fanhui(Ref * obj,Widget::TouchEventType type);
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    void touch_GaiBiaPoist(Node * _node);
    void guideLayer();
    
    DrawNode * pEraser ;
    RenderTexture *  pRTex;
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
};



















#endif /* defined(__PinLe__UI_Logo__) */
