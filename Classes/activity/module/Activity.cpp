//
//  Activity.cpp
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#include "Activity.h"
#include "../../framework/tools/ITools.h"

static ActivityRanking* _activityRanking = NULL;
static vector<ActivityResult> v_aRanking;         //  广告套图信息队列

Activity::Activity()
{
    }

Activity::~Activity()
{
    VECTOR_DELETE(v_advertise);
    VECTOR_DELETE(v_activityPrize);
}


void Activity::addAdvertise(Advertise* ad)
{
    if(ad)
        v_advertise.push_back(ad);
}

vector<ActivityResult>& ActivityRanking::getRankingList()
{
    return v_aRanking;
}




ActivityRanking::ActivityRanking()
{
    pageMax = 0;
    pageIndex = 0;
    pageSize = 0;
}

ActivityRanking::~ActivityRanking()
{
    v_aRanking.clear();
}


ActivityRanking* ActivityRanking::getInstance()
{
    if(_activityRanking == NULL)
    {
        _activityRanking = new ActivityRanking();
    }
    return _activityRanking;
}

void ActivityRanking::destroyInstance()
{
    SAFE_DELETE(_activityRanking);
}

ActivityMatchInfo::ActivityMatchInfo()
{
    betgolds=20;
    totaltime=2000;
    curtime=0;
    myintegral = 0;
}

ActivityMatchInfo::~ActivityMatchInfo()
{
    v_MyResult.clear();
    v_Champion.clear();
}








