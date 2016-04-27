//
//  UI_Account.cpp
//  PinLe
//
//  Created by Kennedy on 15/7/27.
//
//

#include "UI_Account.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

USING_NS_CC_EXT;
using namespace ui;

Scene* UI_Account::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Account::create();
    
    //    auto layer = PersonCenterLayerEx::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Account::UI_Account()
{
    _webview=NULL;
    _player = M_GET_PLAYER;
    IMessageManager::getInstance()->attach(this, SYSTEM_KEYBACK);
}

UI_Account::~UI_Account()
{
    IMessageManager::getInstance()->detach(this, SYSTEM_KEYBACK);
}



enum
{
    B_BACK = 155,
    T_GOLDS = 218,                  //持有金币
//    T_GOLDS_TOTAL = 219,            //金币总收益
    T_GOLDS_TODAY = 220,            //今日金币收益
    T_GOLDS_FRIENDS = 221,          //好友金币收益
    T_POINTS = 222,                 //持有积分
//    T_POINTS_TOTAL = 223,           //积分总收益
    T_POINTS_TODAY = 224,           //今日积分收益
    T_POINTS_FRIENDS = 225,         //好友积分收益
    
    T_GOODS_COUNT = 400,
    T_LOTTERY_COUNT = 401,

    B_GOODS_URL = 130,
    B_LOTTERY_URL = 133,
//    T_GRADE = 172,                  
//    T_GRADENEXT = 173,
//    LB_EXP = 180,
};


void UI_Account::onEnter()
{
    IUILayer::onEnter();
    if(_player->grade >= 5)
        sv_grade->jumpToPercentHorizontal(80);
}


bool UI_Account::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    rootNode = CSLoader::createNode("CSres/Setup/account.csb");
    addChild(rootNode);
    
    
    int arr[] = {B_BACK};
    for(int i = 0; i < sizeof(arr)/sizeof(arr[1]); i ++)
    {
        Widget * widget = (Widget*)rootNode->getChildByTag(arr[i]);
        if(widget && widget->isTouchEnabled())
        {
            widget->addTouchEventListener(CC_CALLBACK_2(UI_Account::eventCallback, this));
        }
    }
    
    Text* text;
    ImageView* image;
    ui::ScrollView* scrollView;
    
    scrollView = (ui::ScrollView*)rootNode->getChildByTag(135);
    
//============金币账户=======
    text = (Text*)scrollView->getChildByTag(T_GOLDS);
    text->setString(StringUtils::format("%d", _player->golds));

//    text = (Text*)scrollView->getChildByTag(T_GOLDS_TOTAL);
//    text->setString(StringUtils::format("%d", _player->totalgolds));
    
    text = (Text*)scrollView->getChildByTag(T_GOLDS_TODAY);
    text->setString(StringUtils::format("%d", _player->todaygolds));
    
    text = (Text*)scrollView->getChildByTag(T_GOLDS_FRIENDS);
    text->setString(StringUtils::format("%d", _player->friendsgolds));
//============积分账户=======
    text = (Text*)scrollView->getChildByTag(T_POINTS);
    text->setString(StringUtils::format("%d", _player->points));
    
//    text = (Text*)scrollView->getChildByTag(T_POINTS_TOTAL);
//    text->setString(StringUtils::format("%d", _player->totalpoints));
    
    text = (Text*)scrollView->getChildByTag(T_POINTS_TODAY);
    text->setString(StringUtils::format("%d", _player->todaypoints));
    
    text = (Text*)scrollView->getChildByTag(T_POINTS_FRIENDS);
    text->setString(StringUtils::format("%d", _player->friendspoints));
    
    text = (Text*)scrollView->getChildByTag(T_GOODS_COUNT);
    text->setString(StringUtils::format("%d", _player->goodsnum));
    
    text = (Text*)scrollView->getChildByTag(T_LOTTERY_COUNT);
    text->setString(StringUtils::format("%d", _player->lotterynum));
    
    int arr1[] = {B_GOODS_URL, B_LOTTERY_URL};
    for(int i = 0; i < sizeof(arr1)/sizeof(arr1[0]); i ++)
    {
        Widget * widget = (Widget*)scrollView->getChildByTag(arr1[i]);
        if(widget && widget->isTouchEnabled())
        {
            widget->addTouchEventListener(CC_CALLBACK_2(UI_Account::eventCallback, this));
        }
    }
    
    sv_grade = (ui::ScrollView*)scrollView->getChildByTag(183);
    
    image = (ImageView*)sv_grade->getChildByTag(216);
    ImageView* headicon = (ImageView*)image->getChildByTag(217);
    headicon->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str()));
    

    Button* b = (Button*)sv_grade->getChildByTag(200 + _player->grade);
    float x;
    if(_player->rankname.compare(_player->nextrankname) != 0)
        x = b->getPositionX() + 109 * _player->getPecentScore();
    else
        x = b->getPositionX();
    image->setPositionX(x);
    
    
    return true;
}


void UI_Account::eventCallback(Ref * pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case B_BACK:
        {
            UICtrl::getInstance()->gotoScene(UI_OPTION);
        }break;
        case B_GOODS_URL:
        {
//            string url = StringUtils::format("%sloginname=%s#tabs-1",
//                                             GameIni::getInstance()->bag_url.c_str(), _player->userid.c_str());
//            
//            string surl = StringUtils::format("%sid=%s&md5=%s&w=%d&h=%d", GameIni::getInstance()->market_ip.c_str(),
//                                              p->userid.c_str(), p->MiYao_key.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT);
            
            string url = StringUtils::format("%sid=%s&md5=%s#tabs-1",
                                             GameIni::getInstance()->bag_url.c_str(), _player->userid.c_str(),
                                             _player->MiYao_key.c_str());
            GAMELOG("url = %s", url.c_str());
            //Application::getInstance()->openURL(url);
            
            if(_webview)
            {
                _webview->removeFromParentAndCleanup(YES);
                _webview = NULL;
            }
            Size winSize = Director::getInstance()->getWinSize();
            float h = 50;
            //    float h = 0;
            _webview = cocos2d::experimental::ui::WebView::create();
            _webview->setContentSize(Size(winSize.width, winSize.height - h*2));
            _webview->loadURL(url);
//            _webview->setOnShouldStartLoading(CC_CALLBACK_2(UI_Market::onWebViewShouldStartLoading, this));
//            _webview->setOnDidFinishLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFinishLoading, this));
//            _webview->setOnDidFailLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFailLoading, this));
            _webview->setPosition(Vec2(winSize.width/2, winSize.height/2 - h));
            //setScalesPageToFit 调整WebView内容以适应所设置内容的大小  兑换商城
            _webview->setScalesPageToFit(true);
            _webview->setFocused(true);
            addChild(_webview);
            
        }break;
        case B_LOTTERY_URL:
        {
//            string url = StringUtils::format("%sloginname=%s#tabs-2",
//                                             GameIni::getInstance()->bag_url.c_str(), _player->userid.c_str());
            string url = StringUtils::format("%sid=%s&md5=%s#tabs-2",
                                             GameIni::getInstance()->bag_url.c_str(), _player->userid.c_str(),
                                             _player->MiYao_key.c_str());
            GAMELOG("url = %s", url.c_str());
           // Application::getInstance()->openURL(url);
            if(_webview)
            {
                _webview->removeFromParentAndCleanup(YES);
                _webview = NULL;
            }
            Size winSize = Director::getInstance()->getWinSize();
            float h = 50;
            //    float h = 0;
            _webview = cocos2d::experimental::ui::WebView::create();
            _webview->setContentSize(Size(winSize.width, winSize.height - h*2));
            _webview->loadURL(url);
            //            _webview->setOnShouldStartLoading(CC_CALLBACK_2(UI_Market::onWebViewShouldStartLoading, this));
            //            _webview->setOnDidFinishLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFinishLoading, this));
            //            _webview->setOnDidFailLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFailLoading, this));
            _webview->setPosition(Vec2(winSize.width/2, winSize.height/2 - h));
            //setScalesPageToFit 调整WebView内容以适应所设置内容的大小  兑换商城
            _webview->setScalesPageToFit(true);
            _webview->setFocused(true);
            addChild(_webview);

        }break;
        default:break;
    }
}

bool UI_Account::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        Ref* sender = nullptr;
//        if(_imageShow->isVisible())
//            sender = _imageShow->getChildByTag(B_IMAGESHOW_BACK);
//        else
        sender = rootNode->getChildByTag(B_BACK);
        if(sender)
            eventCallback(sender, Widget::TouchEventType::ENDED);
        return YES;
    }
    
    
    return true;
}








