//
//  config.h
//  Pinle
//
//  Created by 李辛 on 15/6/5.
//
//

#ifndef Pinle_config_h
#define Pinle_config_h

//==================================[module define]============================
#define M_ROOM                    "module_room"
#define M_GET_ROOM                (IRoomData*)IDataManager::getInstance()->getData(M_ROOM)



//==================================[message define]============================
#define MES_ROOM_GAMESTART                      "MES_PLAYER_GAMESTART"    //游戏开始
#define MES_ROOM_GAMEOVER                       "MES_ROOM_GAMEOVE"  //游戏结束
#define MES_ROOM_COMMONCONTINUE                 "MES_ROOM_COMMONCONTINUE"   //继续游戏
#define MES_ROOM_PVPCONTINUE                    "MES_ROOM_PVPCONTINUE"  //匹配赛继续游戏
#define MES_ROOM_PVPADDENEMY                    "MES_ROOM_PVPADDENEMY"  //匹配赛继续游戏
#define MES_ROOM_SUBMITREWARE                   "MES_ROOM_SUBMITREWARE" //提交订单信息
#define MES_ROOM_IMAGEDOWNLOAD                  "MES_ROOM_IMAGEDOWNLOAD" //下载图片
#define MES_ROOM_CKXQADDGOLDS                   "MES_ROOM_CKXQADDGOLDS" //下载图片
#define MES_ROOM_LAOHUJISHUJU                   "MES_ROOM_LAOHUJISHUJU"  //老虎机结束返回数据
#define MES_ROOM_HONGBAOPAIHANG                 "MES_ROOM_HONGBAOPAIHANG"  //红包排行榜



//===================================[imagePath define]=================================

#define IMAGEDOWLOAD_PATH_AD   FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/"        //广告原图目录(游戏内拼图)
#define IMAGEDOWLOAD_PATH_ADLIST    FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_ADLIST_FINDER + "/"   //活动广告列表图
#define IMAGEDOWLOAD_PATH_ADREWARD    FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_ADREWARD_FINDER + "/"   //活动结束奖励图

//==================================[room notify tag]========================
#define ROOM_IMAGEDOWNLOAD_BACK 2001    //活动结束奖励图下载完成

//==================================[messageBox]===========================
#define mesbox_details 3001

//==================================[lisenterner]================================
#define about_details 296
#define guide_iknow 180

#endif
