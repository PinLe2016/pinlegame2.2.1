//
//  ScrollCycleView.h
//  PinLe
//
//  Created by 李辛 on 15/7/2.
//
//

#ifndef __PinLe__ScrollCycleView__
#define __PinLe__ScrollCycleView__

#include <stdio.h>
//#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../gameini/GameIni.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


#define ITEM_SCALE 0.8
#define ITEM_SCALE_MORE 1.0

#define left_range 194.5
#define right_range 555.5

#define move_scale 220/750
#define move_double 4
#define swop_item 88
#define positionY 400

#define USERDEFAUL_SET_BOOL UserDefault::getInstance()->setBoolForKey
#define USERDEFAUL_GET_BOOL UserDefault::getInstance()->getBoolForKey

enum lrtype
{
    move_right = 0,
    move_left,
};

enum modetype
{
    gogo = 0,
    jinxiba,
    pinleba,
};



enum itemup
{
    left_up = 0,
    module_up,
    right_up,
};


class ItemsCell : public Node
{
public:
    ItemsCell(){};
    ~ItemsCell(){};
    CREATE_FUNC(ItemsCell);
    virtual bool init();
    void setGrey();
    ItemsCell* create(string str);
    ImageView *imageView;
};


class ScrollCycleView : public LayerColor
{
public:
    Sprite * _prompt;
    ScrollCycleView();
    ~ScrollCycleView();
    virtual bool init();
    CREATE_FUNC(ScrollCycleView);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event) ;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) ;
    void eventCallBack(int tag);
    
    ImageView *fristBtn;
    ImageView *secondBtn;
    ImageView *threeBtn;
    
    ImageView *item1;
    ImageView *item2;
    ImageView *item3;
    
    Vec2 fristVec = Vec2(375.5 - 180,300);
    Vec2 secondVec = Vec2(375.5,300);
    Vec2 threeVec = Vec2(375.5 + 180,300);
    
    Sprite *rotateLight;

    int nowPoType;
    int moveType;
    bool movePoint;
    float beginTouchX;
    bool secondOut1;//右
    bool secondOut2;//左
    
    bool fristOut;
    bool threeOut;
    int touchTag;   //纪录当前最上面item tag号
    Vec2 touchBeginPoint;  //纪录触点的起始位置
    bool touchValue;        //判断点击是否生效，不在时移动，而是进入相应的模块
    
    void setNowPotype(float ft);
    void moveItem(float ft);
    void swopItem(int type, bool fristInit = false);
    void checkZorder();
    void moveLeftRight(float ft);
    
    void setItemScale(float pintX);
    void loadImageText(int tag);
    
    bool beginStart;
    
    void update_move(float dt);
    void touch_end();
    bool item1dir;//移动向左为真
    bool item2dir;
    bool item3dir;
    int it=0;
    int _nm=0;
    
    void guide_TouchEnd(Ref *psender,Widget::TouchEventType type); //屏蔽层
private:
    void button_light_control(ImageView* par,int zorder);//灯光控制
    void stop_button();
    void move_guide();
    Sprite* spr_head;
    
};


#endif /* defined(__PinLe__ScrollCycleView__) */
