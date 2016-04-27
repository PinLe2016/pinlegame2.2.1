//
//  TipsLayer.h
//  PinLe
//
//  Created by 李辛 on 15/6/18.
//
//

#ifndef __PinLe__TipsLayer__
#define __PinLe__TipsLayer__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"

using namespace cocos2d;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define tier 33333
#define speedX 0.7

class TipsType
{
    string str;
    int type;
};

class TipsButton : public Button
{
public:
    TipsButton();
    ~TipsButton();
    CREATE_FUNC(TipsButton);
    virtual bool init();
    void setData(Broadcast *_broadcast);
    Broadcast *broadCast;
    
};





//============================================================


#define clippingWidth       574
#define clippingHeight      40
#define broadcastinterval   150

#define MES_BroadcastUIData_data              "MES_BroadcastUIData_data"

typedef struct
{
    Broadcast *b;
    float x;
    float y;
    float width;
}ItemInfo;

class BroadcastUIData : public IDataObserver,public Ref
{
public:    
    vector<ItemInfo> v_bcArr;
    IBroadcastData *broad;
    
    BroadcastUIData();
    ~BroadcastUIData();
    virtual bool Notify(const string& name, Args& args);
    static BroadcastUIData *getInstance();
    bool isInArray(Broadcast *bb);
    void loadBroadcast();   // 加载广播条数
    void updateBCArr();     //  更新最新广播
    void sumBroadcastNum();     //广播个数减一
    void addBroadcastNum();     //广播个数加一
    void update(float ft);      //定时器
    
private:
    uint    sum;
};


#define BroadcastZOrder     1000

class BroadcastUI : public IDataObserver, public LayerColor
{
public:
    BroadcastUI();
    ~BroadcastUI();
    CREATE_FUNC(BroadcastUI);
    virtual bool init();
    
    virtual bool Notify(const string& name, Args& args);
    void setVisible(bool b);
    void update(float ft);
    void loadBC();
    bool isInArrary(Broadcast *bb);
    void eventCallBack(Ref *pSender, Widget::TouchEventType type);
    void removeAllBroadcast();
    
    vector<Node*> v_broadcastArr;
    vector<Broadcast*> v_bcList;
private:
    Layout *layout;
    
};






#endif /* defined(__PinLe__TipsLayer__) */
