//
//  IUILayer.h
//  framework
//
//  Created by Kennedy on 15/5/26.
//
//

#ifndef __framework__config__
#define __framework__config__

#include "core/IData/IData.h"
#include "core/IData/IDataManager.h"
#include "core/IData/IDataObserver.h"
#include "core/IMessage/IMessageManager.h"
#include "network/http/httpwork.h"
#include "tools/ITools.h"
#include "system/file/IFile.h"
#include "system/audio/IAudio.h"
#include "tools/snappy/snappy.h"
#include "tools/md5/Md5.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "ui/Msg_Layer.h"



//================================[NETWORK message define]=============================================
#define NETWORK_ERROR_RESPONSE          "network_error_response"                //服务器响应失败
#define NETWORK_TIMEOUT                 "network_timeout"                       //网络连接超时
#define NETWORK_SENDING                 "network_sending"                       //网络连接中
#define NETWORK_CALLBACK                "network_callback"                      //收到网络回应
#define NETWORK_ERROR_RETRY             "network_error_retry"                   //网络连接失败重试
#define NETWORK_ERROR_SERVICE           "network_error_service"                 //服务端errorcode报错
//#define NETWORK_ERROR_JSON              "network_error_json"                    //网络返回json解析错误


//==================================[MESSAGEBOX message define]============================
#define MESSAGEBOX_ERROR                "messagebox_error"                      //调用失败
#define MESSAGEBOX_OK                   "messagebox_ok"                         //调用失败
#define MESSAGEBOX_CANCEL               "messagebox_cancel"                     //调用失败
#define MESSAGEBOXOK                   "messageboxok"  

//================================[SYSTEM define]=============================================
#define SYSTEM_ENTERBACKGROUND          "system_enterbackground"                //切到外部事件
#define SYSTEM_ENTERFOREGROUND          "system_enterforeground"                //从外部事件切回来
#define SYSTEM_KEYBACK                  "system_keyback"                        //键盘返回按键























#endif /* defined(__framework__config__) */
