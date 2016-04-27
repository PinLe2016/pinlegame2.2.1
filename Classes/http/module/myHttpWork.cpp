//
//  myHttpWork.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/10.
//
//

#include "myHttpWork.h"
#include "../../UIcontroller/UIcontroller.h"




myHttpWork* myHttpWork::createHttp()
{
    myHttpWork *http = new myHttpWork();
    if(http)
    {
        //        http->autorelease();
        return http;
    }
    SAFE_DELETE(http);
    return NULL;
}


void myHttpWork::postNetworkErrorMes(long responseCode, Args *args)
{
    switch (responseCode)
    {
        case -1:
            IMessageManager::getInstance()->postNotification(NETWORK_TIMEOUT, args);
            break;
        case 200:
            break;
        default:
            IMessageManager::getInstance()->postNotification(NETWORK_ERROR_RESPONSE, args);
            break;
    }
}

void myHttpWork::httpRequestCallFunc(HttpClient *sender, HttpResponse *response)
{
    if(_rc._sendingMes)
        IMessageManager::getInstance()->postNotification(NETWORK_CALLBACK);
    
    if ( !response || !response->isSucceed())
    {
        GAMELOG("response failed");
        long statusCode = 0;
        if(response)
        {
            statusCode = response->getResponseCode();
            GAMELOG("error buffer: %s", response->getErrorBuffer());
            GAMELOG("response code: %ld", statusCode);
        }
        else
        {
            GAMELOG("response is null");
        }
        if(_rc._errorMes)
        {
            Args args;
            args.retry = YES;
            args.ptr = (void*)this;
            postNetworkErrorMes(statusCode, &args);
        }
        else
        {
            release();
        }
        return;
    }
    
    string recieveData = getRecieveData(response);
    
    rapidjson::Document doc;
    doc.Parse<0>(recieveData.c_str());
    CCLOG("服务器返回数据:   %s 拼乐",recieveData.c_str());
    if(!doc.HasParseError())
    {
        if(doc.HasMember("err_code") && doc["err_code"].GetInt() != 0 && _rc._errorMes)
        {
            GAMELOG("myHttpWork  recieveData = %s\n", recieveData.c_str());
            string s;
            if(doc.HasMember("err_msg"))
                s = doc["err_msg"].GetString();
            else
                s = StringUtils::format("err_code = %d, 服务端没传报错信息", doc["err_code"].GetInt());
            
            UIMessage* mes = UIMessage::createMesBox(s);
            mes->_args.err_code = doc["err_code"].GetInt();
            
            IMessageManager::getInstance()->postNotification(NETWORK_ERROR_SERVICE);
            
            release();
            return;
        }
//        IMessageManager::getInstance()->postNotification(NETWORK_ERROR_JSON);
//        GAMELOG("json = %s", recieveData.c_str());
//        release();
//        return;
    }
    
    HttpWork::httpRequestCallFunc(sender, response);
}



void myHttpWork::http_request_downLoadImage(vector<__Array*> &imageInfo, const string& mes)
{
    _downloadNum = 0;
    _downloadMes = mes;
    for(int i = 0; i < imageInfo.size(); i ++)
    {
        if(imageInfo[i]->count() != 2)
        {
            GAMELOG("imageInfo[%d]->count() != 2", i);
            continue;
        }
        __String* path = (__String*)imageInfo[i]->getObjectAtIndex(1);
        CCLOG("back___ %s",path->getCString());
        if(!IFile::isFileExist_Writable(path->getCString()))
        {
            _downloadNum ++;
            __String* url = (__String*)imageInfo[i]->getObjectAtIndex(0);
            myHttpWork* http = myHttpWork::createHttp();
            http->_args.content = path->getCString();
            http->_httpCB3 = CC_CALLBACK_2(myHttpWork::http_response_downLoadImage, this);
            http->sendRequest(url->getCString(), 30, NO);
        }
        else
        {
            GAMELOG("has image %d", i);
        }
    }
    if(_downloadNum == 0)
    {
        IMessageManager::getInstance()->postNotification(_downloadMes);
        release();
    }
}

void myHttpWork::http_response_downLoadImage(const string& s, const Args& args)
{
    CCLOG("奖项预览 %s",s.c_str());
    IFile::writeFile(args.content, s);
    _downloadNum --;
    if(_downloadNum == 0)
    {
        IMessageManager::getInstance()->postNotification(_downloadMes);
        release();
    }
}






