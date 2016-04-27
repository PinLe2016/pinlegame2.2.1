//
//  UI_Friend.h
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#ifndef __PinLe__UI_Friend__
#define __PinLe__UI_Friend__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"
#include "../../player/IPlayerData.h"
#include "../../player/config.h"

class UI_Friend : public IUILayer
{
public:
    UI_Friend();
    ~UI_Friend();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Friend);
    
    //    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    
public:
    void        eventCallback(Ref * pSender,Widget::TouchEventType type);
    void        textFieldCallBack(Ref* pSender, TextField::EventType type);
    void        setInviteCode(const string& s);
    void        setFriendsList();
    Node*       rootNode;
    Node*       p_invite;
    Node*       inviteNode;
    ListView*   l_friends;
    ListView*   inviteList;
    
    IPlayerData* _player;
};











#endif /* defined(__PinLe__UI_Friend__) */
