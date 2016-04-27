LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
				   hellocpp/test.cpp \
                   ../../Classes/AppDelegate.cpp \
		   ../../Classes/framework/core/IData/DataManager.cpp \
		   ../../Classes/framework/core/IMessage/Message.cpp \
		   ../../Classes/framework/core/IMessage/MessageManager.cpp \
		   ../../Classes/framework/network/http/httpwork.cpp \
		   ../../Classes/framework/system/audio/Audio.cpp \
		   ../../Classes/framework/system/file/File.cpp \
		   ../../Classes/framework/tools/Tools.cpp \
		   ../../Classes/framework/tools/md5/Md5.cpp \
		   ../../Classes/framework/tools/snappy/snappy-c.cc \
		   ../../Classes/framework/tools/snappy/snappy-sinksource.cc \
		   ../../Classes/framework/tools/snappy/snappy-stubs-internal.cc \
		   ../../Classes/framework/tools/snappy/snappy.cpp \
		   ../../Classes/framework/ui/Msg_Layer.cpp \
		   ../../Classes/gameini/GameIni.cpp \
		   ../../Classes/UIcontroller/UILayer.cpp \
		   ../../Classes/UIcontroller/UIcontroller.cpp \
		   ../../Classes/http/module/myHttpWork.cpp \
		   ../../Classes/main/view/UI_Main.cpp \
		   ../../Classes/main/view/ScrollCycleView.cpp \
		   ../../Classes/main/view/Guide_choose.cpp \
		   ../../Classes/competitive/view/UI_Choice.cpp \
		   ../../Classes/market/view/UI_Market.cpp \
		   ../../Classes/gameroom/view/GameLayerw.cpp \
		   ../../Classes/gameroom/view/GameScene.cpp \
		   ../../Classes/gameroom/view/OverLayer.cpp \
		   ../../Classes/gameroom/module/DebrisSprite.cpp \
		   ../../Classes/gameroom/module/GameLevelSprite.cpp \
		   ../../Classes/gameroom/module/RoomData.cpp \
		   ../../Classes/gameroom/view/LaoHuJiDonghua.cpp \
		   ../../Classes/option/view/PersonCenterLayerEx.cpp \
		   ../../Classes/option/view/UI_Option.cpp \
		   ../../Classes/activity/module/Activity.cpp \
		   ../../Classes/activity/module/ActivityData.cpp \
		   ../../Classes/activity/module/Advertise.cpp \
		   ../../Classes/activity/view/UI_Activity.cpp \
		   ../../Classes/login/view/UI_Login.cpp \
		   ../../Classes/logo/view/UI_Logo.cpp \
		   ../../Classes/broadcast/module/Broadcast.cpp \
		   ../../Classes/broadcast/module/BroadcastData.cpp \
		   ../../Classes/broadcast/view/TipsLayer.cpp \
		   ../../Classes/friend/view/UI_Friend.cpp \
		   ../../Classes/player/module/Player.cpp \
		   ../../Classes/player/module/PlayerData.cpp \
		   ../../Classes/pokedex/module/PokedexData.cpp \
		   ../../Classes/pokedex/view/UI_Pokedex.cpp \
		   ../../Classes/account/view/UI_Account.cpp \
		   ../../Classes/ThirdParty/CocosPush/android/CCPushService.cpp \
		   ../../Classes/ThirdParty/C2DXShareSDK/Android/SdkPublic/C2DXShareSDK.cpp \
		   ../../Classes/ThirdParty/C2DXShareSDK/Android/ShareSDKUtils.cpp \
  		   ../../Classes/ThirdParty/C2DXShareSDK/Android/JSON/CCJSONConverter.cpp \
       	   	   ../../Classes/ThirdParty/C2DXShareSDK/Android/JSON/cJSON/cJSON.c \
	       	   ../../Classes/ThirdParty/C2DXShareSDK/ShareSDKHelper.cpp \
		   ../../Classes/gamelink/GameLink.cpp	\
		   ../../Classes/dazhuanpan/TurntableSystem.cpp	\
		   ../../Classes/hud/HUDLayer.cpp \
		   ../../Classes/chooseCity/ItemPicker.cpp \
		   ../../Classes/chooseCity/LocPlay.cpp \
		   ../../Classes/chooseCity/PersonCenterLayer.cpp



LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/ThirdParty/C2DXShareSDK \
  					$(LOCAL_PATH)/../../Classes/ThirdParty/C2DXShareSDK/Android \
  					$(LOCAL_PATH)/../../Classes/ThirdParty/C2DXShareSDK/Android/JSON \
   					$(LOCAL_PATH)/../../Classes/ThirdParty/C2DXShareSDK/Android/JSON/cJSON

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_SHARED_LIBRARIES := mobclickcpp_shared

include $(BUILD_SHARED_LIBRARY)

$(call import-module,./prebuilt-mk)
$(call import-module,libmobclickcpp)