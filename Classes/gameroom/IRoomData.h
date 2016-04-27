//
//  iRoomData.h
//  Pinle
//
//  Created by 李辛 on 15/6/1.
//
//

#ifndef Pinle_iRoomData_h
#define Pinle_iRoomData_h

#include "../framework/config.h"
#include "module/GameLevelSprite.h"
#include "module/GameMemberType.h"


enum GameMode//游戏模式
{
    daily_simple = 1,               //日常拼图-简单
    daily_hard,                     //日常拼图-复杂(废弃)
    unlockpuzzle,                   //解锁拼图(废弃)
    challenge_simple,               //个人挑战赛-简单
    challenge_hard,                 //个人挑战赛-复杂
    pvp_2,                          //2人竞技赛
    pvp_4,                          //4人竞技赛
    pvp_friends,                    //好友约战(暂未开放)
    activity,                       //活动专区
};


class IRoomData : public IData
{
public:
    unsigned int ChaKan_cishu = 3;
    vector<IRoomData > roomdata_vec;
public:
    virtual void commomGameStart(){}//对战游戏结束发送数据
    virtual void enterChoiceType(){}
    virtual void setGameInfo(GameLevelSprite *gameLevelSprite){}//设置拼图信息
    virtual GameLevelSprite *getGameInfo(){return nullptr;}    //获取拼图信息
    virtual void setGameSuccess(bool b){}  //设置游戏是否成功
    virtual bool isSuccess(){return false;}    //获取游戏是否成功
    virtual void setTimeCount(int time){} //设置游戏消耗时间
    virtual int getTimeCount(){return 0;}  //获取游戏拼图时间
    virtual void setRunCount(int _run){}   //设置游戏拼图所用步数
    virtual int getRunCount(){return 0;}   //获取游戏拼图所用步数
    virtual void setRewardInfo(ActivityRewardInfo *activityRewardInfo){};   //设置完成活动结束奖励信息
    virtual ActivityRewardInfo *getRewardInfo(){return nullptr;}   //获取完成活动结束奖励信息
    virtual void setGameType(unsigned int _type){}; //设置游戏类型
    virtual int getGameType(){return -1;}  //获取游戏类型
    virtual void downLoadImage(string &url, string pathStr = ""){} //下载图片
    virtual void downLoadImageBack(const string &str){}
    virtual void updateGameInfo(const string &str){}  //更新游戏请求数据
    virtual void callBack(const string& str, int code){}  //消息回调
    virtual void downLoadImageBack(const string &str , Args _args){} //下载图片
    virtual void setStartTime(string &str){}         //设置游戏开始时间
    virtual string getStartTime(){return nullptr;}  //获取游戏开始时间
    virtual void setEndTime(string &str){}           //设置游戏结束时间
    virtual string getEndTime(){return nullptr;}       //获取游戏结束时间
    virtual LevelType *getLevelType(int tag){return nullptr;}     //获取游戏关卡类型
    virtual void enterGamePlay(int type, const void* vv = nullptr){}     //请求进入游戏
    virtual void requestOverGame(){}        //游戏结束发送请求
    virtual void Redbaojilu(){}             //红包纪录榜
    virtual void http_surprise(const string& s){}
    virtual EnemyData *getEnemyData(unsigned int _tag){return nullptr;} //获取pvp玩家信息
    virtual void setMapData(map<int,long> _map){}       //设置活动map字段
    virtual void setActivityId(const string &str){}  //设置活动id
    virtual string getActivityId(){return "";}      //获取活动id
//    virtual void setPvpSuccess(const bool b){} //设置pvp匹配成功
    virtual void sendAcivityRward(){}   //提交订单信息
    virtual void destroyRoomId(){}      //销毁房间编号
    virtual void sendSubmitRewardInfo(){}   //提交订单信息
    virtual int getEnemyNum(){return 0;}         //获取对手人数
    virtual void enemyVerClear(){}              //清空敌人列表
    virtual vector<EnemyData>* getEnemyVer(){return nullptr;}    //获得敌人列表
    virtual int getGoldLimit(){return 1;}       //获取玩家日常是否达到上限
    
    
    
    //测试
    virtual string geturl(){return "";}
    
public:
    int lastGols;//玩家进入游戏时的金币数
    int goldscurrent;   //拼图结束，奖励的金币数
    int  golds;  //小伙伴获得金币数
    string  headimageurl ; //小伙伴头像
    string  red_nickname;
    
};

#endif
