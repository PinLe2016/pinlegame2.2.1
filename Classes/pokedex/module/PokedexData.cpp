//
//  PokedexData.cpp
//  PinLe
//
//  Created by Kennedy on 15/7/1.
//
//

#include "PokedexData.h"
#include "../config.h"
#include "../../gameini/GameIni.h"


static vector<Advertise*> v_pokedex;



PokedexData::PokedexData()
{
    
}
PokedexData::~PokedexData()
{
    cleanList();
}


void PokedexData::init()
{
    if(!IDataManager::getInstance()->isExist(M_POKEDEX))
    {
        PokedexData* pokedex = new PokedexData();
        IDataManager::getInstance()->attach(pokedex, M_POKEDEX);
        pokedex->cleanList();
        pokedex->loadfile();
    }
}

void PokedexData::destroy()
{
    PokedexData* b = (PokedexData*)IDataManager::getInstance()->getData(M_POKEDEX);
    b->cleanList();
    IDataManager::getInstance()->detach(M_POKEDEX);
    SAFE_DELETE(b);
}


vector<Advertise*>& PokedexData::getList()
{
    return v_pokedex;
}

Advertise*  PokedexData::getAdvertise(uint index)
{
    return index < v_pokedex.size() ? v_pokedex[index] : nullptr;
}

Advertise* PokedexData::getAdvertise(const string& adid)
{
    for(uint i = 0; i < v_pokedex.size(); i ++)
    {
        if(v_pokedex[i]->adid.compare(adid) == 0)
            return v_pokedex[i];
    }
    return nullptr;
}

void PokedexData::deleteItem(const string& adid)
{
    if(adid.empty())
        return;
    vector<Advertise*>::iterator it = v_pokedex.begin();
    for(; it != v_pokedex.end(); it++)
    {
        if((*it)->adid.compare(adid) == 0)
        {
            SAFE_DELETE(*it);
            v_pokedex.erase(it);
            break;
        }
    }
}

void PokedexData::deleteItemByImgUrl(const string& imgurl)
{
    if(imgurl.empty())
        return;
    vector<Advertise*>::iterator it = v_pokedex.begin();
    for(; it != v_pokedex.end(); it++)
    {
        string  _imgurl = ITools::Intercept_string((*it)->imgurl,"/");//截取图片名
        string  _sender = ITools::Intercept_string(imgurl,"/");//截取图片名
        
        
//        GAMELOG("0imgurl = %s\n0sender = %s", (*it)->imgurl.c_str(), imgurl.c_str());
//        GAMELOG("imgurl = %s\nsender = %s", _imgurl.c_str(), _sender.c_str());
        
        if(_imgurl.compare(_sender) == 0)
        {
            SAFE_DELETE(*it);
            v_pokedex.erase(it);
            break;
        }
    }
}

void PokedexData::insert(const string& adid, const string& ownerid, const string& imgurl,
                         const string& logourl, const string& rewardtitle, const string& rewardurl)
{
    Advertise* a = new Advertise();
    a->adid = adid;
    a->ownerid = ownerid;
    a->imgurl  = imgurl;
    a->logourl = logourl;
    a->rewardtitle = rewardtitle;
    a->rewardurl   = rewardurl;
    insert(a);
}

void PokedexData::insert(Advertise* a)
{
    if(!a)
        return;
    
//    deleteItem(a->adid);
    deleteItemByImgUrl(a->imgurl);
    
    if(v_pokedex.empty())
    {
        v_pokedex.push_back(a);
    }
    else
    {
        uint i = 0;
        for(; i < v_pokedex.size(); i ++)
        {
            if(a->datetime > v_pokedex[i]->datetime)
                break;
        }
        v_pokedex.insert(v_pokedex.begin() + i, a);
        if(v_pokedex.size() >= POKEDEX_MAX)
        {
            vector<Advertise*>::iterator it = v_pokedex.begin() + POKEDEX_MAX;
            while(it != v_pokedex.end())
            {
                //----删除多余图片-----
                string filename = ITools::Intercept_string((*it)->imgurl, "/");//截取图片名
                string path = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + filename;
                bool del = IFile::removeFile(path);
                GAMELOG("PokedexData::insert del = %d", del);
                //----删除存储节点-----
                SAFE_DELETE(*it);
                it = v_pokedex.erase(it);  
            }
        }
//        GAMELOG("PokedexData::insert time = %llu, v_pokedexsize = %lu", a->datetime, v_pokedex.size());
    }
    savefile();
}

void PokedexData::cleanList()
{
    for(uint i = 0; i < v_pokedex.size(); i ++)
    {
        SAFE_DELETE(v_pokedex[i]);
    }
    v_pokedex.clear();
}

void PokedexData::savefile()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    
    for(uint i = 0; i < v_pokedex.size(); i ++)
    {
        Advertise* a = v_pokedex[i];
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("adid", a->adid.c_str(), allocator);
        object.AddMember("ownerid", a->ownerid.c_str(), allocator);
        object.AddMember("imgurl", a->imgurl.c_str(), allocator);
        object.AddMember("logourl", a->logourl.c_str(), allocator);
        object.AddMember("rewardtitle", a->rewardtitle.c_str(), allocator);
        object.AddMember("rewardurl", a->rewardurl.c_str(), allocator);
        object.AddMember("datetime", a->datetime, allocator);
        array.PushBack(object, allocator);
    }
    document.AddMember(pokedex_file_info, array, allocator);
    
    rapidjson::StringBuffer * buffer = new rapidjson::StringBuffer();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    document.Accept(writer);
    
//    GAMELOG("PokedexData savefile = %s", buffer->GetString());
    
    UserDefault::getInstance()->setStringForKey(pokedex_file_key, buffer->GetString());
    UserDefault::getInstance()->flush();
}

void PokedexData::loadfile()
{
    cleanList();
    string load_str = UserDefault::getInstance()->getStringForKey(pokedex_file_key, "");
//    GAMELOG("PokedexData::loadfile()   load_str = %s", load_str.c_str());
    if(load_str.empty())
        return;
    
    rapidjson::Document doc;
    doc.Parse<0>(load_str.c_str());
    
    if(doc.HasParseError() || !doc.HasMember(pokedex_file_info))
    {
        return;
    }
    rapidjson::Value &array = doc[pokedex_file_info];
    if(array.IsArray())
    {
//        GAMELOG("loadfile  array.Size() = %u", array.Size());
        for(uint i = 0; i < array.Size(); i ++)
        {
            rapidjson::Value &d = array[i];
            Advertise* a = new Advertise();
            if(d.HasMember("adid"))         a->adid = d["adid"].GetString();
            if(d.HasMember("ownerid"))      a->ownerid = d["ownerid"].GetString();
            if(d.HasMember("imgurl"))       a->imgurl = d["imgurl"].GetString();
            if(d.HasMember("logourl"))      a->logourl = d["logourl"].GetString();
            if(d.HasMember("rewardtitle"))  a->rewardtitle = d["rewardtitle"].GetString();
            if(d.HasMember("rewardurl"))    a->rewardurl = d["rewardurl"].GetString();
            if(d.HasMember("datetime"))     a->datetime = d["datetime"].GetInt64();
            v_pokedex.push_back(a);
        }
    }
}




