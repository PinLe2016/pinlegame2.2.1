//
//  UI_Main.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/10.
//
//

#include "UI_Main.h"
#include "../../broadcast/config.h"
#include "../../broadcast/view/TipsLayer.h"
#include "ScrollCycleView.h"
#include "../../PluginHelper.h"
#include "../../ThirdParty/C2DXShareSDK/ShareSDKHelper.h"
#include "../../hud/HUDLayer.h"
#include "../../dazhuanpan/TurntableSystem.h"



Scene* UI_Main::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Main::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
//    HUDLayer* hudLayer = HUDLayer::create();
//    scene->addChild(hudLayer);
    
    // return the scene
    return scene;
}

enum OptionType
{
    ADVISE = 0,
    BUSINESS,
};

UI_Main::UI_Main()
{
    _player = M_GET_PLAYER;
    _room = M_GET_ROOM;
    _pokedex = M_GET_POKEDEX;
    m_quitgame = nullptr;
    IUILayer::attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_GETLISTDONE);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->attach(this, MES_ROOM_GAMESTART);
//    IMessageManager::getInstance()->attach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_SENDADVISE);
    IMessageManager::getInstance()->attach(this, GUIDE_MAIN_CHOOSE_GAME);

    
    _option_type = OptionType::ADVISE;
    
    gamelog("----UI_Main::UI_Main---");
    
}

UI_Main::~UI_Main()
{
    gamelog("----UI_Main::~UI_Main---");
    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_GETLISTDONE);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->detach(this, MES_ROOM_GAMESTART);
//    IMessageManager::getInstance()->detach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_SENDADVISE);
    IMessageManager::getInstance()->detach(this, GUIDE_MAIN_CHOOSE_GAME);
}



//1.	头像图片：8
//2.	玩家昵称：10
//3.	玩家等级：11
//4.	日常拼图：13
//5.	惊喜吧：15
//6.	拼了吧：16
//7.	兑换商城：14
//8.	金币数106
//9.	Tips：18

//enum
//{
//    B_ACTIVITY = 15,
//    B_GO = 13,
//    B_PK = 16,
//    B_MARKET = 14,
//    I_HEAD = 8,
//    T_GOLDS = 106,
//    T_NICKNAME = 10,
//    T_LEVEL = 11,
//    LB_GRADE = 43,
//    B_OPTION = 288,
//    B_OPTION_MUSIC = 91,
//    B_OPTION_LOGOUT = 95,
//    B_OPTION_BACK = 97,
//};


#define light_rotate    45

void UI_Main::lightcall(Node *sender)
{
    Sprite* s = (Sprite*)sender;
    float rotate = light_rotate;
    if(s->getRotation() > 0)
        rotate = -rotate;
    float time = _light[s->getTag()].time;
    s->runAction(RepeatForever::create(Sequence::create(RotateTo::create(time, rotate), RotateTo::create(time, -rotate), NULL)));
}

void UI_Main::addLight()
{
    LightIni _lightIni[] =
    {
        {"CSres/Main/FreeRes/light_blue", Vec2(80, 130), -light_rotate},
        {"CSres/Main/FreeRes/light_green", Vec2(220, 130), light_rotate},
        {"CSres/Main/FreeRes/light_yellow", Vec2(470, 130), -light_rotate},
        {"CSres/Main/FreeRes/light_red", Vec2(650, 130), light_rotate},
    };
    
    Sprite* back = (Sprite*)rootNode->getChildByTag(740);
    for(int i = 0; i < sizeof(_lightIni)/sizeof(_lightIni[0]); i ++)
    {
        Sprite* light = Sprite::create(StringUtils::format("%s/light.png", _lightIni[i].path.c_str()));
        light->setTag(i);
        light->setAnchorPoint(Vec2(0.5, 0));
        light->setPosition(_lightIni[i].pos);
        back->addChild(light);
        
        ParticleSystemQuad* p = ParticleSystemQuad::create(StringUtils::format("%s/particle/light_p.plist", _lightIni[i].path.c_str()));
        p->setPosition(Vec2(light->getContentSize().width/2, 0));
        light->addChild(p);

        _light[i].time = ITools::getRand(4, 6);
        light->setRotation(ITools::getRand(-light_rotate, light_rotate));
        
        float time;
        if(i % 2 == 1)
            time = (_lightIni[i].rotate - light->getRotation())/light_rotate * (_light[i].time/2);
        else
            time = (light->getRotation() - _lightIni[i].rotate)/light_rotate * (_light[i].time/2);

        light->runAction(Sequence::create(RotateTo::create(time, _lightIni[i].rotate),
                                          CallFuncN::create(CC_CALLBACK_1(UI_Main::lightcall, this)), NULL));
    }
}


bool UI_Main::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    
//    testddd();
//    return true;        
    
//    GameIni::getInstance()->writeStringToTxt("ddddd谁送我的娃＊＊3来临的肥大kklldjiiii");
    
    rootNode = CSLoader::createNode("CSres/Main/Layer.csb");
    addChild(rootNode);

    int arr[] = {B_MARKET, I_HEAD, B_OPTION};
    for(int i = 0 ; i < sizeof(arr)/sizeof(arr[0]); i ++)
    {
        Widget* b = (Widget*)rootNode->getChildByTag(arr[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Main::eventCallback, this));
        if(arr[i] == I_HEAD)
        {
            ImageView* image = (ImageView*)b;
            image->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str()));
        }
    }
    
    Text* golds = (Text*)rootNode->getChildByTag(T_GOLDS);
    golds->setString(StringUtils::format("%d", _player->golds));
    
    Text* nickname = (Text*)rootNode->getChildByTag(T_NICKNAME);
    if (_player->nickname=="昵称") {
        nickname->setString("拼乐");
    }else{
    nickname->setString(_player->nickname);
    }
    Text* level = (Text*)rootNode->getChildByTag(T_LEVEL);
    level->setString(_player->rankname);
    
    LoadingBar* grade = (LoadingBar*)rootNode->getChildByTag(LB_GRADE);
    grade->setPercent(_player->getPecentScore()*100);
    
    //三个主界面
    ScrollCycleView *scroll = ScrollCycleView::create();
    addChild(scroll);
    
    
    Text *tipddd = (Text*)rootNode->getChildByTag(18);
    tipddd->setVisible(false);


    addLight();//主界面底下光

    option = CSLoader::createNode("CSres/Setup/option.csb");  //设置
    addChild(option, BroadcastZOrder + 1);//91 95
    option->setVisible(NO);
    
    //主界面一些按钮
    setMusic(UserDefault::getInstance()->getBoolForKey(music_key, YES));
    int arr1[] = {B_OPTION_MUSIC, B_OPTION_LOGOUT, B_OPTION_BACK};
    for(int i = 0 ;i < sizeof(arr1)/sizeof(arr1[0]); i ++)
    {
        Widget* b = (Widget*)option->getChildByTag(arr1[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Main::eventCallback, this));
    }
    
//    //引导 //圈39   箭头40 文本37
//    if(!UserDefault::getInstance()->getBoolForKey(guide_main_option, NO))
//    {
//        float x = 55, y = 1250;
//        string filename = "CSres/public/GuideLayer.csb";
//        openGuideLayer(75, Vec2(x, y), 0.3f);
//        Node* node = CSLoader::createNode(filename);
//        guideLayer->addChild(node);
//        
//        Node* arrow = node->getChildByTag(40);
//        arrow->setPosition(x + 130, y - 130);
//        
//        auto action = CSLoader::createTimeline(filename);
//        node->runAction(action);
//        action->gotoFrameAndPlay(0);
//    }
    //广播
    BroadcastUI *bbb = BroadcastUI::create();
    bbb->setPosition(120, 50);
    addChild(bbb);
    
    IPlayerData* iplayerData = M_GET_PLAYER;
    if(iplayerData->prizewinning > 0 && UserDefault::getInstance()->getBoolForKey(guide_main_choose, NO))
    {
        //showRewardGuide();//显示奖励指南
        iplayerData->prizewinning = 0;
    }
    //进场的三个新手引导主界面
//    if(!UserDefault::getInstance()->getBoolForKey(guide_main_choose, NO))
//    {
//        guidechose = GuideChoose::create();
//        addChild(guidechose);
//        guidechose->startGuide();
//        UserDefault::getInstance()->setBoolForKey(guide_main_choose, YES);
//        UserDefault::getInstance()->flush();
//    }
    
    IAudio::getInstance()->playBackground(music_bg_main, NO, YES);
    return true;
}

void UI_Main::setMusic(bool bl)
{
    Button* b = (Button*)option->getChildByTag(B_OPTION_MUSIC);
    ImageView* on  = (ImageView*)b->getChildByTag(92);
    ImageView* off = (ImageView*)b->getChildByTag(93);
    //243 244
    on->setVisible(!bl);
    off->setVisible(bl);
}

#define QUITAPP     10000

bool UI_Main::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    
    Notify_Compare(MES_ACTIVITY_GETLISTDONE)
    {
        popNetConnectingView();
        return YES;
    }
    IUILayer  * IUIlayer;
    if(name.compare(MES_ACTIVITY_GETIMAGEDONE) == 0 && IUIlayer->getis_switch()){
        UICtrl::getInstance()->gotoScene(UI_ACTIVITY);
        return YES;
    }
//    Notify_Compare(MES_ACTIVITY_GETIMAGEDONE)
//    {
//        UICtrl::getInstance()->gotoScene(UI_ACTIVITY);
//        return YES;
//    }
    Notify_Compare(MES_ROOM_GAMESTART)
    {
        UICtrl::getInstance()->gotoScene(UI_ROOM);
        return YES;
    }
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        if(option->isVisible())
        {
            Ref* sender = option->getChildByTag(B_OPTION_BACK);
            if(sender)
                eventCallback(sender, Widget::TouchEventType::ENDED);
            return YES;
        }
        
        if(!m_quitgame)
        {
            m_quitgame = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_quitapp), 1);
            m_quitgame->_args.code = QUITAPP;
        }
        else
        {
            m_quitgame->close();
            m_quitgame = nullptr;
        }
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_OK)
    {
        if(args.code == QUITAPP)
        {
            GameIni::getInstance()->exitApp();
            return YES;
        }
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_CANCEL)
    {
        if(args.code == QUITAPP)
        {
            m_quitgame = nullptr;
            return YES;
        }
        return YES;
    }
//    Notify_Compare(MES_GAMEINI_SCREENSHOOT_SUCCEED)
//    {
//        ShareSDKHelper::getInstance()->sendImage(GameIni::getInstance()->shareSdkInfo.title,
//                                                 GameIni::getInstance()->shareSdkInfo.description,
//                                                 args.content);
//        return YES;
//    }
    
    Notify_Compare(MES_PLAYER_SENDADVISE)
    {
        UIMessage::createMesBox("感谢您的支持！");
        return YES;
    }
    
    Notify_Compare(GUIDE_MAIN_CHOOSE_GAME)
    {
        guidechose->setVisible(false);
        guidechose->removeFromParent();
        guidechose = nullptr;
        return YES;
    }
    
    return true;
}

void UI_Main::setOptionType(int type)
{
    if(_option_type == type)
        return;
    _option_type = type;
    ImageView* yes_advise = (ImageView*)option->getChildByTag(B_OPTION_ADVISE)->getChildByTag(0);
    ImageView* yes_business = (ImageView*)option->getChildByTag(B_OPTION_BUSINESS)->getChildByTag(0);
    TextField* tf_advise = (TextField*)option->getChildByTag(TF_OPTION_ADVISE);
    TextField* tf_business = (TextField*)option->getChildByTag(TF_OPTION_BUSINESS);
    
    if(type == OptionType::ADVISE)
    {
        yes_advise->setVisible(YES);
        yes_business->setVisible(NO);
        tf_advise->setVisible(YES);
        tf_business->setVisible(NO);
    }
    else
    {
        yes_advise->setVisible(NO);
        yes_business->setVisible(YES);
        tf_advise->setVisible(NO);
        tf_business->setVisible(YES);
    }
}

void UI_Main::sendAdvise(int type)
{
    int tag = (type == OptionType::ADVISE) ? TF_OPTION_ADVISE : TF_OPTION_BUSINESS;
    TextField* tf = (TextField*)option->getChildByTag(tag);
    _player->http_request_sendAdvise(type, tf->getString());
}

void UI_Main::showRewardGuide()          //提示玩家提交中奖订单
{
    Node* node = CSLoader::createNode("CSres/public/RewardGuide.csb");
    node->setVisible(false);
    addChild(node, 10000);
    Button* btn = (Button*)node->getChildByTag(B_REWARD_GUIDE);
    btn->addTouchEventListener(CC_CALLBACK_2(UI_Main::eventCallback, this));
}


void UI_Main::eventCallback(Ref * pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;

    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    
    if(isGuide)
    {
        if(tag != I_HEAD)
            return;
    }
    
    if(tag != B_OPTION)
        IAudio::getInstance()->playEffect(music_ef_ok);
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case B_ACTIVITY://惊喜吧
        {
            IActivityData* a = M_GET_ACTIVITY;
            a->http_request_activityList();  
        }break;
        case B_GO://日常GO
        {
            IRoomData* room = M_GET_ROOM;
            room->enterGamePlay(GameMode::daily_simple);
        }break;
        case B_MARKET://商城兑换城
        {
            UICtrl::getInstance()->gotoScene(UI_MARKET);
        }break;
        case B_PK://拼了吧
        {
            UICtrl::getInstance()->gotoScene(UI_PK);
        }break;
        case I_HEAD://头像设置
        {
            UICtrl::getInstance()->gotoScene(UI_OPTION);
        }break;
        case B_OPTION:
        {
//            static int type = 1;
//            if(type % 2)
//            {
//                GameIni::getInstance()->screenShoot();
//            }
//            else
//            {
//                ShareSDKHelper::getInstance()->sendLink(GameIni::getInstance()->shareSdkInfo.title, GameIni::getInstance()->shareSdkInfo.description, GameIni::getInstance()->shareSdkInfo.webpageUrl);
//            }
//            type ++;
//            
//            break;
            IAudio::getInstance()->playEffect(music_ef_setting);
            option->setVisible(YES);
        }break;
        case B_OPTION_MUSIC:
        {
            bool music = UserDefault::getInstance()->getBoolForKey(music_key, YES);
            music = !music;
            IAudio::getInstance()->setVolume(music);
            UserDefault::getInstance()->setBoolForKey(music_key, music);
            UserDefault::getInstance()->flush();
            setMusic(music);
        }break;
        case B_OPTION_LOGOUT:
        {
            option->setVisible(NO);
            _player->loginOut();
            UICtrl::getInstance()->gotoScene(UI_LOGIN);
        }break;
        case B_OPTION_BACK:
        {
            option->setVisible(NO);
        }break;
        case B_OPTION_SUBMIT:
        {
            sendAdvise(_option_type);
        }break;
        case B_OPTION_ADVISE:
        {
            setOptionType(OptionType::ADVISE);
        }break;
        case B_OPTION_BUSINESS:
        {
            setOptionType(OptionType::BUSINESS);
        }break;
        case B_REWARD_GUIDE:
        {
            IActivityData* a = M_GET_ACTIVITY;
            a->http_request_activityList();
        }break;
        default:break;
    }
}




char* UI_Main::readFile()
{
    char* aa;
    string str = "";
    string path = GameIni::getInstance()->getLocalSavedImagePath("share_phone.png", GameIni::THUMBNAIL, YES);
    GAMELOG("##%s",path.c_str());
    FILE* readFile = fopen(path.c_str(), "rt");
    if(readFile)
    {
        ssize_t size = 0;
        while(!feof(readFile))
        {
            char c1 = fgetc(readFile);
            str += c1;
            size++;
        }
        GAMELOG("##str=%s",str.c_str());
        fclose(readFile);
    }
    return aa;
}

void UI_Main::testddd()
{
    TurntableSystem* ts = TurntableSystem::create();
    addChild(ts);    
}










