//
//  UI_Choice.cpp
//  PinLe
//
//  Created by 李辛 on 15/6/11.
//
//

#include "UI_Choice.h"



UI_Choice::UI_Choice()
{
    personal_choice = nullptr;
    layerType = choice_layer;
    pvp_type = 0;
    _pvpFail = false;
    messageBox = nullptr;
    IMessageManager::getInstance()->attach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->attach(this, MES_ROOM_PVPCONTINUE);
    IMessageManager::getInstance()->attach(this, MES_ROOM_PVPADDENEMY);
    
    IUILayer::attachNetworkMes();
    
}

UI_Choice::~UI_Choice()
{
    IMessageManager::getInstance()->detach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->detach(this, MES_ROOM_PVPCONTINUE);
    IMessageManager::getInstance()->detach(this, MES_ROOM_PVPADDENEMY);
    IUILayer::detachNetworkMes();
}

Scene* UI_Choice::createScene()
{
    Scene* s = Scene::create();
    Layer* layer = UI_Choice::create();
    s->addChild(layer);
    return s;
}

bool UI_Choice::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    if(_pvpFail)
    {
        return true;
    }
    
    GAMELOG("UI_Choice::Notify===%s",name.c_str());
    if(name.compare(MES_ROOM_GAMESTART) == 0)
    {
        IRoomData *iroom = M_GET_ROOM;
        if((iroom->getGameType() == GameMode::pvp_4 || iroom->getGameType() == GameMode::pvp_2) && layerType.compare(match_layer) == 0)
        {
            this->unschedule(schedule_selector(UI_Choice::updatePvP));
            showEnemy(true);
            runAction(Sequence::create(
                                       DelayTime::create(3.0f),
                                       CallFuncN::create([=](Node*){
                                UICtrl::getInstance()->gotoScene(UI_ROOM);
                        }),
                      NULL));
        }
        else
        {
            UICtrl::getInstance()->gotoScene(UI_ROOM);
        }
        return YES;
    }
    
    if(name.compare(MES_ROOM_PVPADDENEMY) == 0  && layerType.compare(match_layer) == 0)
    {
        showEnemy();
    }
    
    
    if(name.compare(MES_ROOM_PVPCONTINUE) == 0)
    {
        IRoomData *iroom = M_GET_ROOM;
        pvp_type = iroom->getGameType();
        enterPVP(iroom->getGameType());
        return YES;
    }
    
    if(name.compare(MESSAGEBOX_OK) == 0)
    {
        if(args.code == tips_info_wsgrxx)
        {
            UICtrl::getInstance()->gotoScene(UI_OPTION, false);
        }
        
        messageBox = nullptr;
    }
    
    if(name.compare(SYSTEM_KEYBACK) == 0)
    {
        GAMELOG("*UI_Choice::Notify, SYSTEM_KEYBACK");
        if(layerType.compare(choice_layer) == 0)
        {
            eventCallback(btn_back,Widget::TouchEventType::ENDED);
        }
        else if(layerType.compare(challenge_layer) == 0)
        {
            eventCallback(btn_back_personal,Widget::TouchEventType::ENDED);
        }
        
        return YES;
    }
    return true;
}

bool UI_Choice::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    GAMELOG("======UI_Choice::init()=====");
    Node *rootNode = CSLoader::createNode("CSres/Battle/PVPXuanLayer.csb");
    addChild(rootNode);
    
    ListView *_listButton = (ListView*)rootNode->getChildByTag(416);
    for(int i = 0; i < _listButton->getChildrenCount(); i++)
    {
        Button *b = (Button*)_listButton->getItem(i);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Choice::eventCallback, this));
    }
   
    
    layerType = choice_layer;
    btn_back = (cocos2d::ui::Button*)rootNode->getChildByName("Button_5");//返回
    btn_back->addTouchEventListener(CC_CALLBACK_2(UI_Choice::eventCallback, this));
    
    IAudio::getInstance()->playBackground(music_bg_personchange, NO, YES);
    
    return true;
}

void UI_Choice::enterPersonal()
{
    personal_choice = Layer::create();
    addChild(personal_choice);
    
    Node *personalNode = CSLoader::createNode("CSres/Battle/PVP_Personal_Choice.csb");
    personal_choice->addChild(personalNode);
    
    auto mEasyScore=(Text*)personalNode->getChildByName("Text_2_0");
    auto mEasyTime=(Text*)personalNode->getChildByName("Text_2");
    auto mHardScore=(Text*)personalNode->getChildByName("Text_2_0_0");
    auto mHardTime=(Text*)personalNode->getChildByName("Text_2_1");
    
    IRoomData *iroom = M_GET_ROOM;
    
    __String *temp_str = __String::createWithFormat("%02d",iroom->getLevelType(GameMode::challenge_simple)->igold);
    mEasyScore->setString(temp_str->getCString());
    
    
    
    int utm = round(iroom->getLevelType(GameMode::challenge_simple)->ilimitTime / 60);
    int uts = round(iroom->getLevelType(GameMode::challenge_simple)->ilimitTime % 60);
    temp_str = __String::createWithFormat("%02d:%02d", utm, uts);
    mEasyTime->setString(temp_str->getCString());
    
    temp_str = __String::createWithFormat("%02d",iroom->getLevelType(GameMode::challenge_hard)->igold);
    mHardScore->setString(temp_str->getCString());
    
    utm = round(iroom->getLevelType(GameMode::challenge_hard)->ilimitTime / 60);
    uts = round(iroom->getLevelType(GameMode::challenge_hard)->ilimitTime % 60);
    temp_str = __String::createWithFormat("%02d:%02d", utm, uts);
    mHardTime->setString(temp_str->getCString());
    
    Sprite* spriteLight1 = (Sprite*)personalNode->getChildByTag(personal_light1);
    spriteLight1->runAction(RepeatForever::create(RotateBy::create(2,360)));
    Sprite* spriteLight2 = (Sprite*)personalNode->getChildByTag(personal_light2);
    spriteLight2->runAction(RepeatForever::create(RotateBy::create(2,360)));
    
    
    //简单
    Button * btn_easy_personal = (Button*)personalNode->getChildByName("Button_2");
    btn_easy_personal->addTouchEventListener(CC_CALLBACK_2(UI_Choice::eventCallback,this));
    
    //困难
    Button * btn_diffult_personal = (Button*)personalNode->getChildByName("Button_3");
    btn_diffult_personal->addTouchEventListener(CC_CALLBACK_2(UI_Choice::eventCallback,this));
    
    //返回
    layerType = challenge_layer;
    btn_back_personal = (Button*)personalNode->getChildByName("Button_1");
    btn_back_personal->addTouchEventListener(CC_CALLBACK_2(UI_Choice::eventCallback,this));
}

void UI_Choice::enterPVP(int _tag)
{
    layerType = match_layer;
    pvp_match = Layer::create();
    addChild(pvp_match);
    
    Node *pvpNode = CSLoader::createNode("CSres/Battle/MatchScene.csb");
    pvp_match->addChild(pvpNode);
    
    IPlayerData *iplayer = M_GET_PLAYER;
    
    
    auto *action = CSLoader::createTimeline("CSres/Battle/MatchScene.csb");
    pvpNode->runAction(action);
    action->gotoFrameAndPlay(0);
    
    seekRival = (Text*)pvpNode->getChildByTag(68);
    seekRival->runAction(RepeatForever::create(Sequence::create(
                                                                FadeIn::create(0.5f),
                                                                FadeOut::create(0.5f),
                                                                NULL)));
    //
    Node* m_pTouXiang_duishou_1 = (Node*)pvpNode->getChildByName("Node_2");
    m_pTouXiang_duishou_1->setVisible(false);
    enemyVer.push_back(m_pTouXiang_duishou_1);
    
    Node* m_pTouXiang_duishou_2 = (Node*)pvpNode->getChildByName("Node_3");
    m_pTouXiang_duishou_2->setVisible(false);
    enemyVer.push_back(m_pTouXiang_duishou_2);
    
    Node* m_pTouXiang_duishou_3 = (Node*)pvpNode->getChildByName("Node_4");
    m_pTouXiang_duishou_3->setVisible(false);
    enemyVer.push_back(m_pTouXiang_duishou_3);
    
    //    自己的头像
    Node *myHeadSpNode = (Node*)pvpNode->getChildByName("Node_1");
    Sprite *headSp = (Sprite*)myHeadSpNode->getChildByName("Sprite_2");
    GAMELOG("***iplayer->uHeadIndex == %s",iplayer->uHeadIndex.c_str());
    string headstr = StringUtils::format("res/CSres/Setup/Head/%s",iplayer->uHeadIndex.c_str());
    headSp->setTexture(headstr);

    cocos2d::ui::Text* m_pZiJi_Name = (cocos2d::ui::Text*)myHeadSpNode->getChildByName("Text_1");
    m_pZiJi_Name->setString(iplayer->nickname.c_str());//自己的名字
    // 获取自己称号
    cocos2d::ui::Text* m_pZiJi_NickName = (cocos2d::ui::Text*)myHeadSpNode->getChildByName("Text_1_1");
    m_pZiJi_NickName->setString(iplayer->rankname);
    
    cocos2d::ui::Text* m_pZiJi_Gold = (cocos2d::ui::Text*)myHeadSpNode->getChildByName("Text_2");
    __String * m_pGold_str = __String::createWithFormat("%d",iplayer->golds);
    m_pZiJi_Gold->setString(m_pGold_str->getCString());//自己的金币
    
    enterGameWan = (Text*)pvpNode->getChildByTag(300);
    
    pTime_Show = (Text*)pvpNode->getChildByName("Text_9_0");
    pTime_Show->setString(StringUtils::format("00:00"));
    mStartTime = 0;
    this->schedule(schedule_selector(UI_Choice::updatePvP), 1.0f);
    
}

void UI_Choice::showEnemy(bool b)
{
    if(b)
    {
        pTime_Show->setVisible(false);
        seekRival->setVisible(false);
        enterGameWan->setVisible(true);
        enterGameWan->runAction(RepeatForever::create(
                                                      Sequence::create(
                                                                       FadeIn::create(0.5),
                                                                       FadeOut::create(0.5),
                                                      NULL)
                                ));
    }
    
    IRoomData *iroomData = M_GET_ROOM;
    GAMELOG("***enemyNum == %d",iroomData->getEnemyNum());
    
    for(int i = 0; i < iroomData->getEnemyVer()->size(); i++)
    {
        EnemyData e = iroomData->getEnemyVer()->at(i);
        
        Node* enemyNode = enemyVer.at(i);
        Sprite *headSp = (Sprite*)enemyNode->getChildByName("Sprite_2");
        string headstr = StringUtils::format("CSres/Setup/Head/%s",ITools::Intercept_string(e.u_pvp_headUrl, "/").c_str());
        GAMELOG("***headstr == %s",headstr.c_str());
        headSp->setTexture(headstr);
        
        cocos2d::ui::Text* m_pZiJi_Name = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_1");
        m_pZiJi_Name->setString(e.u_pvp_Name.c_str());//昵称
        
        cocos2d::ui::Text* m_pZiJi_NickName = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_1_1");
        m_pZiJi_NickName->setString(e.u_pvp_randName.c_str());  // 等级
        
        cocos2d::ui::Text* m_pZiJi_Gold = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_2");
        __String * m_pGold_str = __String::createWithFormat("%d",e.u_pvp_Gold);
        m_pZiJi_Gold->setString(m_pGold_str->getCString());//金币
        enemyNode->setVisible(true);
    }
    
    
//    for(int i = 0; i < iroomData->getEnemyNum(); i++)
//    {
//        GAMELOG("***iiii == %d", i);
//        if(iroomData->getEnemyData(i)->u_pvp_Id.compare(iplayer->userid) == 0)
//        {
//            
//        }
//        else
//        {
//            Node* enemyNode = enemyVer.at(i);
//            Sprite *headSp = (Sprite*)enemyNode->getChildByName("Sprite_2");
//            string headstr = StringUtils::format("CSres/Setup/Head/%s",ITools::Intercept_string(iroomData->getEnemyData(i)->u_pvp_headUrl, "/").c_str());
//            GAMELOG("***headstr == %s",headstr.c_str());
//            headSp->setTexture(headstr);
//            
//            cocos2d::ui::Text* m_pZiJi_Name = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_1");
//            m_pZiJi_Name->setString(iroomData->getEnemyData(i)->u_pvp_Id.c_str());//自己的名字
//            // 获取自己称号
//            cocos2d::ui::Text* m_pZiJi_NickName = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_1_1");
//            m_pZiJi_NickName->setString(iroomData->getEnemyData(i)->u_pvp_Name.c_str());
//            
//            cocos2d::ui::Text* m_pZiJi_Gold = (cocos2d::ui::Text*)enemyNode->getChildByName("Text_2");
//            __String * m_pGold_str = __String::createWithFormat("%d",iroomData->getEnemyData(i)->u_pvp_Gold);
//            m_pZiJi_Gold->setString(m_pGold_str->getCString());//自己的金币
//            enemyNode->setVisible(true);
//        }
//    }
}


void UI_Choice::eventCallback(Ref * pSender, Widget::TouchEventType type)
{
    Widget *widget = (Widget*)pSender;
    if(type == Widget::TouchEventType::ENDED)
    {
        IAudio::getInstance()->playEffect(music_ef_ok);
        int btn_tag = widget->getTag();
        GAMELOG("tag == %d",btn_tag);
        IPlayerData *iplayer = M_GET_PLAYER;
        IRoomData *iroom = M_GET_ROOM;
        switch(btn_tag)
        {
            case btn_personal_tag:      //个人挑战
            {
                if(!needModifyInfo())
                {
                    return;
                }
                enterPersonal();
            }break;
            case btn_pvp2_tag:          //二人竞技
            {
                if(!needModifyInfo())
                {
                    return;
                }
                
//                if(iplayer->golds < 20)
                if(iplayer->golds < iroom->getLevelType(GameMode::pvp_2)->iyajin)
                {
                    if(messageBox == nullptr)
                    {
                        string mes = StringUtils::format(GameIni::getInstance()->getTips(tips_info_nomoneypvp2).c_str(),
                                                         iroom->getLevelType(GameMode::pvp_2)->iyajin);
                        messageBox = UIMessage::createMesBox(mes);
                    }
                    return;
                }
                
                pvp_type = GameMode::pvp_2;
                enterPVP(GameMode::pvp_2);
                layerType = match_layer;
                
            }break;
            case btn_pvp4_tag:      //四人竞技
            {
                if(!needModifyInfo())
                {
                    return;
                }
                
                
//                if(iplayer->golds < 20)
                if(iplayer->golds < iroom->getLevelType(GameMode::pvp_4)->iyajin)
                {
                    if(messageBox == nullptr)
                    {
                        string mes = StringUtils::format(GameIni::getInstance()->getTips(tips_info_nomoneypvp4).c_str(),
                                                         iroom->getLevelType(GameMode::pvp_4)->iyajin);
                        messageBox = UIMessage::createMesBox(mes);
                    }
                    return;
                }
                pvp_type = GameMode::pvp_4;
                enterPVP(GameMode::pvp_4);
                layerType = match_layer;
            }break;
            case btn_friend_tag:
            {
                
            }break;
            case btn_back_tag:  //返回主界面
            {
                Args args;
                args.content = ui_pinleba;
                UICtrl::getInstance()->gotoScene(UI_MAIN, args);
            }break;
            case btn_easy_personal_tag:     //个人挑战简单
            {
                if(iplayer->golds < iroom->getLevelType(GameMode::challenge_simple)->iyajin)
                {
                    if(messageBox == nullptr)
                    {
                       messageBox = UIMessage::createMesBox(StringUtils::format(
                                                                GameIni::getInstance()->getTips(tips_info_nomoneysimple).c_str(),iroom->getLevelType(GameMode::challenge_simple)->iyajin));
                    }
                    return;
                }
                
                IRoomData *iroom = M_GET_ROOM;
                iroom->enterGamePlay(GameMode::challenge_simple);
            }break;
            case btn_diffult_personal_tag:      //个人挑战困难
            {
                if(iplayer->golds < iroom->getLevelType(GameMode::challenge_hard)->iyajin)
                {
                    if(messageBox == nullptr)
                    {
                         messageBox = UIMessage::createMesBox(StringUtils::format(
                                                                GameIni::getInstance()->getTips(tips_info_nomoneyhard).c_str(),iroom->getLevelType(GameMode::challenge_hard)->iyajin));
                    }
                    return;
                }
                IRoomData *iroom = M_GET_ROOM;
                iroom->enterGamePlay(GameMode::challenge_hard);
            }break;
            case btn_back_personal_tag:
            {
                if(personal_choice != nullptr)
                {
                    personal_choice->removeFromParentAndCleanup(true);
                    personal_choice = nullptr;
                    layerType = choice_layer;
                }
            }
            default:
                break;
        }
        
    }
}

void UI_Choice::updatePvP(float ft)
{
    __String* ns=__String::createWithFormat("00:%02d",mStartTime + 1);
    string s=ns->getCString();
    pTime_Show->setString(s);
    
    int spaceNum = 0;
    int timeNum = 0;
    if(pvp_type == GameMode::pvp_2)
    {
        spaceNum = GameIni::getInstance()->space_of_wait2;
        timeNum = GameIni::getInstance()->times_of_wait2;
    }
    else if (pvp_type == GameMode::pvp_4)
    {
        spaceNum = GameIni::getInstance()->space_of_wait4;
        timeNum = GameIni::getInstance()->times_of_wait4;
    }
    
    GAMELOG("***spaceNum == %d    ***tiemNum == %d  ***gameType == %d",spaceNum,timeNum,pvp_type);

    if(mStartTime / spaceNum < timeNum)
    {
        if(mStartTime % spaceNum == 0)
        {
            IRoomData *iroom = M_GET_ROOM;
            iroom->enterGamePlay(pvp_type);
        }
    }
    else if(mStartTime / spaceNum > timeNum)
    {
        unschedule(schedule_selector(UI_Choice::updatePvP));
        UICtrl::getInstance()->gotoScene(UI_PK);
        IRoomData* iroom = M_GET_ROOM;
        iroom->destroyRoomId();
        iroom->enemyVerClear();
    }
    
    mStartTime++;
}


bool UI_Choice::needModifyInfo()
{
    IPlayerData *iplayer = M_GET_PLAYER;
    if(!iplayer->isCompletedInfo())
    {
        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_wsgrxx),1);
        messageBox->_args.code = tips_info_wsgrxx;
        return false;
    }
    return true;
}
