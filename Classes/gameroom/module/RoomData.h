//
//  RoomData.h
//  Pinle
//
//  Created by 李辛 on 15/6/1.
//
//

#ifndef __Pinle__RoomData__
#define __Pinle__RoomData__

#include <stdio.h>
#include "../iRoomData.h"
#include "GameMemberType.h"





class RoomData : public IRoomData
{
public:
    RoomData();
    ~RoomData();
    static void init();
    static void destroy();
    
    virtual void commomGameStart();//对战游戏结束发送数据
    virtual void enterChoiceType();
    virtual void setGameInfo(GameLevelSprite *gameLevelSprite);//设置拼图信息
    virtual GameLevelSprite *getGameInfo();    //获取拼图信息
    virtual void setGameSuccess(bool b);  //设置游戏是否成功
    virtual bool isSuccess();    //获取游戏是否成功
    virtual void setTimeCount(int time); //设置游戏消耗时间
    virtual int getTimeCount();  //获取游戏拼图时间
    virtual void setRunCount(int _run);   //设置游戏拼图所用步数
    virtual int getRunCount();   //获取游戏拼图所用步数
    virtual void setRewardInfo(ActivityRewardInfo *activityRewardInfo);   //设置完成活动结束奖励信息
    virtual ActivityRewardInfo *getRewardInfo();   //获取完成活动结束奖励信息
    virtual void setGameType(unsigned int _type); //设置游戏类型
    virtual int getGameType();  //获取游戏类型
    virtual void downLoadImage(string &url, string pathStr = ""); //下载图片
    virtual void updateGameInfo(const string& str);  //更新游戏请求数据
    virtual void callBack(const string& str, int code);  //消息回调
    virtual void downLoadImageBack(const string &str , Args _args); //下载图片
    virtual string geturl();
    
    virtual void setStartTime(string &str);         //设置游戏开始时间
    virtual string getStartTime();  //获取游戏开始时间
    virtual void setEndTime(string &str);          //设置游戏结束时间
    virtual string getEndTime();      //获取游戏结束时间
    virtual LevelType *getLevelType(int tag);    //获取游戏关卡类型
    
    virtual void enterGamePlay(int type,const void* vv = nullptr);     //请求进入游戏
    virtual void requestOverGame();        //游戏结束发送请求
    virtual void Redbaojilu();            //红包纪录榜
    virtual void http_surprise(const string& s);          //惊喜吧结束后返回的数据
    
    virtual void gameOver(const string& str, int code); //游戏结束请求
    virtual void  gameRedbao(const string &str, int code);//红包个数排行榜
    virtual void lhj_Over(const string& str, int code);
    virtual void submitRewardInfoBack(const string& str, int code); //提交订单结束
    virtual EnemyData *getEnemyData(unsigned int _tag); //获取pvp玩家信息
    virtual void setMapData(map<int,long> _map);       //设置活动map字段
    virtual void setActivityId(const string &str);  //设置活动id
//    virtual void setPvpSuccess(const bool b); //设置pvp匹配成功
    virtual string getActivityId();     //获取活动id
    virtual void destroyRoomId();      //销毁房间编号
    virtual void sendSubmitRewardInfo();   //提交订单信息
    virtual int getEnemyNum();        //获取对手人数
    virtual void enemyVerClear();       //清空敌人列表
    virtual vector<EnemyData>* getEnemyVer();   //获得敌人列表
    virtual int getGoldLimit();      //获取玩家日常是否达到上限
    
    
    
    
    bool _pvpSuccess;       //对人匹配成功
private:
    GameLevelSprite *_uGameLevelSprite;
    bool _success;          //多人对战成功
    bool _isPvp;            //判定是匹配赛匹配对手阶段
    
    bool _gamesuccess;      //游戏成功
    unsigned int _utimeCount;
    unsigned int _uRun;
    ActivityRewardInfo *_activityRewardInfo;
    unsigned int _gameType;
    string dec;
    string src;
    string _startTime;
    string _endTime;
    LevelType _levelsType[10];//0日常拼图-简单 1日常拼图-复杂2解锁拼图 3个人挑战赛-简单 4个人挑战赛-复杂 52人竞技赛  6 4人竞技赛 7好友约战 8 活动
    EnemyData _enemyData[4];
    vector<EnemyData> enemyVer;
    bool enemyInVer(string str);
    unsigned int _enemyNum;
    map<int, long> _map;
    string _roomId;
    string _activityId;
    int goldlimit;      //判断日常金币是否达到上限  //1没有达到上限 //0达到上限
};


#endif /* defined(__Pinle__RoomData__) */
