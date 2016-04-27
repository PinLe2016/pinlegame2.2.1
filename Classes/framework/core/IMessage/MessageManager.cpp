
//
//  IMessageManager.cpp
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#include "IMessageManager.h"
#include "IMessage.h"
#include "../../tools/ITools.h"
//#include "../../../option/view/PersonCenterLayerEx.h"   //../../option/view/
//---------
std::mutex _ContainerMutex;


typedef struct
{
    IDataObserver*  observer;
    string          name;
    int             type;
}ob;


static IMessageManager* iMessageManager = NULL;
static vector<IMessage*> v_messageArr;
static vector<IMessage*> v_messageArrTmp;
static map<string, vector<IDataObserver*>> _observerMapArr;
static bool IMessageManager_postNotification = false;

static vector<ob> v_obArrTmp;



IMessageManager* IMessageManager::getInstance()
{
    if(iMessageManager == NULL)
    {
        iMessageManager = new IMessageManager();
        _observerMapArr.clear();
    }
    return iMessageManager;
}

void IMessageManager::destroyInstance()
{
    SAFE_DELETE(iMessageManager);   
    _observerMapArr.clear();
}


void IMessageManager::startMonitor(float time)
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(IMessageManager::postNotification),
                                                      IMessageManager::getInstance(), time, false);
}

void IMessageManager::closeMonitor()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(IMessageManager::postNotification),
                                                        IMessageManager::getInstance());
}

bool IMessageManager::attach(IDataObserver* observer, const string& name)
{
    if(observer == NULL || name.empty())
    {
        GAMELOG("IMessageManager::attach -- observer or name is NULL");
        return false;
    }
    
    if(IMessageManager_postNotification)
    {
        ob _ob;
        _ob.observer = observer;
        _ob.name = name;
        _ob.type = 1;
        v_obArrTmp.push_back(_ob);
//        gamelog("IMessageManager_postNotification attach");
        return true;
    }
    
    
//    GAMELOG("------IMessageManager::attach-----0-");
    std::lock_guard<std::mutex> ul(_ContainerMutex);
//    GAMELOG("------IMessageManager::attach-----1-");
    map<string, vector<IDataObserver*>>::iterator it = _observerMapArr.find(name);
    if(it != _observerMapArr.end())
    {
        vector<IDataObserver*> &v_Observer = _observerMapArr.at(name);
        uint size = (uint)v_Observer.size();
        for(uint i = 0; i < size; i ++)
        {
            if(v_Observer[i] == observer)
            {
                GAMELOG("IMessageManager::attach -- has been found the key_type : %s and observer", name.c_str());
                return false;
            }
        }
        v_Observer.push_back(observer);
    }
    else
    {
        vector<IDataObserver*> v_Observer;
        v_Observer.push_back(observer);
        _observerMapArr.insert(pair<string, vector<IDataObserver*>>(name, v_Observer));
    }
    return true;
}


bool IMessageManager::detach(IDataObserver* observer, const string& name)
{
    if(observer == NULL || name.empty())
    {
        GAMELOG("IMessageManager::detach -- observer or name is NULL");
        return false;
    }
    if(IMessageManager_postNotification)
    {
        ob _ob;
        _ob.observer = observer;
        _ob.name = name;
        _ob.type = 0;
        v_obArrTmp.push_back(_ob);
//        gamelog("IMessageManager_postNotification detach");
        return true;
    }
    
    
//    GAMELOG("------IMessageManager::detach-----0-");
    std::lock_guard<std::mutex> ul(_ContainerMutex);
//    GAMELOG("------IMessageManager::detach-----1-");
    
    map<string, vector<IDataObserver*>>::iterator it = _observerMapArr.find(name);
    if(it != _observerMapArr.end())
    {
        vector<IDataObserver*> &v_Observer = _observerMapArr.at(name);
        
        vector<IDataObserver*>::iterator it;
        for(it = v_Observer.begin(); it != v_Observer.end(); it ++)
        {
            if(*it == observer)
            {
                v_Observer.erase(it);
                return true;
            }
        }
    }
    GAMELOG("IMessageManager::detach -- can't find the key_type : %s and observer", name.c_str());
    return false;
}







void IMessageManager::postNotification(float dt)
{
//    std::lock_guard<std::mutex> ul(_ContainerMutex);

    IMessageManager_postNotification = true;
    vector<IMessage*>::iterator it;
    for(it = v_messageArr.begin(); it != v_messageArr.end();it ++)
    {
        IMessage* m = *it;
        map<string, vector<IDataObserver*>>::iterator iterator = _observerMapArr.find(m->name);
        if(iterator != _observerMapArr.end())
        {
            vector<IDataObserver*> &v_Observer = _observerMapArr.at(m->name);
            if(m->name.compare("system_keyback") == 0)
            {
                GAMELOG("-----#######  size==%ld",v_Observer.size());
            }
            
            uint size = (uint)v_Observer.size();
            for(uint i = 0; i < size; i ++)
            {
                IDataObserver* ob = v_Observer[i];
//                std::lock_guard<std::mutex> ul(_ContainerMutex);
                ob->Notify(m->name, m->args);
            }
        }
        SAFE_DELETE(m);
    }
    v_messageArr.clear();
    IMessageManager_postNotification = false;
    for(uint i = 0; i < v_messageArrTmp.size(); i ++)
        v_messageArr.push_back(v_messageArrTmp[i]);
    v_messageArrTmp.clear();
    
    for(uint i = 0; i < v_obArrTmp.size(); i ++)
    {
        if(v_obArrTmp[i].type == 1)
            IMessageManager::getInstance()->attach(v_obArrTmp[i].observer, v_obArrTmp[i].name);
        else
            IMessageManager::getInstance()->detach(v_obArrTmp[i].observer, v_obArrTmp[i].name);
    }
    v_obArrTmp.clear();
}

void IMessageManager::postNotification(const string& name, Args* args)
{
    if(name.empty())
    {
        GAMELOG("IMessageManager::postNotification -- the key_type can't be NULL");
        return;
    }
//    std::lock_guard<std::mutex> ul(_ContainerMutex);
    IMessage* m = new IMessage();
    m->name = name;
    if(args)
        m->args = *args;
    //    v_messageArr.push_back(m);
    (IMessageManager_postNotification != true) ? v_messageArr.push_back(m) : v_messageArrTmp.push_back(m);
}






















