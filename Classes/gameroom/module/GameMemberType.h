//
//  GameMemberType.h
//  Pinle
//
//  Created by 李辛 on 15/6/2.
//
//

#ifndef Pinle_GameMemberType_h
#define Pinle_GameMemberType_h


class LevelType
{
public:
    int itypeIndex;//类型
    int iscore;//奖励积分
    int igold;//奖励金币
    int ilimitTime;//限时时间
    int iyajin;//玩家押金
};

class ActivityRewardInfo            //活动奖励对象
{
public:
    string goodurl;    //奖品图片的url
    string goodname;   //奖品名称
    string loginname;   //玩家ID
    string phone;    //玩家手机号
    string playername; //玩家姓名
    string address;  //玩家地址
    string goodid;  //奖品编号
    string adweburl;    //了解详情跳转URL
    bool over = false;
};


class EnemyData             //匹配赛对象
{
public:
    string u_pvp_Name;                   //对方名字
    string u_pvp_Id;                    //对方ID
    unsigned int u_pvp_Score;            //对方积分
    unsigned int u_pvp_Gold;             //对方金币
    int u_pvp_HeadIndex;                //对方头像地址
    int u_pvp_Run;                       //对方走的步数
    int u_pvp_TimeCount;                 //对方使用的时间
    int ChengGong;                       //是否成功，0是失败，1是成功
    bool u_pvp_winer;
    int u_pvp_addGold;
    string u_pvp_headUrl;
    string u_pvp_randName;         //等级
};



#endif
