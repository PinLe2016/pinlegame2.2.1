//
//  Activity.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__Activity__
#define __framework__Activity__

#include "cocos2d.h"
#include "Advertise.h"
#include "../../player/module/Player.h"

using namespace std;

enum ACTIVITY_TYPE
{
    nomal_type = 0,
    onece_type,
};



class ActivityResult : public Player
{
public:
//    int64_t         ranking;                //  当前排名(未参与为0)
    int64_t         result;                 //  最佳完成时间(单位：毫秒)
    string          adUrl;                  //  广告图片URL
    int             playerIntegral;         //玩家活动积分总数
};


class ActivityRanking     //排行榜
{
public:
    ActivityRanking();
    ~ActivityRanking();
    
    static  ActivityRanking* getInstance();
    static  void destroyInstance();
    
public:
    int     pageMax;                        //最多显示页
    int     pageIndex;                      //当前显示页
    int     pageSize;                       //一页显示最多内容
    vector<ActivityResult>& getRankingList();       //排行榜列表
};




class ActivityMatchInfo //活动参与比赛信息
{
public:
    ActivityMatchInfo();
    ~ActivityMatchInfo();
    
    int             number;                 //  已参与人数
    int             isplayed;               //  是否已通关(0未通关1已通关)
    int             totaltime;              //  总参与次数
    int             curtime;                //  已参与次数
    int             needgold;               //  参与费(每次参与花费金币不同)
    int             completecount;          //  已拼图数量
    int             totalcount;             //  总拼图数量
    int64_t         ranking;                //  当前排名(未参与为0)
    int             prizewinning;           //  该活动玩家是否中奖，中的几等奖
    int             isswardprize;           //  奖品是否已经发放 1－发奖， 0－未发
    int             Myrank =0;                 //  总排名
    int             myintegralOnc = 0;         //  本次获得积分
    int             bestpoints = 0;             //  本周期最高积分
    int             myintegral = 0;             //  总积分
    int             currentrank = 0;            //  最佳排名
    int             betgolds;                  //老虎机押注金币数
    int            act_type;                   //惊喜吧游戏活动类型
    
    vector<ActivityResult>  v_MyResult;     //  我的成绩
    vector<ActivityResult>  v_Champion;     //  活动排行榜信息（目前是前三名）
    vector<ActivityResult>  v_Winning;      //获奖名单
    vector<ActivityResult>  v_LookWinning;  //奖品抢先看
    vector<ActivityResult>  v_Integral;     //积分排行榜
    vector<ActivityResult>  v_Ranking;
    vector<ActivityResult>  v_RankingRecord;
    vector<int>             needGold;       //  参加活动每次需要的金币数
};

class ActivityPrize //活动奖品信息
{
public:
    string      url;                //奖品图片地址
    string      name;               //奖品名称
    string      describe;           //奖品描述
    int         amount;             //奖品数量
    int       activity_Type;        //周 年 月
    string       activityid;                 //活动id
    int         _prizewinning;    //是否中奖 0未获奖，1已获奖
    int         _isswardprize;    //是否发奖
};

class Activity
{
public:
    Activity();
    ~Activity();
    string          id;                     //  活动id
    string          ownerid;                //  商家id
    string          ownername;              //  商家名称
    string          title;                  //  活动标题
    string          description;            //  描述
    string          awardsummary;           //  奖励描述
    string          requirementsummary;     //  任务要求描述
    int64_t          begintime;              //  开始时间
    int64_t          finishtime;             //  结束时间
    int64_t          currenttime;             //  当前时间
    string          ownerurl;               //  商家图片URL
    string          goodsurl;               //  奖励图片url
    string          adweburl;               //  广告商的宣传网址
    long            remainingtime;          //  剩余时间		秒
    int             status;                 //  状态		0未开始1已开始2已结束
    int             remains;                //  奖品剩余数量(已废弃)
    int             activitytype;          //  活动类型（一次性活动，还是正常活动）
    int             Activity_type;         //活动类型（0年1月2周3日4整点5热门）
    string          title_name;                   //标题名字
    vector<Advertise*> v_advertise;         //  广告套图信息队列
    vector<ActivityPrize*> v_activityPrize; //  奖品信息
    
    vector<string>  hdGuiZeVec;             //  活动规则说明
    
    ActivityMatchInfo   matchInfo;
    public:
    void            addAdvertise(Advertise* ad);         //  加入套图(已废弃)
    string          sumitRewardUrl;
};







#endif /* defined(__framework__Activity__) */
