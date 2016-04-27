//
//  GameScene.h
//  Pinle
//
//  Created by 李辛 on 15/6/2.
//
//

#ifndef __Pinle__GameScene__
#define __Pinle__GameScene__

#include "../../framework/config.h"

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();
    CREATE_FUNC(GameScene);
    static cocos2d::Scene *createScene();
    virtual bool init();
//    virtual bool Notify(const string &str,int code);
};



#endif /* defined(__Pinle__GameScene__) */
