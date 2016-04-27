//
//  IBroadcast.h
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#ifndef PinLe_IBroadcastData_h
#define PinLe_IBroadcastData_h


#include "../framework/config.h"
#include "module/Broadcast.h"

class IBroadcastData : public IData, public Ref
{
public:
    virtual void    http_request_getList(){}
    virtual void    http_response_getList(const string& s){}
    
    virtual void    http_request_readList(){}
    virtual void    http_response_readList(const string& s){}
    
public:
    virtual Broadcast*  getItem(uint index) = 0;
    virtual Broadcast*  getItemByTag(int tag) = 0;
    virtual Broadcast*  getItemByID(const string& ID) = 0;
    virtual Broadcast*  getUnusedItem() = 0;
    virtual Broadcast*  getUnusedItemEx() = 0;
    virtual void        startMonitor(float time = 20){}
    virtual void        stopMonitor(){}
    virtual vector<Broadcast*>& getList() = 0;
};




#endif
