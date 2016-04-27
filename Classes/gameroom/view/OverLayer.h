//
//  OverLayer.h
//  Pinle
//
//  Created by 李辛 on 15/6/9.
//
//

#ifndef __Pinle__OverLayer__
#define __Pinle__OverLayer__

#include <stdio.h>
#include "../IRoomData.h"
//#include "../module/RoomData.h"
#include "../../player/IPlayerData.h"
#include "../../gameini/GameIni.h"
#include "LaoHuJiDonghua.h"
#include "../../framework/config.h"

using namespace cocos2d;

#define common_continue     193
#define common_back    192
#define pvp_continue 3
#define pvp_back    4
//class OverLayer : public IUILayer

class OverLayer :public IUILayer
{
public:
    //bool   is_touch;
    Button * but_began;
    Button * but_end;
    LaoHuJiDonghua* laoHuJi3;
    LaoHuJiDonghua* laoHuJi2;
    LaoHuJiDonghua* laoHuJi1;
    LaoHuJiDonghua* laoHuJi0;
    Node* commonItemNode;
    OverLayer();
    ~OverLayer();
    virtual bool init();
    CREATE_FUNC(OverLayer);
    Node *rootNode;
    Node * Out_war;
    Node * root_Node;
    void commonLayer(); //其他游戏结束
    void common_layer();
    Button * buto;
    void activity_rota(Ref *pSender, Widget::TouchEventType type);
    void pvpLayer();    //对战游戏结束
    void activityLayer();   //活动游戏结束
    void shandengv(float time);
    
    void buttonDelay(Node *sender); //延迟按钮触发事件
    
    void buttonTouchEnd(Ref *pSender, Widget::TouchEventType type);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *touch, Event *unused_event);
    void        initActivity();
    void        initdisplay();
    void        init_display();
    void        inithongbao();
    IRoomData *iroomData;
    IPlayerData *iplayerData;
    
    UIMessage* messageBox;
    ImageView *m_pTu_ordinary;
    
public:
    virtual bool Notify(const string& name, Args& args);
    
public:
    Node* activityItemNode;
    Button* bttt;
    //bool    is_buttonshow;
    ListView*       _redbaoList;
    Text* gameRewardGold;
    Sprite *goldSp0;
    Text* playerGold;
    Sprite *goldSp1;
    Text* playerGameTime;
    Sprite *timeSp;
    Text* playerGameRun;
    Sprite *runSp;
    Text* runNumT;
    Button* temptButton1;
    Button* temptButton3;
    ActionInterval *getAction(float ft);
    void  jinxiba_over();
    Text* jifenText;        //累计积分
    Text* zuijiafenText;    //最佳积分
    Text* zuijiapaiming;    //最佳排名
    Text* benchipaiming;    //本次排名
    ImageView * Red_package;
    void  fun_back();
    Button * m_pJiXu_Button_shu;                        //pvp 继续拼
    Button * m_pFanHui_Button;                          //pvp 返回
    vector<LaoHuJiDonghua*>     lhjVer;                 //老虎机动画
    vector<ImageView*>          shandengVec;            //闪灯
    //string aurl;
    Text * bt_text;
    ImageView * t_text;
};


#endif /* defined(__Pinle__OverLayer__) */
