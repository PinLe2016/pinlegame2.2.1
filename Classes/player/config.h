//
//  config.h
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#ifndef PinLe_player_config_h
#define PinLe_player_config_h


//==================================[module define]============================
#define M_PLAYER                    "module_playerdata"
#define M_GET_PLAYER                (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER)



//==================================[message define]============================
#define MES_PLAYER_nickname                   "MES_PLAYER_nickname"
#define MES_PLAYER_UPDATEINFO                   "MES_PLAYER_UPDATEINFO"
#define MES_PLAYER_LOGIN                        "MES_PLAYER_LOGIN"
#define MES_PLAYER_LOGOUT                       "MES_PLAYER_LOGOUT"
#define MES_PLAYER_REGIST                       "MES_PLAYER_REGIST"
#define MES_PLAYER_VERITYCODE                   "MES_PLAYER_VERITYCODE"             //获取手机验证码
#define MES_PLAYER_SETPASSWORD                  "MES_PLAYER_SETPASSWORD"
#define MES_PLAYER_SETINVITECODE                "MES_PLAYER_SETINVITECODE"          //设置邀请人号
#define MES_PLAYER_GETFRIENDINFO                "MES_PLAYER_GETFRIENDINFO"          //获取好友邀请信息
#define MES_PLAYER_GETFRIENDREWARD              "MES_PLAYER_GETFRIENDREWARD"        //获取来自好友的奖励
#define MES_PLAYER_UPGRADE                      "MES_PLAYER_UPGRADE"                //用户等级提升
#define MES_PLAYER_GETLOCATION                  "MES_PLAYER_GETLOCATION"            //获取地理位置完成
#define MES_PLAYER_GETACCOUNT                   "MES_PLAYER_GETACCOUNT"             //获取账户信息完成
#define MES_PLAYER_SENDADVISE                   "MES_PLAYER_SENDADVISE"             //发送建议反馈成功
#define MES_PLAYER_POSTADDRESSOK                "MES_PLAYER_POSTADDRESSOK"          //玩家修改邮寄地址成功
#define MES_PLAYER_TONGYIXIEYI                  "MES_PLAYER_TONGYIXIEYI"            //玩家同意协议条款
#define MES_PLAYER_REWARDGOLDS                  "MES_PLAYER_REWARDGOLDS"            //玩家点击链接获得金币发出消息

#define MES_PLAYER_UPDATEVERSION                "MES_PLAYER_UPDATEVERSION"


#endif
