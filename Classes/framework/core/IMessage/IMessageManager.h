//
//  IMessageManager.h
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#ifndef __framework__IMessageManager__
#define __framework__IMessageManager__



#include "cocos2d.h"
#include "../IMessage/IMessage.h"
#include "../IData/IDataObserver.h"
#include "../../tools/ITools.h"


using namespace std;
using namespace cocos2d;

class IMessageManager : public Ref
{
public:
    static  IMessageManager* getInstance();
    static  void destroyInstance();
    
    void    startMonitor(float time);
    void    closeMonitor();
    
    bool    attach(IDataObserver* observer, const string& name);
    bool    detach(IDataObserver* observer, const string& name);
    
    void    postNotification(float dt);
    void    postNotification(const string& name, Args* args = NULL);
    
};







#endif /* defined(__framework__IMessageManager__) */
