//
//  UI_Main.h
//  PinLe
//
//  Created by Kennedy on 15/6/10.
//
//

#ifndef __PinLe__UI_Main__
#define __PinLe__UI_Main__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"
#include "Guide_choose.h"

enum
{
    B_ACTIVITY = 15,
    B_GO = 13,
    B_PK = 16,
    B_MARKET = 14,
    I_HEAD = 8,
    T_GOLDS = 106,
    T_NICKNAME = 10,
    T_LEVEL = 11,
    LB_GRADE = 43,
    B_OPTION = 288,
    B_OPTION_MUSIC = 91,
    B_OPTION_LOGOUT = 95,
    B_OPTION_BACK = 97,
    B_OPTION_SUBMIT = 61,
    B_OPTION_ADVISE = 55,
    B_OPTION_BUSINESS = 57,
    TF_OPTION_ADVISE= 310,
    TF_OPTION_BUSINESS= 311,
    B_REWARD_GUIDE = 737,
};

class UI_Main : public IUILayer
{
public:
    UI_Main();
    ~UI_Main();
    
    
    typedef struct
    {
        string  path;
        Vec2    pos;
        float   rotate;
        float   speed;
        int     forward;
        float   time;
    }LightIni;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Main);
    
    //    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    

    
public:
    void        eventCallback(Ref * pSender,Widget::TouchEventType type);
    void        addLight();
    void        lightcall(Node *sender);
    void        setMusic(bool bl);
    void        setOptionType(int type);
    void        sendAdvise(int type);
    void        showRewardGuide();          //提示玩家提交中奖订单
    IPlayerData*    _player;
    IRoomData*      _room;
    IPokedexData*   _pokedex;
    UIMessage*      m_quitgame;
    Node*           rootNode;
    Node*           option;
    Sprite *sprite_light;
    LightIni    _light[4];
    
    int         _option_type;
    void        testddd();
public:
    char*        readFile();
private:
    bool        reward_guide;
    GuideChoose*    guidechose;
};





#endif /* defined(__PinLe__UI_Main__) */
