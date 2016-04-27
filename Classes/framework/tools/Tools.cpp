//
//  ITools.cpp
//  framework
//
//  Created by Kennedy on 15/5/27.
//
//

#include "ITools.h"





//截取字符串从后向前截取http://221.65.133.52:80/logo/91B5FE02-004C-4805-A8CE-0F190494E56B.jpg 结果91B5FE02-004C-4805-A8CE-0F190494E56B.jpg
string ITools::Intercept_string(const string& str, const string& sign)
{
    if(str.empty())
        return "";
    
    uint64_t i = str.find_last_of(sign);//定义去最后一个"/"的位置
    string s = str.substr(i + 1);
    return s;
}
long ITools::getNum(const string&  str){
//    char a[50]="1ab2cd3ef45g";
//    char b[50];
    int cnt_int=0;
    //cnt_int 用于存放字符串中的数字.
    //cnt_index 作为字符串b的下标.
    for(int i=0;i<str.size();++i) //当a数组元素不为结束符时.遍历字符串a.
    {
        if(str[i]>='0'&& str[i]<='9') //如果是数字.
        {
            cnt_int*=10;
            cnt_int+=str[i]-'0'; //数字字符的ascii-字符'0'的ascii码就等于该数字.
        }
        
    }
    return cnt_int;
    
}
vector<string>  ITools::Intercept_string_char(char* str, const char* ch)    //根据分隔符，分割字符串
{
    
    vector<string> stringVec;
    if(strcmp(str, "") == 0)
    {
        return stringVec;
    }
    
    char* c = strtok(str,ch);
    stringVec.push_back((string)c);
    while(c)
    {
        c = strtok(NULL,ch);
        if(c)
            stringVec.push_back((string)c);
    }
    return stringVec;
}

string ITools::Intercept_stringEnd(const string& str, const string& sign)
{
    uint64_t i = str.find_last_of(sign);//定义去最后一个"."的位置
    string s = str.substr(0, i);
    return s;
}


long ITools::GetNowSec()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec/1000;
}

void ITools::swap(int* a, int* b)
{
    if(!a || !b)
    {
        int tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

int ITools::getRand(int start, int end)
{
    if(start > end)
        swap(&start, &end);
    return	rand() % (end + 1 - start) + start;
}


//char fmt[] = "%Y-%m-%d-%H:%M:%S";
//char buf[] = "2000-01-01-00:00:00";
//struct tm tb;
//if (strptime(buf, fmt, &tb) != NULL) {
//    fprintf(stdout "ok");
//}

time_t ITools::strtotime(const char*  date, const char* format)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    strptime(date,format, &tm) ;
    return mktime(&tm);
}

char* ITools::GetYMDHMS()//转化年月日时分秒
{
    time_t nowtime = time(NULL); //获取日历时间
    
    struct tm *local;
    local = localtime(&nowtime);  //获取当前系统时间
    
    char buf[80] = {0};
    strftime(buf, sizeof(buf),"%Y-%m-%d %H:%M:%S", local);
    string times = buf;
    
    return (char*)times.c_str();
}

//#8C59C5
Color3B ITools::color_index2rgb(const string& s)
{
    Color3B color = Color3B::BLACK;
    if(!s.empty())
    {
        string c = ITools::Intercept_string(s, "#");
        string r = c.substr(0, 2);
        color.r  = strtol(r.c_str(), nullptr, 16);
        string g = c.substr(2, 2);
        color.g  = strtol(g.c_str(), nullptr, 16);
        string b = c.substr(4, 2);
        color.b  = strtol(b.c_str(), nullptr, 16);
    }
    return color;
}


void ITools::addUnderline(void* vv)
{
    Text* tt = (Text*)vv;
    Color4B color = tt->getTextColor();
    DrawNode* drawLine = DrawNode::create();
    drawLine->drawLine(Vec2(0,0), Vec2(tt->getContentSize().width,0), Color4F((float)color.r/255, (float)color.g/255, (float)color.b/255, (float)color.a/255));
    drawLine->setPosition(Vec2(0,2));
    tt->addChild(drawLine);
}

string ITools::getTimeString(const long time, const int type)      //将时间转化为年月日形式(时间，返回类型)
{
    string timestr;
    long day,hour,minutes,sconds;
    switch(type)
    {
        case fenhao_sfm:
        {
            timestr = StringUtils::format("%02ld:%02ld:%02ld",
                                          (time / 3600), (time/60)%60, time % 60);
        }break;
        case fenhao_fm:
        {
            timestr = StringUtils::format("%02ld:%02ld",
                                          (time/60)%60, time % 60);
        }break;
        case fenhao_tsfm:
        {
            timestr = StringUtils::format("%02ld:%02ld:%02ld:%02ld",
                                          (time/60/60/24), (time / 3600), (time/60)%60, time % 60);
        }break;
        case fenhao_fmh:
        {
            timestr = StringUtils::format("%02ld:%02ld:%03ld",
                                        time/1000/60, (time/1000)%60, time%60);
        }break;
        case hanzi_sfm:
        {
            timestr = StringUtils::format("%ld小时 %ld分 %ld秒",
                                          (time/60/60)%24, (time/60)%60, time%60);
        }break;
        case hanzi_fm:
        {
            timestr = StringUtils::format("%ld分 %ld秒",
                                          (time/60)%60, time%60);
        }break;
        case hanzi_tsfm:
        {
            day = time/60/60/24;
            hour = (time/60/60)%24;
            minutes = (time/60)%60;
            sconds = time%60;
            if(day >= 1)
            {
                timestr = StringUtils::format("%ld天 %ld小时 %ld分 %ld秒",
                                              day, hour, minutes, sconds);
            }
            else if(hour >= 1)
            {
                timestr = StringUtils::format("%ld小时 %ld分 %ld秒",
                                              hour, minutes, sconds);
            }
            else if(minutes >= 1)
            {
                timestr = StringUtils::format("%ld分 %ld秒",
                                              minutes, sconds);
            }
            else
            {
                timestr = StringUtils::format("%ld秒",
                                              sconds);
            }
            
        }break;
        default:break;
    }
    
//    GAMELOG("##timestr = %s",timestr.c_str());
    return timestr;
}

float ITools::getTextSpriteScale(Text* text, Sprite* sprite)            //获取字体与底下背景图片所要拉伸的比例
{
    float f;
    f = (text->getContentSize().width + 50) / (sprite->getContentSize().width);
    GAMELOG("ffff==%f",f);
    return f;
}

