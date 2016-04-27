//
//  PluginHelper.h
//  PinLe
//
//  Created by 李辛 on 15/6/17.
//
//

#ifndef PinLe_PluginHelper_h
#define PinLe_PluginHelper_h
#include "cocos2d.h"


class PluginHelper
{
public:
    PluginHelper();
    ~PluginHelper();
    static  PluginHelper *getInstance();
    void    *viewController;
    void    getPlayerPosition();
    void    getCity();
    void    getProvince();
    void    getConty();
    void    phonecall(const std::string& number);
    void    comment(const std::string& appID);

};

#endif
