//
//  IDataObserver.h
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#ifndef __framework__IDataObserver__
#define __framework__IDataObserver__


#include "cocos2d.h"
#include "network/HttpClient.h"

using namespace std;
using namespace cocos2d::network;


typedef struct
{
    string  content = "";
    int     code  = 0;
    int     code2 = 0;
    int     code3 = 0;
    int     err_code = 0;//预留字段 禁止修改
    void*   ptr = nullptr;
    bool    retry = false;//预留字段 禁止修改
}Args;

#define INIT_ARGS(args)             \
        args.content = "";          \
        args.code = args.code2 = args.code3 = 0;\
        args.err_code = 0;                     \
        args.ptr = nullptr;         \
        args.retry = false;         \

#define MAKE_ARGS   Args args;


#define MAKE_ARGS_Ex(string, b)     \
        Args args;                  \
        args.content = string;      \
        args.code = b;              \





class IDataObserver
{
public:
    virtual bool Notify(const string& name, Args& args) = 0;
};



#endif /* defined(__framework__IDataObserver__) */
