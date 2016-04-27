//
//  IMessage.h
//  framework
//
//  Created by Kennedy on 15/5/25.
//
//

#ifndef __framework__IMessage__
#define __framework__IMessage__


#include "../IData/IDataObserver.h"

using namespace std;


class IMessage
{
public:
    IMessage();
    ~IMessage();
    
    string  name;
    Args    args;
};

#endif /* defined(__framework__IMessage__) */
