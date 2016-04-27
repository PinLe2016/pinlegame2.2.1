//
//  UI_Logo.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/11.
//
//

#include "UI_Logo.h"


Scene* UI_Logo::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Logo::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Logo::UI_Logo()
{
    
    
    
    yemianDian = nullptr;
    _player = M_GET_PLAYER;
    IUILayer::attachNetworkMes();
    getgameurltype = VERSIONBACK::no_back;
    error_time = 0;
    diango = false;
    
    IMessageManager::getInstance()->attach(this, MES_PLAYER_UPDATEVERSION);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_LOGIN);
}

UI_Logo::~UI_Logo()
{
    if(yemianDian)
    {
        yemianDian->removeAllObjects();
        yemianDian->release();
    }
    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_PLAYER_UPDATEVERSION);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_LOGIN);
}
bool UI_Logo::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void UI_Logo::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    Point touchPoint = touch->getLocation();
    pEraser->setPosition(touchPoint);
    
    // 设置混合模式
    BlendFunc blendFunc = { GL_ONE, GL_ZERO };
    pEraser->setBlendFunc(blendFunc);
    
    // 将橡皮擦的像素渲染到画布上，与原来的像素进行混合
    pRTex->begin();
    pEraser->visit();
    pRTex->end();
}
bool UI_Logo::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    //Application::getInstance()->openURL("http://www.baidu.com");
//    auto listener = EventListenerTouchOneByOne::create();  //创建一个单点触摸事件 :EventListenerTouchAllAtOnce 为多点
//    //设置这些事件的的回调方法
//    listener->onTouchBegan = CC_CALLBACK_2(UI_Logo::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(UI_Logo::onTouchMoved, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    //...
//    // 显示背景图片，方便查看“擦除到透明”效果
//    Point center = Vec2(400,500);
//    Sprite* pSprite = Sprite::create("CSres/Activities/ActivitiesUI/1activity_back.png");
//    pSprite->setPosition(center);
//    this->addChild(pSprite, 0);
//    
//    // 创建一个橡皮擦，注意颜色的设置是全透明黑色
//    pEraser = DrawNode::create();
//    pEraser->drawSolidRect(Vec2(0, 0), Vec2(80,80), Color4F(255, 0, 0, 0));
//    pEraser->retain();
//    
//    // 创建画布，并显示它
//    pRTex = RenderTexture::create(800,1000);
//    pRTex->setPosition(Vec2(400,500));
//    this->addChild(pRTex, 10);
//    
//    // 创建被擦除的内容，将其渲染到画布上
//    Sprite* pBg = Sprite::create("CSres/Activities/ActivitiesUI/reward_testpicture.jpg");
//    pBg->setAnchorPoint(Vec2(0.5,0.5));
//    pBg->setPosition(center);
//    pRTex->begin();
//    pBg->visit();
//    pRTex->end();
    
    setColor(Color3B::WHITE);
    
    //加入动画
    string filepath = "CSres/login/logo.csb";
    Node* armimationNode = CSLoader::createNode(filepath);
    this->addChild(armimationNode);
    auto startedd = CSLoader::createTimeline(filepath);
    armimationNode->runAction(startedd);
    startedd->gotoFrameAndPlay(0, NO);
    startedd->setLastFrameCallFunc(CC_CALLBACK_0(UI_Logo::actionBack, this));
    
    http_GetGameUrl();
    
    pinle = (Sprite*)armimationNode->getChildByTag(772);
    pinle->setVisible(NO);
    
    IAudio::getInstance()->playEffect(music_ef_logo);
    
       return true;
}


#define mes_updatetag   100

bool UI_Logo::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    Notify_Compare(MES_PLAYER_UPDATEVERSION)
    {
        
        if(getgameurltype == VERSIONBACK::no_back)
        {
            schedule(schedule_selector(UI_Logo::updateCheckUrlBack), 1);
            return true;
        }
        if(getgameurltype == VERSIONBACK::error_infor)
        {
            UIMessage* m = NULL;
            m = UIMessage::createMesBox("获取游戏数据失败！");
            return true;
        }
        
        _args.code = args.code;
        _args.code2 = args.code2;
        _args.code3 = args.code3;
        _args.content = args.content;
        
        
        _player->getCoordinate();
        checkVersionOver();
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_OK)
    {
        if(args.err_code > 0)
        {
            UICtrl::getInstance()->gotoScene(UI_LOGIN);
            return YES;
        }

        if(args.code == mes_updatetag)
        {
            Application::getInstance()->openURL(args.content);
        }
        return YES;
    }
    
    Notify_Compare(MESSAGEBOX_CANCEL)
    {
        if(args.retry)
        {
            GameIni::getInstance()->exitApp();
            return YES;
        }
        if(args.code == mes_updatetag)
        {
            if(_player->setDataByLocal())
                _player->http_request_login();
            else
                UICtrl::getInstance()->gotoScene(UI_LOGIN);
        }
        return YES;
    }
    Notify_Compare(MES_PLAYER_LOGIN)
    {
        GAMELOG("denglu------------------22");
        UICtrl::getInstance()->gotoScene(UI_MAIN);
        return YES;
    }
    
    return true;
}




void UI_Logo::actionBack()
{
    pinle->setVisible(YES);
    pinle->setOpacity(0);
    pinle->runAction(FadeIn::create(1.0f));
    
    scheduleOnce(schedule_selector(UI_Logo::checkVersion), 3.0f);
    
}

void UI_Logo::checkVersion(float dt)
{
    _player->http_request_checkVersion();
}


void UI_Logo::guideLayer()
{
    GAMELOG("****UI_Logo::guideLaye");
//    _player->getCoordinate();
    PageView * pageView = PageView::create();
    pageView->setContentSize(Size(SCREEN_WIDTH, SCREEN_HEIGHT));
    pageView->setAnchorPoint(Vec2(0.5, 0.5f));
    pageView->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    pageView->removeAllPages();
    pageView->setTouchEnabled(true);
    //设置自定义移动距离
    pageView->setCustomScrollThreshold(10);
    pageView->addEventListener(CC_CALLBACK_2(UI_Logo::pageViewEvent, this));
    pageView->setName("pageView");
    addChild(pageView);
    UserDefault::getInstance()->setBoolForKey("showstartlayer", false);
    UserDefault::getInstance()->flush();
    yemianDian = __Array::create();
    yemianDian->retain();
    int QiShiX = 400-4*50/2;
    for (int i = 0; i < 4; i++)
    {
        __String * m_pString = __String::createWithFormat("CSres/logo/FreeRes/QDY_%d.jpg",i+1);
        Layout * m_pLayout = Layout::create();
        //m_pLayout->setContentSize(Size(1000,1400.0f));
        Sprite * m_pSprite = Sprite::create(m_pString->getCString());
        m_pSprite->setAnchorPoint(Vec2(0, 0));
        m_pLayout->cocos2d::Node::addChild(m_pSprite);
        if (i == 3)
        {
            Button * m_pButton = Button::create("CSres/logo/FreeRes/chengshi_1.png",
                                                "CSres/logo/FreeRes/chengshi_1.png",
                                                "CSres/logo/FreeRes/chengshi_1.png");
            m_pButton->setPosition(Vec2(m_pSprite->getContentSize().width/2, 70));
            m_pButton->addTouchEventListener(CC_CALLBACK_2(UI_Logo::fanhui,this));
            m_pButton->setTitleFontSize(36);
            m_pButton->setTitleText("Go~");
            m_pLayout->addChild(m_pButton);
        }
        pageView->addPage(m_pLayout);
        //点点
        Sprite* tempDian = Sprite::create("CSres/logo/FreeRes/Lyedi.png");
        tempDian->setPosition(Vec2(QiShiX+i*50,140));
        addChild(tempDian);
        yemianDian->addObject(tempDian);
    }
    
    DangQianDian = Sprite::create("CSres/logo/FreeRes/Vbaidian.png");
    addChild(DangQianDian);
    if (yemianDian->count() > 0)
    {
        temppos = (Sprite*)yemianDian->getObjectAtIndex(0);
        DangQianDian->setPosition(temppos->getPosition());
    }
}


void  UI_Logo::fanhui(Ref * obj,Widget::TouchEventType type){
    if (type == Widget::TouchEventType::ENDED)
    {
        
        if(getgameurltype == VERSIONBACK::no_back)
        {
            diango = true;
            schedule(schedule_selector(UI_Logo::updateCheckUrlBack), 1);
            return;
        }
        if(getgameurltype == VERSIONBACK::error_infor)
        {
            UIMessage* m = NULL;
            m = UIMessage::createMesBox("获取游戏数据失败！");
            return;
        }
        
        
        
        
        IAudio::getInstance()->playEffect(music_ef_ok);
        UICtrl::getInstance()->gotoScene(UI_LOGIN);
    }
}
void UI_Logo::pageViewEvent(Ref *pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            temppos = (Sprite*)yemianDian->getObjectAtIndex(pageView->getCurPageIndex());
            DangQianDian->runAction(Sequence::create(FadeOut::create(0.2),
                                                     CallFuncN::create(CC_CALLBACK_1(UI_Logo::touch_GaiBiaPoist,this)),
                                                     FadeIn::create(0.2),
                                                     NULL));
        }break;
        default:
            break;
    }
}
void UI_Logo::touch_GaiBiaPoist(Node * _node){
    DangQianDian->setPosition(temppos->getPosition());
}


void UI_Logo::http_GetGameUrl()
{
    myHttpWork *httpWork = myHttpWork::createHttp();
    GAMELOG(GETSERVERURL_URL, GETGAMEURL_URL,APP_VERSION);
    httpWork->sendRequest(StringUtils::format(GETSERVERURL_URL, GETGAMEURL_URL, APP_VERSION), 30);
    httpWork->_httpCB = CC_CALLBACK_1(UI_Logo::http_backGameUrl, this);
    
    
//    httpWork->_args.content = path;
//    httpWork->_httpCB3 = CC_CALLBACK_2(RoomData::downLoadImageBack, this);
    
}
void UI_Logo::http_backGameUrl(string str)
{
    getgameurltype = VERSIONBACK::error_infor;
    GAMELOG("http_backGameUrl===%s",str.c_str());
    if(str.empty())
    {
        return;
    }
    
    if(str.compare("error") == 0)
    {
        return;
    }
    
    GameIni::getInstance()->gamePlayerUrl = str;
    GameIni::getInstance()->root_ip = str;
    getgameurltype = VERSIONBACK::true_infor;
    GAMELOG("GameIni::getInstance()->gamePlayerUrl == %s", GameIni::getInstance()->gamePlayerUrl.c_str());
    return;
    
//    rapidjson::Document d;
//    d.Parse<0>(str.c_str());
//    
//    if(d.HasParseError())
//    {
//        return;
//    }
//    if(d.HasMember("getgameurl"))
//    {
//        GameIni::getInstance()->gamePlayerUrl = d["getgameurl"].GetString();
//        getgameurltype = VERSIONBACK::true_infor;
//    }
}

void UI_Logo::checkVersionOver(bool b)
{
    
    if(b)
    {
        IAudio::getInstance()->playEffect(music_ef_ok);
        UICtrl::getInstance()->gotoScene(UI_LOGIN);
        return;
    }
    
    
    if(_args.code == 0)//更新开关
    {
        //            _player->getCoordinate();
        if(UserDefault::getInstance()->getBoolForKey("showstartlayer",true))
        {
            guideLayer();
        }
        else if(_player->setDataByLocal())
        {
            _player->http_request_login();
        }
        else
        {
            UICtrl::getInstance()->gotoScene(UI_LOGIN);
        }
    }
    else
    {
        if(_args.code2 == 1)//是否需要更新
        {
            UIMessage* m = NULL;
            if(_args.code3 == 1)//是否强制更新
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
                m->_args.content = _args.content;
            }
        }
        else
        {
            //                _player->getCoordinate();
            if(UserDefault::getInstance()->getBoolForKey("showstartlayer", true))
            {
                guideLayer();
            }
            else
            {
                if(_player->setDataByLocal())
                {
                    _player->http_request_login();
                }
                else
                {
                    UICtrl::getInstance()->gotoScene(UI_LOGIN);
                }
            }
        }
    }
}



void UI_Logo::updateCheckUrlBack(float ft)
{
    error_time++;
    if(error_time > 20)
    {
        unschedule(schedule_selector(UI_Logo::updateCheckUrlBack));
        error_time = 0;
        
        
        UIMessage* m = NULL;
        m = UIMessage::createMesBox("获取游戏数据失败");
        
        
    }
    
    if(getgameurltype == VERSIONBACK::true_infor)
    {
        unschedule(schedule_selector(UI_Logo::updateCheckUrlBack));
        
        if(diango)
            checkVersionOver(true);
        else
        {
            checkVersionOver();
        }
    }
}





