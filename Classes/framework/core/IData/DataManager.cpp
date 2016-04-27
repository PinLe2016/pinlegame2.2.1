//
//  IDataManager.cpp
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#include "IDataManager.h"
#include "../../tools/ITools.h"
#include "cocos2d.h"

using namespace cocos2d;

static IDataManager* _idataManager = NULL;
static map<string, IData*> _dataMapArr;


IDataManager* IDataManager::getInstance()
{
    if(_idataManager == NULL)
    {
        _idataManager = new IDataManager();
        _dataMapArr.clear();
    }
    return _idataManager;
}


void IDataManager::destroyInstance()
{
    SAFE_DELETE(_idataManager);
    _dataMapArr.clear();
}


bool IDataManager::attach(IData* data, const string& name)
{
//    GAMELOG("---------IDataManager::attach-----------");
    if(data == NULL || name.empty())
    {
        GAMELOG("IDataManager::attach -- data or name is NULL");
        return false;
    }
    if(!isExist(name))
    {
        _dataMapArr.insert(pair<string, IData*>(name, data));
        return true;
    }
    else
    {
        GAMELOG("IDataManager::attach -- has been found the key_type : %s", name.c_str());
        return false;
    }
}


bool IDataManager::detach(const string& name)
{
//    GAMELOG("---------IDataManager::detach-----------");
    if(name.empty())
    {
        GAMELOG("IDataManager::detach -- name is NULL");
        return false;
    }
    if(isExist(name))
    {
        _dataMapArr.erase(name);
        return true;
    }
    else
    {
        GAMELOG("IDataManager::detach -- can't find the key_type : %s", name.c_str());
        return false;
    }
}


bool IDataManager::isExist(const string& name)
{
    map<string, IData*>::iterator it = _dataMapArr.find(name);
    return (it != _dataMapArr.end());
}



IData*  IDataManager::getData(const string& name)
{
//    GAMELOG("---------IDataManager::getData-----------");
    if(name.empty())
    {
        GAMELOG("IDataManager::getIData -- name is NULL");
        return NULL;
    }
    map<string, IData*>::iterator it = _dataMapArr.find(name);
    if(it != _dataMapArr.end())
    {
        return _dataMapArr.at(name);
    }
    else
    {
        GAMELOG("IDataManager::getIData -- can't find the key_type : %s", name.c_str());
        return NULL;
    }
}




































