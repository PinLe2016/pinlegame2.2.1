//
//  ActivityData.cpp
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#include "ActivityData.h"

#include "../../gameini/GameIni.h"

#include "../../player/IPlayerData.h"
//#include "UI_Activity.h"
static  string str2=" ";
static  int    jx_num=0;
static  int    w_jifen=0;
static  int    w_paiming=0;
static  vector<string>  guize;
//static  string  _titlename;
ActivityData::ActivityData()
{
    _selectedType  = 0;
    _selectedIndex = 0;
    _downloadNum = 0;
    postWay = 0;
}

ActivityData::~ActivityData()
{
    cleanActivityList();
}


void ActivityData::init()
{
    if(!IDataManager::getInstance()->isExist(M_ACTIVITY))
    {
        ActivityData* activity = new ActivityData();
        IDataManager::getInstance()->attach(activity, M_ACTIVITY);
    }
}
void ActivityData::destroy()
{
    ActivityData* activity = (ActivityData*)IDataManager::getInstance()->getData(M_ACTIVITY);
    IDataManager::getInstance()->detach(M_ACTIVITY);
    SAFE_DELETE(activity);
    ActivityRanking::destroyInstance();
}



void ActivityData::cleanActivityList()
{
    for(uint i = 0; i < E_ActivityType::end; i ++)
    {
        for(int j = 0; j < v_activityList[i].size(); j ++)
        {
            SAFE_DELETE(v_activityList[i][j]);
        }
        v_activityList[i].clear();
    }
}

void ActivityData::addActivity(int type, Activity* a)
{
    if(type < E_ActivityType::end && a != NULL)
    {
        GAMELOG("%d--#-%d",a->matchInfo.curtime, a->matchInfo.totaltime);
        if(a->matchInfo.curtime >= a->matchInfo.totaltime)
            v_activityList[type].push_back(a);
        else
            v_activityList[type].insert(v_activityList[type].begin(), a);
    }
    else
    {
        GAMELOG("can't find the E_ActivityType : %d or Activity *a = NULL", type);
    }
}

Activity* ActivityData::getActivity(int type, uint index)
{
    if(type < E_ActivityType::end)
    {
        if(index < v_activityList[type].size())
            return v_activityList[type][index];
    }
    else
    {
        GAMELOG("can't find the E_ActivityType : %d", type);
    }
    return NULL;
}

vector<ActivityResult>& ActivityData::getRankingList()
{
    return ActivityRanking::getInstance()->getRankingList();
}

Activity* ActivityData::getSelectedActivity()
{
    return getActivity(_selectedType, _selectedIndex);
}
void ActivityData::setSelectedActivity(int type, uint index)
{
    _selectedType = type;
    _selectedIndex = index;
}
void  ActivityData:: Eventdetails(){
    IMessageManager::getInstance()->postNotification(MES_Eventdetails);
}
//=====================================[NETWORK]===========================================//
void ActivityData::http_request_activityList(int type)
{
    IActivityData* a = M_GET_ACTIVITY;
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    CCLOG(" Activity_status==%s ",a->Activity_status.c_str());
    if (a->Activity_status.empty()) {
        object.AddMember("status","1" , allocator);//1默认是从本期活动开始
    }else{
        object.AddMember("status",a->Activity_status.c_str() , allocator);
    }
    object.AddMember("latitude", p->latitude.c_str(), allocator);
    object.AddMember("longtitude", p->longitude.c_str(), allocator);
    object.AddMember("cityid", type, allocator);

    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, np_getsactivitieslist, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    CCLOG("平安吧   %s 拼乐",buffer->GetString());
  
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("pinle server_ip==%s", GameIni::getInstance()->server_ip.c_str());
    GAMELOG("平安URL = %s  拼乐", newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_activityList, this);
}
string ActivityData::getstr_url(){
    return str2;
}
void ActivityData::http_response_activityList(const string& s)
{
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    GAMELOG("惊喜吧  %s 信息 ",s.c_str());
    if(doc.HasParseError() || !doc.HasMember(np_game))
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    cleanActivityList();
    
    rapidjson::Value &game = doc[np_game];
    CCLOG("-=-=-=   %d",game.Size());
    if(game.IsArray())
    {
        for(uint i = 0; i < game.Size(); i ++)
        {
            rapidjson::Value &d = game[i];
            Activity* J_activity = new Activity();
             //-----------------------活动基本信息---------------
            
            if(d.HasMember("id"))                   J_activity->id = d["id"].GetString();  //1
            if(d.HasMember("title"))                J_activity->title = d["title"].GetString(); //2
            if(d.HasMember("begintime"))            J_activity->begintime = d["begintime"].GetInt64(); //3
            if(d.HasMember("finishtime"))           J_activity->finishtime = d["finishtime"].GetInt64();  //4
            if(d.HasMember("currenttime"))              J_activity->currenttime = d["currenttime"].GetInt64();  //当前时间
            if(d.HasMember("remains"))              J_activity->remains = d["remains"].GetInt();  //5
            if(d.HasMember("ownerurl"))             J_activity->ownerurl = d["ownerurl"].GetString();
            if(d.HasMember("type"))                 J_activity->Activity_type = d["type"].GetInt();
            if(d.HasMember("gsname"))               J_activity->title_name = d["gsname"].GetString();
//            if(d.HasMember("prizewinning"))         J_activity->matchInfo.prizewinning = d["prizewinning"].GetInt();//是否中奖
//             if(d.HasMember("isswardprize"))        J_activity->matchInfo.isswardprize = d["isswardprize"].GetInt();
            ActivityPrize* ap = new ActivityPrize();
            if(d.HasMember("ownerurl"))             ap->url = d["ownerurl"].GetString();
            if(d.HasMember("gsname"))               ap->name = d["gsname"].GetString();     //6
            if(d.HasMember("type"))                 ap->activity_Type = d["type"].GetInt();
            if(d.HasMember("gscount"))              ap->amount = d["gscount"].GetInt();//奖品数量
            if(d.HasMember("id"))                   ap->activityid = d["id"].GetString();  //1
            if(d.HasMember("prizewinning"))         ap->_prizewinning = d["prizewinning"].GetInt();//是否中奖
            if(d.HasMember("isswardprize"))         ap->_isswardprize = d["isswardprize"].GetInt();
            
            J_activity->v_activityPrize.push_back(ap);
            
            rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
            rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
            d.Accept(writer);
            CCLOG("活动奖品信息 %s  %s  拼乐",buffer->GetString() ,ap->url.c_str() );
            
            
            
            
            
//            if(d.HasMember("id"))                   a->id = d["id"].GetString();  //1
//            if(d.HasMember("ownerid"))              a->ownerid = d["ownerid"].GetString();
//            if(d.HasMember("ownername"))            a->ownername = d["ownername"].GetString();
//            if(d.HasMember("title"))                a->title = d["title"].GetString(); //2
//            if(d.HasMember("description"))          a->description = d["description"].GetString();
//            if(d.HasMember("awardsummary"))         a->awardsummary = d["awardsummary"].GetString();
//            if(d.HasMember("requirementsummary"))   a->requirementsummary = d["requirementsummary"].GetString();
//            if(!a->requirementsummary.empty())
//                a->hdGuiZeVec = ITools::Intercept_string_char((char*)a->requirementsummary.c_str(), "*");
//            if(d.HasMember("begintime"))            a->begintime = d["begintime"].GetString(); //3
//            if(d.HasMember("finishtime"))           a->finishtime = d["finishtime"].GetString();  //4
//            if(d.HasMember("ownerurl"))             a->ownerurl = d["ownerurl"].GetString();
//            if(d.HasMember("goodsurl"))             a->goodsurl = d["goodsurl"].GetString();
//            if(d.HasMember("adweburl"))             a->adweburl = d["adweburl"].GetString();
//            if(d.HasMember("status"))               a->status = d["status"].GetInt();
//            if(d.HasMember("remainingtime"))        a->remainingtime = d["remainingtime"].GetInt64();倒计时
//            if(d.HasMember("remains"))              a->remains = d["remains"].GetInt();  //5
//            if(d.HasMember("number"))               a->matchInfo.number = d["number"].GetInt();
//            if(d.HasMember("completecount"))        a->matchInfo.completecount = d["completecount"].GetInt();
//            if(d.HasMember("totalcount"))           a->matchInfo.totalcount = d["totalcount"].GetInt();
//            if(d.HasMember("isplayed"))             a->matchInfo.isplayed = d["isplayed"].GetInt();
//            if(d.HasMember("mypointsdrop"))         a->matchInfo.myintegral = d["mypointsdrop"].GetInt();
//            if(d.HasMember("activitytype"))         a->activitytype = d["activitytype"].GetInt();
//        //平安活动测试
//            
//            rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
//            rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
//            d.Accept(writer);
//            if (!a->adweburl.empty()) {
//                str2=a->adweburl;
//            }
//            CCLOG("广告商网址   %s 拼乐",a->adweburl.c_str());
//            CCLOG("惊喜吧活动基本信息   %s 拼乐",buffer->GetString());
            //-----------------------活动比赛参与信息---------------
//            a->requirementsummary = "——————玩法介绍————－*参与并成功完成平安证劵活动的用*户，100%获得由平安证券提供的*20元话费大礼。*——————兑换流程————－*1、通过完成拼乐的平安证券活动*，进入平安证劵下载页面；*2、必须填写正确的姓名和手机号*，然后点击下载平安证劵 App；*3、根据平安证劵App提示完成注*册等相关信息。*——————使用规则————－*1、本次活动奖品仅限在拼乐 App*中兑换；*2、下载后十天内开户有效；*3、收到平安公司开户成功短信的*三个工作日内给予   充值；*4、同一手机号码只能兑换一次。*——————温馨提示————－*拼乐客服电话：*平安证劵客服电话：*此次活动奖品由平安证券和拼乐公*司双方提供。";
            
            //----擂主前三名信息-----
//            if(d.HasMember("best") && d["best"].IsArray())
//            {
//                rapidjson::Value &best = d["best"];
//                GAMELOG("number8==%d",best.Size());
//                for(int n = 0; n < best.Size(); n ++)
//                {
//                    rapidjson::Value &b = best[n];
//                    ActivityResult r;
//                    if(b.HasMember("bestloginname"))    r.userid = b["bestloginname"].GetString();
//                    if(b.HasMember("bestnickname"))     r.nickname = b["bestnickname"].GetString();
//                    if(b.HasMember("besttime"))         r.result = b["besttime"].GetInt64();
//                    if(b.HasMember("besturl"))          r.adUrl = b["besturl"].GetString();
//                    if(b.HasMember("bestgrade"))        r.rankname = b["bestgrade"].GetString();
//                    if(b.HasMember("bestheadurl"))      r.uHeadIndex = b["bestheadurl"].GetString();
//                    if(b.HasMember("bestpointsdrop"))   r.playerIntegral = b["bestpointsdrop"].GetInt();
//                    a->matchInfo.v_Champion.push_back(r);
//                }
//            }
//            //----我的比赛信息-----
//            if(d.HasMember("mytime") && d["mytime"].IsArray())
//            {
//                rapidjson::Value &arrtime = d["mytime"];
//                for(int n = 0; n < arrtime.Size(); n ++)
//                {
//                    rapidjson::Value &m = arrtime[n];
//                    ActivityResult r;
//                    if(m.HasMember("mytime"))       r.result = m["mytime"].GetInt64();
//                    if(m.HasMember("myurl"))        r.adUrl  = m["myurl"].GetString();
//                    a->matchInfo.v_MyResult.push_back(r);
//                }
//            }
//            if(d.HasMember("totaltime"))            a->matchInfo.totaltime = d["totaltime"].GetInt();
//            if(d.HasMember("curtime"))              a->matchInfo.curtime = d["curtime"].GetInt();
//            if(d.HasMember("ranking"))              a->matchInfo.ranking = d["ranking"].GetInt();
//            if(d.HasMember("needgold"))
//            {
//                string str = d["needgold"].GetString();
//                vector<string> vec = ITools::Intercept_string_char((char*)str.c_str(), ",");
//                for(int vn = 0; vn < vec.size(); vn++)
//                    a->matchInfo.needGold.push_back(atoi(vec[vn].c_str()));
//            }
//            if(d.HasMember("prizewinning"))         a->matchInfo.prizewinning = d["prizewinning"].GetInt();
//            if(d.HasMember("isswardprize"))         a->matchInfo.isswardprize = d["isswardprize"].GetInt();
            //------------------------活动奖品信息----------------------
//            if(d.HasMember("goods") && d["goods"].IsArray())
//            {
//                rapidjson::Value &gs = d["goods"];
//                for(uint n = 0; n < gs.Size(); n ++)
//                {
//                    rapidjson::Value &g = gs[n];
//                    ActivityPrize* ap = new ActivityPrize();
////                    if(g.HasMember("gsname"))   ap->describe = g["gsname"].GetString();
////                    if(g.HasMember("gsurl"))    ap->url = g["gsurl"].GetString();
////                    if(g.HasMember("gscount"))  ap->amount = g["gscount"].GetInt();
//                    if(g.HasMember("gsname"))   ap->name = g["gsname"].GetString();     //6
//                    if(g.HasMember("gsurl"))    ap->url = g["gsurl"].GetString();
//                    if(g.HasMember("gscount"))  ap->amount = g["gscount"].GetInt();
//                    if(g.HasMember("gsmemo"))  ap->describe = g["gsmemo"].GetString();
//                    a->v_activityPrize.push_back(ap);
//                    
//                    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
//                    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
//                    d.Accept(writer);
//                    CCLOG("活动奖品信息 %s  %s  %d 拼乐",ap->describe.c_str(),ap->url.c_str(),ap->amount);
//                }
//            }
            GAMELOG("--------------------iiiii == %d", i);
            IActivityData* b = M_GET_ACTIVITY;
            //addActivity(a->status, a);//放入惊喜吧活动列表  a->Activity_status.c_str()  //atoi
            if (b->Activity_status.empty()) {
                addActivity(atoi("1"), J_activity);
            }else{
                addActivity(atoi(b->Activity_status.c_str()), J_activity);
            }
            
        }
    }
    
    IMessageManager::getInstance()->postNotification(MES_ACTIVITY_GETLISTDONE);
    http_response_downloadimage();
}

int  ActivityData::getActivitCount()
{
    int sum = 0;
    for(int i = 0 ; i < E_ActivityType::end; i ++)
    {
        for(int j = 0; j < v_activityList[i].size(); j ++)
            sum ++;
    }
    return sum;
}


void ActivityData::http_response_downloadimage()
{
    vector<__Array*> imageInfo;
    for(int i = 0 ; i < E_ActivityType::end; i ++)
    {
        for(int j = 0; j < v_activityList[i].size(); j ++)
        {
            Activity *a = v_activityList[i][j];
            string filename = ITools::Intercept_string(a->ownerurl.c_str(),"/");//截取图片名
            __String* path = __String::create(StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER,
                                                                  IMAGE_ADLIST_FINDER, filename.c_str()));
            __String* url  = __String::create(a->ownerurl);
            CCLOG("拼乐 %s  活动图片",a->ownerurl.c_str());
            __Array* arr = __Array::create(url, path, NULL);
            imageInfo.push_back(arr);
        }
    }
    myHttpWork* http = myHttpWork::createHttp();
    http->http_request_downLoadImage(imageInfo, MES_ACTIVITY_GETIMAGEDONE);
    imageInfo.clear();
}


void ActivityData::http_request_activityInfo(const string& id)
{
    
        IActivityData* a = M_GET_ACTIVITY;
        IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("activityid", id.c_str(), allocator);
        document.AddMember(np_methodtype, "json", allocator);
        document.AddMember(np_createtime, "1413800515", allocator);
        document.AddMember(np_functionname, np_getactivitybyid, allocator);
        document.AddMember(np_functionparams, object, allocator);
        
        rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
        rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
        document.Accept(writer);
        CCLOG("活动详情   %s 拼乐",buffer->GetString());
        
        string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                            p->LandKey.c_str(), p->MiYao_key.c_str());
        
        GAMELOG("pinle server_ip==%s", GameIni::getInstance()->server_ip.c_str());
        GAMELOG("活动详情URL = %s  拼乐", newurl.c_str());
        myHttpWork* http = myHttpWork::createHttp();
        http->sendRequest(newurl, "0", buffer->GetString(), 30);
        http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_activityInfo, this);

}

void ActivityData::http_response_activityInfo(const string &s)
{
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
     CCLOG("活动详情－－－  %s 信息 ",s.c_str());
    if(doc.HasParseError() || !doc.HasMember("activity"))
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    //cleanActivityList();
    
    rapidjson::Value &game = doc["activity"];
    
            rapidjson::Value &d = game;
            Activity* J_activity = new Activity();
            if(d.HasMember("id"))                   J_activity->id = d["id"].GetString();
            if(d.HasMember("title"))                J_activity->title = d["title"].GetString();
            if(d.HasMember("begintime"))            J_activity->begintime = d["begintime"].GetInt64();
            if(d.HasMember("finishtime"))           J_activity->finishtime = d["finishtime"].GetInt64();
            if(d.HasMember("requirementsummary"))   J_activity->requirementsummary = d["requirementsummary"].GetString();
            if(d.HasMember("Remains"))              J_activity->remains = d["Remains"].GetInt();
            if(d.HasMember("mypoints"))             J_activity->matchInfo.myintegral = d["mypoints"].GetInt();
            if(d.HasMember("myrank"))               J_activity->matchInfo.Myrank = d["myrank"].GetInt();
            ActivityPrize* ap = new ActivityPrize();
            if(d.HasMember("Gsname"))               ap->name = d["Gsname"].GetString();     //6
            if(d.HasMember("gsamount"))             ap->amount = d["gsamount"].GetInt();//奖品数量
            if(d.HasMember("type"))                 ap->activity_Type = d["type"].GetInt();
    J_activity->hdGuiZeVec = ITools::Intercept_string_char((char*)J_activity->requirementsummary.c_str(), "*");
    guize=J_activity->hdGuiZeVec;
    
            jx_num=ap->amount;
            w_jifen=J_activity->matchInfo.myintegral;
            w_paiming=J_activity->matchInfo.Myrank;
            J_activity->v_activityPrize.push_back(ap);
            rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
            rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
            d.Accept(writer);
            CCLOG("活动详情信息 %s  拼乐",buffer->GetString());
    IMessageManager::getInstance()->postNotification("liaojian_xq");
}
//排行榜
void ActivityData::http_request_activityRankingList(const string& id, uint pageIndex)
{
    if(id.empty())
        return;
    
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("activityid", id.c_str(), allocator);
    string page = StringUtils::format("%d", pageIndex);
    object.AddMember("pageindex", page.c_str(), allocator);
    
    object.AddMember("latitude", p->latitude.c_str(), allocator);
    object.AddMember("longtitude", p->longitude.c_str(), allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, np_getrankinglistofactivies, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("newurl = %s", newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_activityRankingList, this);
}

void ActivityData::http_response_activityRankingList(const string& s)
{
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    if(doc.HasMember("best") && doc["best"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_Champion.clear();
        rapidjson::Value &best = doc["best"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("loginname"))        r.userid = b["loginname"].GetString();
            if(b.HasMember("nickname"))         r.nickname = b["nickname"].GetString();
            if(b.HasMember("time"))             r.result = b["time"].GetInt64();
            if(b.HasMember("imgurl"))           r.adUrl = b["imgurl"].GetString();
            if(b.HasMember("rankname"))         r.rankname = b["rankname"].GetString();
            if(b.HasMember("imgurl"))           r.uHeadIndex = b["imgurl"].GetString();
            if(b.HasMember("bestpointsdrop"))   r.playerIntegral = b["bestpointsdrop"].GetInt();
            
            a->matchInfo.v_Champion.push_back(r);
        }
    }
 
//    ActivityRanking* ar = ActivityRanking::getInstance();
//    ar->getRankingList().clear();
//    
//    if(doc.HasMember("pageindex"))          ar->pageIndex = doc["pageindex"].GetInt();
//    if(doc.HasMember("pagesize"))           ar->pageSize = doc["pagesize"].GetInt();
//    if(doc.HasMember("totalsize"))
//    {
//        ar->pageMax = doc["totalsize"].GetInt() / ar->pageSize;
//        if(ar->pageMax <= 0)
//            ar->pageMax = 1;
//    }
//    if(doc.HasMember("playerlist"))
//    {
//        rapidjson::Value &list = doc["playerlist"];
//        if(list.IsArray())
//        {
//            for(int i = 0; i < list.Size(); i ++)
//            {
//                rapidjson::Value &player = list[i];
//                ActivityResult p;
//                if(player.HasMember("loginname"))       p.userid = player["loginname"].GetString();
//                if(player.HasMember("nickname"))        p.nickname = player["nickname"].GetString();
//                if(player.HasMember("rankname"))        p.rankname = player["rankname"].GetString();
//                if(player.HasMember("imgurl"))          p.uHeadIndex = player["imgurl"].GetString();
//                if(player.HasMember("time"))            p.result = player["time"].GetInt();
//                ar->getRankingList().push_back(p);
//            }
//        }
//    }
    IMessageManager::getInstance()->postNotification(MES_ACTIVITY_PHB_INFOR);
}
//奖品抢先看
void  ActivityData::   http_request_FirstlookList(const string& id, const string& name){
    if(id.empty())
        return;
    
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("activityid", id.c_str(), allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getactivityawards", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("newurl = %s  buffer=%s  ", newurl.c_str(),buffer->GetString());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_GetFirstlookList, this);

    
    
}
void ActivityData::   http_response_GetFirstlookList(const string& s){
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("奖品抢先看名单   %s",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    if(doc.HasMember("awardlist") && doc["awardlist"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_LookWinning.clear();
        rapidjson::Value &best = doc["awardlist"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("awardorder"))        r.lookawardorder = b["awardorder"].GetInt();
            if(b.HasMember("awardcount"))       r.lookawardcount = b["awardcount"].GetInt();
            if(b.HasMember("goodsname"))        r.lookgoodsname = b["goodsname"].GetString();
            if(b.HasMember("goodsimageurl"))         r.lookgoodsimageurl = b["goodsimageurl"].GetString();
            a->matchInfo.v_LookWinning.push_back(r);
        }
    }
    
    IMessageManager::getInstance()->postNotification(MES_ACTIVITY_AwardPreview);

}
//获奖名单
void ActivityData::   http_request_WinnersList(const string& id, const string& name){
    if(id.empty())
        return;
    
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("activityid", id.c_str(), allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getactivitywinners", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("newurl = %s  buffer=%s  ", newurl.c_str(),buffer->GetString());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_GetWinnersList, this);

}
void  ActivityData::  http_response_GetWinnersList(const string& s){
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("获奖名单   %s",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    if(doc.HasMember("winnerlist") && doc["winnerlist"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_Winning.clear();
        rapidjson::Value &best = doc["winnerlist"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("rank"))        r.Winningrank = b["rank"].GetInt();
            if(b.HasMember("points"))       r.Winningpoints = b["points"].GetInt();
            if(b.HasMember("nickname"))        r.Winningnickname = b["nickname"].GetString();
            if(b.HasMember("headimageurl"))         r.Winningheadimageurl = b["headimageurl"].GetString();
            if(b.HasMember("goodsname"))        r.Winninggoodsname = b["goodsname"].GetString();
            if(b.HasMember("goodsimageurl"))         r.Winninggoodsimageurl = b["goodsimageurl"].GetString();
            a->matchInfo.v_Winning.push_back(r);
        }
    }
    
     IMessageManager::getInstance()->postNotification(MES_ACTIVITY_Winning);

}
//对比排行榜
void  ActivityData::  http_request_RankingRecordList(const string& id, const string& name){
    if(id.empty())
        return;
    
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("activityid", id.c_str(), allocator);
    object.AddMember("loginname",name.c_str(), allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getactivitypointsdetail", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("newurl = %s", newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::get_response_RankingRecordList, this);
}
void  ActivityData::  get_response_RankingRecordList(const string& s){
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("积分排行榜  %s",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    if(doc.HasMember("mypointslist") && doc["mypointslist"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_Ranking.clear();
        rapidjson::Value &best = doc["mypointslist"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("cycle"))        r.cycle = b["cycle"].GetInt();
            if(b.HasMember("points"))         r._points = b["points"].GetInt();
            CCLOG("77777777  %d %d", r.cycle,r._points);
            a->matchInfo.v_Ranking.push_back(r);
        }
    }
    if(doc.HasMember("playerpointslist") && doc["playerpointslist"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_RankingRecord.clear();
        rapidjson::Value &best = doc["playerpointslist"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("cycle"))        r.cycle = b["cycle"].GetInt();
            if(b.HasMember("points"))         r._points = b["points"].GetInt();
            CCLOG("77777777  %d %d", r.cycle,r._points);
            a->matchInfo.v_RankingRecord.push_back(r);
        }
    }
    
    
    IMessageManager::getInstance()->postNotification(MES_ACTIVITY_RankingRecord);
}
//积分排行榜
void  ActivityData::http_request_integralRankingList(const string& id, const string& name){
    if(id.empty())
        return;
    
    IPlayerData* p = (IPlayerData*)IDataManager::getInstance()->getData(M_PLAYER);
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("activityid", id.c_str(), allocator);
    object.AddMember("loginname", " ", allocator);
//    if (name.empty()) {
//        object.AddMember("loginname", " ", allocator);
//    }
//    else{
//    object.AddMember("loginname", name.c_str(), allocator);
//    }
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getactivitypointsdetail", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    
    GAMELOG("newurl = %s", newurl.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB2 = CC_CALLBACK_1(ActivityData::http_response_integralRankingList, this);
}
//获取积分排行榜
void  ActivityData::http_response_integralRankingList(const string& s){
    if(s.empty())
        return;
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    CCLOG("积分排行榜  %s",s.c_str());
    if(doc.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    if(doc.HasMember("mypointslist") && doc["mypointslist"].IsArray())
    {
        Activity* a = getSelectedActivity();
        a->matchInfo.v_Integral.clear();
        rapidjson::Value &best = doc["mypointslist"];
        for(int n = 0; n < best.Size(); n ++)
        {
            rapidjson::Value &b = best[n];
            ActivityResult r;
            if(b.HasMember("cycle"))        r.cycle = b["cycle"].GetInt();
            if(b.HasMember("points"))         r._points = b["points"].GetInt();
            CCLOG("77777777  %d %d", r.cycle,r._points);
            a->matchInfo.v_Integral.push_back(r);
        }
    }

    
    IMessageManager::getInstance()->postNotification(MES_ACTIVITY_JF_INFOR);

}
int ActivityData::getjx_nun(){
    return jx_num;
}
int ActivityData::getw_jifen(){
    return w_jifen;
}
int ActivityData::getw_paiming(){
    return w_paiming;
}
vector<string> ActivityData:: get_guize(){
    return guize;
}







