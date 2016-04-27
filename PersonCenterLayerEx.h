//
//  PersonCenterLayerEx.h
//  PinLe5
//
//  Created by Kennedy on 15/5/5.
//
//

#ifndef __PinLe5__PersonCenterLayerEx__
#define __PinLe5__PersonCenterLayerEx__


#include "../../framework/config.h"
#include "../../gameini/GameIni.h"
#include "../../player/IPlayerData.h"
#include "../../player/config.h"
#define  NI_CHENG   "请输入昵称"
#define  _NI_CHENG  "山大王"

class PersonCenterLayerEx:public IUILayer, public TextFieldDelegate, public IMEDelegate
{

public:
    bool is_TH;
    string Gethead_img();
    Button * _preservation;
    PersonCenterLayerEx();
    ~PersonCenterLayerEx();
    static  PersonCenterLayerEx*    getInstance();
    
    virtual bool Notify(const string& name, Args& args);
    
    TextField*  TF_nickname;                //昵称编辑框
    Text*       T_nickname;                 //昵称Text
    Text*       T_gender;                   //性别Text
    
    Node*       p_nickname;                 //昵称panel
    Node*       p_gender;                   //性别panel
    Node*       p_mailaddress;              //邮寄地址panel
    Node*       p_head;                     //头像panel
    Node*       p_city;                     //市panel
    Node*       p_province;                 //省panel
    Node*       p_birthday;                 //生日panel
    Node*       p_now;
    
    IPlayerData* _player;
    
    int         address_province;
    string      addressprovince_name;
    string      mailprovince_name;
    int         address_city;
    string      addresscity_name;
    string      mailcity_name;
    int         mailaddress_province;
    int         mailaddress_city;
    
    int         address_province_id;
    int         address_city_id;
    int         mailaddress_province_id;
    int         mailaddress_city_id;
    
    rapidjson::Document readdoc;
    rapidjson::Document namedoc;
    string getName(int num);
    ListView*   L_provinceList;
    ListView*   L_cityList;
    
    ListView*   L_yearList;
    ListView*   L_monthList;
    ListView*   L_dayList;
    
    uint        _year;
    uint        _month;
    uint        _day;

    virtual bool init();
    CREATE_FUNC(PersonCenterLayerEx);
    
    void setTextInfoByTag(uint tag);
    void setTextInfoByIndex(uint tag);
    string getStringByIndex(uint tag);
    
    
    void getDays(string s, uint* year, uint* month, uint* day);
    
    int  tabtype;
    
    void setCityList(int i, bool b = true);
    void setSelectCity(int index);
    void setProvinceList();
    
    void openCalendar();
    void openMailaddress();
    void openGender();
    
    int  goldTmp;

    void setDay(int year, int month);
    
    bool isVauled(TextField* tf);
    
    bool isVaild();
    Node *rootNode;
    
    void setGoldUp(int gold);
    
public:
    void _EventCallBack(Ref *pSender, Widget::TouchEventType type);
    void _selectedItemEvent(Ref *pSender, ListView::EventType type);
    void _city_selectedItemEvent(Ref *pSender, ListView::EventType type);
    
    void _year_selectedItemEvent(Ref *pSender, ListView::EventType type);
    void _month_selectedItemEvent(Ref *pSender, ListView::EventType type);
    void _day_selectedItemEvent(Ref *pSender, ListView::EventType type);
    void sendHttpSaveInfo();
    void removeDialog(Ref *pSender, Widget::TouchEventType type );
    
    void textFieldCallBack(Ref* pSender, TextField::EventType type);
    void setAddress(int tag);
    int  getaddress_province(string name);
    void setDefultProvince();
    
    void GoldUp(Node *sender);
    bool    isChange;
    bool checkYQHY();       //检测邀请好友是否添加
    
    Player*    pNew;
    TextField* callMeFriend;        //邀请我的好友
    UIMessage* messageBox;              //
    bool    isInfoChange;           //玩家信息是否修改
    
};















#endif /* defined(__PinLe5__PersonCenterLayerEx__) */
