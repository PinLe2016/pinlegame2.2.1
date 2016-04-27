//
//  myHttpWork.h
//  PinLe
//
//  Created by Kennedy on 15/6/10.
//
//

#ifndef __PinLe__myHttpWork__
#define __PinLe__myHttpWork__

#include "../../framework/network/http/httpwork.h"

class myHttpWork : public HttpWork
{
public:
    static  myHttpWork* createHttp();   
    virtual void    httpRequestCallFunc(HttpClient *sender, HttpResponse *response);
    void    postNetworkErrorMes(long responseCode, Args *args = nullptr);
    
public:
    //请求下载图片 string[0]为图片url地址 string[1]为图片存储图片 mes为下载成功后发送的消息
    void    http_request_downLoadImage(vector<__Array*> &imageInfo, const string& mes);
    //下载图片回调
    void    http_response_downLoadImage(const string& s, const Args& args);
    uint    _downloadNum;   //当前下载图片数量
    string  _downloadMes;   //当前下载图片传入的消息
};



#endif /* defined(__PinLe__myHttpWork__) */
