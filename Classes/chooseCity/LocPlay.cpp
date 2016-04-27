//
//  LocPlay.cpp
//  PinLe
//
//  Created by Kennedy on 16/4/2.
//
//

#include "LocPlay.h"
#include "cocos2d.h"
#include "../framework/config.h"
using namespace cocos2d;

static LocPlay* locPlay=NULL;

LocPlay* LocPlay::getInstance()
{
    if(locPlay == NULL)
    {
        locPlay = new LocPlay();
        locPlay->setCity("");
        locPlay->setConty("");
        locPlay->setProvince("");
        locPlay->setIs_iphone(true);
    }
    return locPlay;
}


void LocPlay::destroyInstance()
{
    _DELETE(locPlay);
}


void LocPlay::setCity(string city)
{
    if (city=="")
        return;
        
    string::size_type   pos(0);
    string str="市";
    if( (pos=city.find(str))!=city.npos)
        city=city.replace(pos,str.length(),"");
    
    lastCity=city;
    IMessageManager::getInstance()->postNotification("HTTP_POS_CITY");
}

string LocPlay::getCity()
{
    return lastCity;
}

void LocPlay::setProvince(string province)
{
    
    string::size_type   pos(0);
    string str="省";
    string str_1="市";
    if( (pos=province.find(str))!=province.npos)
        province=province.replace(pos,str.length(),"");
    if( (pos=province.find(str_1))!=province.npos)
        province=province.replace(pos,str.length(),"");
    
    lastProvince=province;
}

string LocPlay::getProvince()
{
    return lastProvince;
}

void LocPlay::setConty(string conty)
{
    lastConty=conty;
}

string LocPlay::getConty()
{
    return lastConty;
}