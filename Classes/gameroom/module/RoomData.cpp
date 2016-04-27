//
//  RoomData.cpp
//  Pinle
//
//  Created by 李辛 on 15/6/1.
//
//

#include "RoomData.h"
#include "../../framework/system/file/IFile.h"
#include "../../gameini/GameIni.h"
#include "../../player/module/PlayerData.h"



RoomData::RoomData()
{
    _success = false;
    _uGameLevelSprite = new GameLevelSprite();
    _uGameLevelSprite->imgurl = "http://test.pinlegame.com/GameImage/2fdc30ea-7a20-43ee-ba33-4ec26e102169.jpg";
    _activityRewardInfo = new ActivityRewardInfo();
    _pvpSuccess = false;
    _roomId = "";
    _isPvp = false;
    
}


RoomData::~RoomData()
{
    
}


void RoomData::init()
{
    if(!IDataManager::getInstance()->isExist(M_ROOM))
    {
        RoomData *roomdata = new RoomData();
        IDataManager::getInstance()->attach(roomdata, M_ROOM);
    }
}

void RoomData::destroy()
{
    IDataManager::getInstance()->detach(M_ROOM);
}


void RoomData::commomGameStart()            //多人赛轮训发送消息
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    IPlayerData * iplayerData = M_GET_PLAYER;
    document.AddMember("loginname",iplayerData->userid.c_str(), allocator);
    document.AddMember("loginkey", iplayerData->LandKey.c_str(), allocator);
    document.AddMember("roomid", _roomId.c_str(), allocator);
    document.AddMember("adid",_uGameLevelSprite->adid.c_str(),allocator);
    if(_gameType == GameMode::pvp_2)
    {
        document.AddMember("roomtype", 2, allocator);
    }
    else if(_gameType == GameMode::pvp_4)
    {
        document.AddMember("roomtype", 4, allocator);
    }
    
    if(_success)
    {
        document.AddMember("iswinner", "true", allocator);
    }
    else
    {
        document.AddMember("iswinner", "false", allocator);
    }
    
    int sn[100];
    long timeNum[100];
    string str[100];
    int i = 0;
    rapidjson::Value stepList(rapidjson::kArrayType);
    map<int,long>::iterator tempMap;
    for(tempMap = _map.begin();tempMap != _map.end();tempMap++)
    {
        rapidjson::Value temp(rapidjson::kObjectType);
        sn[i] = tempMap->first;
        timeNum[i] = tempMap->second;
        temp.AddMember("step", sn[i], allocator);
        temp.AddMember("time", (int)timeNum[i], allocator);
        str[i] = StringUtils::format("%d-%d",sn[i],(sn[i]+1));
        temp.AddMember("fromto", str[i].c_str(), allocator);
        stepList.PushBack(temp, allocator);
        i++;
    }
    document.AddMember("userdata", stepList, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    
    //网络请求ip 按照协议加密
//    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", ROOT_IP,
//                                        iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
    string newurl = GameIni::getInstance()->game_ip;
//    newurl = "http://servernew.pinlegame.com/Waring.aspx?";
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30,false);
    http->_httpCB = CC_CALLBACK_2(RoomData::gameOver, this);
}

void RoomData::callBack(const string& str, int code)
{
    if(str.empty())
    {
        return;
    }
    GAMELOG("RoomData::callBack str == %s    code == %d",str.c_str(),code);
    IPlayerData *iplayer = M_GET_PLAYER;
    
    updateGameInfo(str);
    if(!_pvpSuccess && (_gameType == GameMode::pvp_2 || _gameType == GameMode::pvp_4))
    {
        return;
    }
    _pvpSuccess = false;
    _isPvp = false;
    iplayer->updateInfo(str);

    string path = "";
    path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(getGameInfo()->imgurl, "/");
    GAMELOG("imagePath == %s",path.c_str());
    if(IFile::isFileExist_Writable(path))
    {
        IMessageManager::getInstance()->postNotification(MES_ROOM_GAMESTART);
        GAMELOG("***picture is IN FILE");
    }
    else
    {
        GAMELOG("***picture is NO FILE");
        downLoadImage(getGameInfo()->imgurl);
    }
    
}

void RoomData::enterGamePlay(int type,const void* vv)     //请求进入游戏
{
    _gameType = type;
    
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);

    IPlayerData * iplayerData = M_GET_PLAYER;
    
    switch(type)
    {
        case GameMode::daily_simple:
        {
            object.AddMember("gamesettingid", _gameType, allocator);
            object.AddMember("latitude", iplayerData->latitude.c_str(), allocator);
            object.AddMember("longtitude", iplayerData->longitude.c_str(), allocator);
            
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        case GameMode::daily_hard:
        {
            object.AddMember("gamesettingid", _gameType, allocator);
            object.AddMember("latitude", iplayerData->latitude.c_str(), allocator);
            object.AddMember("longtitude", iplayerData->longitude.c_str(), allocator);
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        case GameMode::challenge_hard:
        {
            object.AddMember("gamesettingid", _gameType, allocator);
            object.AddMember("latitude", iplayerData->latitude.c_str(), allocator);
            object.AddMember("longtitude", iplayerData->longitude.c_str(), allocator);
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        case GameMode::challenge_simple:
        {
            object.AddMember("gamesettingid", _gameType, allocator);
            object.AddMember("latitude", iplayerData->latitude.c_str(), allocator);
            object.AddMember("longtitude", iplayerData->longitude.c_str(), allocator);
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        case GameMode::pvp_2:
        {
//            object.AddMember("gamesettingid", _gameType, allocator);
//            rapidjson::Value player1(rapidjson::kObjectType);
//            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
//            array.PushBack(player1, allocator);
//            object.AddMember("playerlist", array, allocator);
            
            
//            document.AddMember(np_methodtype, "json", allocator);
//            document.AddMember(np_createtime, "1413800515", allocator);
//            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
//            document.AddMember(np_functionparams, object, allocator);
            
            GAMELOG("***game PvP2");
            document.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            document.AddMember("loginkey", iplayerData->LandKey.c_str(), allocator);
            if (_roomId.empty()) {
                
            }
            else{
            document.AddMember("roomid", _roomId.c_str(), allocator);
            }
            GAMELOG("###roomid =%s", _roomId.c_str());
            document.AddMember("roomtype", 2, allocator);
            _isPvp = true;
        }break;
        case GameMode::pvp_4:
        {
//            object.AddMember("gamesettingid", _gameType, allocator);
//            rapidjson::Value player1(rapidjson::kObjectType);
//            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
//            array.PushBack(player1, allocator);
//            object.AddMember("playerlist", array, allocator);
//            
//            
//            document.AddMember(np_methodtype, "json", allocator);
//            document.AddMember(np_createtime, "1413800515", allocator);
//            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
//            document.AddMember(np_functionparams, object, allocator);
            
            document.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            document.AddMember("loginkey", iplayerData->LandKey.c_str(), allocator);
            document.AddMember("roomid", _roomId.c_str(), allocator);
            document.AddMember("roomtype", 4, allocator);
            _isPvp = true;
        }break;
        case GameMode::pvp_friends:
        {
            object.AddMember("gamesettingid", _gameType, allocator);
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        case GameMode::activity:
        {
            if(vv != nullptr)
            {
                Activity* activity = (Activity*)vv;
                _activityId = activity->id;
                _activityRewardInfo->adweburl = activity->adweburl;
            }            
            
            object.AddMember("gamesettingid", _gameType, allocator);
            object.AddMember("activitiesid", _activityId.c_str(), allocator);
            object.AddMember("latitude", iplayerData->latitude.c_str(), allocator);
            object.AddMember("longtitude", iplayerData->longitude.c_str(), allocator);
            rapidjson::Value player1(rapidjson::kObjectType);
            player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
            array.PushBack(player1, allocator);
            object.AddMember("playerlist", array, allocator);
            
            
            document.AddMember(np_methodtype, "json", allocator);
            document.AddMember(np_createtime, "1413800515", allocator);
            document.AddMember(np_functionname, "setgamebeginrecord", allocator);
            document.AddMember(np_functionparams, object, allocator);
        }break;
        default:
            break;
            
    }
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    if(type == GameMode::pvp_4 || type == GameMode::pvp_2)
    {
        string newurl = GameIni::getInstance()->match_ip;
//        newurl = "http://servernew.pinlegame.com/Waiting.aspx?";
        myHttpWork* http = myHttpWork::createHttp();
        http->sendRequest(newurl, "0", buffer->GetString(), 30, false);
        http->_httpCB = CC_CALLBACK_2(RoomData::callBack, this);
        
//        GAMELOG("****_pvpSuccess == %d",_pvpSuccess);
    }
    else
    {
        string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                            iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
        myHttpWork* http = myHttpWork::createHttp();
        http->sendRequest(newurl, "0", buffer->GetString(), 30);
        http->_httpCB = CC_CALLBACK_2(RoomData::callBack, this);
    }
    
   
    
   
}

void RoomData::enterChoiceType()
{
    
}

//void RoomData::setGameInfo(GameLevelSprite *gameLevelSprite)
//{
//    _uGameLevelSprite = gameLevelSprite;
//}


void RoomData::setGameInfo(GameLevelSprite *gameLevelSprite)
{
    _uGameLevelSprite->TypeIndex = gameLevelSprite->TypeIndex;
    _uGameLevelSprite->CheckpointType = gameLevelSprite->CheckpointType;
    _uGameLevelSprite->ownerid = gameLevelSprite->ownerid;
    _uGameLevelSprite->adid = gameLevelSprite->adid;
    _uGameLevelSprite->logourl = gameLevelSprite->logourl;
    _uGameLevelSprite->imgurl = gameLevelSprite->imgurl;
    _uGameLevelSprite->rewardtitle = gameLevelSprite->rewardtitle;
    _uGameLevelSprite->rewardurl = gameLevelSprite->rewardurl;
    _uGameLevelSprite->sbireorder = gameLevelSprite->sbireorder;
    for(int i = 0; i < 12; i ++)
    {
        _uGameLevelSprite->spireorder[i] = gameLevelSprite->spireorder[i];
    }
    _uGameLevelSprite->addGoldNum = gameLevelSprite->addGoldNum;
}



GameLevelSprite *RoomData::getGameInfo()
{
    return _uGameLevelSprite;
}

void RoomData::setGameSuccess(bool b)
{
    _gamesuccess = b;
    _success = b;
    return;
}
bool RoomData::isSuccess()
{
    return _gamesuccess;
}

void RoomData::setTimeCount(int time)
{
    _utimeCount = time;
}
int RoomData::getTimeCount()
{
    return _utimeCount;
}

void RoomData::setRunCount(int _run)
{
    _uRun = _run;
}
int RoomData::getRunCount()
{
    return _uRun;
}


void RoomData::setRewardInfo(ActivityRewardInfo *activityRewardInfo)
{
    _activityRewardInfo = activityRewardInfo;
}
ActivityRewardInfo *RoomData::getRewardInfo()
{
    return _activityRewardInfo;
}

void RoomData::setGameType(unsigned int _type)
{
    _gameType = _type;
}
int RoomData::getGameType()
{
    return _gameType;
}

void RoomData::downLoadImage(string &url, string pathStr) //下载图片
{
    GAMELOG("***downLoadImage==%s",url.c_str());
    myHttpWork *httpWork = myHttpWork::createHttp();
    httpWork->sendRequest((char*)url.c_str(), 30);
    string path = "";
    if(pathStr.compare(IMAGE_ADREWARD_FINDER) == 0)
    {
        path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_ADREWARD_FINDER + "/" + ITools::Intercept_string(url, "/");
        httpWork->_args.code = ROOM_IMAGEDOWNLOAD_BACK;
        GAMELOG("***pathStr == %s",path.c_str());
    }
    else
    {
        path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(url, "/");
    }
    
    httpWork->_args.content = path;
    httpWork->_httpCB3 = CC_CALLBACK_2(RoomData::downLoadImageBack, this);
}
void RoomData::downLoadImageBack(const string &str , Args _args)
{
    GAMELOG("***imageContent == %s",str.c_str());
    IFile::writeFile(_args.content, str);
    if(_args.code == ROOM_IMAGEDOWNLOAD_BACK)
    {
        IMessageManager::getInstance()->postNotification(MES_ROOM_IMAGEDOWNLOAD);
    }
    else
    {
        IMessageManager::getInstance()->postNotification(MES_ROOM_GAMESTART);
    }
    
}


void RoomData::updateGameInfo(const string &str)  //更新游戏请求数据
{
    if(str.empty())
    {
        return;
    }
    
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());
    CCLOG("拼吧返回数据 %s",str.c_str());

//刑展-----------
    if(doc.HasMember("roomid"))
    {
        string roomid = doc["roomid"].GetString();
        if(!roomid.empty())
        {
            _roomId = doc["roomid"].GetString();
        }
    }
    
    if(doc.HasMember("suc"))
    {
        if(!doc["suc"].GetInt())
        {
//            return;
        }
        else
        {
            
            if(doc.HasMember("order"))
                _uGameLevelSprite->sbireorder = doc["order"].GetString();
            if(doc.HasMember("adid"))
                _uGameLevelSprite->adid = doc["adid"].GetString();
            if(doc.HasMember("imageurl"))
                _uGameLevelSprite->imgurl = doc["imageurl"].GetString();
            if(doc.HasMember("logo"))
                _uGameLevelSprite->logourl = doc["logo"].GetString();
            if(doc.HasMember("realaward"))
                _uGameLevelSprite->rewardtitle = doc["realaward"].GetString();
            if(doc.HasMember("realawardurl"))
                _uGameLevelSprite->rewardurl = doc["realawardurl"].GetString();
            
            string sd = _uGameLevelSprite->sbireorder;
            istringstream iss(sd);
            string part;
            int sbirnum = 0;
            getline(iss,part,',');
            _uGameLevelSprite->spireorder[sbirnum] = atoi(part.c_str());
            while(getline(iss,part,','))
            {
                sbirnum++;
                _uGameLevelSprite->spireorder[sbirnum] = atoi(part.c_str());
            }
            
            if(sd.empty())
            {
                _uGameLevelSprite->spireorder[0] = 11;
                _uGameLevelSprite->spireorder[1] = 6;
                _uGameLevelSprite->spireorder[2] = 10;
                _uGameLevelSprite->spireorder[3] = 1;
                _uGameLevelSprite->spireorder[4] = 8;
                _uGameLevelSprite->spireorder[5] = 3;
                _uGameLevelSprite->spireorder[6] = 12;
                _uGameLevelSprite->spireorder[7] = 2;
                _uGameLevelSprite->spireorder[8] = 4;
                _uGameLevelSprite->spireorder[9] = 9;
                _uGameLevelSprite->spireorder[10] = 5;
                _uGameLevelSprite->spireorder[11] = 7;
                
            }
            
//            if(doc.HasMember("players"))
//            {
//                rapidjson::Value &playerList = doc["players"];
//                if(playerList.IsArray())
//                {
//                    _enemyNum = 0;
//                    
//                    for(int i=0;i<playerList.Size() && i<4 ;i++)
//                    {
//                        _enemyNum++;
//                        rapidjson::Value &value = playerList[i];
//                        if(value.HasMember("loginname"))
//                            _enemyData[i].u_pvp_Id = value["loginname"].GetString();
//                        if(value.HasMember("nickname"))
//                            _enemyData[i].u_pvp_Name = value["nickname"].GetString();
//                        if(value.HasMember("golds"))
//                            _enemyData[i].u_pvp_Gold = value["golds"].GetInt();
//                        if(value.HasMember("score"))
//                            _enemyData[i].u_pvp_Score = value["score"].GetInt();
//                        if(value.HasMember("headurl"))
//                            _enemyData[i].u_pvp_headUrl = value["headurl"].GetString();
//                        
//                        IPlayerData *iplayerData = M_GET_PLAYER;
//                        if(iplayerData->userid.compare(value["loginname"].GetString()) == 0)
//                        {
//                            log("***比赛前金币：%d",value["golds"].GetInt());
//                            if(value.HasMember("golds"))
//                                iplayerData->golds = value["golds"].GetInt();
//                            if(value.HasMember("golds"))
//                                iplayerData->points = value["score"].GetInt();
//                        }
//                    }
//                }
//            }
        }
    }
    
    //游戏结束
    
    if(doc.HasMember("players"))
    {
        rapidjson::Value &playerList = doc["players"];
        if(playerList.IsArray())
        {
            _success = true;
            _enemyNum = 0;
            
            int insertNum = 1;
            int n = 0;
            for(int i=0;i<playerList.Size() && i<4 ;i++)
            {
                rapidjson::Value &value = playerList[i];
                if(value.HasMember("iswinner"))
                {
                    
                    if(value["iswinner"].GetInt() != 0)
                    {
                        n = 0;
                    }
                    else
                    {
                        n = insertNum;
                        insertNum++;
                    }
                }
                else
                {
                    n = i;
//                    if(n >= 4)
//                    {
//                        n = 0;
//                    }
                }
                
                if(value.HasMember("loginname"))
                    _enemyData[n].u_pvp_Id = value["loginname"].GetString();
                if(value.HasMember("steps"))
                    _enemyData[n].u_pvp_Run = value["steps"].GetInt();
                if(value.HasMember("times"))
                    _enemyData[n].u_pvp_TimeCount = value["times"].GetInt();
                if(value.HasMember("iswinner"))
                    _enemyData[n].ChengGong = value["iswinner"].GetInt();
                if(value.HasMember("golds"))
                    _enemyData[n].u_pvp_Gold = value["golds"].GetInt();
                if(value.HasMember("score"))
                    _enemyData[n].u_pvp_Score = value["score"].GetInt();
                if(value.HasMember("headurl"))
                    _enemyData[n].u_pvp_headUrl = value["headurl"].GetString();
                if(value.HasMember("addgolds"))
                    _enemyData[n].u_pvp_addGold = value["addgolds"].GetInt();
                if(value.HasMember("nickname"))
                    _enemyData[n].u_pvp_Name = value["nickname"].GetString();
                if(value.HasMember("rankname"))
                    _enemyData[n].u_pvp_randName = value["rankname"].GetString();
                
                
                if(i == 3)
                {
                    for(int j = 0; j < 4; j++)
                    {
                        GAMELOG("***name == %s",_enemyData[j].u_pvp_Id.c_str());
                        GAMELOG("***name == %s",_enemyData[j].u_pvp_Name.c_str());
                    }
                }
                
                IPlayerData *iplayerData = M_GET_PLAYER;
                if(iplayerData->userid.compare(value["loginname"].GetString()) == 0)
                {
                    if(value.HasMember("golds"))
                        iplayerData->golds = value["golds"].GetInt();
                    if(value.HasMember("score"))
                        iplayerData->points = value["score"].GetInt();
                    if(value.HasMember("iswinner"))
                    {
                        if(value["iswinner"].GetInt() == 1)
                        {
                            _gamesuccess = true;
                        }
                        else
                        {
                            _gamesuccess = false;
                        }
                    }
                    
                }
                else if(_isPvp)
                {
                    _enemyNum++;
                    if(!enemyInVer(_enemyData[n].u_pvp_Id))
                    {
                        enemyVer.push_back(_enemyData[n]);
                    }
                }
                
                
            }
            
            
            
            if(getGameType() == GameMode::pvp_2 && _enemyNum == 1 && _isPvp)
            {
                _pvpSuccess = true;
            }
            else if( getGameType() == GameMode::pvp_4 && _enemyNum == 3 && _isPvp)
            {
                _pvpSuccess = true;
            }
            else
            {
                _pvpSuccess = false;
            }
            if(!_pvpSuccess && _isPvp)
            {
                IMessageManager::getInstance()->postNotification(MES_ROOM_PVPADDENEMY);
            }
        }
        
    }

//---------------//---------------//---------------//---------------//---------------//---------------//---------------
    if(doc.HasMember(np_game_ad))
    {
      
        rapidjson::Value &d = doc[np_game_ad];
        if(d.HasMember("categoryid"))
            _uGameLevelSprite->CheckpointType = atoi(d["categoryid"].GetString());
        if(d.HasMember("adid"))
            _uGameLevelSprite->adid = d["adid"].GetString();
        if(d.HasMember("ownerid"))
            _uGameLevelSprite->ownerid = d["ownerid"].GetString();
        if(d.HasMember("imgurl"))
            _uGameLevelSprite->imgurl = d["imgurl"].GetString();
        if(d.HasMember("logourl"))
            _uGameLevelSprite->logourl = d["logourl"].GetString();
        if(d.HasMember("rewardtitle"))
            _uGameLevelSprite->rewardtitle = d["rewardtitle"].GetString();
        if(d.HasMember("rewardurl"))
            _uGameLevelSprite->rewardurl = d["rewardurl"].GetString();
        _activityRewardInfo->over = false;
        
//        IPokedexData* pokedex = M_GET_POKEDEX;
//        pokedex->insert(d["adid"].GetString(), d["ownerid"].GetString(), d["imgurl"].GetString(),
//                        d["logourl"].GetString(), d["rewardtitle"].GetString(), d["rewardurl"].GetString());
        
    }
    
    if(doc.HasMember(np_game_goods))
    {
        rapidjson::Value &d = doc[np_game_goods];
        if(d.HasMember("goodurl"))
            _activityRewardInfo->goodurl = d["goodurl"].GetString();
        if(d.HasMember("goodname"))
            _activityRewardInfo->goodname = d["goodname"].GetString();
        if(d.HasMember("loginname"))
            _activityRewardInfo->loginname = d["loginname"].GetString();
        if(d.HasMember("phone"))
            _activityRewardInfo->phone = d["phone"].GetString();
        if(d.HasMember("playername"))
            _activityRewardInfo->playername = d["playername"].GetString();
        if(d.HasMember("address"))
        {
            _activityRewardInfo->over = true;
            _activityRewardInfo->address = d["address"].GetString();
        }
        else
        {
            _activityRewardInfo->over = false;
        }
    }
    if(doc.HasMember("gamesettinglist"))
    {
        rapidjson::Value &d = doc["gamesettinglist"];
        if(d.IsArray())
        {
            for(int i = 0; i < d.Size(); i++)
            {
                rapidjson::Value &d1 = d[i];
                _levelsType[i + 1].itypeIndex = d1["gsid"].GetInt();
                _levelsType[i + 1].iscore = d1["rewardpoints"].GetInt();
                _levelsType[i + 1].igold = d1["rewardgolds"].GetInt();
                _levelsType[i + 1].ilimitTime = d1["timelimits"].GetInt();
                _levelsType[i + 1].iyajin = d1["betgolds"].GetInt();
            }
        }
    }
    
    if(doc.HasMember("goldslimit"))
    {
        goldlimit = doc["goldslimit"].GetInt();
    }
    else
    {
        goldlimit = 1;
    }
    
    if(doc.HasMember("goldscurrent"))
    {
        goldscurrent = doc["goldscurrent"].GetInt();
    }
    
//    if(doc.HasMember(np_game))
//    {
//        rapidjson::Value &d = doc[np_game];
//        if(d.HasMember("gsid"))
//            getGameInfo()->TypeIndex = d["gsid"].GetInt();
//    }
    
    if(doc.HasMember("activityinfo") && (getGameType() == GameMode::activity))
    {
        IActivityData* activity = M_GET_ACTIVITY;
        
        rapidjson::Value &d = doc["activityinfo"];
        //rapidjson::Document d;
        
        if(d.HasMember("bestpoints"))
            activity->getSelectedActivity()->matchInfo.bestpoints = d["bestpoints"].GetInt();
        if(d.HasMember("totalPoints"))
            activity->getSelectedActivity()->matchInfo.myintegral = d["totalPoints"].GetInt();
        if(d.HasMember("points"))
            activity->getSelectedActivity()->matchInfo.myintegralOnc = d["points"].GetInt();
        if(d.HasMember("rank"))
            activity->getSelectedActivity()->matchInfo.Myrank = d["rank"].GetInt();
        if(d.HasMember("currentrank"))
            activity->getSelectedActivity()->matchInfo.currentrank = d["currentrank"].GetInt();
        
        
        CCLOG("%d %d %d %d %d",activity->getSelectedActivity()->matchInfo.bestpoints, activity->getSelectedActivity()->matchInfo.myintegral,activity->getSelectedActivity()->matchInfo.myintegralOnc);

    }
    return;
}

vector<EnemyData>* RoomData::getEnemyVer()
{
    return &enemyVer;
}


bool RoomData::enemyInVer(string str)
{
    for(int i = 0; i < enemyVer.size(); i++)
    {
        EnemyData e = enemyVer.at(i);
        if(e.u_pvp_Id.compare(str) == 0)
        {
            return true;
        }
    }
    return false;
}


int RoomData::getEnemyNum()        //获取对手人数
{
    return _enemyNum;
}


void RoomData::setStartTime(string &str)        //设置游戏开始时间
{
    _startTime = str;
}
string RoomData::getStartTime()  //获取游戏开始时间
{
    return _startTime;
}
void RoomData::setEndTime(string &str)           //设置游戏结束时间
{
    _endTime = str;
}
string RoomData::getEndTime()      //获取游戏结束时间
{
    return _endTime;
}



string RoomData::geturl()
{
    return _uGameLevelSprite->imgurl;
}

LevelType *RoomData::getLevelType(int tag)    //获取游戏关卡类型
{
    
    return &(_levelsType[tag]);
}
//老虎机链接
void RoomData::http_surprise(const string& s){
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    //object.AddMember("gamesettingid", _gameType, allocator);
    object.AddMember("activityid", s.c_str(), allocator);//42FCD9B4-EE76-4B74-BAEE-8402D697EE29
    IPlayerData * iplayerData = M_GET_PLAYER;
    rapidjson::Value player1(rapidjson::kObjectType);

    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getactivitypoints", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB = CC_CALLBACK_2(RoomData::lhj_Over, this);
    GAMELOG("老虎机结束显示 == %s===URL %s", buffer->GetString(),newurl.c_str());
}
void RoomData::lhj_Over(const string &str, int code) //游戏结束请求
{
    if(str.empty())
    {
        return;
    }
   
    IPlayerData *ipayerData = M_GET_PLAYER;
    ipayerData->updateInfo(str);
    
    rapidjson::Document d;
    d.Parse<0>(str.c_str());
    CCLOG("老虎机返回－－－  %s 信息 ",str.c_str());
    if(d.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
        IActivityData* activity = M_GET_ACTIVITY;
        
        //rapidjson::Value &d = doc["activityinfo"];
        if(d.HasMember("bestpoints"))
            activity->getSelectedActivity()->matchInfo.bestpoints = d["bestpoints"].GetInt();
        if(d.HasMember("totalPoints"))
            activity->getSelectedActivity()->matchInfo.myintegral = d["totalPoints"].GetInt();
        if(d.HasMember("points"))
            activity->getSelectedActivity()->matchInfo.myintegralOnc = d["points"].GetInt();
        if(d.HasMember("rank"))
            activity->getSelectedActivity()->matchInfo.Myrank = d["rank"].GetInt();
        if(d.HasMember("currentrank"))
            activity->getSelectedActivity()->matchInfo.currentrank = d["currentrank"].GetInt();
        if(d.HasMember("betgolds"))
            activity->getSelectedActivity()->matchInfo.betgolds = d["betgolds"].GetInt();
        if(d.HasMember("golds"))
           ipayerData->golds = d["golds"].GetInt();
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    d.Accept(writer);
    CCLOG("老虎机返回结果 %s  拼乐",buffer->GetString());
    //updateGameInfo(str);
    IMessageManager::getInstance()->postNotification(MES_ROOM_LAOHUJISHUJU);
}
//红包排行榜
void RoomData::Redbaojilu(){
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("count", 10, allocator);
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "getrecentgoldslist", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    IPlayerData * iplayerData = M_GET_PLAYER;
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB = CC_CALLBACK_2(RoomData::gameRedbao, this);
    GAMELOG("SENDINFO == %s", buffer->GetString());

}
//红包排行榜返回数据
void  RoomData::gameRedbao(const string &str, int code){
    if(str.empty())
    {
        return;
    }
    
    rapidjson::Document d;
    d.Parse<0>(str.c_str());
    CCLOG("红包排行榜－－－  %s 信息 ",str.c_str());
    if(d.HasParseError())
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    roomdata_vec.clear();
    rapidjson::Value &goldslist = d["goldslist"];
//    if(d.HasMember("goldslist") && d["goldslist"].IsArray())
//    {
        //rapidjson::Value &drc = d["goldslist"];
        for(int n = 0; n < goldslist.Size(); n ++)
        {
            rapidjson::Value &drc = goldslist[n];

        IRoomData _roomdata ;
        
        if(drc.HasMember("headimageurl"))
            _roomdata.headimageurl = drc["headimageurl"].GetString();
        if(drc.HasMember("golds"))
            _roomdata.golds = drc["golds"].GetInt();
        if(drc.HasMember("nickname"))
            _roomdata.red_nickname = drc["nickname"].GetString();
        roomdata_vec.push_back(_roomdata);
//        }
    }
    CCLOG("%lu",roomdata_vec.size());
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    d.Accept(writer);
    CCLOG("红包返回数据 %s  拼乐",buffer->GetString());
//    string path = "";
//    path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(getGameInfo()->imgurl, "/");
//    GAMELOG("imagePath == %s",path.c_str());
     IMessageManager::getInstance()->postNotification(MES_ROOM_HONGBAOPAIHANG);
    }
void RoomData::requestOverGame()        //游戏结束发送请求
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("gamesettingid", _gameType, allocator);
    GAMELOG("***imageURL == %s",_uGameLevelSprite->imgurl.c_str());
    string strname = ITools::Intercept_string(_uGameLevelSprite->imgurl, "/");
    string strend = ITools::Intercept_stringEnd(strname, ".");
    object.AddMember("imageid", strend.c_str(), allocator);
    object.AddMember("starttime", _startTime.c_str(), allocator);
    object.AddMember("finishtime", _endTime.c_str(), allocator);
    object.AddMember("adid", _uGameLevelSprite->adid.c_str(), allocator);
    object.AddMember("description", "", allocator);
    
    IPlayerData * iplayerData = M_GET_PLAYER;
    rapidjson::Value player1(rapidjson::kObjectType);
    player1.AddMember("loginname", iplayerData->userid.c_str(), allocator);
    player1.AddMember("steps", _uRun, allocator);
    player1.AddMember("time", _utimeCount, allocator);
    
    if(_success)
    {
        player1.AddMember("issuccess", "true", allocator);
        player1.AddMember("iswinner", "true", allocator);
    }
    else
    {
        player1.AddMember("issuccess", "false", allocator);
        player1.AddMember("iswinner", "false", allocator);
    }
    player1.AddMember("playerstatus", "1", allocator);
    array.PushBack(player1, allocator);
    object.AddMember("playerlist", array, allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setgamerecord", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);

    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB = CC_CALLBACK_2(RoomData::gameOver, this);
    GAMELOG("SENDINFO == %s", buffer->GetString());
}

void RoomData::gameOver(const string &str, int code) //游戏结束请求
{
    if(str.empty())
    {
        return;
    }
    IPlayerData *ipayerData = M_GET_PLAYER;
    ipayerData->updateInfo(str);
    
    updateGameInfo(str);
    if(!_success && (_gameType == GameMode::pvp_2 || _gameType == GameMode::pvp_4))
    {
        return;
    }
    _success = false;
    IMessageManager::getInstance()->postNotification(MES_ROOM_GAMEOVER);
}

void RoomData::sendSubmitRewardInfo()   //提交订单信息
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    IPlayerData * iplayerData = M_GET_PLAYER;
    IActivityData* iactivityData = M_GET_ACTIVITY;
    object.AddMember("loginname", iplayerData->userid.c_str(), allocator);
    object.AddMember("goodsid", iactivityData->rewardUrl.c_str(), allocator);
    object.AddMember("postage", iactivityData->postWay ,allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setorder", allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->gamePlayerUrl.c_str(),
                                        iplayerData->LandKey.c_str(),iplayerData->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->sendRequest(newurl, "0", buffer->GetString(), 30);
    http->_httpCB = CC_CALLBACK_2(RoomData::submitRewardInfoBack, this);
    GAMELOG("SENDINFO == %s", buffer->GetString());
}

void RoomData::submitRewardInfoBack(const string& str, int code) //提交订单结束
{
    GAMELOG("RoomData::submitRewardInfoBack == %s", str.c_str());
    if(str.empty())
    {
        return;
    }
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());
    
    if(doc.HasMember("err_code"))
    {
        if(doc["err_code"].GetInt() != 0)
            return;
    }
    if(doc.HasMember("isswardprize"))
    {
        IActivityData* activity = M_GET_ACTIVITY;
        activity->getSelectedActivity()->matchInfo.isswardprize = doc["isswardprize"].GetInt();
    }
    
    IMessageManager::getInstance()->postNotification(MES_ROOM_SUBMITREWARE);
}




EnemyData *RoomData::getEnemyData(unsigned int _tag) //获取pvp玩家信息
{
    return &(_enemyData[_tag]);
}

void RoomData::setMapData(map<int,long> _map)       //设置活动map字段
{
    this->_map.clear();
    this->_map.swap(_map);
}

void RoomData::setActivityId(const string &str)  //设置活动id
{
    _activityId = str;
}

string RoomData::getActivityId()     //获取活动id
{
    return _activityId;
}

//void RoomData::setPvpSuccess(const bool b) //设置pvp匹配成功
//{
////    _pvpSuccess = b;
//}

void RoomData::destroyRoomId()      //销毁房间编号
{
    _roomId = "";
}


void RoomData::enemyVerClear()
{
    enemyVer.clear();
    _pvpSuccess = false;
    _gamesuccess = false;
    _isPvp = false;
    _success = false;
    _enemyNum = 0;
    _roomId = "";
}

int RoomData::getGoldLimit()      //获取玩家日常是否达到上限
{
    return goldlimit;
}


