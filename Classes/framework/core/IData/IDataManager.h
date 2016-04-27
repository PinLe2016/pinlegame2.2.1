//
//  IDataManager.h
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#ifndef __framework__IDataManager__
#define __framework__IDataManager__


#include "IData.h"
//#include "../tools/ITools.h"

using namespace std;

class IDataManager
{
public:
    static  IDataManager* getInstance();
    static  void destroyInstance();
    
    bool    attach(IData* data, const string& name);
    bool    detach(const string& name);
    bool    isExist(const string& name);
    IData*  getData(const string& name);
};





#endif /* defined(__framework__IDataManager__) */
