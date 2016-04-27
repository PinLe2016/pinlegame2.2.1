//
//  IPlayerData.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__IPlayerData__
#define __framework__IPlayerData__


#include "../framework/config.h"
#include "module/Player.h"

#define userid_key          "userid_key"
#define userpsw_key         "userpsw_key"

class IPlayerData : public IData, public Player 
{
public:
    string          LandKey;                //登陆返回的key
    string          MiYao_key;              //组合字符串需要的秘钥
    string          gametitle;              //用户等级对应称号
    string          lastlogintime;          //上次登录时间
    string          invitecode;             //推荐码
    string          longitude;              //经度
    string          latitude;               //纬度
    vector<Player>  v_friends;
    int  _isexsits;
    string  _nickname;
public:

    virtual void    http_request_login(){}
    virtual void    http_response_login(const string& s){}
    
    virtual void    http_request_regist(const string& invitecode){}
    virtual void    http_response_regist(const string& s){}
    
    virtual void    http_request_changePassword(){}
    virtual void    http_response_changePassword(const string& s){}
    
    virtual void    http_request_updateInfo(Player* p){}
    virtual void    Invitation_code(Player* p,const string& invitecode){}
    virtual void    http_request_cityInfo(Player* p){}
    virtual void    http_request_nicknameInfo(const string& s){}
    virtual void    http_response_updateInfo(const string& s){}
    
    virtual void    http_request_checkVersion(){}
    virtual void    http_response_checkVersion(const string& s){}
    
    virtual void    http_request_setInviteCode(const string& s){}
    virtual void    http_response_setInviteCode(const string& s){}
    
    virtual void    http_request_getFriendsInfo(){}
    virtual void    http_response_getFriendsInfo(const string& s){}
    
    virtual void    http_request_getFriendsReward(){}
    virtual void    http_response_getFriendsReward(const string& s){}
    
    //type 1是注册 2是修改密码
    virtual void    http_request_getVerityCode(const string& phone, const string& verifycode, int type){}
    virtual void    http_response_getVerityCode(const string& s){}
    
    virtual void    http_request_getAccount(){}
    virtual void    http_response_getAccount(const string& s){}
    
    virtual void    http_request_setReward(int rewardType, const string& s = ""){}
    virtual void    http_response_setReward(const string& s){}
    
    virtual void    http_request_sendAdvise(int type, const string& s = ""){}
    virtual void    http_response_sendAdvise(const string& s){}
    
    virtual void    http_request_sendPostAddress(string pname, string pphone, int shengid, int shiid, string paddress){}
    virtual void    http_response_sendPostAddress(const string& s){}
    
    //协议请求返回
    virtual void    http_request_sendXieyi(){}
    virtual void    http_response_sendXieyi(const string& s){}

    
    virtual void    loginOut(){}
    virtual void    updateInfo(const string& s){}
    virtual void    setCoordinate(double _longitude, double _latitude){}
    virtual void    getCoordinate(){}
    virtual void    phonecall(const string& s){}
    virtual void    comment(const string& appID){}
//    virtual string  getSDcardPath() = 0;
    //是否为完整信息
    virtual bool    isCompletedInfo() = 0;
    
    virtual void    saveLocalData(){}
    virtual bool    localDataUseful() = 0;
    virtual bool    setDataByLocal()  = 0;
    virtual string  getLocalUserID()  = 0;
    virtual string  getLocalUserPsw() = 0;

    virtual float   getPecentScore()  = 0;                //根据积分获得等级进度条的百分比
    virtual void    release(){}
};




#endif /* defined(__framework__IPlayerData__) */
