//
//  PokedexData.h
//  PinLe
//
//  Created by Kennedy on 15/7/1.
//
//

#ifndef __PinLe__PokedexData__
#define __PinLe__PokedexData__

#include "../IPokedexData.h"
#include "../../activity/module/Advertise.h"



class PokedexData : public IPokedexData
{
public:
    PokedexData();
    ~PokedexData();
    
    static  void    init();
    static  void    destroy();
    
    Advertise*  getAdvertise(uint index);
    Advertise*  getAdvertise(const string& adid);
    void        insert(Advertise* a);
    void        insert(const string& adid, const string& ownerid, const string& imgurl,
                       const string& logourl, const string& rewardtitle, const string& rewardurl);
    void        cleanList();
    void        savefile();
    void        loadfile();
    
    vector<Advertise*>& getList();
    
private:
    void        deleteItem(const string& adid);
    void        deleteItemByImgUrl(const string& imgurl);
};




#endif /* defined(__PinLe__PokedexData__) */
