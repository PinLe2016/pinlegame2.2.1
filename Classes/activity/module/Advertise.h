//
//  Advertise.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__Advertise__
#define __framework__Advertise__

#include "cocos2d.h"
using namespace std;

class Advertise
{
public:
    Advertise();
    string          adid;               //  广告id
    string          ownerid;            //  广告主(商家)id
    string          imgurl;             //  原图Url
    string          logourl;            //  缩略图Url
    string          rewardtitle;        //  实物奖励标题
    string          rewardurl;          //  实物奖励url
    uint64_t        datetime;           //  创建时间
};

#endif /* defined(__framework__Advertise__) */
