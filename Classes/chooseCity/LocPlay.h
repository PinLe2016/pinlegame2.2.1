//
//  LocPlay.hpp
//  PinLe
//
//  Created by Kennedy on 16/4/2.
//
//

#ifndef LocPlay_h
#define LocPlay_h

#include <stdio.h>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

#define _DELETE(p)              do { if(p) { delete (p); (p) = nullptr; } } while(0)

class LocPlay :public Ref
{
public:
    static  LocPlay* getInstance();
    static  void destroyInstance();
public:
    void    setCity(string  city);
    string    getCity();
    
    void    setProvince(string  province);
    string    getProvince();
    
    void    setConty(string  conty);
    string    getConty();
    
    CC_SYNTHESIZE(bool, is_iphone, Is_iphone);
    CC_SYNTHESIZE(string, beforCity, beforCity);
public:
    string          lastCity;              //城市信息
    string          lastProvince;          //省
    string          lastConty;              //区
    
//    string          beforCity;
};

#endif /* LocPlay_hpp */
