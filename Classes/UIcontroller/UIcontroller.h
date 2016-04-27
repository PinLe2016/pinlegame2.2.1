//
//  UIcontroller.h
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#ifndef __PinLe__UIcontroller__
#define __PinLe__UIcontroller__


#include "../framework/config.h"
#include "IUILayer.h"
#include "cocos2d.h"

using namespace cocos2d;


enum
{
    UI_NONE,
    UI_LOGIN,                               //登陆界面
    UI_ACTIVITY,                            //惊喜吧活动界面
    UI_ROOM,                                //战斗拼图界面
    UI_OPTION,                              //设置
    UI_MAIN,                                //主界面
    UI_MARKET,                              //商城
    UI_LOGO,                                //logo界面
    UI_PK,                                  //拼了吧
    UI_FRIEND,                              //好友邀请
    UI_POKEDEX,                             //图鉴
    UI_ACCOUNT,                             //个人账户
    UI_LINK,                                //链接
    
    UI_END
};


#define ui_pinleba  "ui_pinleba"
#define ui_jinxiba  "ui_jinxiba"
#define ui_gogo     "ui_gogo"





class UICtrl
{
public:
    UICtrl();
    ~UICtrl();
    
    static  UICtrl*     getInstance();
    static  void        destroyInstance();
    
    
    enum TransType
    {
        Normal,
        Fade,
        MoveIn,
        CrossFade,
    };
    
public:
    int             sceneID;
    int             lastSceneID;
    Scene*          scene;
    Scene*          lastScene;
    long            stoptime;                   
    long            pasttime;                   //处在外部事件的时间(单位:毫秒)
    
    Args            _args;
    
public:
    void    gotoScene(int sceneid, bool replace = true);
    void    gotoScene(int sceneid, Args args, bool replace = true);
    void    gotoScene(int sceneid, int  transType, bool replace = true);
    void    gotoScene(int sceneid, int  transType, Args args, bool replace = true);
    //transitions
    void    poptoScene(int sceneid);
    Scene*  getScene(int sceneid);
    bool    needModifyInfo();
    
    static  Msg_Layer*  createMesBox(const string& s, int type);
    
};


#define MESBOX_OK_TAG           76
#define MESBOX_CANCEL_TAG       70

enum IconAddSumType
{
    addicon = 0,        //金币增加
    sumbicon,           //金币减少
};

class UIMessage
{
public:
    UIMessage();
    ~UIMessage();
    
    static  UIMessage*  createMesBox(const string& s, int type = 0);
    static  UIMessage*  create_box(const string& s, int type = 0);
    static  UIMessage*  createMesBox(const string& s, const Args* args, int type = 0);
    static  UIMessage*  createMesBox(const string& s, const Args& args, int type = 0);
    static  void createIconAnition(int type, int num);      //添加金币减少增加动画
    void    eventCallback(Ref * pSender, Widget::TouchEventType type);
    void    event_Callback(Ref * pSender, Widget::TouchEventType type);
    void    close();
    void    setButtonString(const string& left, const string& right);
    
public:
    Button*         bOK;
    Button*         bCancel;
    Text*           content;
    Node*           root;
    Msg_Layer*      msgLayer;
    Args            _args;
    
};




#endif /* defined(__PinLe__UIcontroller__) */
