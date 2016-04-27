//
//  config.h
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#ifndef PinLe_activity_config_h
#define PinLe_activity_config_h


//==================================[module define]============================
#define M_ACTIVITY                  "module_activity"
#define M_GET_ACTIVITY              (IActivityData*)IDataManager::getInstance()->getData(M_ACTIVITY)



//====Eventdetails==============================[message define]============================
#define MES_ACTIVITY_GETLISTDONE            "MES_ACTIVITY_GETLISTDONE"      //获取活动列表信息OK
#define MES_Eventdetails            "Eventdetails"      //显示界面
#define MES_ACTIVITY_GETIMAGEDONE           "MES_ACTIVITY_GETIMAGEDONE"     //获取所有活动图OK
#define MES_ACTIVITY_PHB_INFOR              "MES_ACTIVITY_PHB_INFOR"        //请求排行榜信息
#define MES_ACTIVITY_JF_INFOR              "MES_ACTIVITY_JF_INFOR"        //请求积分纪录排行榜
#define MES_ACTIVITY_RankingRecord              "MES_ACTIVITY_RankingRecord"        //请求对比积分纪录排行榜
#define MES_ACTIVITY_PHB_IMAGE_OK           "MES_ACTIVITY_PHB_IMAGE_OK"     //排行榜玩家拼图图片下载完成
#define MES_ACTIVITY_JPXQ_IMAGE_OK          "MES_ACTIVITY_JPXQ_IMAGE_OK"    //奖品详情图片下载完成

#define change_player_info                  "change_reward_info"     //提示修改个人信息
#define MES_ACTIVITY_Winning           "MES_ACTIVITY_Winning" 
#define MES_ACTIVITY_AwardPreview           "MES_ACTIVITY_AwardPreview"  

#define mesbox_submit_over 1003






#endif
