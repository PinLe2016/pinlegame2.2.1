//
//  broadcast.h
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#ifndef __PinLe__BroadcastData__
#define __PinLe__BroadcastData__


#include "../IBroadcastData.h"

#include "Broadcast.h"

class BroadcastData : public IBroadcastData, public IDataObserver
{
public:
    BroadcastData();
    ~BroadcastData();
    
    static  void    init();
    static  void    destroy();
    virtual bool    Notify(const string& name, Args& args);
    
public:
    
    void        http_request_getList();
    void        http_response_getList(const string& s);
    
    void        http_request_readList();
    void        http_response_readList(const string& s);

public:
    
    void        insert(Broadcast* b);
    void        cleanList();
    void        deleteItem(Broadcast* b);
    void        deleteItem(uint index);
    void        updateList();//删除时效的广播
    void        postBroadcast(float dt);
    void        startMonitor(float time = 20);
    void        stopMonitor();
    Broadcast*  getItem(uint index);
    Broadcast*  getItemByTag(int tag);
    Broadcast*  getItemByID(const string& ID);
    Broadcast*  getUnusedItem();
    Broadcast*  getUnusedItemEx();
    vector<Broadcast*>& getList();
    
private:
    void        savefile();
    void        loadfile();

};




#endif /* defined(__PinLe__broadcast__) */
