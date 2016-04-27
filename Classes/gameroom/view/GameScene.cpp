//
//  GameScene.cpp
//  Pinle
//
//  Created by 李辛 on 15/6/2.
//
//

#include "GameScene.h"
#include "GameLayerw.h"
//#include "TurntableSystem.h"
GameScene::GameScene()
{
    
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
    GAMELOG("GameScene::init1");
    if(!CCScene::init())
    {
        GAMELOG("GameScene::init2");
        return false;
    }
    GAMELOG("GameScene::init3");
    
    GameLayerw *gameLayer = GameLayerw::create();
    this->addChild(gameLayer);
    
    
    return true;
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
