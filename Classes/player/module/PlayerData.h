//
//  PlayerData.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__PlayerData__
#define __framework__PlayerData__

#include "../IPlayerData.h"
#include "../../ThirdParty/CocosPush/CocosPush.h"




class PlayerData : public IPlayerData, public Cocos::PushListener ,public LayerColor
{
public:
    PlayerData();
    ~PlayerData();
    
    static  void    initp();
    static  void    destroy();
    
public:
    
    void    http_request_login();
    void    http_response_login(const string& s);
    
    void    http_request_regist(const string& invitecode);
    void    http_response_regist(const string& s);
    
    void    http_request_changePassword();
    void    http_response_changePassword(const string& s);
    
    void    http_request_updateInfo(Player* p);
    void    Invitation_code(Player* p,const string& invitecode);
    void    GetInvitation_code(const string& s, Args args);
    int    get_error();
    void    http_request_cityInfo(Player* p);
    int     Getisexsits();
    string  Getnickname();
    void    http_request_nicknameInfo(const string& s);
    void    http_response_cityInfo(const string& s, Args args);
    void    Get_request_nicknameInfo(const string& s);
    void    http_response_updateInfo(const string& s, Args args);
    
    void    http_request_getVerityCode(const string& phone, const string& verifycode, int type);
    void    http_response_getVerityCode(const string& s);
    
    void    http_request_checkVersion();
    void    http_response_checkVersion(const string& s);
    
    void    http_request_setInviteCode(const string& s);
    void    http_response_setInviteCode(const string& s);
    
    void    http_request_getFriendsInfo();
    void    http_response_getFriendsInfo(const string& s);
    
    void    http_request_getFriendsReward();
    void    http_response_getFriendsReward(const string& s);
    
    void    http_request_getAccount();
    void    http_response_getAccount(const string& s);
    
    void    http_request_setReward(int rewardType, const string& s = "");
    void    http_response_setReward(const string& s);
    
    void    http_request_sendAdvise(int type, const string& s = "");
    void    http_response_sendAdvise(const string& s);
    
    void    http_request_sendPostAddress(string pname, string pphone, int shengid, int shiid, string paddress);
    void    http_response_sendPostAddress(const string& s);
    
    
    void    http_request_sendXieyi();
    void    http_response_sendXieyi(const string& s);
    
    void    setCoordinate(double _longitude, double _latitude);
    void    getCoordinate();
    void    phonecall(const string& s);
    void    comment(const string& appID);
//    string  getSDcardPath();
    
    bool    setDataByLocal();
    void    saveLocalData();
    bool    localDataUseful();
    string  getLocalUserID();
    string  getLocalUserPsw();
    
    void    loginOut();
    void    updateInfo(const string& s);
    float   getPecentScore();                 //根据积分获得等级进度条的百分比
    
    void    release();
    
    bool    isCompletedInfo();
    
   
    
private://=========PushListener=============
    virtual void onSetTags(int retCode, const Cocos::Tags& failTags);
    virtual void onMessage(const string& customContent);
    
};





#endif /* defined(__framework__PlayerData__) */
