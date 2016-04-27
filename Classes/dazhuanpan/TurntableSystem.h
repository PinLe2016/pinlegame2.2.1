//
//  TurntableSystem.h
//  PinLe
//
//  Created by 李辛 on 15/11/4.
//
//

#ifndef __PinLe__TurntableSystem__
#define __PinLe__TurntableSystem__


#include "../framework/config.h"
#include "../gameini/GameIni.h"

#include <stdio.h>


#define moduleNum 6

enum TurntableNumber
{
    updateSumTime = 5,      //停止减速运行次数
    rotatenumber = 4,       //旋转的圈数后开始减速
    addangle = 720,         //减速增加的旋转角度
    
};



class TurntableSystem : public LayerColor
{
public:
    TurntableSystem();
    ~TurntableSystem();
    virtual bool init();
    CREATE_FUNC(TurntableSystem);
private:
    Size            size;
    Sprite*         zhizhenSp;
    int             updateTimeNumber;
    int             nowModuleNum;
    vector<float>   angleVector;
    
    
    void        _eventTouch(Ref *pSender, Widget::TouchEventType type);
    void        startRotate();
    void        setRotateAngleNumberVector(float angleNumber);
    void        performAction();
    
    
private:
    bool        startAction;
    int         quanshunamber;
    int         dianjicishu;
    int         angleVectorNumber;
    float       angleFloat;
    void        performAction1();
    
    
    
    
    
    
};

#endif /* defined(__PinLe__TurntableSystem__) */
