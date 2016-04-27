//
//  PlayerData.cpp
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#include "PlayerData.h"
#include "../../gameini/GameIni.h"
#include "../../PluginHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "platform/android/jni/JniHelper.h"
    #define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"
#endif
#include "../../chooseCity/LocPlay.h"
static int isexsits_;
static string nickname_;
static int yaoqinghaoyou=1;
PlayerData::PlayerData()
{
//    userid  = "13691001650";
//    userpsw = "1111";
//    identity = 1;
//    invitecode = "";
//
//    headIndex = -1;
//    gender = -1;
//    postprovinceid = -1;
//    postcityid = -1;
//    provinceid = -1;
//    cityid = -1;
//    postphone = "";
//    nickname = "";
//    postname = "";
//    birthday = "";
//    postprovincename = "";
//    postcityname = "";
//    postaddress = "";
    
    setCoordinate(0.0, 0.0);
    
    Cocos::PushService::registCallBack(this);
}

PlayerData::~PlayerData()
{
    v_friends.clear();
}


void PlayerData::initp()
{
    if(!IDataManager::getInstance()->isExist(M_PLAYER))
    {
        PlayerData* player = new PlayerData();
        IDataManager::getInstance()->attach(player, M_PLAYER);
    }
}

void PlayerData::destroy()
{
    PlayerData* player = (PlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    IDataManager::getInstance()->detach(M_PLAYER);
    SAFE_DELETE(player);
}

void PlayerData::release()
{
    delete this;
}


void PlayerData::saveLocalData()
{
    UserDefault::getInstance()->setStringForKey(userid_key, userid);
    UserDefault::getInstance()->setStringForKey(userpsw_key, userpsw);
    UserDefault::getInstance()->flush();
}


string PlayerData::getLocalUserID()
{
    return  UserDefault::getInstance()->getStringForKey(userid_key, "");
}
string PlayerData::getLocalUserPsw()
{
    return UserDefault::getInstance()->getStringForKey(userpsw_key, "");
}

bool PlayerData::localDataUseful()
{
    return (!getLocalUserID().empty() && !getLocalUserPsw().empty());
}

void PlayerData::loginOut()
{
    UserDefault::getInstance()->setStringForKey(userid_key, "");
    UserDefault::getInstance()->setStringForKey(userpsw_key, "");
    UserDefault::getInstance()->flush();
    IMessageManager::getInstance()->postNotification(MES_PLAYER_LOGOUT);
}

bool PlayerData::isCompletedInfo()  
{
    if(birthday.empty())
        return NO;
//    if(cityid <= 0)
//        return NO;
    if(cityname.empty())
        return NO;
    if(gender == -1)
        return NO;
//    if(postcityid <= 0 || postprovinceid <= 0)
//        return NO;
//    if(postphone.empty() || postaddress.empty())
//        return NO;
    return YES;
}


bool PlayerData::setDataByLocal()
{
    if(localDataUseful())
    {
        userid = getLocalUserID();
        userpsw = getLocalUserPsw();
        return YES;
    }
    return NO;
}

void PlayerData::setCoordinate(double _longitude, double _latitude)
{
    longitude = StringUtils::format("%lf", _longitude);
    latitude  = StringUtils::format("%lf", _latitude);
    
    GAMELOG("PlayerData::setCoordinate longitude = %s, latitude = %s", longitude.c_str(), latitude.c_str());
}




void PlayerData::getCoordinate()
{
    string last_all;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        PluginHelper::getInstance()->getPlayerPosition();
        PluginHelper::getInstance()->getCity();//获取位置
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        JniMethodInfo t;
        //        string location;
        //        double _longitude = 0.0;
        //        double _latitude  = 0.0;
        //        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getLocation", "(I)D"))
        //        {
        //            _longitude = t.env->CallStaticDoubleMethod(t.classID, t.methodID, 0);//参数0取经度
        //            t.env->DeleteLocalRef(t.classID);
        //        }
        //        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getLocation", "(I)D"))
        //        {
        //            _latitude = t.env->CallStaticDoubleMethod(t.classID, t.methodID, 1);//参数0取纬度
        //            t.env->DeleteLocalRef(t.classID);
        //        }
        //        setCoordinate(_longitude, _latitude);
        //        IMessageManager::getInstance()->postNotification(MES_PLAYER_GETLOCATION);
        
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getLocationString", "(I)Ljava/lang/String;"))
        {
            jstring channel = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID,0);
            last_all=JniHelper::jstring2string(channel);
            LocPlay::getInstance()->setProvince(last_all);
            GAMELOG("111 %s",last_all.c_str());
            
        }
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getLocationString", "(I)Ljava/lang/String;"))
        {
            
            jstring channel = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID,1);
            last_all=JniHelper::jstring2string(channel);
            LocPlay::getInstance()->setIs_iphone(false);
            LocPlay::getInstance()->setCity(last_all);
            GAMELOG("222 %s",last_all.c_str());
        }
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getLocationString", "(I)Ljava/lang/String;"))
        {
            jstring channel = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID,2);
            last_all=JniHelper::jstring2string(channel);
            LocPlay::getInstance()->setConty(last_all);
            GAMELOG("333 %s",last_all.c_str());
        }
    }
#endif
    
    GAMELOG("getCoordinate longitude2222222 = %s, ", last_all.c_str());
}





//string PlayerData::getSDcardPath()
//{
//    string path = "";
//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    {
//        JniMethodInfo t;
//        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getSDPath", "()Ljava/lang/String;"))
//        {
//            jstring sdpath = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
//            path = JniHelper::jstring2string(sdpath);
//            t.env->DeleteLocalRef(t.classID);
//        }
//    }
//    #endif
//    
//    return path;
//}


void PlayerData::phonecall(const string& s)
{
    if(s.empty())
        return;
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        PluginHelper::getInstance()->phonecall(s);
    }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "phonecall", "(Ljava/lang/String;)V"))
        {
            jstring str = t.env->NewStringUTF(s.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, str);
            t.env->DeleteLocalRef(t.classID);
        }
    }
#endif
    
    GAMELOG("phonecall: %s", s.c_str());
}

void PlayerData::comment(const string& appID)
{
    if(appID.empty())
        return;
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        PluginHelper::getInstance()->comment(appID);
    }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        
    }
    #endif
}


float PlayerData::getPecentScore()
{
    float pecent = 0;
    if(pointslimit != 0)
    {
        pecent = (float)(points - pointscur) / (float)(pointslimit - pointscur);
//        pecent = points / (float)pointslimit;
    }

//    190 - 0 、 500 - 0
//    
//    "rankname":"平民","needpoints":0,"nextrankname":"骑士","nextneedpoints":500

    
    GAMELOG("pecent = %f", pecent);
    return pecent > 1.f ? 1.f : pecent;
}

void PlayerData::updateInfo(const string& s)
{
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    
    if(doc.HasParseError() || !doc.HasMember(np_playerinfo))
    {
        //auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    rapidjson::Value &d = doc[np_playerinfo];
    
    if(d.HasMember("loginkey"))
    {
        LandKey = d["loginkey"].GetString();
        MiYao_key = StringUtils::format("%s%s", userid.c_str(), LandKey.c_str());
        MiYao_key = MD5::GenerateMD5(MiYao_key.c_str(), (int)MiYao_key.length());
    }
    if(d.HasMember("imageurl"))
    {
        uHeadIndex = ITools::Intercept_string(d["imageurl"].GetString(), "/");
        headIndex  = GameIni::getInstance()->getHeadIndex(uHeadIndex);
    }
    
    if(d.HasMember("loginname"))        userid = d["loginname"].GetString();
    if(d.HasMember("nickname"))         nickname = d["nickname"].GetString();
    if(d.HasMember("rankname"))         rankname = d["rankname"].GetString();
    if(d.HasMember("nextrankname"))     nextrankname = d["nextrankname"].GetString();
    if(d.HasMember("points"))           points = d["points"].GetInt();
    if(d.HasMember("needpoints"))       pointscur = d["needpoints"].GetInt();
    if(d.HasMember("nextneedpoints"))   pointslimit = d["nextneedpoints"].GetInt();
    if(d.HasMember("todaypoints"))      todaypoints = d["todaypoints"].GetInt();
    if(d.HasMember("totalpoints"))      totalpoints = d["totalpoints"].GetInt();
    if(d.HasMember("friendspoints"))    friendspoints = d["friendspoints"].GetInt();
    if(d.HasMember("golds"))            golds = d["golds"].GetInt();
    if(d.HasMember("goldslimit"))       goldslimit = d["goldslimit"].GetInt();
    if(d.HasMember("totalgolds"))       totalgolds = d["totalgolds"].GetInt();
    if(d.HasMember("todaygolds"))       todaygolds = d["todaygolds"].GetInt();
    if(d.HasMember("gamegolds"))        gamegolds = d["gamegolds"].GetInt();
    if(d.HasMember("friendsgolds"))     friendsgolds = d["friendsgolds"].GetInt();
    if(d.HasMember("email"))            email = d["email"].GetString();
    if(d.HasMember("mobile"))           mobile = d["mobile"].GetString();
    if(d.HasMember("grade"))            grade = d["grade"].GetInt();
    if(d.HasMember("registertime"))     registertime = d["registertime"].GetString();
    if(d.HasMember("description"))      description = d["description"].GetString();
    if(d.HasMember("idnumber"))         idnumber = d["idnumber"].GetString();
    if(d.HasMember("firstname"))        firstname = d["firstname"].GetString();
    if(d.HasMember("lastname"))         lastname = d["lastname"].GetString();
    if(d.HasMember("fullname"))         fullname = d["fullname"].GetString();
    if(d.HasMember("company"))          company = d["company"].GetString();
    if(d.HasMember("department"))       department = d["department"].GetString();
    if(d.HasMember("duty"))             duty = d["duty"].GetString();
    if(d.HasMember("title"))            title = d["title"].GetString();
    if(d.HasMember("countryname"))      countryname = d["countryname"].GetString();
    if(d.HasMember("provincename"))     provincename = d["provincename"].GetString();
    if(d.HasMember("cityname"))         cityname = d["cityname"].GetString();
    if(d.HasMember("postalcode"))       postalcode = d["postalcode"].GetString();
    if(d.HasMember("address"))          address = d["address"].GetString(); 
    if(d.HasMember("birthday"))         birthday = d["birthday"].GetString();
    if(d.HasMember("gender"))           gender = d["gender"].GetInt();
    if(d.HasMember("fax"))              fax = d["fax"].GetString();
    if(d.HasMember("ismarried"))        ismarried = d["ismarried"].GetInt();
    if(d.HasMember("postalcode"))       postalcode = d["postalcode"].GetString();
    if(d.HasMember("childrencount"))    childrencount = d["childrencount"].GetString();
    if(d.HasMember("personalincome"))   personalincome = d["personalincome"].GetString();
    if(d.HasMember("familyincome"))     familyincome = d["familyincome"].GetString();
    if(d.HasMember("industryname"))     industryname = d["industryname"].GetString();
    if(d.HasMember("vocationname"))     vocationname = d["vocationname"].GetString();
    if(d.HasMember("postname"))         postname = d["postname"].GetString();
    if(d.HasMember("postphone"))        postphone = d["postphone"].GetString();
    if(d.HasMember("postprovinceid"))   postprovinceid = d["postprovinceid"].GetInt();
    if(d.HasMember("postprovincename")) postprovincename = d["postprovincename"].GetString();
    if(d.HasMember("postcityid"))       postcityid = d["postcityid"].GetInt();
    if(d.HasMember("postcityname"))     postcityname = d["postcityname"].GetString();
    if(d.HasMember("postaddress"))      postaddress = d["postaddress"].GetString();
    if(d.HasMember("invitecode"))       myFriendId = d["invitecode"].GetString();
    if(d.HasMember("num_goods"))        goodsnum = d["num_goods"].GetInt();
    if(d.HasMember("num_ticket"))       lotterynum = d["num_ticket"].GetInt();
    if(d.HasMember("curpoints"))        friendsTotalPoints = d["curpoints"].GetInt();
    if(d.HasMember("curgolds"))         friendsTotalGolds = d["curgolds"].GetInt();
    if(d.HasMember("cityid"))           cityid = d["cityid"].GetInt();
    if(d.HasMember("prizewinning"))     prizewinning = d["prizewinning"].GetInt();
    if(d.HasMember("goldincrement"))
        tempAddGolds = d["goldincrement"].GetInt();
    else
        tempAddGolds = 0;
    
    
    
    if(d.HasMember("isrank") && d["isrank"].GetInt() != 0)
    {
        IMessageManager::getInstance()->postNotification(MES_PLAYER_UPGRADE);
    }
    IMessageManager::getInstance()->postNotification(MES_PLAYER_UPDATEINFO);
}

void PlayerData::http_request_login()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
     Player _player;
    _player.setloginname(userid);
    object.AddMember("loginname", userid.c_str(), allocator);
    CCLOG("拼乐发送密码 %s",userpsw.c_str());
    object.AddMember("password", MD5::GenerateMD5(userpsw.c_str(), (int)userpsw.length()), allocator);
    
    //ip
    object.AddMember("ip","219.202.134.68" , allocator);
    object.AddMember("latitude", latitude.c_str(), allocator);
    object.AddMember("longtitude", longitude.c_str(), allocator);
    object.AddMember("systemtype", APP_CHANNEL, allocator);
    object.AddMember("origin", APP_ORIGIN, allocator);

    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "loginin", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    CCLOG("登录时候 %s  弹出错误框",buffer->GetString());
    CCLOG("123333  %s  登录",newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_login, this);

}

//登录后返回数据
void PlayerData::http_response_login(const string& s)
{
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("注册后返回数据 %s",s.c_str());
    if(doc.HasParseError() )
    {
       auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        
        return;
    }
    GAMELOG("denglu------------------11");
    IRoomData* r = M_GET_ROOM;
    GAMELOG("denglu------------------12");
    r->updateGameInfo(s);
    GAMELOG("denglu------------------13");
    updateInfo(s);
    GAMELOG("denglu------------------14");
    GameIni::getInstance()->getNetworkIP(s);
    GAMELOG("denglu------------------15");
    saveLocalData();
    GAMELOG("denglu------------------16");
    IMessageManager::getInstance()->postNotification(MES_PLAYER_LOGIN);
    GAMELOG("denglu------------------17");
    //CocosPush设置用户标签
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Cocos::PushService::setTags(CocosPush::getStrTag(userid));
#else
    Cocos::PushService::setStringTags(userid.c_str());
//    Cocos::PushService::setTags(CocosPush::getStrTag(userid));
#endif
    
//    GAMELOG("denglu------------------18");
//    //CocosPush设置用户账号
    Cocos::PushService::setAccount(userid.c_str());
    GAMELOG("denglu------------------19");
}
//注册后进入
void PlayerData::http_request_regist(const string& invitecode)
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    Player _player;
    _player.setloginname(userid);
    object.AddMember("loginname", userid.c_str(), allocator);
    string password = MD5::GenerateMD5(userpsw.c_str(), (int)userpsw.length());
    object.AddMember("password", password.c_str(), allocator);
    CCLOG("loginname=%s ,  password=%s ,length=%d ",userid.c_str(),userpsw.c_str(),(int)userpsw.length());
    //ip
    object.AddMember("ip","219.202.134.68" , allocator);
    object.AddMember("latitude", latitude.c_str(), allocator);
    object.AddMember("longtitude", longitude.c_str(), allocator);
    object.AddMember("systemtype", APP_CHANNEL, allocator);
    object.AddMember("origin", APP_ORIGIN, allocator);
    string _identity = StringUtils::format("%d", identity);
    object.AddMember("type", _identity.c_str(), allocator);
    if(!invitecode.empty())
        object.AddMember("invitecode", invitecode.c_str(), allocator);
    
    rapidjson::StringBuffer * buffer12 = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer1(*buffer12);
    object.Accept(writer1);
    CCLOG("组册后数据库保存的内容  %s ＝＝＝＝＝＝",buffer12->GetString());
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, np_reg, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    CCLOG("注册登录前发送显示数据 %s ＝＝＝＝%s====拼乐",buffer->GetString(),userpsw.c_str());
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    CCLOG("注册后url %s  注册",newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_regist, this);
}
void PlayerData::http_response_regist(const string& s)
{
    http_response_login(s);
}
void PlayerData::http_request_nicknameInfo(const string& s){
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("nickname", s.c_str(), allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "validatenickname", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    
    
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB = CC_CALLBACK_1(PlayerData::Get_request_nicknameInfo, this);
}
void  PlayerData::  Get_request_nicknameInfo(const string& s){
     CCLOG("昵称－－－  %s 信息 ",s.c_str());
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
   
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }

    //char *  _isexsits;
    if(doc.HasMember("isexists"))        _isexsits = doc["isexists"].GetInt();
    if(doc.HasMember("nickname"))         _nickname = doc["nickname"].GetString();
    //_isexsits=true;
    isexsits_=_isexsits;
    nickname_=_nickname;
    
    
    //CCLOG("111111  %d  %d   %s",num , b ,_nickname.c_str());
    IMessageManager::getInstance()->postNotification(MES_PLAYER_nickname);
}
int  PlayerData::   Getisexsits(){
    return isexsits_;
}
string PlayerData::  Getnickname(){
    return nickname_;
}
void  PlayerData:: http_request_cityInfo(Player* p){
    if(!p)
        return;
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getusercitybyphone", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    
    
    myHttpWork* http = myHttpWork::createHttp();
    http->_args.ptr = (void*)p;
    http->_args.code = 50;
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB3 = CC_CALLBACK_2(PlayerData::http_response_cityInfo, this);
}
void  PlayerData::  http_response_cityInfo(const string& s, Args args){
    if(s.empty())
        return;
   
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("城市－－－  %s 信息 ",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
   
//    Player* p = (Player*)args.ptr;
//    if(p->cityid > -1)
//        cityid = p->cityid;
//    if(!p->cityname.empty())
//        cityname = p->cityname;
    LocPlay::getInstance()->setProvince(doc["provincename"].GetString());
    LocPlay::getInstance()->setCity(doc["cityname"].GetString());
    LocPlay::getInstance()->setConty("");
    LocPlay::getInstance()->setIs_iphone(true);
    
}
void  PlayerData::  Invitation_code(Player* p,const string& invitecode){
    if(!p)
        return;
    Player _player;
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    //object.AddMember("invitecode", invitecode.c_str(), allocator);
    object.AddMember("playerid", _player.getloginname().c_str(), allocator);
    object.AddMember("friendplayerid",invitecode.c_str() , allocator);
    object.AddMember("type", "1", allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setfriend", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    
    myHttpWork* http = myHttpWork::createHttp();
    http->_args.ptr = (void*)p;
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB3 = CC_CALLBACK_2(PlayerData::GetInvitation_code, this);
}
int PlayerData::get_error(){
    return yaoqinghaoyou;
}
void  PlayerData::  GetInvitation_code(const string& s, Args args){
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("邀请好友－－－  %s 信息 ",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("输入错误，请重新输入");
         yaoqinghaoyou=1;
         IMessageManager::getInstance()->postNotification("fried_yaoqing");
        return;
    }
    if(doc.HasMember("err_code"))      err_code = doc["err_code"].GetInt();
    CCLOG("%d  %d",err_code,yaoqinghaoyou);
    yaoqinghaoyou=err_code;
    if (err_code==0) {
        auto messageBox = UIMessage::createMesBox("恭喜邀请好友成功");
        return;
    }
//    rapidjson::Value &game = doc["playerinfo"];
//    rapidjson::Value &d = game;
//    Player* p = (Player*)args.ptr;
//    if(d.HasMember("invitecode"))      _invitecode = d["invitecode"].GetString();
//     p->golds=atoi(_invitecode.c_str());
    
}
void PlayerData::http_request_updateInfo(Player* p)
{
    if(!p)
        return;
    
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("loginname", userid.c_str(), allocator);
    
    string headicon = StringUtils::format("%d", p->headIndex);
    //------头像---------
    if(p->headIndex > -1)
    {
        object.AddMember("imageurl", headicon.c_str(), allocator);
    }
    //------昵称---------
    if(!p->nickname.empty())
        object.AddMember("nickname", p->nickname.c_str(), allocator);
    //------性别---------
    if(p->gender == 0 || p->gender == 1){
        object.AddMember("gender", p->gender, allocator);
    }
    //------手机号---------
    if(!p->postphone.empty())
        object.AddMember("postphone", p->postphone.c_str(), allocator);
    //------邮寄真名---------
    if(!p->postname.empty())
        object.AddMember("postname", p->postname.c_str(), allocator);
    //------生日---------
    if(!p->birthday.empty())
        object.AddMember("birthday", p->birthday.c_str(), allocator);
    //------邮寄省ID---------
    if(p->postprovinceid > -1)
        object.AddMember("postprovinceid", p->postprovinceid, allocator);
    //------邮寄市ID---------
    if(p->postcityid > -1)
        object.AddMember("postcityid", p->postcityid, allocator);
    //------所在省ID---------
    if(p->provinceid > -1)
        object.AddMember("provinceid", p->provinceid, allocator);
    //------所在市ID---------
    if(p->cityid > 0)
        object.AddMember("cityid", p->cityid, allocator);

    //------所在街道名字---------
    if(postaddress.compare(p->postaddress) != 0 && !p->postaddress.empty())
        object.AddMember("postaddress", p->postaddress.c_str(), allocator);
    
    object.AddMember("latitude", latitude.c_str(), allocator);
    object.AddMember("longtitude", longitude.c_str(), allocator);
    
    //--------邀请我的好友-----------------
    if(!p->myFriendId.empty()  && yaoqinghaoyou==0){
        object.AddMember("invitecode",p->myFriendId.c_str(), allocator);
    }
    GAMELOG("##%s",p->myFriendId.c_str());

    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, np_setuserinfo, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    
    myHttpWork* http = myHttpWork::createHttp();
    http->_args.ptr = (void*)p;
    http->_args.code = 50;
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB3 = CC_CALLBACK_2(PlayerData::http_response_updateInfo, this);
}

void PlayerData::http_response_updateInfo(const string& s, Args args)
{
    Player* p = (Player*)args.ptr;
    //------头像---------
    if(p->headIndex > -1)
    {
        uHeadIndex = StringUtils::format("headicon_%d.jpg", p->headIndex);
        headIndex  = p->headIndex;
    }
    //------昵称---------
    if(!p->nickname.empty())
        nickname = p->nickname;
    //------性别---------
    if(p->gender == 0 || p->gender == 1)
        gender = p->gender;
    //------手机号---------
    if(!p->postphone.empty())
        postphone = p->postphone;
    //------邮寄真名---------
    if(!p->postname.empty())
        postname = p->postname;
    //------生日---------
    if(!p->birthday.empty())
        birthday = p->birthday;
    //------邮寄省ID---------
    if(p->postprovinceid > -1)
        postprovinceid = p->postprovinceid;
    if(!p->postprovincename.empty())
        postprovincename = p->postprovincename;
    //------邮寄市ID---------
    if(p->postcityid > -1)
        postcityid = p->postcityid;
    if(!p->postcityname.empty())
        postcityname = p->postcityname;
    //------所在省ID---------
    if(p->provinceid > -1)
        provinceid = p->provinceid;
    if(!p->provincename.empty())
        provincename = p->provincename;
    //------所在市ID---------
    if(p->cityid > -1)
        cityid = p->cityid;
    if(!p->cityname.empty())
        cityname = p->cityname;
    //详细邮寄地址
    if(!p->postaddress.empty() && postaddress.compare(p->postaddress) != 0)
        postaddress = p->postaddress;

    updateInfo(s);
}



void PlayerData::http_request_changePassword()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("loginname", userid.c_str(), allocator);
    object.AddMember("newpassword", MD5::GenerateMD5(userpsw.c_str(), (int)userpsw.length()), allocator);
    object.AddMember("systemtype", APP_CHANNEL, allocator);
    object.AddMember("origin", APP_ORIGIN, allocator);
    object.AddMember("latitude", latitude.c_str(), allocator);
    object.AddMember("longtitude", longitude.c_str(), allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setpassword", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_changePassword, this);
}

void PlayerData::http_response_changePassword(const string& s)
{
    IMessageManager::getInstance()->postNotification(MES_PLAYER_SETPASSWORD);
}


void PlayerData::http_request_getVerityCode(const string& phone, const string& verifycode, int type)
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocatcor = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);

    string _type = StringUtils::format("%d", type);
    object.AddMember("type", _type.c_str(), allocatcor);
    object.AddMember("phone", phone.c_str(), allocatcor);//手机号
    object.AddMember("code", verifycode.c_str(), allocatcor);//验证码
    object.AddMember("systemtype", APP_CHANNEL, allocatcor);
    object.AddMember("origin", APP_ORIGIN, allocatcor);
    object.AddMember("latitude", latitude.c_str(), allocatcor);
    object.AddMember("longtitude", longitude.c_str(), allocatcor);

    document.AddMember(np_methodtype, "json", allocatcor);
    document.AddMember(np_createtime, "1413800515", allocatcor);
    document.AddMember(np_functionname, np_sendmessage, allocatcor);
    document.AddMember(np_functionparams, object, allocatcor);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_getVerityCode, this);

}
void PlayerData::http_response_getVerityCode(const string& s)
{
    IMessageManager::getInstance()->postNotification(MES_PLAYER_VERITYCODE);
}


void PlayerData::http_request_checkVersion()
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("devicetype", APP_CHANNEL, allocator);
    object.AddMember("versioncode", APP_VERSION, allocator);
    object.AddMember("latitude", latitude.c_str(), allocator);
    object.AddMember("longtitude", longitude.c_str(), allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getversion", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_checkVersion, this);
}
void PlayerData::http_response_checkVersion(const string& s)
{
    if(s.empty())
        return;
    
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    
    if(d.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    Args args;
    INIT_ARGS(args);
    if(d.HasMember("isused"))
        args.code = d["isused"].GetInt();
    args.code2 = d["update"].GetInt();
    args.code3 = d["forcedupdate"].GetInt();
    args.content = d["url"].GetString();
    
    IMessageManager::getInstance()->postNotification(MES_PLAYER_UPDATEVERSION, &args);
}



void PlayerData::http_request_setInviteCode(const string& s)
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("invitecode", s.c_str(), allocator);

    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setinvitecode", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->_args.content = s;
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_setInviteCode, this);
}
void PlayerData::http_response_setInviteCode(const string& s)
{
    updateInfo(s);  
    IMessageManager::getInstance()->postNotification(MES_PLAYER_SETINVITECODE);
}


void PlayerData::http_request_getFriendsInfo()
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "get_reward_friend_list", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_getFriendsInfo, this);
    
}
void PlayerData::http_response_getFriendsInfo(const string& s)
{
    CCLOG("%s",s.c_str());
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }    if(doc.HasMember("points"))         friendsTotalPoints = doc["points"].GetInt();
    if(doc.HasMember("golds"))          friendsTotalGolds = doc["golds"].GetInt();
    if(doc.HasMember("one_points"))     friendspoints = doc["one_points"].GetInt();
    if(doc.HasMember("one_golds"))      friendsgolds = doc["one_golds"].GetInt();
    
    if(doc.HasMember("friendlist"))
    {
        v_friends.clear();
        rapidjson::Value &friendlist = doc["friendlist"];
        if(friendlist.IsArray())
        {
            for(uint i = 0; i < friendlist.Size(); i ++)
            {
                rapidjson::Value &d = friendlist[i];
                Player p;
                if(d.HasMember("loginname"))        p.userid = d["loginname"].GetString();
                if(d.HasMember("nickname"))         p.nickname = d["nickname"].GetString();
                if(d.HasMember("playergrade"))      p.rankname = d["playergrade"].GetString();//当前等级
                if(d.HasMember("total_golds"))      p.totalgolds = d["total_golds"].GetInt();//
                if(d.HasMember("today_golds"))      p.todaygolds = d["today_golds"].GetInt();
                if(d.HasMember("noreceived_points"))p.ungetPoints = d["noreceived_points"].GetInt();
                if(d.HasMember("imgurl"))           p.uHeadIndex = ITools::Intercept_string(d["imgurl"].GetString(), "/");
                v_friends.push_back(p);
                //根据今日贡献金币数排序
//                uint n = 0;
//                for(; n < v_friends.size(); n ++)
//                {
//                    if(p.todaygolds > v_friends[n].todaygolds)
//                        break;
//                }
//                v_friends.insert(v_friends.begin() + n, p);
            }
        }
    }
    
    IMessageManager::getInstance()->postNotification(MES_PLAYER_GETFRIENDINFO);
}


void PlayerData::http_request_getFriendsReward()
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "get_reward_of_friends_levelup", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_getFriendsReward, this);
}

void PlayerData::http_response_getFriendsReward(const string& s)
{
    updateInfo(s);
    
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    if(doc.HasMember("friendlist"))
    {
        v_friends.clear();
        rapidjson::Value &friendlist = doc["friendlist"];
        if(friendlist.IsArray())
        {
            for(uint i = 0; i < friendlist.Size(); i ++)
            {
                rapidjson::Value &d = friendlist[i];
                Player p;
                if(d.HasMember("loginname"))        p.userid = d["loginname"].GetString();
                if(d.HasMember("nickname"))         p.nickname = d["nickname"].GetString();
                if(d.HasMember("playergrade"))      p.rankname = d["playergrade"].GetString();//当前等级
                if(d.HasMember("total_golds"))      p.totalgolds = d["total_golds"].GetInt();//
                if(d.HasMember("today_golds"))      p.todaygolds = d["today_golds"].GetInt();
                if(d.HasMember("noreceived_points"))p.ungetPoints = d["noreceived_points"].GetInt();
                if(d.HasMember("imgurl"))           p.uHeadIndex = ITools::Intercept_string(d["imgurl"].GetString(), "/");
                v_friends.push_back(p);
                //根据今日贡献金币数排序
                //                uint n = 0;
                //                for(; n < v_friends.size(); n ++)
                //                {
                //                    if(p.todaygolds > v_friends[n].todaygolds)
                //                        break;
                //                }
                //                v_friends.insert(v_friends.begin() + n, p);
            }
        }
    }
    
    friendsgolds = friendspoints = 0;
    IMessageManager::getInstance()->postNotification(MES_PLAYER_GETFRIENDREWARD);
}




void PlayerData::http_request_getAccount()
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getuserstatisticsofgolds", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_getAccount, this);
}

void PlayerData::http_response_getAccount(const string& s)
{
    updateInfo(s);

    IMessageManager::getInstance()->postNotification(MES_PLAYER_GETACCOUNT);
}



void PlayerData::http_request_setReward(int rewardType, const string& s)
{
    rapidjson::Document document;//组合json
    document.SetObject(); 
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("rewardtype", rewardType, allocator);
    if(!s.empty())
    {
        object.AddMember("operation", s.c_str(), allocator);
    }
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setplayerreward", allocator);
    document.AddMember(np_functionparams, object, allocator);

    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->_rc._errorMes = NO;
    http->sendRequest(newurl, "0", buffer->GetString(), 30, NO);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_setReward, this);
}

void PlayerData::http_response_setReward(const string& s)
{
    if(s.empty())
    {
        return;
    }
    
    updateInfo(s);
//    IMessageManager::getInstance()->postNotification(MES_ROOM_CKXQADDGOLDS);
}



void PlayerData::http_request_sendAdvise(int type, const string& s)
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("type", type, allocator);
    object.AddMember("content", s.c_str(), allocator);

    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setfeedbook", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_sendAdvise, this);
    GAMELOG("***sendString = %s", buffer->GetString());
}

void PlayerData::http_response_sendAdvise(const string& s)
{
    IMessageManager::getInstance()->postNotification(MES_PLAYER_SENDADVISE);
}

void  PlayerData::http_request_sendPostAddress(string pname, string pphone, int shengid, int shiid, string paddress)
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("loginname", userid.c_str(), allocator);
    object.AddMember("postname", pname.c_str(), allocator);
    object.AddMember("postphone", pphone.c_str(), allocator);
    object.AddMember("postprovinceid", shengid, allocator);
    object.AddMember("postcityid", shiid, allocator);
    object.AddMember("postaddress", paddress.c_str(), allocator);
    
    
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setusersimple", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
                                        GameIni::getInstance()->server_ip.c_str(),
                                        LandKey.c_str(), MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_sendPostAddress, this);
    GAMELOG("***sendString = %s", buffer->GetString());
}
void PlayerData::http_response_sendPostAddress(const string& s)
{
    GAMELOG("##=%s",s.c_str());
    if(s.empty())
    {
        return;
    }
    
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    int i = doc["err_code"].GetInt();
    if(i == 0)
    {
        IMessageManager::getInstance()->postNotification(MES_PLAYER_POSTADDRESSOK);
    }
}


void PlayerData::http_request_sendXieyi()
{
    rapidjson::Document document;//组合json
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("agreedeal", 1, allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "userdeal", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
//    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s",
//                                        GameIni::getInstance()->server_ip.c_str(),
//                                        LandKey.c_str(), MiYao_key.c_str());
//    
//    GAMELOG("###--%s---%s--%s", GameIni::getInstance()->server_ip.c_str(), LandKey.c_str(), MiYao_key.c_str());
//    myHttpWork* http = myHttpWork::createHttp();
//    http->sendRequest(newurl, "0", buffer->GetString(), 30);
//    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_sendXieyi, this);
//    GAMELOG("***sendString = %s", buffer->GetString());
    
    
    
    string md5str = StringUtils::format("%s%s%s", np_PINLEGAME, buffer->GetString(), np_PINLEGAME);
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        "0", MD5::GenerateMD5(md5str.c_str(), (int)md5str.length()));
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(PlayerData::http_response_sendXieyi, this);
}
void PlayerData::http_response_sendXieyi(const string& s)
{
    GAMELOG("##=%s",s.c_str());
    if(s.empty())
    {
//        GameIni::getInstance()->agreeXiyi = 1;
//        IMessageManager::getInstance()->postNotification(MES_PLAYER_TONGYIXIEYI);
        return;
    }
    
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    if(doc.HasParseError() )
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    int i = doc["err_code"].GetInt();
    if(i == 0)
    {
        GameIni::getInstance()->agreeXiyi = 1;
        IMessageManager::getInstance()->postNotification(MES_PLAYER_TONGYIXIEYI);
    }
}


//===============================【PushListener】======================================================

void PlayerData::onSetTags(int retCode, const Cocos::Tags& failTags)
{
    GAMELOG("----onSetTags----");
}

void PlayerData::onMessage(const string& customContent)
{
    GAMELOG("----onMessage---- = %s", customContent.c_str());
}





