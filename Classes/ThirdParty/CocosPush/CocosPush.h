//
//  CocosPush.h
//  PinLe
//
//  Created by Kennedy on 15/8/18.
//
//

#ifndef PinLe_CocosPush_h
#define PinLe_CocosPush_h

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/CCPushService.h"
#else
    #include "android/CCPushService.h"
#endif


class CocosPush
{
public:
    static std::list<std::string> SplitString(std::string str, char split)
    {
        std::list<std::string> strGet;
        std::string  recogScop = str;
        int64_t pos = 0;
        //    std::list<int> list;
        //    list.push_back("123");
        //    list.push_back("ddd");
        
        for (int64_t i = 0, j = 0; i < str.length(); i ++,j ++)
        {
//            CCLog("i%d",i);
            pos = recogScop.find(",", i);
            if(pos == -1)
            {
                std::string subEnd = recogScop.substr(i, str.length() - i); //最后一个字符串
                strGet.push_back(subEnd);
                break;
            }
            //        "daa,www,sss,uyu"
//            CCLog("sada%d",pos);
            //        i=pos;
            //        list.push_back(pos);
            if(pos > 0)
            {
                std::string sub = recogScop.substr(i, pos-i);
                strGet.push_back(sub);
                i = pos;
            }
        }
        for (std::list<std::string>::iterator itr = strGet.begin() ; itr != strGet.end(); itr ++)
        {
            std::string str = *itr;
//            CCLog("str..%s",str.c_str());
        }
        return strGet;
        
    }
    
    static std::list<std::string> getStrTag(const std::string& str)
    {
        return SplitString(str, ',');
    }

};




#endif
