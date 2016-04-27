//
//  IPokedexData.h
//  PinLe
//
//  Created by Kennedy on 15/7/1.
//
//

#ifndef PinLe_IPokedexData_h
#define PinLe_IPokedexData_h


#include "../framework/config.h"
#include "../activity/module/Advertise.h"

class IPokedexData : public IData
{
public:
    virtual Advertise*  getAdvertise(uint index) = 0;
    virtual Advertise*  getAdvertise(const string& adid) = 0;
    virtual vector<Advertise*>& getList() = 0;
    virtual void        insert(Advertise* a){}
    virtual void        insert(const string& adid, const string& ownerid, const string& imgurl,
                               const string& logourl, const string& rewardtitle, const string& rewardurl){}
    virtual void        savefile(){}
    virtual void        loadfile(){}
    
    
};

#endif
