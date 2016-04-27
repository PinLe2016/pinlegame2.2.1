//
//  moduleInit.h
//  framework
//
//  Created by Kennedy on 15/6/1.
//
//

#ifndef __framework__moduleInit__
#define __framework__moduleInit__

#include "player/module/PlayerData.h"
#include "activity/module/ActivityData.h"
#include "framework/system/file/IFile.h"
#include "gameroom/module/RoomData.h"
#include "broadcast/module/BroadcastData.h"
#include "pokedex/module/PokedexData.h"

class Module
{
public:
    static  void init()
    {
        srand((uint32_t)time(nullptr));
        PlayerData::initp();
        ActivityData::init();
        RoomData::init();
        BroadcastData::init();
        PokedexData::init();
 
        IMessageManager::getInstance()->startMonitor(1/60.f); 
    }
    
    static  void destroy()
    {
        PlayerData::destroy();
        ActivityData::destroy();
        RoomData::destroy();
        BroadcastData::destroy();
        PokedexData:destroy();
        
        IMessageManager::getInstance()->closeMonitor();
        IDataManager::destroyInstance();
        IMessageManager::destroyInstance();
    }
};


#endif /* defined(__framework__moduleInit__) */
