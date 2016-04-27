//
//  Broadcast.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#include "Broadcast.h"
#include "../../framework/config.h"

Broadcast::Broadcast()
{
    id = "";
    content = "";
    linkstr = "";
    starttime = "";
    endtime = "";
    color3B = Color3B::BLACK;
    type = Broadcast::TYPE::normal;
    tag  = 0;
    existtime = 0;
    used = false;
    isResponse = false;
}
Broadcast::~Broadcast()
{
}

bool Broadcast::isOnce()
{
    if(starttime.empty() || endtime.empty() || starttime.compare(endtime) == 0)
        return true;
    else
        return false;
}

bool Broadcast::isValid()
{
    if(isOnce())
    {
        if(!used)
            return true;
        else
            return false;
    }
    time_t timenow = time(nullptr);//获取日历时间
    if(timenow > ITools::strtotime(endtime.c_str()))
        return false;
    return true;
}

Broadcast* Broadcast::copy()
{
    Broadcast* b = new Broadcast();
    b->id = id;
    b->title = title;
    b->content = content;
    b->linkstr = linkstr;
    b->starttime = starttime;
    b->endtime   = endtime;
    b->lastplay = lastplay;
    b->color3B = color3B;
    b->type = type;
    b->tag = tag;
    b->used = used;
    b->existtime = existtime;
    b->isResponse = isResponse;
    return b;
}













