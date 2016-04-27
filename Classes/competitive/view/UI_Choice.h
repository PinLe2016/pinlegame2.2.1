//
//  UI_Choice.h
//  PinLe
//
//  Created by 李辛 on 15/6/11.
//
//

#ifndef __PinLe__UI_Choice__
#define __PinLe__UI_Choice__

#include <stdio.h>
#include "../../framework/config.h"
#include "../../gameini/GameIni.h"

#define btn_personal_tag    417   //403
#define btn_pvp2_tag    418       //406
#define btn_pvp4_tag    419       //405
#define btn_friend_tag  4
#define btn_back_tag    407

#define btn_easy_personal_tag   150
#define btn_diffult_personal_tag    151
#define btn_back_personal_tag   40
#define personal_light1 159
#define personal_light2 160

#define choice_layer "choice_layer"     // 
#define challenge_layer "challenge_layer"       //
#define match_layer     "match_layer"       //多人赛匹配

using namespace cocos2d::experimental::ui;

class UI_Choice : public IUILayer
{
public:
    UI_Choice();
    ~UI_Choice();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(UI_Choice);
    virtual bool Notify(const string& name, Args& args);
    
public:
    void        eventCallback(Ref * pSender, Widget::TouchEventType type);
    WebView*    _webview;
public:
    void enterPersonal();
    void enterPVP(int _tag);
    Layer *personal_choice;
    Layer *pvp_match;
    
    Text* pTime_Show;
    Sprite* pPVP_tishi;
    int mStartTime;
    int pvp_type;
    void updatePvP(float ft);
    
    
    //
    void showEnemy(bool b = false);
    vector<Node*> enemyVer;
    Text* seekRival;
    Sprite* seekBackBg;
    Text* enterGameWan;
private:
    cocos2d::ui::Button* btn_back;
    Button * btn_back_personal;
    string layerType;
    bool _pvpFail;  //匹配失败
    bool needModifyInfo();
    
    UIMessage* messageBox;
//    Node *personalNode;
};


#endif /* defined(__PinLe__UI_Choice__) */
