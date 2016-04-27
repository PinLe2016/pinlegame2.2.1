//
//  PersonCenterLayer.hpp
//  PinLe
//
//  Created by Kennedy on 16/3/31.
//
//

#ifndef PersonCenterLayer_h
#define PersonCenterLayer_h

#include <stdio.h>
#include "../framework/config.h"
#include "../gameini/GameIni.h"
#include "../player/IPlayerData.h"
#include "../player/config.h"
#include "ItemPicker.h"
#include "../option/view/PersonCenterLayerEx.h"
using namespace cocos2d;
enum
{
    _PROVINCE   = 23,               //省份panel
    _CITY       = 24,                   //城市panel
    _CONTY      = 25,                  //县（区）panel
    _TITLE      =  2235,                   //定位头表
    _TXTNAME    =  2244,              //LIST内部字体
    _CLOSE      =  2231,               //关闭按钮
    _SAVE_DATA  =  2247,                //保存按钮
    _RESET      =  26,                  //刷新
};


class PersonCenterLayer:public IUILayer, public TextFieldDelegate, public IMEDelegate
{
public:
    PersonCenterLayer();
    ~PersonCenterLayer();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(PersonCenterLayer);
    
public:
    void _selectedItemEvent(Ref *pSender, ScrollviewEventType type);
    void _city_selectedItemEvent(Ref *pSender, ScrollviewEventType type);
    
    void setProvinceList();
    void setCityList(int index, bool b=true);
    void setConty();
    ItemPicker * addItemPickerData(ui::ScrollView* scorll,ModeType type);
    
    void _EventCallBack(Ref *pSender, Widget::TouchEventType type);
    
    
    virtual bool Notify(const string& name, Args& args);
    
    void changTitle();
    
    void inline setDelegate(PersonCenterLayerEx* _delegate){delegate=_delegate;}
public:
     Node*              p_province;                 //省 市 区panel
     Text*              m_title;                   //投标地址显示
     Text*              m_Belonging;                   //定位或者归属地
    ItemPicker*         L_provinceList;             //省
    ItemPicker*         L_cityList;                 //市
    ItemPicker*         L_county;                   //县（区）
    int                 m_offset_cell;
    rapidjson::Document readdoc;
    
    string              m_address;                  //保存头标地址
    IPlayerData*        iplayer;                   //获取数据
    bool                is_iphone;                  //手机归属地地址
    PersonCenterLayerEx* delegate;                  //设置代理
};
#endif /* PersonCenterLayer_hpp */
