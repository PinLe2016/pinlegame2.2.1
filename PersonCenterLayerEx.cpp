//
//  PersonCenterLayerEx.cpp
//  PinLe5
//
//  Created by Kennedy on 15/5/5.
//
//

#include "PersonCenterLayerEx.h"
#include "../config.h"
#include "../../player/module/PlayerData.h"
static string heard_image;
#define _addCallBack        CC_CALLBACK_2(PersonCenterLayerEx::_EventCallBack, this)
#include "../../chooseCity/PersonCenterLayer.h"

#define BUTTON_SAVE     50
#define INFO_SAVE       200         //修改了个人信息


enum TAB_TYPE
{
    TAB_CITY,
    TAB_MAILADDRESS
};

enum
{
    AUTOCLOSE_BIRTHDAY = 210,
    AUTOCLOSE_HEAD     = 357,
    AUTOCLOSE_NICKNAME = 358,
    AUTOCLOSE_GENDER   = 359,
    AUTOCLOSE_MAILADDRESS = 360,
};

enum
{
    IMAGE_HEADICON = 48,
};


enum
{
    P_NICKNAME = 160,               //昵称panel
    P_GENDER = 62,                  //性别panel
    P_MAILADDRESS = 114,            //邮寄地址panel
    P_HEAD = 96,                    //头像panel
    P_PROVINCE = 118,               //省份panel
    P_CITY = 142,                   //城市panel
    P_BRITHDAY = 116,               //生日panel
};

enum
{
    TF_NICKNAME = 180,
    TF_REALNAME = 126,
    TF_PHONENUMBER = 128,
    TF_MAILADDRESS = 137,
};



enum
{
    B_BACK = 44,                    //返回上级界面
    B_SAVE = 95,                    //提交保存
    B_NICKNAME = 50,                 //弹出更改昵称框
    B_NICKNAMESAVE = 182,           //昵称保存按钮
    B_GENDER_MAN = 72,
    B_GENDER_WOMAN = 73,
    B_GENDER = 55,                  //主页性别按钮
    B_HEAD = 46,                    //更改头像按钮
    B_MAILADDRESS = 86,
    B_MAILADDRESS_AREA = 130,       //地区信息按钮
    B_MAILADDRESSSAVE = 361,
    
    B_ADDRESS = 82,                 //所在城市按钮
    
    B_PROVINCEBACK = 140,
    B_CITYBACK = 145,
    
    B_BIRTHDAY = 59,
    B_BIRTHDAYSAVE = 200,
    B_YEAR = 131,
    B_MONTH = 132,
    B_DAY = 133,
    B_LOGOUT = 207,
    
    B_PROVINCEBTN = 1007,   //城市－－省选择按钮
    B_CITYBTN   = 1008,     //城市－－市选择按钮
    B_PROVINCELIST = 1009,  //城市－－省列表选择按钮
    B_CITYLIST = 1010,      //城市－－市列表选择按钮
    B_SAVE_ADDRESS = 1006,     //城市－－保存省，市
    
};


enum
{
    L_PROVINCE = 147,
    L_CITY = 153,
    L_YEAR = 134,
    L_MONTH = 138,
    L_DAY = 141,
};


enum
{
    T_NICKNAME      = 54,                //玩家昵称
    T_GENDER        = 57,                //性别
    T_BRITHDAY      = 61,                //生日
    T_CITY          = 84,                //所在城市
    T_MAILADDRESS   = 88,                //邮寄地址
    T_REGTIME       = 93,                //注册时间
};
static PersonCenterLayerEx* _personCenterLayerEx = NULL;
PersonCenterLayerEx* PersonCenterLayerEx::getInstance()
{
    if(_personCenterLayerEx == NULL)
    {
        _personCenterLayerEx = new PersonCenterLayerEx();
    }
    return _personCenterLayerEx;
}
static PersonCenterLayerEx* create()
{
    PersonCenterLayerEx *pRet = new(std::nothrow) PersonCenterLayerEx();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
PersonCenterLayerEx::PersonCenterLayerEx()
{
    //is_TH=true;
    gamelog("----------PersonCenterLayerEx::PersonCenterLayerEx---------");
    _player = M_GET_PLAYER;
    address_province = -1;
    address_city = -1;
    mailaddress_province = -1;
    mailaddress_city = -1;
    address_province_id = 0;
    address_city_id = 0;
    mailaddress_province_id = 0;
    mailaddress_city_id = 0;
    isChange = NO;
    messageBox = nullptr;
    
    _year = _month = _day = 0;
    isInfoChange = false;
    
//    _year = 1990;
//    _month = 1;
//    _day = 1;
    
    pNew = new Player();
    pNew->copy(_player);
    IUILayer::attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_PLAYER_UPDATEINFO);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_nickname);
}


PersonCenterLayerEx::~PersonCenterLayerEx()
{
    gamelog("----------PersonCenterLayerEx::~PersonCenterLayerEx---------");
//    readdoc.Clear();
//    pNew->release();
    SAFE_DELETE(pNew);

    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_PLAYER_UPDATEINFO);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_nickname);
}




void PersonCenterLayerEx::setTextInfoByTag(uint tag)
{
    Text* text = (Text*)rootNode->getChildByTag(tag);
    if(!text)
        return;
    
    switch (tag)
    {
        case T_NICKNAME:
            if (pNew->nickname=="昵称") {
                text->setString("拼乐");
            }
            else{
            text->setString(pNew->nickname);
            }
            break;
        case T_GENDER:
        {
            if(pNew->gender == 1)
                text->setString("男");
            else if(pNew->gender == 0)
                text->setString("女");
            else{
                text->setString(" ");
            }
        }break;
        case T_BRITHDAY:
        {
            if(_year == 0 || _month == 0 || _day == 0)
                text->setString("请选择");
            else
                text->setString(StringUtils::format("%d/%d/%d", _year, _month, _day));
        }break;
            
        case T_CITY:
            text->setString(getStringByIndex(T_CITY));
            break;
            
        case T_MAILADDRESS:
            text->setString(getStringByIndex(T_MAILADDRESS));
            break;
            
        case T_REGTIME:
            text->setString(_player->registertime);
            break;
        default:
            break;
    }
}


void PersonCenterLayerEx::setCityList(int index, bool b)
{
    L_cityList->removeAllItems();
    
    rapidjson::Value& provinces = readdoc["provinces"];
    
    GAMELOG("index = %d  size = %d", index, provinces.Size());
    
    if(provinces.IsArray() && index < provinces.Size())
    {
        rapidjson::Value& citys = provinces[index]["citys"];
        if(citys.IsArray())
        {
            GAMELOG("citys.Size = %d", citys.Size());
            for(int i = 0; i < citys.Size(); i ++)
            {
                L_cityList->pushBackDefaultItem();
                Button* b = (Button*)L_cityList->getItem(i);
                b->setTitleText(citys[i]["name"].GetString());
            }
        }
    }
    
    L_cityList->setVisible(true);
    if(b)
        setSelectCity(0);
//    p_city->setVisible(1);
//    p_now = p_city;
}


void PersonCenterLayerEx::setProvinceList()
{
    L_provinceList->jumpToTop();
    L_provinceList->removeAllItems();
    L_provinceList->setVisible(true);
    
    rapidjson::Value& provinces = readdoc["provinces"];
    Button* button;
    for(int i = 0; i < provinces.Size(); i ++)
    {
        L_provinceList->pushBackDefaultItem();
        
        button = (Button*)L_provinceList->getItem(i);
        button->setTitleText(provinces[i]["name"].GetString());
    }
}



string PersonCenterLayerEx::getStringByIndex(uint tag)
{
    string s = "";
    switch (tag)
    {
        case T_CITY:
        {
            if(address_province < 0 || address_city < 0)
            {
                s = "请选择";
                break;
            }
            else
            {
                s = StringUtils::format("%s-%s", readdoc["provinces"][address_province]["name"].GetString(),
                                        readdoc["provinces"][address_province]["citys"][address_city]["name"].GetString());
            }
        }break;
        
        case B_MAILADDRESS_AREA:
        {
            if(mailaddress_province < 0 || mailaddress_city < 0)
            {
                if((!pNew->postprovincename.empty() || !pNew->postcityname.empty()))
                {
                    s = StringUtils::format("%s-%s", pNew->postprovincename.c_str(),
                                                     pNew->postcityname.c_str());
                }
                else
                    s = "请选择";
            }
            else
            {
                s = StringUtils::format("%s-%s", readdoc["provinces"][mailaddress_province]["name"].GetString(),
                                        readdoc["provinces"][mailaddress_province]["citys"][mailaddress_city]["name"].GetString());
            }

        }break;
            
        case T_MAILADDRESS:
        {
            if(mailaddress_province < 0 || mailaddress_city < 0)
            {
                if((!_player->postprovincename.empty() || !_player->postcityname.empty()))
                {
                    s = StringUtils::format("%s-%s", pNew->postprovincename.c_str(),
                                                     pNew->postcityname.c_str());
                    if(!pNew->postaddress.empty())
                    {
                        s = StringUtils::format("%s-%s", s.c_str(), pNew->postaddress.c_str());
                    }
                }
                else
                    s = "请选择";
                break;
            }
            else
            {
                s = StringUtils::format("%s-%s", readdoc["provinces"][mailaddress_province]["name"].GetString(),
                                        readdoc["provinces"][mailaddress_province]["citys"][mailaddress_city]["name"].GetString());
                if(!pNew->postaddress.empty())
                {
                    s = StringUtils::format("%s-%s", s.c_str(), pNew->postaddress.c_str());
                }
            }
            

        }break;
        default:
            break;
    }
    return s;
}

void PersonCenterLayerEx::setTextInfoByIndex(uint tag)
{
    Text* text = (Text*)rootNode->getChildByTag(tag);
    if(!text)
        return;
    
    switch (tag)
    {
//        case T_NICKNAME:
//            text->setString(BaseGame::GetBaseGame()->uName);
//            break;
//        case T_GENDER:
//            if(BaseGame::GetBaseGame()->uGender.compare("1") == 0)
//                text->setString("男");
//            else if(BaseGame::GetBaseGame()->uGender.compare("0") == 0)
//                text->setString("女");
//            else
//                text->setString("请选择");
//            break;
//        case T_BRITHDAY:
//            text->setString(_player->birthday);
//            break;
        case T_CITY:
        {
            text->setString(getStringByIndex(tag));
        }break;

        case T_MAILADDRESS:
        {
            text->setString(getStringByIndex(tag));
        }break;
        default:
            break;
    }
}




void PersonCenterLayerEx::textFieldCallBack(Ref* pSender, TextField::EventType type)
{
    TextField* tf = (TextField*)pSender;
    GAMELOG("tf->getTag = %d, type = %d", tf->getTag(), type);
    if(tf->getTag() == 353)
    {
        return;
    }
    
    switch (type)
    {
        case TextField::EventType::ATTACH_WITH_IME:
        {
            IAudio::getInstance()->playEffect(music_ef_ok);
            if(!tf->getString().compare(tf->getPlaceHolder()) || tf->getString().empty())
            {
                tf->setString("");
            }
        }break;
            
        case TextField::EventType::DETACH_WITH_IME:
        {
            if(tf->getString().empty())
            {
                tf->setString(tf->getPlaceHolder());
                if(tf->getTag() == 287)
                {
                    tf->setString("");
                }
            }
        }break;
            
        default:
            break;
    }
}

void PersonCenterLayerEx::getDays(string s, uint* year, uint* month, uint* day)
{
    if(!year || !month || !day || s.empty())
        return;
    string tmp = s;
    uint64_t n;
    
    
    GAMELOG("生日 = %s", s.c_str());
    n = s.find_first_of("/");
    tmp = s.substr(0, n);
    *year = atoi(tmp.c_str());
    s = s.substr(n+1);

    n = s.find_first_of("/");
    tmp = s.substr(0, n);
    *month = atoi(tmp.c_str());
    s = s.substr(n+1);
  
    *day = atoi(s.c_str());
    
    GAMELOG("year = %d, month = %d, day = %d", *year, *month, *day);
}


bool PersonCenterLayerEx::isVaild()
{
    return (_year != 0 && _month != 0 && _day != 0);
}

void PersonCenterLayerEx::openCalendar()
{
    p_birthday->setVisible(1);
    L_yearList->setVisible(0);
    L_monthList->setVisible(0);
    L_dayList->setVisible(0);
    p_now = p_birthday;
    
    Button* b;
    

    if(!isVaild())
    {
//        struct timeval nowTimeval;
//        gettimeofday(&nowTimeval, NULL);
//        struct tm * tm;
//        time_t time_sec ;
//        time_sec = nowTimeval.tv_sec;
//        tm = localtime(&time_sec);
//        
        _year = 1990;
        _month = 1;
        _day = 1;

    }
    setDay(_year, _month);
    
    b = (Button*)p_birthday->getChildByTag(B_YEAR);
    b->setTitleText(StringUtils::format("%d", _year));
    
    b = (Button*)p_birthday->getChildByTag(B_MONTH);
    b->setTitleText(StringUtils::format("%d", _month));
    
    b = (Button*)p_birthday->getChildByTag(B_DAY);
    b->setTitleText(StringUtils::format("%d", _day));
}
string PersonCenterLayerEx::getName(int num){
    string s = "";
    string key = StringUtils::format("%d", num);
    if(namedoc.IsArray())
    {
        uint i = 0;
        if(namedoc[i].HasMember(key.c_str()))
            s = namedoc[i][key.c_str()].GetString();
    }
    return s;
}

bool PersonCenterLayerEx::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    setKeyboardEnabled(true);
    
    goldTmp = _player->golds;
    getDays(_player->birthday, &_year, &_month, &_day);
    
    
    
//    rapidjson::Document readdoc;
    ssize_t size = 0;
    std::string load_str;
    
    unsigned char* titlech = FileUtils::getInstance()->getFileData("CSres/Setup/FreeRes/city.json", "r", &size);
    load_str = std::string((const char*)titlech,size);
    readdoc.Parse<0>(load_str.c_str());
    
    ssize_t size1 = 0;
    std::string load_str1;
    unsigned char* titlech1 = FileUtils::getInstance()->getFileData("CSres/Setup/FreeRes/name.json", "r", &size1);
    load_str1 = std::string((const char*)titlech1,size1);
    namedoc.Parse<0>(load_str1.c_str());
    
    Button* button;
    ImageView* image;
    Text* text;
    TextField* tf;
    rootNode = CSLoader::createNode("CSres/Setup/Information.csb");
    this->addChild(rootNode);
 //   auto ni_name = rootNode->getChildByTag(P_NICKNAME);
//    auto player_name = (TextField *)ni_name->getChildByTag(TF_NICKNAME);
    //player_name->setString(getName(rand() % 32+1 ).c_str());//昵称初始化
    image = (ImageView*)rootNode->getChildByTag(IMAGE_HEADICON);
    
    string path = StringUtils::format("res/newUI/resUI/Information/Head/%s",
                                      ITools::Intercept_string(_player->uHeadIndex, "/").c_str());
        image->loadTexture(path);
    setTextInfoByTag(T_NICKNAME);
    setTextInfoByTag(T_GENDER);
    setTextInfoByTag(T_BRITHDAY);
    setTextInfoByTag(T_CITY);
    setTextInfoByTag(T_MAILADDRESS);
    setTextInfoByTag(T_REGTIME);
    
    text = (Text*)rootNode->getChildByTag(T_CITY);
    if(!_player->provincename.empty() || !_player->cityname.empty())
        text->setString( StringUtils::format("%s-%s", _player->provincename.c_str(),
                                   _player->cityname.c_str()));
    
    image = (ImageView*)rootNode->getChildByTag(IMAGE_HEADICON);
    image->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str()));
    
    button = (Button*)rootNode->getChildByTag(B_BACK);
    button->addTouchEventListener(_addCallBack);
//    button = (Button*)rootNode->getChildByTag(B_SAVE);
//    button->addTouchEventListener(_addCallBack);
//-------------------------昵称--------------------------------------
    
    p_nickname = rootNode->getChildByTag(P_NICKNAME);
    p_nickname->setVisible(true);
    p_nickname->setVisible(false);
    p_now = p_nickname;
    TF_nickname = (TextField*)p_nickname->getChildByTag(TF_NICKNAME);
    CCLOG("昵称是什么  %s  ",TF_nickname->getString().c_str());
//    if (TF_nickname->getString()==(NI_CHENG) ) {
//       TF_nickname->setString(_NI_CHENG);
//    }
//    else {
    TF_nickname->setString(_player->nickname);//(pNew->nickname);//(_player->nickname);
//    }
    CCLOG("昵称是 %s ",TF_nickname->getString().c_str());
    TF_nickname->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    TF_nickname->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::textFieldCallBack, this));
    button = (Button*)rootNode->getChildByTag(B_NICKNAME);//修改昵称按钮
    button->addTouchEventListener(_addCallBack);
    
    button = (Button*)p_nickname->getChildByTag(B_NICKNAMESAVE);
    button->addTouchEventListener(_addCallBack);
    
    button = (Button*)p_nickname->getChildByTag(AUTOCLOSE_NICKNAME);
    button->addTouchEventListener(_addCallBack);

//-----------------------------性别----------------------------------
    button = (Button*)rootNode->getChildByTag(B_GENDER);
    button->addTouchEventListener(_addCallBack);
    
    p_gender = rootNode->getChildByTag(P_GENDER);
    
    image = (ImageView*)p_gender->getChildByTag(AUTOCLOSE_GENDER);
    image->addTouchEventListener(_addCallBack);

    button = (Button*)p_gender->getChildByTag(B_GENDER_MAN);
    button->addTouchEventListener(_addCallBack);
    button = (Button*)p_gender->getChildByTag(B_GENDER_WOMAN);
    button->addTouchEventListener(_addCallBack);
//-----------------------------头像----------------------------------
    button = (Button*)rootNode->getChildByTag(B_HEAD);
    button->addTouchEventListener(_addCallBack);
    
    p_head = rootNode->getChildByTag(P_HEAD);
    
    for(int i = 105; i <= 120; i ++)
    {
        image = (ImageView*)p_head->getChildByTag(i);
        image->addTouchEventListener(_addCallBack);
        image->loadTexture(StringUtils::format("CSres/Setup/Head/headicon_%d.jpg", i - 105));
    }
    image = (ImageView*)p_head->getChildByTag(AUTOCLOSE_HEAD);
    image->addTouchEventListener(_addCallBack);

//-----------------------------邮寄地址----------------------------------
    p_mailaddress = rootNode->getChildByTag(P_MAILADDRESS);
    
    tf = (TextField*)p_mailaddress->getChildByTag(TF_REALNAME);
    tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    tf->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::textFieldCallBack, this));
    tf = (TextField*)p_mailaddress->getChildByTag(TF_PHONENUMBER);
    tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    tf->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::textFieldCallBack, this));
    tf = (TextField*)p_mailaddress->getChildByTag(TF_MAILADDRESS);
    tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    tf->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::textFieldCallBack, this));
    
    
//    button = (Button*)rootNode->getChildByTag(B_MAILADDRESS);
//    button->addTouchEventListener(_addCallBack);
    
    
    image = (ImageView*)p_mailaddress->getChildByTag(AUTOCLOSE_MAILADDRESS);
    image->addTouchEventListener(_addCallBack);
    
    
//    button = (Button*)p_mailaddress->getChildByTag(B_MAILADDRESSSAVE);
//    button->addTouchEventListener(_addCallBack);
    
    button = (Button*)p_mailaddress->getChildByTag(B_MAILADDRESS_AREA);
    button->addTouchEventListener(_addCallBack);
    button->setTitleText(getStringByIndex(B_MAILADDRESS_AREA));
    
////-----------------------------省市界面----------------------------------
    button = (Button*)rootNode->getChildByTag(B_ADDRESS);
    button->addTouchEventListener(_addCallBack);
//
    rapidjson::Value& provinces = readdoc["provinces"];
    
    p_province = rootNode->getChildByTag(P_PROVINCE);
    button = (Button*)p_province->getChildByTag(B_PROVINCEBACK);
    button->addTouchEventListener(_addCallBack);

//    L_provinceList = (ListView*)p_province->getChildByTag(L_PROVINCE);
    

    button = (Button*)p_province->getChildByTag(B_PROVINCEBTN);
    button->addTouchEventListener(_addCallBack);
    
    L_provinceList = (ListView*)p_province->getChildByTag(B_PROVINCELIST);
    L_provinceList->setItemModel(L_provinceList->getItem(0));
    L_provinceList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_selectedItemEvent, this));
    L_provinceList->setVisible(false);
    
//    L_cityList = (ListView*)p_city->getChildByTag(L_CITY);
    button = (Button*)p_province->getChildByTag(B_CITYBTN);
    button->addTouchEventListener(_addCallBack);
    
    L_cityList = (ListView*)p_province->getChildByTag(B_CITYLIST);
    L_cityList->setItemModel(L_cityList->getItem(0));
    L_cityList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_city_selectedItemEvent, this));
    L_cityList->setVisible(false);
    
    button = (Button*)p_province->getChildByTag(B_SAVE_ADDRESS);
     button->addTouchEventListener(_addCallBack);

//    rapidjson::Value& provinces = readdoc["provinces"];
//    for(int i = 0; i < provinces.Size(); i ++)
//    {
//        if(i != 0)
//            L_provinceList->pushBackDefaultItem();
//        
//        button = (Button*)L_provinceList->getItem(i);
//        button->setTitleText(provinces[i]["name"].GetString());
//    }
////-----------------------------市界面----------------------------------
//    p_city = rootNode->getChildByTag(P_CITY);
//    button = (Button*)p_city->getChildByTag(B_CITYBACK);
//    button->addTouchEventListener(_addCallBack);
//    
//    L_cityList = (ListView*)p_city->getChildByTag(L_CITY);
//    L_cityList->setItemModel(L_cityList->getItem(0));
//    L_cityList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_city_selectedItemEvent, this));
    
//-----------------------------生日----------------------------------
    p_birthday = rootNode->getChildByTag(P_BRITHDAY);
    
    image = (ImageView*)p_birthday->getChildByTag(AUTOCLOSE_BIRTHDAY);
    image->addTouchEventListener(_addCallBack);
    
    button = (Button*)p_birthday->getChildByTag(B_BIRTHDAYSAVE);
    button->addTouchEventListener(_addCallBack);
    
    button = (Button*)rootNode->getChildByTag(B_BIRTHDAY);
    button->addTouchEventListener(_addCallBack);

    button = (Button*)p_birthday->getChildByTag(B_YEAR);
    button->addTouchEventListener(_addCallBack);
    button = (Button*)p_birthday->getChildByTag(B_MONTH);
    button->addTouchEventListener(_addCallBack);
    button = (Button*)p_birthday->getChildByTag(B_DAY);
    button->addTouchEventListener(_addCallBack);
    
    
    L_yearList = (ListView*)p_birthday->getChildByTag(L_YEAR);
    L_yearList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_year_selectedItemEvent, this));
    L_yearList->setItemModel(L_yearList->getItem(0));
    for(int i = 0; i < 80; i ++)
    {
        if(i != 0)
            L_yearList->pushBackDefaultItem();
        button = (Button*)L_yearList->getItem(i);
        button->setTitleText(StringUtils::format("%d", 2015 - i));
    }
        

    L_monthList = (ListView*)p_birthday->getChildByTag(L_MONTH);
    L_monthList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_month_selectedItemEvent, this));
    L_monthList->setItemModel(L_monthList->getItem(0));
    for(int i = 0; i < 12; i ++)
    {
        if(i != 0)
            L_monthList->pushBackDefaultItem();
        button = (Button*)L_monthList->getItem(i);
        button->setTitleText(StringUtils::format("%d", i+1));
    }
    
    L_dayList = (ListView*)p_birthday->getChildByTag(L_DAY);
    L_dayList->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::_day_selectedItemEvent, this));
    L_dayList->setItemModel(L_dayList->getItem(0));
    
/////------邀请我的好友------------------------
    callMeFriend = (TextField*)rootNode->getChildByTag(287);
//    callMeFriend->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    callMeFriend->addEventListener(CC_CALLBACK_2(PersonCenterLayerEx::textFieldCallBack, this));
    //UserDefault::getInstance()->setBoolForKey("TOUCH", true);
   
    _preservation = (Button*)rootNode->getChildByTag(366);
    //    callMeFriend->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    _preservation->addTouchEventListener(_addCallBack);
    if(!pNew->myFriendId.empty())
    {
        _preservation->setVisible(false);//(UserDefault::getInstance()->getBoolForKey("TOUCH"));
        
        callMeFriend->setTouchEnabled(false);
        callMeFriend->setString(pNew->myFriendId);
    }
    else
    {
        
    }
    return true;
}

void PersonCenterLayerEx::_year_selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);
    GAMELOG("select child end index = %ld", listView->getCurSelectedIndex());
    
    Button* b = (Button*)p_birthday->getChildByTag(B_YEAR);
    Button* y = (Button*)listView->getItem(listView->getCurSelectedIndex());
    b->setTitleText(y->getTitleText());
    _year = atoi(y->getTitleText().c_str());
    listView->setVisible(0);
    setDay(_year, _month);
    
    
}
void PersonCenterLayerEx::_month_selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);

    Button* b = (Button*)p_birthday->getChildByTag(B_MONTH);
    Button* m = (Button*)listView->getItem(listView->getCurSelectedIndex());
    b->setTitleText(m->getTitleText());
    _month = atoi(m->getTitleText().c_str());
    listView->setVisible(0);
    setDay(_year, _month);
    
}
void PersonCenterLayerEx::_day_selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);

    Button* b = (Button*)p_birthday->getChildByTag(B_DAY);
    Button* d = (Button*)listView->getItem(listView->getCurSelectedIndex());
    b->setTitleText(d->getTitleText());
    _day = atoi(d->getTitleText().c_str());
    listView->setVisible(0);
}


void PersonCenterLayerEx::setDay(int year, int month)
{
    if(month < 1 || month > 12)
        return;
    
    int day = 31;
    
    if(month == 2)
    {
        day = (year % 4 == 0) ? 29 : 28;
    }
    else if( month == 4 || month == 6 || month == 9 || month == 11)//4，6，9，11
        day = 30;
    
    L_dayList->removeAllItems();

    for(int i = 0; i < day; i ++)
    {
        L_dayList->pushBackDefaultItem();
        Button* b = (Button*)L_dayList->getItem(i);
        b->setTitleText(StringUtils::format("%d", i + 1));
    }
    Button* button = (Button*)p_birthday->getChildByTag(B_DAY);
    if(atoi(button->getTitleText().c_str()) > day )
        button->setTitleText(StringUtils::format("%d", day));
    
}


void PersonCenterLayerEx::_selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);
    GAMELOG("select child end index = %ld", listView->getCurSelectedIndex());
    
    uint index = (uint)listView->getCurSelectedIndex();
    uint id = readdoc["provinces"][index]["id"].GetInt();
    string name = readdoc["provinces"][index]["name"].GetString();
    
    GAMELOG("_selectedItemEvent 省份id = %d", id);
    
    if(tabtype == TAB_TYPE::TAB_CITY)
    {
        address_province = index;
        address_province_id = id;
        addressprovince_name = name;
    }
    else
    {
        mailaddress_province = index;
        mailaddress_province_id = id;
    }
    L_provinceList->setVisible(false);
    Button* button = (Button*)p_province->getChildByTag(B_PROVINCEBTN);
    button->setTitleText(name);
    
    setCityList(index);
    GAMELOG("_selectedItemEvent end");
}

void PersonCenterLayerEx::_city_selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);
    GAMELOG("select child end index = %ld", listView->getCurSelectedIndex());

//    p_city->setVisible(0);
//    p_province->setVisible(0);
    L_cityList->setVisible(false);
    
    
    uint index = (uint)listView->getCurSelectedIndex();
    setSelectCity(index);
    
}

void PersonCenterLayerEx::setSelectCity(int index)
{
    uint id;
    string name;
    
    if(tabtype == TAB_TYPE::TAB_CITY)
    {
        id = readdoc["provinces"][address_province]["citys"][index]["id"].GetInt();
        name = readdoc["provinces"][address_province]["citys"][index]["name"].GetString();
        address_city_id = id;
        address_city = index;
        addresscity_name = name;
//        Text* text = (Text*)rootNode->getChildByTag(T_CITY);
//        text->setString(getStringByIndex(T_CITY));
//        pNew->cityid = id;
//        pNew->provinceid = address_province_id;
//        pNew->cityname = readdoc["provinces"][address_province]["citys"][index]["name"].GetString();
//        pNew->provincename = readdoc["provinces"][address_province]["name"].GetString();
    }
    else
    {
        id = readdoc["provinces"][mailaddress_province]["citys"][index]["id"].GetInt();
        mailaddress_city_id = id;
        mailaddress_city = index;
        Button* button = (Button*)p_mailaddress->getChildByTag(B_MAILADDRESS_AREA);
        button->setTitleText(getStringByIndex(B_MAILADDRESS_AREA));
        pNew->postcityid = id;
        pNew->postprovinceid = mailaddress_province_id;
        pNew->postcityname = readdoc["provinces"][mailaddress_province]["citys"][index]["name"].GetString();
        name = pNew->postcityname;
        pNew->postprovincename = readdoc["provinces"][mailaddress_province]["name"].GetString();
    }
    GAMELOG("_city_selectedItemEvent 城市id = %d", id);
    GAMELOG("_city_selectedItemEvent end");
    
    Button* button = (Button*)p_province->getChildByTag(B_CITYBTN);
    button->setTitleText(name);
}

void PersonCenterLayerEx::setAddress(int tag)
{
    if(tabtype == TAB_TYPE::TAB_CITY)
    {
        
        pNew->cityid = address_city_id;
        pNew->provinceid = address_province_id;
        pNew->cityname = addresscity_name;
        pNew->provincename = addressprovince_name;
        
//        address_province
        
        
        Text* text = (Text*)rootNode->getChildByTag(T_CITY);
        text->setString(getStringByIndex(T_CITY));
        p_province->setVisible(false);
    }
    else
    {
        
    }
}

void PersonCenterLayerEx::setDefultProvince()
{
    address_city_id = pNew->cityid;
    address_province_id = pNew->provinceid;
    addresscity_name = pNew->cityname;
    addressprovince_name = pNew->provincename;
    
    address_province = getaddress_province(pNew->provincename);
    GAMELOG("address_province=%d   name = %s",address_province, pNew->provincename.c_str());
    setCityList(address_province, false);
    L_provinceList->setVisible(false);
    L_cityList->setVisible(false);
    
    
    
    Button* button = (Button*)p_province->getChildByTag(B_PROVINCEBTN);
    button->setTitleText(pNew->provincename);
    button = (Button*)p_province->getChildByTag(B_CITYBTN);
    button->setTitleText(pNew->cityname);
    
}

int  PersonCenterLayerEx::getaddress_province(string name)
{
    
    GAMELOG("name = %s",name.c_str());
    rapidjson::Value& provinces = readdoc["provinces"];
    string str;
    for(int i = 0; i < provinces.Size(); i ++)
    {
        str = provinces[i]["name"].GetString();
        if(str.compare(name) == 0)
        {
            return i;
        }
    }
    return -1;
}




bool PersonCenterLayerEx::isVauled(TextField* tf)
{
    if(!tf)
        return false;
    return (tf->getPlaceHolder().compare(tf->getString()) != 0);
}

void PersonCenterLayerEx::openMailaddress()
{
    Button* button;
//    Text* text;
    TextField* tf;
    
    tabtype = TAB_TYPE::TAB_MAILADDRESS;
    p_mailaddress->setVisible(YES);
    p_now = p_mailaddress;
    button = (Button*)p_mailaddress->getChildByTag(B_MAILADDRESS_AREA);
    button->setTitleText(getStringByIndex(B_MAILADDRESS_AREA));
    //真名
    tf = (TextField*)p_mailaddress->getChildByTag(TF_REALNAME);
    if(!isVauled(tf) && !_player->postname.empty())
    {
        tf->setString(_player->postname);
    }
    //手机号
    tf = (TextField*)p_mailaddress->getChildByTag(TF_PHONENUMBER);
    if(!isVauled(tf) && !_player->postphone.empty())
    {
        tf->setString(_player->postphone);
    }
    //收货人详细地址
    tf = (TextField*)p_mailaddress->getChildByTag(TF_MAILADDRESS);
    if(!isVauled(tf) && !_player->postaddress.empty())
    {
        tf->setString(_player->postaddress);
    }
}

void PersonCenterLayerEx::openGender()
{
    ImageView* man = (ImageView*)p_gender->getChildByTag(111);
    man->setVisible(0);
    ImageView* woman = (ImageView*)p_gender->getChildByTag(112);
    woman->setVisible(0);
    
    if(pNew->gender == 1)
        man->setVisible(YES);
    else
        woman->setVisible(YES);
    p_gender->setVisible(YES);
    p_now = p_gender;
}

bool PersonCenterLayerEx::checkYQHY()       //检测邀请好友是否添加
{
    if(!callMeFriend->getString().empty())
    {
        if(callMeFriend->getString().compare(_player->userid) == 0)
        {
            messageBox = UIMessage::createMesBox("邀请好友ID不能为自己的手机号！！！");
            return false;
        }
        if( IsPhoneNumber(callMeFriend->getString()))
        {
            pNew->myFriendId = callMeFriend->getString();
            callMeFriend->setEnabled(false);
            return true;
        }
        else
        {
            messageBox = UIMessage::createMesBox("邀请我的好友ID格式不正确，为手机号");
            return false;
        }
    }
    else
    {
        pNew->myFriendId = "";
    }
    return true;
}
string PersonCenterLayerEx::Gethead_img(){
    return heard_image;
}
void PersonCenterLayerEx::_EventCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget = %d", tag);

    if(tag >= 105 && tag <= 120)
    {
        p_head->setVisible(NO);
        pNew->headIndex = tag - 105;
        ImageView* image = (ImageView*)rootNode->getChildByTag(IMAGE_HEADICON);
        image->loadTexture(StringUtils::format("CSres/Setup/Head/headicon_%d.jpg", pNew->headIndex));
        heard_image=StringUtils::format("CSres/Setup/Head/headicon_%d.jpg", pNew->headIndex);
        return;
    }
    
    switch (tag)
    {
        case B_NICKNAME: //更改昵称界面
            p_nickname->setVisible(YES);
            p_now = p_nickname;
            
            break;
        case B_NICKNAMESAVE:
        {
            _player->http_request_nicknameInfo(TF_nickname->getString());
            
//            if (_player->_isexsits) {
//            UIMessage::createMesBox("当前名字已存在");
//                return;
//            }
//            p_nickname->setVisible(NO);
//            Director::getInstance()->getOpenGLView()->setIMEKeyboardState(0);
//            if(!TF_nickname->getString().empty())
//            {
//                Text* text = (Text*)rootNode->getChildByTag(T_NICKNAME);
//                text->setString(TF_nickname->getString());
//                pNew->nickname = TF_nickname->getString();
//            }
            
        }break;
        case AUTOCLOSE_NICKNAME:
        {
            p_nickname->setVisible(NO);
        }break;
            
        case B_GENDER:
        {
            openGender();
            
        }break;
        case B_GENDER_MAN:
        {
            pNew->gender = 1;
            setTextInfoByTag(T_GENDER);
            p_gender->setVisible(NO);
        }break;
        case B_GENDER_WOMAN:
        {
            pNew->gender = 0;
            setTextInfoByTag(T_GENDER);
            p_gender->setVisible(NO);
            
        }break;
            
        case B_HEAD:
        {
            p_head->setVisible(YES);
            p_now = p_head;
        }break;
            
        case AUTOCLOSE_HEAD:
        {
            p_head->setVisible(NO);
        }break;
        case AUTOCLOSE_GENDER:
        {
            p_gender->setVisible(NO);
        }break;
        case AUTOCLOSE_MAILADDRESS:
        {
            p_mailaddress->setVisible(NO);
        }break;
        case 366:
        {
            if (!checkYQHY()) {
                //is_TH=false;
                pNew->myFriendId=" ";
                //UserDefault::getInstance()->setBoolForKey("TOUCH", false);
                return;
            }
            _player->Invitation_code(pNew,callMeFriend->getString());
            //is_TH=true;
           //            if (error_code=0) {
//                messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_saved), 1);
//                messageBox->_args.code = INFO_SAVE;
//                isInfoChange = true;
//
//            }
            
            
        }break;
        case B_BACK:
        {
            if(_player->compare(pNew) == false || (!callMeFriend->getString().empty() && _player->myFriendId.empty()))//有修改项
                //            if(_player->compare(pNew) == false)//有修改项
            {
                if(messageBox == nullptr)
                {
                    messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_saved), 1);
                    messageBox->_args.code = INFO_SAVE;
                    isInfoChange = true;
                }
                else
                {
                    isInfoChange = false;
                }
                break;
            }
            

            isInfoChange = false;
            if(UICtrl::getInstance()->needModifyInfo())
            {
                UICtrl::getInstance()->poptoScene(UICtrl::getInstance()->lastSceneID);
            }
            IMessageManager::getInstance()->postNotification(MES_OPTION_CLOSE);
            UICtrl::getInstance()->gotoScene(UI_MAIN);
        }break;
            
        case B_MAILADDRESS:
        {
            openMailaddress();
        }break;
            
        case B_MAILADDRESSSAVE:
        {
            TextField* tf;
            
            tf = (TextField*)p_mailaddress->getChildByTag(TF_PHONENUMBER);
            if(!IsPhoneNumber(tf->getString()))
            {
                messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_phone));
                
                break;
            }
            if(isVauled(tf))
            {
                pNew->postphone = tf->getString();
            }
            
            tf = (TextField*)p_mailaddress->getChildByTag(TF_REALNAME);
            if(isVauled(tf) && !tf->getString().empty())
            {
                pNew->postname = tf->getString();
            }
            tf = (TextField*)p_mailaddress->getChildByTag(TF_MAILADDRESS);
            if(isVauled(tf) && !tf->getString().empty())
            {
                pNew->postaddress = tf->getString();
            }
            p_mailaddress->setVisible(NO);
            setTextInfoByIndex(T_MAILADDRESS);
            isChange = YES;
        }break;
            
        case B_PROVINCEBACK:
        {
            p_province->setVisible(NO);
            if(tabtype == TAB_TYPE::TAB_CITY)
            {
                address_city_id = -1;
                address_province_id = -1;
                
//                pNew->provinceid = pNew->cityid = -1;
                
            }
            else
            {
                mailaddress_city_id = -1;
                mailaddress_province_id = -1;
                
//                pNew->postcityid = pNew->postprovinceid = -1;
            }
            
        }break;
            
        case B_CITYBACK:
        {
            p_city->setVisible(NO);
        }break;
            
        case B_ADDRESS:
        {
            PersonCenterLayer* per=PersonCenterLayer::create();
            per->setDelegate(this);
            addChild(per,20);
            
//             _player->http_request_cityInfo(pNew);//城市链接
            tabtype = TAB_TYPE::TAB_CITY;
            
//            p_province->setVisible(YES);
            
            //初始化
//            setDefultProvince();
            ///////设置状态
            p_now = p_province;
        }break;
            
        case B_MAILADDRESS_AREA:
        {
            p_province->setVisible(YES);
            p_now = p_province;
        }break;
            
        case B_BIRTHDAYSAVE:
        {
            p_birthday->setVisible(NO);
            Button* b;
            b = (Button*)p_birthday->getChildByTag(B_YEAR);
            _year = atoi(b->getTitleText().c_str());
            b = (Button*)p_birthday->getChildByTag(B_MONTH);
            _month = atoi(b->getTitleText().c_str());
            b = (Button*)p_birthday->getChildByTag(B_DAY);
            _day = atoi(b->getTitleText().c_str());
            
            pNew->birthday = StringUtils::format("%d/%d/%d", _year, _month, _day);
            
            setTextInfoByTag(T_BRITHDAY);
        }break;
            
        case B_BIRTHDAY:
        {
            openCalendar();
        }break;
        case B_YEAR:
        {
            L_yearList->setVisible(!L_yearList->isVisible());
            L_monthList->setVisible(NO);
            L_dayList->setVisible(NO);
        }break;
        case B_MONTH:
        {
            L_yearList->setVisible(NO);
            L_monthList->setVisible(!L_monthList->isVisible());
            L_dayList->setVisible(NO);
        }break;
        case B_DAY:
        {
            L_yearList->setVisible(NO);
            L_monthList->setVisible(NO);
            L_dayList->setVisible(!L_dayList->isVisible());
        }break;
        case AUTOCLOSE_BIRTHDAY:
        {
            L_yearList->setVisible(NO);
            L_monthList->setVisible(NO);
            L_dayList->setVisible(NO);
            p_birthday->setVisible(NO);
        }break;
        case B_SAVE:
        {
            if(!checkYQHY())
            {
                break;
            }
            if(_player->compare(pNew) == false)
            {
                if(!pNew->myFriendId.empty())
                {
                    _player->myFriendId = pNew->myFriendId;
                }
                _player->http_request_updateInfo(pNew);
            }
            else
            {
               messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_notchange));
            }
        }break;
        case B_PROVINCEBTN:
        {
            setProvinceList();
        }break;
        case B_CITYBTN:
        {
            L_cityList->setVisible(true);
            L_cityList->jumpToTop();
        }break;
        case B_SAVE_ADDRESS:
        {
            setAddress(0);
        }break;
            
        default:
            break;
    }
    
    
}


void PersonCenterLayerEx::removeDialog(Ref *pSender, Widget::TouchEventType type )
{
    if (type == Widget::TouchEventType::ENDED)
    {
        setGoldUp(_player->golds);
    }
}





void PersonCenterLayerEx::GoldUp(Node *sender)
{
    string filename = "CSres/Setup/Prompt.csb";
    Node *moneyNode = CSLoader::createNode(filename);
    addChild(moneyNode, 111);
    auto moneyAction = CSLoader::createTimeline(filename);
    moneyNode->runAction(moneyAction);
    moneyAction->gotoFrameAndPlay(0, false);

}

void PersonCenterLayerEx::setGoldUp(int gold)
{
    int n = abs((gold - goldTmp)/20);
    for(int i = 0; i < n; i ++)
    {
        auto s = Sequence::create(DelayTime::create(i*0.2),
                                  CallFuncN::create(CC_CALLBACK_1(PersonCenterLayerEx::GoldUp, this)), NULL);
        runAction(s);
    }
    goldTmp = gold;
}



bool PersonCenterLayerEx::Notify(const string& name, Args& args)
{  
    if(IUILayer::Notify(name, args))
        return true;
    
    Notify_Compare(MES_PLAYER_UPDATEINFO)
    {
//        setTextInfoByTag(T_MAILADDRESS);
        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_changesuc));
        messageBox->_args.code = BUTTON_SAVE;
        return YES;
    }
    Notify_Compare(MES_PLAYER_nickname)
    {
       
        //IPlayerData * player;
        PlayerData player;
        CCLOG("%d",player.Getisexsits());
        if (player.Getisexsits()==1) {
         string  str= StringUtils::format("当前名字已经存在,建议叫:%s",player.Getnickname().c_str());

            UIMessage::createMesBox(str);
            TF_nickname->setString(_player->_nickname.c_str());
            return false;
        }
        p_nickname->setVisible(NO);
        Director::getInstance()->getOpenGLView()->setIMEKeyboardState(0);
        if(!TF_nickname->getString().empty())
        {
            Text* text = (Text*)rootNode->getChildByTag(T_NICKNAME);
            text->setString(TF_nickname->getString());
            pNew->nickname = TF_nickname->getString();
        }

        return YES;
    }
    
    
    
    
    Notify_Compare(MESSAGEBOX_OK)
    {
        switch (args.code)
        {
            case BUTTON_SAVE:
            {
                setGoldUp(_player->golds);
            }break;
            case INFO_SAVE:
            {
                if(!checkYQHY())
                {
                    break;
                }
                if(!pNew->myFriendId.empty())
                {
                    _player->myFriendId = pNew->myFriendId;
                }
                _player->http_request_updateInfo(pNew);
            }break;
            default:
                break;
        }
        
        messageBox = nullptr;
        return YES;
    }
    Notify_Compare(MESSAGEBOX_CANCEL)
    {
        switch (args.code)
        {
            case INFO_SAVE:
            {
                if(UICtrl::getInstance()->lastSceneID == UI_ROOM)
                {
                    UICtrl::getInstance()->poptoScene(UI_ROOM);
                }
                IMessageManager::getInstance()->postNotification(MES_OPTION_CLOSE);
            }break;
            default:
                break;
        }
        
        messageBox = nullptr;
        return YES;
    }
    
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        Ref *pSender = rootNode->getChildByTag(B_BACK);
        
        if(p_nickname->isVisible())
        {
            pSender = p_nickname->getChildByTag(AUTOCLOSE_NICKNAME);
        }
        else if(p_gender->isVisible())
        {
            pSender = p_gender->getChildByTag(AUTOCLOSE_GENDER);
        }
        else if(p_head->isVisible())
        {
            pSender = p_head->getChildByTag(AUTOCLOSE_HEAD);
        }
        else if(p_birthday->isVisible())
        {
            pSender = p_birthday->getChildByTag(AUTOCLOSE_BIRTHDAY);
        }
        else if(p_province->isVisible())
        {
            pSender = p_province->getChildByTag(B_PROVINCEBACK);
        }
        if(pSender)
            _EventCallBack(pSender, Widget::TouchEventType::ENDED);
        
        if(messageBox != nullptr && !isInfoChange)
        {
            messageBox->close();
            messageBox = nullptr;
            IMessageManager::getInstance()->postNotification(MES_OPTION_CLOSE);
        }
        return YES;
    }
    
    return true;
}






