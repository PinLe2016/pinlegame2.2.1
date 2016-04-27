//
//  ITools.h
//  framework
//
//  Created by Kennedy on 15/5/27.
//
//

#ifndef __framework__ITools__
#define __framework__ITools__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

//===================================================================================================
#define DISPLAY_LOG                 //if define this macro, the log trace will display

#ifndef DISPLAY_LOG
    #define GAMELOG(...)            do {} while (0)
#else
    #define GAMELOG(format, ...)    log(format, ##__VA_ARGS__)
#endif

#define gamelog     GAMELOG
//===================================================================================================
//if define this macro, the network request and response will use the functions "Compress" and "Uncompress" of snappy.
//#define SNAPPYMODE

//if define this macro, the network request and response will Encode and Decode.
//#define HTTP_ENCODE

//===================================================================================================
#define _ASSERT_                    // assert开关

#ifndef _ASSERT_
    #define GAMEASSERT(x)           do {} while (0)
#else
    #define GAMEASSERT(x)           assert(x)
#endif
//===================================================================================================

#define SAFE_DELETE(p)              do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)                do { if(p) { free(p); (p) = nullptr; } }    while(0)
#define VECTOR_DELETE(vector)       for(uint i = 0; i < vector.size(); i ++) SAFE_DELETE(vector[i]); vector.clear();


#define YES     true
#define NO      false


//===================================================================================================

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

enum timeType
{
    fenhao_sfm = 0,
    fenhao_fm,
    fenhao_tsfm,
    fenhao_fmh,
    hanzi_sfm,
    hanzi_fm,
    hanzi_tsfm,
};

class ITools
{
public:
    static  long getNum(const string& str);
    static  string  Intercept_string(const string& str, const string& sign);
    static  string  Intercept_stringEnd(const string& str, const string& sign);
    //get millisecond of system
    static  long    GetNowSec();
    static  int     getRand(int start, int end);
    static  void    swap(int* a, int* b);
    static  char*   GetYMDHMS();//转化年月日时分秒
    static  Color3B color_index2rgb(const string& s);
    static  void    addUnderline(void* vv);     //为文本添加下划线    
    static time_t   strtotime(const char* date, const char* format = "%Y-%m-%d %H:%M:%S");
    static vector<string>   Intercept_string_char(char* str, const char* ch);    //根据分隔符，分割字符串
    static  string  getTimeString(const long time, const int type);      //将时间转化为年月日形式(时间，返回类型)
    static float    getTextSpriteScale(Text* text, Sprite* sprite);            //获取字体与底下背景图片所要拉伸的比例
};
















#endif /* defined(__framework__ITools__) */
