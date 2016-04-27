//
//  UI_Option.h
//  PinLe
//
//  Created by Kennedy on 15/6/8.
//
//

#ifndef __PinLe__UI_Option__
#define __PinLe__UI_Option__

#include "../../framework/config.h"
#include "PersonCenterLayerEx.h"

enum businessType
{
    jianyifankui = 0,       //建议反馈
    shangwuhezuo = 1,           //商务合作
};


class UI_Option : public IUILayer
{
public:
    UI_Option();
    ~UI_Option();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Option);
    
//    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    
public:
    void        eventCallback(Ref * pSender,Widget::TouchEventType type);
    void        setMusic(bool bl);
    void        openPersonInfo(bool bOpen);
    void        setHeadImage();
    void        addline(Text* t);
    void        enterPokedex(float dt);
    
    Button*     musicOpenBtn;
    Sprite*     musicOpenSp;
    
//    Button*     musicCloseBtn;
//    Sprite*     musicCloseSp;
    
    Node*       rootNode;
    Node*       company;                // 关于拼乐
    Node*       company_hezuo;          // 关于拼了，合作反馈
    Node*       panel_android;
    Node*       panel_ios;
    IPlayerData* _player;
    
    PersonCenterLayerEx* personCenter;
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    void initAboutPinle();
    int chooseType;         //建议合作类型
    void chooseBusiness(int type);      //选择合作类型
    void sendAdvise(int type);       //提交建议合作
    
private:
    UIMessage* message;
    bool        notBack;
public:
    Button * b4_tijiao;
    
};













#endif /* defined(__PinLe__UI_Option__) */
