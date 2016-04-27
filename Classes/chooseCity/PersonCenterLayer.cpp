//
//  PersonCenterLayer.cpp
//  PinLe
//
//  Created by Kennedy on 16/3/31.
//
//

#include "PersonCenterLayer.h"
#include "../option/config.h"
#include "ui/UIScrollView.h"
#include "LocPlay.h"
#define _play           LocPlay::getInstance()
using namespace cocos2d;


PersonCenterLayer::PersonCenterLayer()
{
    m_offset_cell=0;
    is_iphone=false;
    iplayer=M_GET_PLAYER;
    IMessageManager::getInstance()->attach(this, "POS_PROVEINCE");
    IMessageManager::getInstance()->attach(this, "POS_CITY");
    IMessageManager::getInstance()->attach(this, "HTTP_POS_CITY");

}
PersonCenterLayer::~PersonCenterLayer()
{
    IMessageManager::getInstance()->detach(this, "POS_PROVEINCE");
    IMessageManager::getInstance()->detach(this, "POS_CITY");
    IMessageManager::getInstance()->detach(this, "HTTP_POS_CITY");
}

bool PersonCenterLayer::init()
{
    if ( !IUILayer::init() )
    {
        return false;
    }

    
    Size size_=Director::getInstance()->getWinSize();
  
    p_province = CSLoader::createNode("CSres/Setup/CityInformation.csb");
    p_province->setAnchorPoint(Vec2(0.5, 0.5));
    p_province->setPosition(Vec2(size_.width/2, size_.height/2));
    this->addChild(p_province);
    
    //关闭按钮
    Button* button_close=(Button*)p_province->getChildByTag(_CLOSE);
    button_close->addTouchEventListener(CC_CALLBACK_2(PersonCenterLayer::_EventCallBack, this));
    //保存按钮
    Button * button =(Button*)p_province->getChildByTag(_SAVE_DATA);
    button->setLocalZOrder(4);
    button->addTouchEventListener(CC_CALLBACK_2(PersonCenterLayer::_EventCallBack, this));
    //刷新
    Button * button_reset =(Button*)p_province->getChildByTag(_RESET);
    button_reset->setVisible(false);
//    button_reset->addTouchEventListener(CC_CALLBACK_2(PersonCenterLayer::_EventCallBack, this));

    
    ssize_t size = 0;
    std::string load_str;
    //CSres/Setup/FreeRes/
    unsigned char* titlech = FileUtils::getInstance()->getFileData("CSres/Setup/FreeRes/city.json", "r", &size);
    load_str = std::string((const char*)titlech,size);
    readdoc.Parse<0>(load_str.c_str());
    
    //头标字体
    m_title=(Text*)p_province->getChildByTag(2234)->getChildByTag(_TITLE);
    m_Belonging=(Text*)p_province->getChildByTag(2232)->getChildByTag(2233);
    
    
    ui::ScrollView* scroll_province=(ui::ScrollView*)p_province->getChildByTag(_PROVINCE);
    addChild(scroll_province);
    
    ui::ScrollView* scroll_city=(ui::ScrollView*)p_province->getChildByTag(_CITY);
    addChild(scroll_city);
    
    ui::ScrollView* scroll_conty=(ui::ScrollView*)p_province->getChildByTag(_CONTY);
    addChild(scroll_conty);
    
    
    L_provinceList=addItemPickerData(scroll_province,ModeType::S_PROVINCE);
    L_cityList=addItemPickerData(scroll_city,ModeType::S_CITY);
    L_county=addItemPickerData(scroll_conty,ModeType::S_CONTY);
    p_province->addChild(L_provinceList);
    p_province->addChild(L_cityList);
    p_province->addChild(L_county);
//    L_provinceList->addEventListenerScrollView(this, scrollvieweventselector(PersonCenterLayer::_selectedItemEvent));//
//    L_cityList->addEventListenerScrollView(this, scrollvieweventselector(PersonCenterLayer::_city_selectedItemEvent));
    
    changTitle();
}

void PersonCenterLayer::changTitle()
{
//    CCLOG(" pro %s %s  %s  ",_play->getProvince().c_str(),_play->getCity().c_str(),_play->getConty().c_str());
    
    if(_play->getIs_iphone())
    {
        is_iphone=true;
       Player* pNew = new Player();
        //请求手机归属地
        iplayer->http_request_cityInfo(pNew);//城市链接
    }
    else
    {
        string addres=_play->getProvince()+_play->getCity()+_play->getConty();
        m_title->setString(addres);
        m_Belonging->setString("定位城市");
        setProvinceList();
    }
    
    
}

ItemPicker * PersonCenterLayer::addItemPickerData(ui::ScrollView *scorll,ModeType type)
{
    
    ItemPicker*picker =ItemPicker::create();
    picker->setDirection(ItemPicker::Direction::VERTICAL);
    picker->setContentSize(Size(220, 34*50));
    picker->setPosition(scorll->getPosition());
    picker->setSize(scorll->getSize());
//    picker->setBounceEnabled(false);
    picker->setType(type);
    scorll->removeFromParent();
    return picker;
}

void PersonCenterLayer::_city_selectedItemEvent(Ref *pSender, ScrollviewEventType type)
{
    if(type == ScrollviewEventType::SCROLLVIEW_EVENT_SCROLLING)
        return;
    
//    IAudio::getInstance()->playEffect(music_ef_ok);
    
//    ItemPicker* listView = static_cast<ItemPicker*>(pSender);
//    CC_UNUSED_PARAM(listView);
    
    
}


void PersonCenterLayer::_selectedItemEvent(Ref *pSender, ScrollviewEventType type)
{
    if(type == ScrollviewEventType::SCROLLVIEW_EVENT_SCROLLING)
        return;
    
//    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ItemPicker* listView = static_cast<ItemPicker*>(pSender);
    CC_UNUSED_PARAM(listView);

}


void PersonCenterLayer::setProvinceList()
{
    L_provinceList->clearItems();
    L_provinceList->removeAllChildren();
    CCLOG("province  %s",_play->getProvince().c_str());
    rapidjson::Value& provinces = readdoc["provinces"];
    for(int i = 0; i <provinces.Size()+7; i++)
    {
        Layout* button = Layout::create();
        button->setSize(Size(140, 50));
        button->setColor(Color3B(150,150,255));
        string name;
        if(i<provinces.Size()+3 and i-3>=0){
            Text* text=Text::create();
            text->setFontSize(30);
            text->setString(provinces[i-3]["name"].GetString());
            text->setPosition(Vec2(23, 22));
//            text->setTextHorizontalAlignment(TextHAlignment::LEFT);
            text->setAnchorPoint(Vec2(0,0.5));
            button->addChild(text);
            name=provinces[i-3]["name"].GetString();
           
            if (name.find(_play->getProvince().c_str())!=name.npos ) {
                m_offset_cell=i-3;
            }
          }
        L_provinceList->addChild(button);
        L_provinceList->pushBackItem(button);
        
    }
    
    L_provinceList->setOffsetLayout(m_offset_cell);
}

void PersonCenterLayer::setCityList(int index, bool b)
{
    
    L_cityList->clearItems();
    L_cityList->removeAllChildren();
    rapidjson::Value& provinces = readdoc["provinces"];
    
//    GAMELOG("index = %d  size = %d", index, provinces.Size());
    
    int offset_cell=0;
    if(provinces.IsArray() && index < provinces.Size())
    {
        rapidjson::Value& citys = provinces[index]["citys"];
        if(citys.IsArray())
        {
//            GAMELOG("citys.Size = %d", citys.Size());
            for(int i = 0; i <citys.Size()+7 ; i++)
            {
                
                Layout* button = Layout::create();
                button->setSize(Size(365, 50));
                button->setColor(Color3B(150,150,255));
                if(i<citys.Size()+3 and i-3>=0){
                    Text* text=Text::create();
                    text->setFontSize(30);
                    text->setString(citys[i-3]["name"].GetString());
                    text->setPosition(Vec2(180, 22));
                    button->addChild(text);
//                    text->setAnchorPoint(Vec2(0,0.5));
                    string name=citys[i-3]["name"].GetString();
                    string sub = name.substr(name.find("（")+1 ,name.find("）")-name.find("（"));
                    name.replace(name.find("（")+1, sub.length(), "");
//                    CCLOG("name %s",name.c_str());
                    if (name.find(_play->getCity().c_str())!=name.npos ) {
                        offset_cell=i-3;
                    }
                }

                L_cityList->addChild(button);
                L_cityList->pushBackItem(button);
            }
        }
    }
     L_cityList->setOffsetLayout(offset_cell);
    L_cityList->m_bNeedRemedy=true;
}

void PersonCenterLayer::setConty()
{
    L_county->clearItems();
    L_county->removeAllChildren();
    rapidjson::Value& provinces = readdoc["provinces"];
    GAMELOG("L_cityList->getCellPos() = %d", L_cityList->getCellPos());
    int offset_cell=0;
    if(!provinces.IsArray()) return;

    rapidjson::Value& citys = provinces[L_provinceList->getCellPos()]["citys"];
    if (!citys.IsArray() ||(citys.IsArray() and citys.Size()==0) ) return;
    rapidjson::Value& conty = citys[L_cityList->getCellPos()]["areas"];
    
    if (!conty.IsArray()) return;
    
    if( conty.Size()!=0)
    {
        GAMELOG("conty = %d", conty.Size());
        for(int i = 0; i <conty.Size()+7 ; ++i)
        {
            
            Layout* button = Layout::create();
            button->setSize(Size(220, 50));
            button->setColor(Color3B(150,150,255));
            if(i-3<conty.Size()&& i-3>=0){
                Text* text=Text::create();
                text->setFontSize(30);
                text->setString(conty[i-3]["name"].GetString());
                text->setPosition(Vec2(33, 22));
                button->addChild(text);
                text->setAnchorPoint(Vec2(0,0.5));
                string name=conty[i-3]["name"].GetString();
                if (name.find(_play->getConty().c_str())!=name.npos ) {
                    offset_cell=i-3;
                }
            }
            
            L_county->addChild(button);
            L_county->pushBackItem(button);
        }
    }
    L_county->setOffsetLayout(offset_cell);
    L_county->m_bNeedRemedy=true;
}

void PersonCenterLayer::_EventCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    
    if (tag==_SAVE_DATA)
    {
        
        rapidjson::Value& provinces = readdoc["provinces"];
        if(!provinces.IsArray()) return;
        rapidjson::Value& citys = provinces[L_provinceList->getCellPos()]["citys"];
        if (!citys.IsArray() ||(citys.IsArray() and citys.Size()==0) ) return;
        rapidjson::Value& conty = citys[L_cityList->getCellPos()]["areas"];
        if (!conty.IsArray()) return;
        
        if( conty.Size()!=0)

            
        delegate->address_province_id = provinces[L_provinceList->getCellPos()]["id"].GetInt();
        delegate->address_city_id = citys[L_cityList->getCellPos()]["id"].GetInt();
        delegate->pNew->cityname = citys[L_cityList->getCellPos()]["name"].GetString();
        delegate->pNew->provincename = provinces[L_provinceList->getCellPos()]["name"].GetString();
        delegate->pNew->cityid=delegate->address_city_id;
        delegate->pNew->provinceid=delegate->address_province_id;
        delegate->addresscity_name=delegate->pNew->cityname;
        delegate->addressprovince_name=delegate->pNew->provincename;
        
        Text* text = (Text*)delegate->rootNode->getChildByTag(84);
        string str=StringUtils::format("%s-%s",delegate->pNew->provincename.c_str(),delegate->pNew->cityname.c_str());
        text->setString(str);

    }

//    _play->setProvince("");
    this->removeFromParentAndCleanup(true);
}


bool PersonCenterLayer::Notify(const string& name, Args& args)
{
    if(name.compare("POS_PROVEINCE") == 0)
    {
        setCityList(L_provinceList->getCellPos());
        return true;
    }
    Notify_Compare("POS_CITY")
    {
        setConty();
        return  true;
    }
    Notify_Compare("HTTP_POS_CITY")
    {
        if (_play->getIs_iphone()) {
            
            string addres=_play->getProvince()+_play->getCity()+_play->getConty();
            m_title->setString(addres);
            setProvinceList();
        }else{
            
            changTitle();
        }
        return  true;
    }

    return true;
}

