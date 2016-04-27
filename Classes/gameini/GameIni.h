//
//  GameIni.h
//  framework
//
//  Created by Kennedy on 15/6/3.
//
//

#ifndef __framework__GameIni__
#define __framework__GameIni__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "../activity/config.h"
#include "../activity/IActivityData.h"
#include "../player/config.h"
#include "../player/IPlayerData.h"
#include "../gameroom/config.h"
#include "../gameroom/IRoomData.h"
#include "../UIcontroller/UIcontroller.h"
#include "../http/module/myHttpWork.h"
#include "../broadcast/IBroadcastData.h"
#include "../broadcast/config.h"
#include "../broadcast/view/TipsLayer.h"
#include "../pokedex/IPokedexData.h"
#include "../pokedex/config.h"
#include "../ThirdParty/CocosPush/CocosPush.h"

using namespace CocosDenshion;


#define GAME_TEST           //测试模式


//======================================【当前版本号和渠道版本】================================================
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #define APP_CHANNEL     "IOS"           //苹果渠道标识
    #define APP_VERSION     "2.2.1"         //系统版本号
    #define APP_ORIGIN      "0"             //

    #ifdef GAME_TEST
        #define GETGAMEURL_URL      "ios"
        #define GETSERVERURL_URL "http://test.pinlegame.com/geturl.aspx?os=%s&ver=%s"
    #else
        #define GETGAMEURL_URL      "ios"
        #define GETSERVERURL_URL "http://www.pinlegame.com/geturl.aspx?os=%s&ver=%s"
    #endif

#else

    #define APP_CHANNEL     "ANDROID"       //安卓渠道标识
    #define APP_VERSION     "2.2.1"         //系统版本号
    #define APP_ORIGIN      "0"             //版本来源 比如 小米 360

    #ifdef GAME_TEST
        #define GETGAMEURL_URL      "android"
        #define GETSERVERURL_URL "http://test.pinlegame.com/geturl.aspx?os=%s&ver=%s"
    #else
        #define GETGAMEURL_URL      "android"
        #define GETSERVERURL_URL "http://www.pinlegame.com/geturl.aspx?os=%s&ver=%s"
    #endif


//http://pinlegame.com/geturl.aspx?os={0}

#endif


#ifdef GAME_TEST
#define _RandomMoveDebris_      1
#else
#define _RandomMoveDebris_      1
#endif


#define APP_NAME            "拼乐Game"

//======================================【CocosPush配置】==========================================
#define CocosPush_AppID             "1100014820"            //CocosPush后台推送APPID
#define CocosPush_ChannelId         APP_ORIGIN              //CocosPush后台推送渠道编号

//======================================【拼图碎片随机数量】=========================================




//======================================【系统广播公告】==================================================
#define BROAD_CAST true


//======================================【系统配置】================================================
#define SCREEN_WIDTH    750
#define SCREEN_HEIGHT   1334

//========================================[图片资源存储目录]========================================
#define IMAGE_ROOT_FINDER           "image"                 //资源根目录
#define IMAGE_AD_FINDER             "ad"                    //广告原图目录(游戏内拼图)
#define IMAGE_ADLIST_FINDER         "adlist"                //活动广告列表图
#define IMAGE_ADREWARD_FINDER       "adreward"              //活动结束奖励图

#define IMAGE_SCREENSHOOT_FINDER    "screenshoot"           //截屏图片存储位置
#define IMAGE_SCREENSHOOT_FILENAME  "screenshoot.jpg"
//#define IMAGE_SCREENSHOOT_PATH      (FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_SCREENSHOOT_FINDER + "/" + "screenshoot.jpg")

#define IMAGE_THUMBNAIL_FINDER      "thumbnail"             //ICON缩略图位置，用于分享
#define IMAGE_THUMBNAIL_FILENAME    "thumbnail.png"         //ICON缩略图名字
//#define IMAGE_THUMBNAIL_PATH        (FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_THUMBNAIL_FINDER + "/" + IMAGE_THUMBNAIL_FILENAME)




//;=======================================[网络协议network protocol]===============================
#define np_playerinfo               "playerinfo"
#define np_adlist                   "activitieslist"
#define np_methodtype               "methodtype"
#define np_createtime               "createtime"
#define np_functionname             "functionname"
#define np_functionparams           "functionparams"
#define np_PINLEGAME                "PINLEGAME"
#define np_reg                      "reg"
#define np_setuserinfo              "setuserinfo"
#define np_getsactivitieslist       "getsactivitieslist"
#define np_getactivitybyid           "getactivitybyid"
#define np_getrankinglistofactivies "getrankinglistofactivies"
#define np_serverconfig             "serverconfig"
#define np_market_ip                "market_ip"
#define np_server_ip                "server_ip"
#define np_match_ip                 "match_ip"
#define np_game_ip                  "game_ip"
#define np_bag_url                  "mygoods_url"
#define np_weburl                   "weburl"        //官方网址
#define np_add                      "add"           //官方地址
#define np_tel                      "tel"           //官方电话
#define np_appleappid               "appleappid"    //苹果应用APPID
#define np_space_of_wait2           "space_of_wait2"
#define np_times_of_wait2           "times_of_wait2"
#define np_space_of_war2            "space_of_war2"
#define np_space_of_wait4           "space_of_wait4"
#define np_times_of_wait4           "times_of_wait4"
#define np_space_of_war4            "space_of_war4"
#define np_lostgold_acdsee          "lostgold_acdsee"
#define np_share_sdk_info           "share_sdk_info"
#define np_share_sdk_url            "share_sdk_url"





#define np_game                     "game"
#define np_sendmessage              "sendmessage"
#define np_game_ad                  "ad"            //拼图信息
#define np_game_goods               "goods"         //活动结束游戏奖品
#define np_getaffiche               "getaffiche"    //获取广播


//====================================================================================================
#define guide_main_option           "guide_main_option"
#define guide_main_choose           "guide_main_choose"
#define guide_game_view             "guide_game_view"



//=====================================[音乐资源存储目]===================================================
#define music_key                   "music_key"
#define music_bg_game               "Music/background/gameBg.mp3"
#define music_bg_main               "Music/background/menumusic.mp3"
#define music_bg_activity           "Music/background/activity.mp3"
#define music_bg_personchange       "Music/background/personalchage.mp3"

#define music_ef_321                "Music/effect/321.mp3"
#define music_ef_back               "Music/effect/back.mp3"
#define music_ef_ok                 "Music/effect/button.mp3"
#define music_ef_cancel             "Music/effect/cancel.mp3"
#define music_ef_droppic            "Music/effect/droppic.mp3"
#define music_ef_error              "Music/effect/error.mp3"
#define music_ef_fallmoney          "Music/effect/fallmoney.mp3"
#define music_ef_logo               "Music/effect/logo.mp3"
#define music_ef_lost               "Music/effect/lost.mp3"
#define music_ef_move               "Music/effect/move.mp3"
#define music_ef_pagechange         "Music/effect/pagechange.mp3"
#define music_ef_pageplay           "Music/effect/pageplay.mp3"
#define music_ef_preview            "Music/effect/preview.mp3"
#define music_ef_setting            "Music/effect/setting.mp3"
#define music_ef_win                "Music/effect/win.mp3"



//====================================================================================================

//=====================================[游戏提示语]===================================================
enum
{
    tips_info_quitapp = 1,
    tips_error_net,
    tips_error_phone,
    tips_info_inputphone,
    tips_info_inputpsw,         //5
    tips_error_psw,
    tips_error_veritycode,
    tips_error_psw2,
    tips_error_pswtype,
    tips_info_changesuc,        //10
    tips_info_address,
    tips_info_quitgame,
    tips_info_nocount,
    tips_info_actover,
    tips_info_getreward,        //15
    tips_info_nogolds,
    tips_error_timeout,
    tips_info_changepswsuc,
    tips_info_actunstart,
    tips_info_actjoined,        //20
    tips_info_details,
    tips_info_saved,
    tips_info_notchange,
    tips_info_checkImage,
    tips_info_changeinfo,       //25
    tips_error_invitenotself,
    tips_info_nomoneypvp2,
    tips_info_nomoneypvp4,
    tips_info_nomoneysimple,
    tips_info_nomoneyhard,      //30
    tips_info_adlinkinfo,
    tips_info_wsgrxx,                   //完善个人信息
    
};

//====================================================================================================
enum class GameReward
{
    none_0 = 0,                 //保留
    none_1,                     //保留
    closure,                    //封禁用户
    deblock,                    //解封用户
    activateUser,               //首次登录完成账户激活
    baseUserInfo,               //用户基本信息
    detailedUserInfo,           //用户高级信息
    signIn,                     //每日签到登录
    repairUserInfo,             //完善个人资料奖励
    dailySimpleWin,             //日常拼图-简单获胜
    dailyHardWin,               //日常拼图-困难获胜
    activity,                   //活动专区获胜
    challengeSimpleWin,         //个人挑战赛-简单获胜
    challengeHardWin,           //个人挑战赛-困难获胜
    pvp_2Win,                   //2人竞技赛-获胜
    pvp_4Win,                   //4人竞技赛-获胜
    pvp_friendsWin,             //好友约战-获胜
    challengeSimpleLose,        //个人挑战赛-简单获胜
    challengeHardLose,          //个人挑战赛-困难获胜
    pvp_2Lose,                  //2人竞技赛-获胜
    pvp_4Lose,                  //4人竞技赛-获胜
    pvp_friendsLose,            //好友约战-获胜
    shareResult,                //比赛结果分享奖励
    inviteFriend,               //好友邀请奖励
    friendUpgrade,              //好友升级奖励
    viewImage,                  //查看原图扣金币
    advertiseDetails,           //查看广告详情奖励
    activityDetails,            //27.查看活动详情链接奖励金币
};;


//0.保留
//1.保留
//2.封禁用户
//3.解封用户
//4.首次登录完成账户激活
//5.用户基本信息
//6.用户高级信息
//7.每日签到登录
//8.完善个人资料奖励
//9.日常拼图-简单获胜
//10.日常拼图-困难获胜
//11.活动专区获胜
//12.个人挑战赛-简单获胜
//13.个人挑战赛-困难获胜
//14.2人竞技赛-获胜
//15.4人竞技赛-获胜
//16.好友约战-获胜
//17.个人挑战赛-简单失败
//18.个人挑战赛-困难失败
//19.2人竞技赛-失败
//20.4人竞技赛-失败
//21.好友约战-失败
//22.比赛结果分享奖励
//23.好友邀请奖励
//24.好友升级奖励
//25.查看原图扣金币


//====================================================================================================

using namespace std;


//==================================[message define]============================
#define MES_GAMEINI_SCREENSHOOT_SUCCEED             "MES_GAMEINI_SCREENSHOOT_SUCCEED"
#define MES_GAMEINI_SCREENSHOOT_FAILED              "MES_GAMEINI_SCREENSHOOT_FAILED"
#define GUIDE_MAIN_CHOOSE_GAME                      "GUIDE_MAIN_CHOOSE_GAME"


class GameIni
{
public:
    GameIni();
    ~GameIni();
    
    static  GameIni*    getInstance();
    static  void        destroyInstance();
    
    typedef struct
    {
        string  title;          //标题
        string  description;    //描述
        string  webpageUrl;     //link
        string  url;            //分享图片url
    }ShareSdkInfo;//微信分享的信息
    
    enum ImagePathType
    {
        AD,
        ADLIST,
        ADREWARD,
        SCREENSHOOT,
        THUMBNAIL,
        HEAD,
    };
    
public:
    string          root_ip;                //主接口地址(用来进行版本校验，登陆注册和修改密码)
    string          server_ip;              //数据业务服务器地址
    string          market_ip;              //商城地址
    string          match_ip;               //对战匹配地址(游戏服务器)
    string          game_ip;                //对战地址(游戏服务器)
    string          bag_url;                 //我的商品地址
    string          officialwebsite;        // 官方网址
    string          officialtel;            // 官方电话
    string          officialadd;            // 官方电话
    string          appleAppID;             //  苹果应用包APPID
    int             space_of_wait2;         //二人匹配 匹配轮训时间间隔
    int             times_of_wait2;         //二人匹配 匹配轮训次数
    int             space_of_war2;          //二人匹配 游戏轮训间隔
    int             space_of_wait4;         //匹配轮训时间间隔
    int             times_of_wait4;         //匹配轮训次数
    int             space_of_war4;          //游戏轮训间隔
    int             lostgold_acdsee;        //查看原图消耗的金币数
    bool            agreeXiyi;              //是否同意协议条款
    ShareSdkInfo    shareSdkInfo;
    
public:
    void            exitApp();
    void            getNetworkIP(const string& s);
    void            preloadMusic();
    void            comment(const string& appID);
    void            screenShoot();//屏幕截屏
    string          getTips(int num);
    string          getLocalSavedImagePath(const string& imageName, int type, bool isWritablePath = false);
    string          getShareLocalPath(const string& surName);
    int             getHeadIndex(const string& s);
    void            getDeviceInfo();
    
    
    
    void writeStringToTxt(const string& str);
    
public:
    string          gamePlayerUrl;

private:
    rapidjson::Document tapsdoc;
    static  void    afterCapture(bool succeed, const string& outputFile);
};

bool IsPhoneNumber(string str);
bool PassWordRule(string str);//判断是否为数字，字母，下划线


void getdeviceInfo(string str);
void getLocationInfo(double x, double y);


#endif /* defined(__framework__GameIni__) */
