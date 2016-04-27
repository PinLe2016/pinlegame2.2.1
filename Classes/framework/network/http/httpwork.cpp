//
//  httpwork.cpp
//  framework
//
//  Created by Kennedy on 14-6-26.
//
//

#include "../../config.h"


HttpWork::HttpWork()
{
    _httpCB  = nullptr;
    _httpCB2 = nullptr;
    _httpCB3 = nullptr;
    INIT_ARGS(_args);
}

HttpWork::~HttpWork()
{
}

//HttpWork* HttpWork::createHttp()
//{
//    HttpWork *http = new HttpWork();
//    if(http)
//    {
////        http->autorelease();
//        return http;
//    }
//    CC_SAFE_DELETE(http);
//    return NULL;
//}


string HttpWork::getRecieveData(HttpResponse *response)
{
    string recieveData = "";
    if (!response)
        return recieveData;
    
    std::vector<char> *buffer = response->getResponseData();
        //定义一个字符串接收数据
    uint64_t i = 0;
    while (i < buffer->size())
    {
        recieveData += (*buffer)[i];
        i ++;
    }
    CCLOG("liuyali   %s",recieveData.c_str());
#ifdef SNAPPYMODE
    if(response->getHttpRequest()->getRequestType() != HttpRequest::Type::GET)
    {
        string uncompressed = recieveData;
        snappy::Uncompress(uncompressed.c_str(), uncompressed.size(), &recieveData);
    }
#endif
    
#ifdef HTTP_ENCODE
    recieveData = URLDecode(recieveData.c_str());
#endif
    
    return recieveData;
}

string HttpWork::makeRequestData(const string& requestData)
{
    string s = requestData;
    if(!s.empty())
    {
#ifdef HTTP_ENCODE
        s = URLEncode(s);
#endif
        
#ifdef SNAPPYMODE
        string output;
        snappy::Compress(s.c_str(), s.size(), &output);
        s = output;
#endif
    }
    return s;
}


void HttpWork::httpRequestCallFunc(HttpClient *sender, HttpResponse *response)
{
    if(_rc._sendingMes)
        IMessageManager::getInstance()->postNotification(NETWORK_CALLBACK);
    
    if (!response || !response->isSucceed())
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
            switch (statusCode)
            {
                case -1:
                    IMessageManager::getInstance()->postNotification(NETWORK_TIMEOUT);
                    break;
                default:
                    IMessageManager::getInstance()->postNotification(NETWORK_ERROR_RESPONSE);
                    break;
            }
        }
        release();
        return;
    }
    
    // You can get original request type from: response->request->reqType
    //    if (0 != strlen(response->getHttpRequest()->getTag()))
    //    {
    //        GAMELOG("%s completed", response->getHttpRequest()->getTag());
    //    }
    
    string recieveData = getRecieveData(response);

    GAMELOG("**########recieveSize = %lu, recieveData = %s#######**\n", recieveData.size(), recieveData.c_str());
    
    if(_httpCB  != nullptr)
        _httpCB(recieveData, 1);
    
    if(_httpCB2 != nullptr)
        _httpCB2(recieveData);
    
    if(_httpCB3 != nullptr)
        _httpCB3(recieveData, _args);
    
    release();
}


void HttpWork::sendRequest(const string& url, const string& setTag, const string& requestData, int Timeout, bool ui_network)
{
    _rc.url = url;
    _rc.tag = setTag;
    _rc.requestData = requestData;
    _rc.timeout = Timeout;
    _rc.callback = CC_CALLBACK_2(HttpWork::httpRequestCallFunc, this);
    _rc.type = HttpRequest::Type::POST;
    _rc._sendingMes = ui_network;

    sendRequest();
}


void HttpWork::sendRequest(const string& url, int Timeout, bool ui_network)
{
    _rc.url = url;
    _rc.timeout = Timeout;
    _rc.callback = CC_CALLBACK_2(HttpWork::httpRequestCallFunc, this);
    _rc.type = HttpRequest::Type::GET;
    _rc._sendingMes = ui_network;
    
    sendRequest();
}

void HttpWork::sendRequest()
{
    HttpClient *httpClient = HttpClient::getInstance();
    HttpRequest* request = new HttpRequest();
    
    request->setUrl(_rc.url.c_str());
    request->setRequestType(_rc.type);
    request->setResponseCallback(_rc.callback);
    request->setTag(_rc.tag.c_str());
    
    if(!_rc.requestData.empty())
    {
        string sendRequestData = makeRequestData(_rc.requestData);
        request->setRequestData(sendRequestData.c_str(), strlen(sendRequestData.c_str()));
        GAMELOG("发送前json_HttpWork::sendRequest = %s", sendRequestData.c_str());
    }
    
    //设置响应时间
    httpClient->setTimeoutForConnect(_rc.timeout);
    httpClient->send(request);
    request->release();
    
    if(_rc._sendingMes)
        IMessageManager::getInstance()->postNotification(NETWORK_SENDING, NULL);
}


//========================================================================================================






