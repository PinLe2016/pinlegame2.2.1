//
//  httpwork.h
//  framework
//
//  Created by Kennedy on 14-6-26.
//
//

#ifndef __framework__httpwork__
#define __framework__httpwork__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"
#include "../../tools/ITools.h"
#include "../../core/IData/IDataObserver.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::network;


class HttpWork : public Ref
{
public:
    HttpWork();
    ~HttpWork();
//    static HttpWork* createHttp();
    
    typedef struct
    {
        string  url = "";
        string  tag = "";
        string  requestData = "";
        int     timeout = 30;
        bool    _sendingMes = YES;
        bool    _errorMes   = YES;    //当为FALSE时候 不发送NETWORK_ERROR_RESPONSE消息
        HttpRequest::Type   type;
        ccHttpRequestCallback callback = nullptr;
    }requestCache;
    
    // use "POST" reqeust type, it'll post NETWORK_SENDING message when ui_network is true.
    void sendRequest(const string& url, const string& setTag, const string& requestData,
                     int Timeout, bool ui_network = true);

    // use "GET" reqeust type, it'll post NETWORK_SENDING message when ui_network is true.
    void sendRequest(const string& url, int Timeout, bool ui_network = true);
    
    void sendRequest();
    
    string  getRecieveData(HttpResponse *response);
    string  makeRequestData(const string& requestData);
    
    virtual void httpRequestCallFunc(HttpClient *sender, HttpResponse *response);
    
    // the callback function when the http response
    typedef std::function<void(string, int)> HttpCB;
    HttpCB  _httpCB;
    typedef std::function<void(string)> HttpCB2;
    HttpCB2 _httpCB2;
    typedef std::function<void(string, Args)> HttpCB3;
    HttpCB3 _httpCB3;
    
public:
    Args            _args;
    requestCache    _rc;
};














namespace HttpUtility
{
    typedef unsigned char BYTE;
    
    inline BYTE toHex(const BYTE &x)
    {
        return x > 9 ? x -10 + 'A': x + '0';
    }

    inline BYTE fromHex(const BYTE &x)
    {
        return isdigit(x) ? x-'0' : x-'A'+10;
    }
    
    inline string URLEncode(const string &sIn)
    {
        string sOut;
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {
            BYTE buf[4];
            memset( buf, 0, sizeof(buf));
            if( isalnum( (BYTE)sIn[ix] ) )
            {
                buf[0] = sIn[ix];
            }
            //else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
            //{
            //    buf[0] = '+';
            //}
            else
            {
                buf[0] = '%';
                buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
                buf[2] = toHex( (BYTE)sIn[ix] % 16);
            }
            sOut += (char *)buf;
        }
        return sOut;
    };
    
    inline string URLDecode(const string &sIn)
    {
        string sOut;
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {
            BYTE ch = 0;
            if(sIn[ix]=='%')
            {
                ch = (fromHex(sIn[ix+1])<<4);
                ch |= fromHex(sIn[ix+2]);
                ix += 2;
            }
            else if(sIn[ix] == '+')
            {
                ch = ' ';
            }
            else
            {
                ch = sIn[ix];
            }
            sOut += (char)ch;
        }
        return sOut;
    }
}
using namespace HttpUtility;




#endif /* defined(__framework__httpwork__) */
