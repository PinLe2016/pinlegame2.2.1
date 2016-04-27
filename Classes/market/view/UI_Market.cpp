//
//  UI_Market.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/11.
//
//

#include "UI_Market.h"
//#include "../../gameroom/IRoomData.h"
//安卓平台
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include <jni.h>
//#include "platform/android/jni/JniHelper.h"
//#endif


Scene* UI_Market::createScene()
{
    gamelog("----UI_Option::createScene---");
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Market::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Market::UI_Market()
{
    gamelog("----UI_Market::UI_Market---");
    IUILayer::attachNetworkMes();
    _webview = NULL;
    //    IMessageManager::getInstance()->attach(this, SYSTEM_ENTERFOREGROUND);
}

UI_Market::~UI_Market()
{
    IUILayer::detachNetworkMes();
    gamelog("----UI_Market::~UI_Market---");
    
    //    IMessageManager::getInstance()->detach(this, SYSTEM_ENTERFOREGROUND);
}

enum
{
    B_BACK = 275,
};

bool UI_Market::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    

    
    IPlayerData* p = M_GET_PLAYER;
    
    string surl = StringUtils::format("%sid=%s&md5=%s&w=%d&h=%d", GameIni::getInstance()->market_ip.c_str(),
                                      p->userid.c_str(), p->MiYao_key.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT);
    Layer* l = Layer::create();
    addChild(l, 200);
    
    
    rootNode = CSLoader::createNode("CSres/Main/web.csb");
    rootNode->setVisible(false);
    l->addChild(rootNode);
    Button* b = (Button*)rootNode->getChildByTag(B_BACK);
    b->addTouchEventListener(CC_CALLBACK_2(UI_Market::eventCallback, this));

    
    openWebView(surl);
    
    
    return true;
}
void UI_Market::openWebView(const string& url)
{

//    if(_webview)
//    {
//        _webview->removeFromParentAndCleanup(YES);
//        _webview = NULL;
//    }
//    Size winSize = Director::getInstance()->getWinSize();
//    float h = 50;
//    //    float h = 0;
////#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    _webview = cocos2d::experimental::ui::WebView::create();
//    _webview->setContentSize(Size(winSize.width, winSize.height - h*2));
//    _webview->loadURL(url);
//    _webview->setOnShouldStartLoading(CC_CALLBACK_2(UI_Market::onWebViewShouldStartLoading, this));
//    _webview->setOnDidFinishLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFinishLoading, this));
//    _webview->setOnDidFailLoading(CC_CALLBACK_2(UI_Market::onWebViewDidFailLoading, this));
//    _webview->setPosition(Vec2(winSize.width/2, winSize.height/2 - h));
//    //setScalesPageToFit 调整WebView内容以适应所设置内容的大小  兑换商城
//    _webview->setScalesPageToFit(true);
//    _webview->setFocused(true);
//    //    _webview->setFocusEnabled(true);
//    //    _webview->setPropagateTouchEvents(true);
//    _webview->setSwallowTouches(true);
//    addChild(_webview);
//    currentURL = url;
//    //Director::getInstance()->getOpenGLView()->setIMEKeyboardState(1);
//    
    
        Application::getInstance()->openURL(url);
    //    CCLOG("商城 %s",url.c_str());
        UICtrl::getInstance()->gotoScene(UI_MAIN);
}


void UI_Market::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    
//    IRoomData * romdata  = M_GET_ROOM;
//    romdata->http_surprise("87f1f99b-0f95-4c2c-8c63-85457694a073");
    
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
            UICtrl::getInstance()->gotoScene(UI_MAIN);//UI_MAIN
//            _webview->goBack();
//            _webview->goForward();
            
        }break;
        default:break;
    }
}

bool UI_Market::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    
    Notify_Compare(SYSTEM_ENTERFOREGROUND)
    {
        openWebView(currentURL);
        return YES;
    }
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        rootNode->setVisible(true);
        
        eventCallback(rootNode->getChildByTag(B_BACK), Widget::TouchEventType::ENDED);
        return YES;
    }
    
    return true;
}




bool UI_Market::onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url)
{
    GAMELOG("onWebViewShouldStartLoading, url is %s", url.c_str());
    
    return true;
}

void UI_Market::onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url)
{
    GAMELOG("兑换商城onWebViewDidFinishLoading, url is %s", url.c_str());
    currentURL = url;
    
}

void UI_Market::onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url)
{
    GAMELOG("onWebViewDidFailLoading, url is %s", url.c_str());
    
}












