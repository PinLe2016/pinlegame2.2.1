//
//  IActivityData.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef framework_IActivityData_h
#define framework_IActivityData_h


#include "../framework/config.h"
#include "module/Activity.h"


enum  E_ActivityType
{
    unstart,                //未开始
    start,                  //进行中
    done,                   //已结束
    end
};

class IActivityData : public IData
{
public:
    virtual void        cleanActivityList(){}
    virtual void        addActivity(int type, Activity* a){}
    //获取活动信息
    virtual Activity*   getActivity(int type, uint index) = 0;
    virtual Activity*   getSelectedActivity() = 0;
    virtual void        setSelectedActivity(int type, uint index) = 0;
    //排行榜列表 (当前活动排行榜列表)
    virtual vector<ActivityResult>& getRankingList() = 0;
    
    //请求获取活动专区列表
    virtual void    http_request_activityList(int type = 0){}
    //获取活动列表
    virtual void    http_response_activityList(const string& s){}
    //请求活动详细信息
    virtual void    http_request_activityInfo(const string& id){}
    //获取活动详细信息
    virtual void    http_response_activityInfo(const string& s){}
    //请求活动排行榜信息
    virtual void    http_request_activityRankingList(const string& id, uint pageIndex){}
    //积分排行榜
    virtual void    http_request_integralRankingList(const string& id, const string& name){}
    virtual void    http_request_RankingRecordList(const string& id, const string& name){}

    //获取活动排行榜信息
    virtual void    http_response_activityRankingList(const string& s){}
    //获取积分排行榜
    virtual void    http_response_integralRankingList(const string& s){}
    virtual void    get_response_RankingRecordList(const string& s){}
    //获奖名单
    virtual void    http_request_WinnersList(const string& id, const string& name){}
    //获取获奖名单
    virtual void    http_response_GetWinnersList(const string& s){}
    //奖品抢先看
    virtual void    http_request_FirstlookList(const string& id, const string& name){}
    //获取奖品抢先看
    virtual void    http_response_GetFirstlookList(const string& s){}
    //显示详情页面
    virtual void    Eventdetails(){}
    
    
//======================================================================
public:    
    vector<Activity*> v_activityList[E_ActivityType::end];                   //活动列表
    int             postWay;                //  邮寄方式
    string          rewardUrl;               //奖品URL，根据url来判断来对玩家进行发送奖品
    //string       Activity_status;   //活动状态
};













#endif
