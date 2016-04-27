//
//  UI_Login.h
//  PinLe
//
//  Created by Kennedy on 15/6/8.
//
//

#ifndef __PinLe__UI_Login__
#define __PinLe__UI_Login__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"
#include "../../player/IPlayerData.h"
#include "../../player/config.h"



class   UI_Login : public IUILayer//, public TableViewDataSource, public TableViewDelegate
{
public:
    UI_Login();
    ~UI_Login();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Login);
    
//    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    bool        Notify(const string& name, Args& args);
    
    
public:
    bool  is_show;
    void            eventCallback(Ref * pSender,Widget::TouchEventType type);
    void            mesBoxCallback(Ref * pSender,Widget::TouchEventType type);
    
    void            textFieldCallBack(Ref* pSender, TextField::EventType type);
    void            openRegist(bool bOpen);
    void            setVerifyButton(int type);
    void            updateVerify(float dt);
    void            showPassword(bool b);
    void            initXieyiLayer();
    
//    TextField*      TF_phoneNumber;
//    TextField*      TF_passWord;
    
    TextField*      TF_textPhone;
    TextField*      TF_textVerify;
    TextField*      TF_textNewCode;
    TextField*      TF_textNewCode2;
    
    
    Node*           P_Login;
    Node*           P_Psw;
    Node*           P_Invite;
    Node*           _xieyiNode;
    
    string          RandVerifycode;
    
    IPlayerData*    _player;
    long            verifyTime;     //验证码倒计时时间记录
    
    bool            isForgetPsw;
    
    UIMessage*      mesbox;
    UIMessage*      m_quitgame;
private:
    Node*           rootNode;
    Node*           registNode;
    Node*           forgetNode;
    
    bool            showPsw;    //是否显示密码
    bool            isFristSendPsw; //第一次获取验证码
    
};












#endif /* defined(__PinLe__UI_Login__) */
