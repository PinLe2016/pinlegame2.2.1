//
//  GameIni.cpp
//  framework
//
//  Created by Kennedy on 15/6/3.
//
//

#include "GameIni.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocos-ext.h"
#include "../framework/system/file/IFile.h"
#include "../moduleInit.h"

static GameIni* gameini = NULL;

GameIni::GameIni()
{
//    root_ip   = ROOT_IP;
    appleAppID= "995972730";
    agreeXiyi = false;
    server_ip = "";
    market_ip = "";
    match_ip  = "";
    game_ip   = "";
    gamePlayerUrl = "";
    shareSdkInfo.title = "赶快加入拼乐吧！海量奖品免费送不停就等你来拿！";
    shareSdkInfo.description = "用最短的时间拼出被打乱的图片，积累你的金币财富，温馨实用小物、尖端科技产品、Apple全线产品等你免费赢取！";
    shareSdkInfo.webpageUrl = "http://playtest.pinlegame.com/WeChatShare.aspx";
    
    IFile::createDirectory(IMAGE_ROOT_FINDER);
    IFile::createDirectory(StringUtils::format("%s/%s", IMAGE_ROOT_FINDER, IMAGE_AD_FINDER));
    IFile::createDirectory(StringUtils::format("%s/%s", IMAGE_ROOT_FINDER, IMAGE_ADLIST_FINDER));
    IFile::createDirectory(StringUtils::format("%s/%s", IMAGE_ROOT_FINDER, IMAGE_ADREWARD_FINDER));
    IFile::createDirectory(StringUtils::format("%s/%s", IMAGE_ROOT_FINDER, IMAGE_SCREENSHOOT_FINDER));
    IFile::createDirectory(StringUtils::format("%s/%s", IMAGE_ROOT_FINDER, IMAGE_THUMBNAIL_FINDER));
    
    
    ssize_t size = 0;
    unsigned char* titlech = FileUtils::getInstance()->getFileData("CSres/public/taps.json", "r", &size);
    std::string load_str = std::string((const char*)titlech,size);
    
    tapsdoc.Parse<0>(load_str.c_str());
    
}

GameIni::~GameIni()
{
    
}

GameIni* GameIni::getInstance()
{
    if(gameini == NULL)
    {
        gameini = new GameIni();
    }
    return gameini;
}


void GameIni::destroyInstance()
{
    if(gameini)
    {
        delete gameini;
        gameini = NULL;
    }
}

void GameIni::getNetworkIP(const string& s)
{
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    
    if(doc.HasParseError() || !doc.HasMember(np_serverconfig))
    {
        return;
    }
    rapidjson::Value &d = doc[np_serverconfig];
    if(d.HasMember(np_market_ip))       market_ip = d[np_market_ip].GetString();
    if(d.HasMember(np_server_ip))       server_ip = d[np_server_ip].GetString();
    if(d.HasMember(np_match_ip))        match_ip  = d[np_match_ip].GetString();
    if(d.HasMember(np_game_ip))         game_ip   = d[np_game_ip].GetString();
    if(d.HasMember(np_bag_url))         bag_url   = d[np_bag_url].GetString();
    if(d.HasMember(np_tel))             officialtel = d[np_tel].GetString();
    if(d.HasMember(np_add))             officialadd = d[np_add].GetString();
    if(d.HasMember(np_weburl))          officialwebsite = d[np_weburl].GetString();
    if(d.HasMember(np_appleappid))      appleAppID = d[np_appleappid].GetString();
    if(d.HasMember(np_space_of_wait2))  space_of_wait2 = d[np_space_of_wait2].GetInt();
    if(d.HasMember(np_times_of_wait2))  times_of_wait2 = d[np_times_of_wait2].GetInt();
    if(d.HasMember(np_space_of_war2))   space_of_war2 = d[np_space_of_war2].GetInt();
    if(d.HasMember(np_space_of_wait4))  space_of_wait4 = d[np_space_of_wait4].GetInt();
    if(d.HasMember(np_times_of_wait4))  times_of_wait4 = d[np_times_of_wait4].GetInt();
    if(d.HasMember(np_space_of_war4))   space_of_war4 = d[np_space_of_war4].GetInt();
    if(d.HasMember(np_lostgold_acdsee)) lostgold_acdsee = d[np_lostgold_acdsee].GetInt();
    if(d.HasMember(np_share_sdk_info))
    {
        shareSdkInfo.title = d[np_share_sdk_info]["s_title"].GetString();
        shareSdkInfo.webpageUrl = d[np_share_sdk_info]["s_link"].GetString();
        shareSdkInfo.description = d[np_share_sdk_info]["s_content"].GetString();
    }
    
}


void GameIni::preloadMusic()
{
    const char bg[][100] = {music_bg_game, music_bg_main, music_bg_activity, music_bg_personchange, music_ef_pageplay};
    for(int i = 0; i < sizeof(bg)/sizeof(bg[0]); i ++)
    {
        IAudio::getInstance()->preloadBackground(bg[i]);
    }
    
    const char effect[][100] = {music_ef_321, music_ef_back, music_ef_ok, music_ef_cancel, music_ef_droppic, music_ef_error, music_ef_fallmoney,
        music_ef_logo, music_ef_lost, music_ef_move, music_ef_pagechange, music_ef_pageplay, music_ef_preview, music_ef_setting, music_ef_win};
    for(int i = 0; i < sizeof(effect)/sizeof(effect[0]); i ++)
    {
        IAudio::getInstance()->preloadEffect(effect[i]);
    }
    
    bool music = UserDefault::getInstance()->getBoolForKey(music_key, YES);
    IAudio::getInstance()->setVolume(music ? 1.0 : 0);
}


string GameIni::getTips(int num)
{
    string s = "";
    string key = StringUtils::format("%d", num);
    if(tapsdoc.IsArray())
    {
        uint i = 0;
        if(tapsdoc[i].HasMember(key.c_str()))
            s = tapsdoc[i][key.c_str()].GetString();
    }
    return s;
}

int GameIni::getHeadIndex(const string& s)
{
    string str  = ITools::Intercept_string(s, "/");
    string str2 = ITools::Intercept_stringEnd(str, ".");
    return atoi(ITools::Intercept_string(str2, "_").c_str());
}


void GameIni::exitApp()
{
    Director::getInstance()->end();
//    Module::destroy();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameIni::afterCapture(bool succeed, const string& outputFile)
{
    if(succeed)
    {
        Args args;
        args.content = outputFile;
        GAMELOG("outfile == %s", outputFile.c_str());
        IMessageManager::getInstance()->postNotification(MES_GAMEINI_SCREENSHOOT_SUCCEED, &args);
    }
    else
    {
        IMessageManager::getInstance()->postNotification(MES_GAMEINI_SCREENSHOOT_FAILED);
    }
}

string GameIni::getLocalSavedImagePath(const string& imageName, int type, bool isWritablePath)
{
    string path = "";
    switch (type)
    {
        case AD:
        {
            path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_AD_FINDER, imageName.c_str());
        }break;
        case ADLIST:
        {
            path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_ADLIST_FINDER, imageName.c_str());
        }break;
        case ADREWARD:
        {
            path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_ADREWARD_FINDER, imageName.c_str());
        }break;
        case SCREENSHOOT:
        {
            path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_SCREENSHOOT_FINDER, imageName.c_str());
        }break;
        case THUMBNAIL:
        {
            path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_THUMBNAIL_FINDER, imageName.c_str());
        }break;
        case HEAD:
        {
            path = StringUtils::format("CSres/Setup/Head/%s", imageName.c_str());
            isWritablePath = false;
        }break;
        default:
            break;
    }
    if(isWritablePath)
        path = FileUtils::getInstance()->getWritablePath() + path;
    return path;
}
//获取本地图片，将本地图片保存到沙盒内。（surName－本地图片路径）
string GameIni::getShareLocalPath(const string& surName)
{
    string path = getLocalSavedImagePath(ITools::Intercept_string(surName, "/"), THUMBNAIL);
    if(IFile::shareLocalSprite(surName, path))
    {
        return FileUtils::getInstance()->getWritablePath() + path;
    }
    return "";
}



//void GameIni::screenShoot(const string& filepath)
//{
//    utils::captureScreen(afterCapture, filepath);
//}


void GameIni::screenShoot()
{
    utils::captureScreen(afterCapture, getLocalSavedImagePath(IMAGE_SCREENSHOOT_FILENAME, ImagePathType::SCREENSHOOT, YES));
}


void GameIni::writeStringToTxt(const string& str)
{
    IFile::writeStringToFile(str, GameIni::getInstance()->getLocalSavedImagePath("logceshi.txt", GameIni::ImagePathType::SCREENSHOOT, true));
}


bool IsPhoneNumber(string str)
{
    GAMELOG("IsPhoneNumber %s", str.c_str());
    if(str.empty() || str.length() < 11)
        return false;
    
    
    for(int i = 0; i < str.size(); i ++)
    {
        if(i == 0 && str[i] == '0')
        {
            return false;
        }
        
        if ( (str[i] > '9') || (str[i] < '0') )
        {
            return false;
        }
    }
    return true;
}



bool PassWordRule(string str)//判断是否为数字，字母，下划线
{
    if( str.empty() )
        return false;
    
    for(uint i = 0; i < str.size(); i ++)
    {
        if((str[i] >= '0' && str[i] <= '9') ||
           (str[i] >= 'a' && str[i] <= 'z') ||
           (str[i] >= 'A' && str[i] <= 'Z') ||
           (str[i] == '_')
           )
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}


//void PlayerData::phonecall(const string& s)
//{
//    if(s.empty())
//        return;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    {
//        PluginHelper::getInstance()->phonecall(s);
//    }
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    {
//        JniMethodInfo t;
//        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "phonecall", "(Ljava/lang/String;)V"))
//        {
//            jstring str = t.env->NewStringUTF(s.c_str());
//            t.env->CallStaticVoidMethod(t.classID, t.methodID, str);
//            t.env->DeleteLocalRef(t.classID);
//        }
//    }
//#endif
//    
//    GAMELOG("phonecall: %s", s.c_str());
//}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

void GameIni::getDeviceInfo()
{
    string deviceInfo = "";
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        
    }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/NgsteamPhoneInfo",
                                           "getRegPhoneInfoString", "()Ljava/lang/String;"))
        {
            jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
            deviceInfo = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(t.classID);
        }
    }
    #endif
    
    GAMELOG("----gameini---getdeviceInfo-----\n%s", deviceInfo.c_str());
}


void getLocationInfo(double x, double y)
{
    GAMELOG("----gameini---getLocationInfo----x = %f-y = %f", x, y);
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    jboolean Java_org_cocos2dx_cpp_AppActivity_isTestMode(JNIEnv* env, jobject thiz)
    {
        #ifdef GAME_TEST
            return true;
        #else
            return false;
        #endif
    }
    
    jint Java_org_cocos2dx_cpp_AppActivity_getScreenWidth(JNIEnv* env, jobject thiz)
    {
        return SCREEN_WIDTH;
    }
    
    jint Java_org_cocos2dx_cpp_AppActivity_getScreenHeight(JNIEnv* env, jobject thiz)
    {
        return SCREEN_HEIGHT;
    }
}
#endif





