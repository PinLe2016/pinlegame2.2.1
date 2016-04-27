//
//  ActivityData.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__ActivityData__
#define __framework__ActivityData__

#include "../IActivityData.h"

class ActivityData : public IActivityData
{
public:
    ActivityData();
    ~ActivityData();
    string getstr_url();
    static  void    init();
    static  void    destroy();
    int getjx_nun();
    int getw_jifen();
    int getw_paiming();
    vector<string>  get_guize();
    void        cleanActivityList();
    void        addActivity(int type, Activity* a);
    void        setSelectedActivity(int type, uint index);
    Activity*   getActivity(int type, uint index);
    Activity*   getSelectedActivity();
    vector<ActivityResult>& getRankingList();
    //显示
    void    Eventdetails();
    //请求活动列表
    void    http_request_activityList(int type = 0);
    //获取活动列表
    void    http_response_activityList(const string& s);
    //请求活动详细信息
    void    http_request_activityInfo(const string& id);
    //获取活动详细信息
    void    http_response_activityInfo(const string& s);
    //请求活动排行榜信息
    void    http_request_activityRankingList(const string& id, uint pageIndex);
    //积分排行榜
    void    http_request_integralRankingList(const string& id, const string& name);
    void    http_request_RankingRecordList(const string& id, const string& name);
    //获取活动排行榜信息
    void    http_response_activityRankingList(const string& s);
    //获取积分排行榜
    void    http_response_integralRankingList(const string& s);
    void    get_response_RankingRecordList(const string& s);
    //获奖名单
    void    http_request_WinnersList(const string& id, const string& name);
    //获取获奖名单
    void    http_response_GetWinnersList(const string& s);
    //奖品抢先看
    void    http_request_FirstlookList(const string& id, const string& name);
    //获取奖品抢先看
    void    http_response_GetFirstlookList(const string& s);
//======================================================================
private:
    void    http_response_downloadimage();
    int     getActivitCount();
    
    uint    _downloadNum;
    uint    _selectedType;
    uint    _selectedIndex;
    
};




#endif /* defined(__framework__ActivityData__) */
