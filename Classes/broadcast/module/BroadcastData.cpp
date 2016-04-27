//
//  broadcast.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#include "BroadcastData.h"
#include "../config.h"
#include "../../player/IPlayerData.h"
#include "../../gameini/GameIni.h"

static  vector<Broadcast*> v_bcArr;

BroadcastData::BroadcastData()
{
//    IMessageManager::getInstance()->attach(this, MES_BROADCAST_POPITEM);
    IMessageManager::getInstance()->attach(this, MES_BROADCAST_DISPLAYITEM);
    IMessageManager::getInstance()->attach(this, MES_BROADCAST_GETLIST);
    
}
BroadcastData::~BroadcastData()
{
//    IMessageManager::getInstance()->detach(this, MES_BROADCAST_POPITEM);
    IMessageManager::getInstance()->detach(this, MES_BROADCAST_DISPLAYITEM);
    IMessageManager::getInstance()->detach(this, MES_BROADCAST_GETLIST);
    
}


void BroadcastData::init()
{
    if(!IDataManager::getInstance()->isExist(M_BROADCAST))
    {
        BroadcastData* broadcast = new BroadcastData();
        IDataManager::getInstance()->attach(broadcast, M_BROADCAST);
        broadcast->cleanList();
        broadcast->loadfile();
    }
}

void BroadcastData::destroy()
{
    BroadcastData* b = (BroadcastData*)IDataManager::getInstance()->getData(M_BROADCAST);
    b->cleanList();
    IDataManager::getInstance()->detach(M_BROADCAST);
    SAFE_DELETE(b);
}


void BroadcastData::insert(Broadcast* b)
{
    if(!b)
        return;
    v_bcArr.push_back(b);
}

Broadcast* BroadcastData::getItem(uint index)
{
    return index < v_bcArr.size() ? v_bcArr[index] : nullptr;
}

vector<Broadcast*>& BroadcastData::getList()
{
    return v_bcArr;
}

void BroadcastData::cleanList()
{
    for(int i = 0; i < v_bcArr.size(); i ++)
    {
        SAFE_DELETE(v_bcArr[i]);
    }
    v_bcArr.clear();
}

void BroadcastData::deleteItem(Broadcast* b)
{
    if(!b)
        return;
    vector<Broadcast*>::iterator it = v_bcArr.begin();
    for(; it != v_bcArr.end(); it++)
    {
        if(b == *it)
        {
            SAFE_DELETE(b);
            v_bcArr.erase(it);
            break;
        }
    }
}

void BroadcastData::deleteItem(uint index)
{
    if(index < v_bcArr.size())
    {
        vector<Broadcast*>::iterator it = v_bcArr.begin() + index;
        SAFE_DELETE(*it);
        v_bcArr.erase(it);
    }
}

Broadcast* BroadcastData::getItemByID(const string& ID)
{
    for(uint i = 0; i < v_bcArr.size(); i ++)
    {
        if(v_bcArr[i]->id.compare(ID) == 0)
            return v_bcArr[i];
    }
    return nullptr;
}

Broadcast* BroadcastData::getItemByTag(int tag)
{
    for(uint i = 0; i < v_bcArr.size(); i ++)
    {
        if(v_bcArr[i]->tag == tag)
            return v_bcArr[i];
    }
    return nullptr;
}

Broadcast* BroadcastData::getUnusedItem()
{
    for(uint i = 0; i < v_bcArr.size(); i ++)
    {
        if(v_bcArr[i]->used == false)
            return v_bcArr[i];
    }
    return nullptr;
}

Broadcast* BroadcastData::getUnusedItemEx()
{
    Broadcast* b = getUnusedItem();
    if(b)
        b->used = true;
    return b;
}

void BroadcastData::updateList()
{
    bool update = false;
    vector<Broadcast*>::iterator it = v_bcArr.begin();
    while(it != v_bcArr.end())
    {
        Broadcast* b = *it;
        if(!b->isValid())
        {
            SAFE_DELETE(b);
            it = v_bcArr.erase(it);
            update = true;
        }
        else
        {
            if(!b->used)
                update = true;
            it ++;
        }
    }
    if(update)
    {
       //存档本地广播信息$
        savefile();
        IMessageManager::getInstance()->postNotification(MES_BROADCAST_UPDATELIST);
    }
}

void BroadcastData::postBroadcast(float dt)
{
    if(BROAD_CAST)
    {
        http_request_getList();
    }
    updateList();
}

static bool isStart = false;
void BroadcastData::startMonitor(float time)
{
    if(!isStart)
    {
        postBroadcast(1);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(BroadcastData::postBroadcast),
                                                      this, time, false);
        isStart = true;
    }
}
void BroadcastData::stopMonitor()
{
    if(isStart)
    {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(BroadcastData::postBroadcast),
                                                            this);
        isStart = false;
    }
}

void BroadcastData::savefile()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//    rapidjson::Value object(rapidjson::kObjectType);
    rapidjson::Value array(rapidjson::kArrayType);
    
    for(uint i = 0; i < v_bcArr.size(); i ++)
    {
        Broadcast* b = v_bcArr[i];
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("id", b->id.c_str(), allocator);
        object.AddMember("title", b->title.c_str(), allocator);
        object.AddMember("content", b->content.c_str(), allocator);
        object.AddMember("linkstr", b->linkstr.c_str(), allocator);
        object.AddMember("starttime", b->starttime.c_str(), allocator);
        object.AddMember("endtime", b->endtime.c_str(), allocator);
        object.AddMember("lastplay", b->lastplay.c_str(), allocator);
        object.AddMember("colorR", b->color3B.r, allocator);
        object.AddMember("colorG", b->color3B.g, allocator);
        object.AddMember("colorB", b->color3B.b, allocator);
        object.AddMember("type", b->type, allocator);
        object.AddMember("tag", b->tag, allocator);
        object.AddMember("used", b->used, allocator);
        object.AddMember("existtime", b->existtime, allocator);
        object.AddMember("isResponse", b->isResponse, allocator);
        array.PushBack(object, allocator);
    }
    document.AddMember(broadcast_file_info, array, allocator);

    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
//    GAMELOG("savefile = %s", buffer->GetString());
    
    UserDefault::getInstance()->setStringForKey(broadcast_file_key, buffer->GetString());
    UserDefault::getInstance()->flush();
}



void BroadcastData::loadfile()
{
    string load_str = UserDefault::getInstance()->getStringForKey(broadcast_file_key, "");
//    GAMELOG("BroadcastData::loadfile()   load_str = %s", load_str.c_str());
    if(load_str.empty())
        return;

    rapidjson::Document doc;
    doc.Parse<0>(load_str.c_str());
    
    if(doc.HasParseError() || !doc.HasMember(broadcast_file_info))
    {
        return;
    }
    rapidjson::Value &array = doc[broadcast_file_info];
    if(array.IsArray())
    {
        GAMELOG("loadfile  array.Size() = %u", array.Size());
        for(uint i = 0; i < array.Size(); i ++)
        {
            rapidjson::Value &d = array[i];
            Broadcast* b = new Broadcast();
            if(d.HasMember("id"))           b->id = d["id"].GetString();
            if(d.HasMember("title"))        b->title = d["title"].GetString();
            if(d.HasMember("content"))      b->content = d["content"].GetString();
            if(d.HasMember("linkstr"))      b->linkstr = d["linkstr"].GetString();
            if(d.HasMember("starttime"))    b->starttime = d["starttime"].GetString();
            if(d.HasMember("endtime"))      b->endtime = d["endtime"].GetString();
            if(d.HasMember("lastplay"))     b->lastplay = d["lastplay"].GetString();
            if(d.HasMember("colorR"))       b->color3B.r = d["colorR"].GetInt();
            if(d.HasMember("colorG"))       b->color3B.g = d["colorG"].GetInt();
            if(d.HasMember("colorB"))       b->color3B.b = d["colorB"].GetInt();
            if(d.HasMember("type"))         b->type = d["type"].GetInt();
            if(d.HasMember("tag"))          b->tag = d["tag"].GetInt();
            if(d.HasMember("used"))         b->used = d["used"].GetBool();
            if(d.HasMember("isResponse"))   b->isResponse = d["isResponse"].GetBool();
            if(d.HasMember("existtime"))    b->existtime = d["existtime"].GetUint64();
            v_bcArr.push_back(b);
        }
    }
    updateList();
}


bool BroadcastData::Notify(const string& name, Args& args)
{
//    if(name.compare(MES_BROADCAST_POPITEM) == 0)
//    {
//        if(args.ptr)
//        {
//            deleteItem((Broadcast*)args.ptr);
//            return YES;
//        }
//        deleteItem(args.code);
//        return YES;
//    }
    
    if(name.compare(MES_BROADCAST_DISPLAYITEM) == 0)
    {
        Broadcast* b = getItemByID(args.content);
        if(b)
        {
            b->used = YES;
            if(b->isOnce())
            {
                deleteItem(b);
                //存档本地广播信息$
                savefile();
            }
        }
        return YES;
    }
    
    if(name.compare(MES_BROADCAST_GETLIST) == 0)
    {
        http_request_readList();
        return YES;
    }
    
    return YES;
}




//=============================================================================================

void BroadcastData::http_request_getList()
{
    IPlayerData* p = M_GET_PLAYER;
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("latitude", p->latitude.c_str(), allocator);
    object.AddMember("longtitude", p->longitude.c_str(), allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, np_getaffiche, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->_rc._errorMes = NO;
    http->sendRequest(newurl, "0", buffer->GetString(), 30, NO);
    http->_httpCB2 = CC_CALLBACK_1(BroadcastData::http_response_getList, this);
}

void BroadcastData::http_response_getList(const string& s)
{
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    if(doc.HasParseError() || doc["err_code"].GetInt()==16)
    {
        auto messageBox = UIMessage::createMesBox("其他设备已登录您的账号，若非您本人操作，请尽快修改密码！");
        return;
    }
    if(doc.HasParseError() || !doc.HasMember("affichelist"))
    {
        auto messageBox = UIMessage::createMesBox("服务器不响应,稍后再试");
        return;
    }
    
    
    bool isPost = false;
    rapidjson::Value &affichelist = doc["affichelist"];
    if(affichelist.IsArray())
    {
//        GAMELOG("affichelist.Size() = %u", affichelist.Size());
        for(uint i = 0; i < affichelist.Size(); i ++)
        {
            rapidjson::Value &d = affichelist[i];
            if(!d.HasMember("id") || getItemByID(d["id"].GetString()))
                continue;
            isPost = true;
            Broadcast* b = new Broadcast();
            if(d.HasMember("id"))           b->id = d["id"].GetString();
            if(d.HasMember("title"))        b->title = d["title"].GetString();
            if(d.HasMember("content"))      b->content = d["content"].GetString();
            if(d.HasMember("color"))        b->color3B = ITools::color_index2rgb(d["color"].GetString());
            if(d.HasMember("starttime"))    b->starttime = d["starttime"].GetString();
            if(d.HasMember("endtime"))      b->endtime = d["endtime"].GetString();
            if(d.HasMember("linktype"))
            {
                b->type = d["linktype"].GetInt();
                if(d.HasMember("linkcontent"))
                {
                    switch (b->type)
                    {
                        case 1://外联 url
                            b->linkstr = d["linkcontent"].GetString();
                            break;
                        case 2://内链
                        {
                            int id = atoi(d["linkcontent"].GetString());
                            switch (id)
                            {
                                case GameMode::activity:    b->tag = UI_ACTIVITY;   break;
                                case GameMode::pvp_2:
                                case GameMode::pvp_4:
                                case GameMode::challenge_simple:
                                case GameMode::challenge_hard:
                                {
                                    b->tag = UI_PK;
                                }break;
                                default:                    b->tag = UI_MAIN;       break;
                            }
                        }break;
                        
                        default:
                            break;
                    }
                }
            }
            insert(b);
        }
        
//        for(int i = 0; i < v_bcArr.size(); i++)
//        {
//            GAMELOG("contentsss===%s   tag == %d, type==%d", v_bcArr.at(i)->content.c_str(), v_bcArr.at(i)->tag, v_bcArr.at(i)->type);
//        }
    }
    if(isPost)
    {
        IMessageManager::getInstance()->postNotification(MES_BROADCAST_GETLIST);
    }
}


void BroadcastData::http_request_readList()
{
    IPlayerData* p = M_GET_PLAYER;
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    rapidjson::Value array(rapidjson::kArrayType);
    
    object.AddMember("latitude", p->latitude.c_str(), allocator);
    object.AddMember("longtitude", p->longitude.c_str(), allocator);
    
    document.AddMember(np_methodtype, "json", allocator);
    document.AddMember(np_createtime, "1413800515", allocator);
    document.AddMember(np_functionname, "setafficheread", allocator);
    
    for(uint i = 0; i < v_bcArr.size(); i ++)
    {
        Broadcast* b = v_bcArr[i];
        if(!b->isResponse)
        {
            b->isResponse = YES;
            rapidjson::Value obj(rapidjson::kObjectType);
            obj.AddMember("id", b->id.c_str(), allocator);
            array.PushBack(obj, allocator);
        }
    }
    object.AddMember("messageid", array, allocator);
    document.AddMember(np_functionparams, object, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
    //网络请求ip 按照协议加密
    string newurl = StringUtils::format("%stype=json&key=%s&md5=%s", GameIni::getInstance()->server_ip.c_str(),
                                        p->LandKey.c_str(), p->MiYao_key.c_str());
    myHttpWork* http = myHttpWork::createHttp();
    http->_rc._errorMes = NO;
    http->sendRequest(newurl, "0", buffer->GetString(), 30, NO);
    http->_httpCB2 = CC_CALLBACK_1(BroadcastData::http_response_getList, this);
}


void BroadcastData::http_response_readList(const string& s)
{
    
}













