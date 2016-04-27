//
//  UI_Option.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/8.
//
//

#include "UI_Option.h"
#include "../../UIcontroller/UIcontroller.h"
#include "PersonCenterLayerEx.h"
#include "../config.h"
#include "../../PluginHelper.h"

Scene* UI_Option::createScene()
{
    gamelog("----UI_Option::createScene---");
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Option::create();

//    auto layer = PersonCenterLayerEx::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Option::UI_Option()
{
    rootNode = nullptr;
    personCenter = NULL;
    _player = M_GET_PLAYER;
    message = NULL;
    notBack = false;
    gamelog("----UI_Option::UI_Option---");
    
    attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_OPTION_CLOSE);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_GETFRIENDINFO);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_GETACCOUNT);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_UPDATEVERSION);
    IMessageManager::getInstance()->attach(this, MES_POKEDEX_GOTOPOKEDEX);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_SENDADVISE);
    
}

UI_Option::~UI_Option()
{
    gamelog("----UI_Option::~UI_Option---");
    
    detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_OPTION_CLOSE);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_GETFRIENDINFO);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_GETACCOUNT);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_UPDATEVERSION);
    IMessageManager::getInstance()->detach(this, MES_POKEDEX_GOTOPOKEDEX);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_SENDADVISE);
}

enum
{
    B_INFO = 38,//个人信息
    B_MUSIC = 96,
    B_ACCOUNT = 197,
    B_FRIEND = 56,
    B_ABOUT = 169,
    B_LOGOUT = 181,
    B_BACK = 187,
    B_POKEDEX = 237,
    B_ABOUT_BACK = 451,
    B_ABOUT_TEL = 148,
    B_ABOUT_COMMENT = 256,
    B_ABOUT_WEBSITE = 149,
    B_ABOUT_HEZUO = 65,
    T_ABOUT_ADDRESS = 43,
    P_ANDROID = 178,
    P_IOS = 324,
    T_CHECKUPDADE = 177,
    T_VERSION = 176,
    B_HEZUO_BACK = 29,
    B_HZ_CHOOSE1 = 302,
    B_HZ_CHOOSE2 = 303,
    B_HZ_SUBMIT = 301,
    T_HZ_JIANYI = 306,
    T_HZ_HEZUO = 309,
};

//comment

void UI_Option::setHeadImage()
{
//    string path = StringUtils::format("res/newUI/resUI/Information/Head/%s",
//                                      ITools::Intercept_string(_player->uHeadIndex, "/").c_str());
//
    if(rootNode)
    {
        ImageView* image = (ImageView*)rootNode->getChildByTag(241);
        if(image)
            image->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str()));
    }

}

void UI_Option::addline(Text* t)
{
    if(!t)
        return;
    DrawNode* drawLine = DrawNode::create();
    drawLine->drawLine(Vec2(0,0), Vec2(t->getContentSize().width,0),
                       Color4F(t->getTextColor().r/255.f, t->getTextColor().g/255.f, t->getTextColor().b/255.f, 1));
    t->addChild(drawLine);
}

bool UI_Option::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    
    if(UICtrl::getInstance()->needModifyInfo())
    {
        openPersonInfo(YES);
        return YES;
    }

    rootNode = CSLoader::createNode("CSres/Setup/GeRenZhongXinScene.csb");
    addChild(rootNode);
    
    setHeadImage();
    
    Text* t;
    //当前版本
    t = (Text*)rootNode->getChildByTag(974);
    t->setString(StringUtils::format("v%s", APP_VERSION));
    
    int arr[] = {B_INFO, B_MUSIC, B_FRIEND, B_ABOUT, B_ACCOUNT, B_LOGOUT, B_BACK, B_POKEDEX};
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i ++)
    {
        Button* b = (Button*)rootNode->getChildByTag(arr[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Option::eventCallback, this));
    }
    setMusic(UserDefault::getInstance()->getBoolForKey(music_key, YES));
//关于拼乐
    initAboutPinle();
////////////////////
    if(!UserDefault::getInstance()->getBoolForKey(guide_main_option, NO))
    {
        UserDefault::getInstance()->setBoolForKey(guide_main_option, YES);
        UserDefault::getInstance()->flush();
        openPersonInfo(YES);
    }
    
    
    return true;
}


void UI_Option::initAboutPinle()
{
    Text* t;
    company = CSLoader::createNode("CSres/Setup/GuangGaoScene.csb");
    company->setVisible(NO);
    addChild(company);
    
    t = (Text*)company->getChildByTag(T_ABOUT_ADDRESS);//公司地址
    t->setString(GameIni::getInstance()->officialadd);
    
    int arr1[] = {B_ABOUT_BACK, B_ABOUT_TEL, B_ABOUT_WEBSITE, B_ABOUT_HEZUO};
    for(int i = 0; i < sizeof(arr1)/sizeof(arr1[0]); i ++)
    {
        Widget* b = (Button*)company->getChildByTag(arr1[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Option::eventCallback, this));
        if(arr1[i] == B_ABOUT_TEL)
        {
            Text* t = (Text*)b;
            t->setString(GameIni::getInstance()->officialtel);
            addline(t);
        }
        else if(arr1[i] == B_ABOUT_WEBSITE)
        {
            Text* t = (Text*)b;
            t->setString(GameIni::getInstance()->officialwebsite);
            addline(t);
        }
    }
    
    panel_android = company->getChildByTag(P_ANDROID);
    int arr2[] = {T_CHECKUPDADE};
    for(int i = 0; i < sizeof(arr2)/sizeof(arr2[0]); i ++)
    {
        Widget* b = (Widget*)panel_android->getChildByTag(arr2[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Option::eventCallback, this));
        addline((Text*)b);
    }
    panel_ios = company->getChildByTag(P_IOS);
    int arr3[] = {B_ABOUT_COMMENT};
    for(int i = 0; i < sizeof(arr3)/sizeof(arr3[0]); i ++)
    {
        Widget* b = (Widget*)panel_ios->getChildByTag(arr3[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Option::eventCallback, this));
        addline((Text*)b);
    }
    
    t = (Text*)panel_android->getChildByTag(T_VERSION);
    t->setString(StringUtils::format("%s", APP_VERSION));
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        panel_ios->setVisible(YES);
    }
#else
    {
        panel_android->setVisible(YES);
    }
#endif
    
//建议合作
    chooseType = jianyifankui;
    company_hezuo = company->getChildByTag(25);

    int arr4[] = {B_HEZUO_BACK, B_HZ_CHOOSE1,B_HZ_CHOOSE2,B_HZ_SUBMIT};
    for(int i = 0; i < sizeof(arr4) / sizeof(arr4[0]); i++)
    {
        Button* b4 = (Button*)company_hezuo->getChildByTag(arr4[i]);
        b4->addTouchEventListener(CC_CALLBACK_2(UI_Option::eventCallback, this));
        if (i==3) {
            b4_tijiao=b4;
            //company_hezuo->setVisible(false);
            b4_tijiao->setTitleText("提交");
        }
    }
    
    
    
}

void UI_Option::chooseBusiness(int type)      //选择合作类型
{
    chooseType = type;
    TextField* tf;
    Button* bt;
    Sprite* sp;
    bool bl1;
    switch (chooseType) {
        case jianyifankui:
        {
            bl1 = true;
        }break;
        case shangwuhezuo:
        {
            bl1 = false;
        }break;
        default:
            break;
    }
    
    tf = (TextField*)company_hezuo->getChildByTag(T_HZ_JIANYI);
    tf->setVisible(bl1);
    tf = (TextField*)company_hezuo->getChildByTag(T_HZ_HEZUO);
    tf->setVisible(!bl1);
    
    bt = (Button*)company_hezuo->getChildByTag(B_HZ_CHOOSE1);
    sp = (Sprite*)bt->getChildByTag(307);
    sp->setVisible(bl1);
    
    bt = (Button*)company_hezuo->getChildByTag(B_HZ_CHOOSE2);
    sp = (Sprite*)bt->getChildByTag(308);
    sp->setVisible(!bl1);
}

void UI_Option::sendAdvise(int type)      //提交建议合作
{
    notBack = false;
    int tag = (type == jianyifankui) ? T_HZ_JIANYI : T_HZ_HEZUO;
    TextField* tf = (TextField*)company_hezuo->getChildByTag(tag);
    if(tf->getString().empty())
    {
        UIMessage::createMesBox("输入内容不能为空！");
        return;
    }
    _player->http_request_sendAdvise(type, tf->getString());
}

void UI_Option::setMusic(bool bl)
{
    Button* b = (Button*)rootNode->getChildByTag(B_MUSIC);
    ImageView* on  = (ImageView*)b->getChildByTag(243);
    ImageView* off = (ImageView*)b->getChildByTag(244);
    //243 244
    on->setVisible(!bl);
    off->setVisible(bl);
}




void UI_Option::openPersonInfo(bool bOpen)
{
    if(bOpen)
    {
        if(!personCenter)
        {
            personCenter = PersonCenterLayerEx::create();
            addChild(personCenter);
        }
    }
    else
    {
        if(personCenter)
        {
            personCenter->removeFromParentAndCleanup(YES);
            personCenter = nullptr;
        }
    }
}

void UI_Option::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case B_INFO://个人信息
        {        
            openPersonInfo(YES);
        }break;
        case B_ACCOUNT://个人账户
        {
//            UICtrl::getInstance()->gotoScene(UI_ACCOUNT);
            _player->http_request_getAccount();
        }break;
        case B_MUSIC://音乐快关
        {
            bool music = UserDefault::getInstance()->getBoolForKey(music_key, YES);
            music = !music;
            IAudio::getInstance()->setVolume(music);
            UserDefault::getInstance()->setBoolForKey(music_key, music);
            UserDefault::getInstance()->flush();
            setMusic(music);
        }break;
        case B_ABOUT://关于拼乐
        {
              b4_tijiao->setTitleText("提交");
            company->setVisible(YES); 
        }break;
        case B_ABOUT_HEZUO:     //建议合作
        {
            company_hezuo->setVisible(true);
            notBack = true;
            
            TextField* tf;
            tf = (TextField*)company_hezuo->getChildByTag(T_HZ_JIANYI);
            tf->setString("");
            tf = (TextField*)company_hezuo->getChildByTag(T_HZ_HEZUO);
            tf->setString("");
            
        }break;
        case B_HZ_CHOOSE1:      //建议反馈选择按钮
        {
            chooseBusiness(jianyifankui);
        }break;
        case B_HZ_CHOOSE2:      //商务合作选择按钮
        {
            chooseBusiness(shangwuhezuo);
        }break;
        case B_HZ_SUBMIT:       //提交按钮
        {
                sendAdvise(chooseType);
                b4_tijiao->setTitleText("返回");
            
        }break;
        case B_HEZUO_BACK:
        {
            b4_tijiao->setTitleText("提交");
            int tag = (chooseType == jianyifankui) ? T_HZ_JIANYI : T_HZ_HEZUO;
            TextField* tf = (TextField*)company_hezuo->getChildByTag(tag);
            if(!tf->getString().empty() && notBack)
            {
                if(!message)
                {
                    message = UIMessage::createMesBox("是否提交建议合作内容", 1);
                    message->_args.content = "jianyihezuoneirongtijiao";
                    break;
                }
            }
            
            
            company_hezuo->setVisible(false);
        }break;
        case B_FRIEND://好友邀请
        {
            _player->http_request_getFriendsInfo();
        }break;
        case B_POKEDEX://我的图鉴
        {
            popNetConnectingView();
            IMessageManager::getInstance()->postNotification(MES_POKEDEX_GOTOPOKEDEX);
        }break;
        case B_LOGOUT://注销登陆
        {
            IPlayerData* p = M_GET_PLAYER;
            p->loginOut();
            UICtrl::getInstance()->gotoScene(UI_LOGIN);
        }break;
        case B_BACK:// 返回
        {
            if(!personCenter)
                UICtrl::getInstance()->gotoScene(UI_MAIN);
        }break;
        case B_ABOUT_BACK: //关于拼乐返回
        {
            company->setVisible(NO);
        }break;
        case B_ABOUT_TEL://电话
        {
            _player->phonecall(GameIni::getInstance()->officialtel);
        }break;
        case B_ABOUT_WEBSITE://官网
        {
            Application::getInstance()->openURL(GameIni::getInstance()->officialwebsite);
        }break;
        case B_ABOUT_COMMENT:
        {
            _player->comment(GameIni::getInstance()->appleAppID);
        }break;
        case T_CHECKUPDADE:
        {
            _player->http_request_checkVersion();
        }break;
        default:    break;
    }
}

void UI_Option::enterPokedex(float dt)
{
    UICtrl::getInstance()->gotoScene(UI_POKEDEX);
}

#define mes_updatetag   100

bool UI_Option::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        if(personCenter)
            return YES;
        
        Ref* sender = NULL;
        if(company_hezuo->isVisible())
            sender = company_hezuo->getChildByTag(B_HEZUO_BACK);
        else if(company->isVisible())
            sender = company->getChildByTag(451);
        else if(!personCenter)
            sender = rootNode->getChildByTag(187);
        
        if(sender)
            eventCallback(sender, Widget::TouchEventType::ENDED);
        return YES;
    }
    Notify_Compare(MES_OPTION_CLOSE)
    {
        openPersonInfo(NO);
        setHeadImage();
        return YES;
    }
    Notify_Compare(MES_POKEDEX_GOTOPOKEDEX)
    {
        scheduleOnce(schedule_selector(UI_Option::enterPokedex), 0.0f);
//        UICtrl::getInstance()->gotoScene(UI_POKEDEX);
        return YES;
    }
    
    Notify_Compare(MES_PLAYER_GETFRIENDINFO)
    {
        UICtrl::getInstance()->gotoScene(UI_FRIEND);
        return YES;
    }
    Notify_Compare(MES_PLAYER_GETACCOUNT)
    {
        UICtrl::getInstance()->gotoScene(UI_ACCOUNT);
        return YES;
    }
    
    Notify_Compare(MES_PLAYER_UPDATEVERSION)
    {
        if(args.code == 0)//更新开关
        {
            UIMessage::createMesBox("您的版本是最新版本。");
        }
        else
        {
            if(args.code2 == 1)//是否需要更新
            {
                UIMessage* m = NULL;
                if(args.code3 == 1)//是否强制更新
                {
                    m = UIMessage::createMesBox("发现新版本请更新");
                }
                else
                {
                    m = UIMessage::createMesBox("发现新版本是否需要更新", 1);
                }
                if(m)
                {
                    m->_args.code = mes_updatetag;
                    m->_args.content = args.content;
                }
            }
            else
            {
                UIMessage::createMesBox("您的版本是最新版本。");
            }
        }
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_OK)
    {
        if (!company) {
            //company->setVisible(false);
        }
        //company->setVisible(false);
        if(args.code == mes_updatetag)
        {
            Application::getInstance()->openURL(args.content);
            return YES;
        }
        if(args.content.compare("jianyihezuoneirongtijiao") == 0)
        {
            sendAdvise(chooseType);
            message = NULL;
            return YES;
        }
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_CANCEL)
    {
        company_hezuo->setVisible(false);
        return YES;
    }
    
    Notify_Compare(MES_PLAYER_SENDADVISE)
    {
        UIMessage::createMesBox("感谢您的支持！");
    }
    
    return true;
}



void UI_Option::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
     if(!personCenter)
    {
        IMessageManager::getInstance()->postNotification(SYSTEM_KEYBACK);
    }
}









