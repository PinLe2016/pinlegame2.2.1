//
//  GameLink.cpp
//  PinLe
//
//  Created by 李辛 on 15/9/16.
//
//

#include "GameLink.h"

GameLink *__GameLink = NULL;


GameLink::GameLink()
{
//    IUILayer::attachNetworkMes();
    _webview = NULL;
    
    
    
}

GameLink::~GameLink()
{
//    IUILayer::detachNetworkMes();
    
}

static GameLink* getInstance()
{
    if(__GameLink)
    {
        __GameLink = GameLink::create();
    }
    return __GameLink;
}


bool GameLink::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
    
    initGameLink(UICtrl::getInstance()->_args.content);
    
    return true;
}

bool GameLink::initGameLink(string linkUrl)
{
    Layer*l = Layer::create();
    addChild(l, 200);
    rootNode = CSLoader::createNode("CSres/public/web.csb");
    l->addChild(rootNode);
    
    Button* btn = (Button*)rootNode->getChildByTag(275);
    btn->addTouchEventListener(CC_CALLBACK_2(GameLink::eventCall, this));
    
    if(_webview)
    {
        _webview->removeFromParentAndCleanup(true);
        _webview = NULL;
    }
    Size winSize = Director::getInstance()->getWinSize();
    float h = 50;           //设置页面上方预留高度
    
    _webview = WebView::create();
    _webview->setContentSize(Size(winSize.width, winSize.height - h*2));
    _webview->loadURL(linkUrl);
    _webview->setOnShouldStartLoading(CC_CALLBACK_2(GameLink::onWebViewShouldStartLoading, this));
    _webview->setOnDidFinishLoading(CC_CALLBACK_2(GameLink::onWebViewDidFinishLoading, this));
    _webview->setOnDidFailLoading(CC_CALLBACK_2(GameLink::onWebViewDidFailLoading, this));
    _webview->setPosition(Vec2(winSize.width/2, winSize.height/2 - h));
    _webview->setScalesPageToFit(true);
    addChild(_webview);
    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(0);
    return true;
    
}

bool GameLink::onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url)
{
    return true;
}
void GameLink::onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url)
{
    
}
void GameLink::onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url)
{
    
}


Scene* GameLink::createScene(string linkUrl)
{
    Scene* scene = Scene::create();
    GameLink* linkLayer = GameLink::create();
    scene->addChild(linkLayer);
    return scene;
}

void GameLink::eventCall(Ref* pSender, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
    {
        return;
    }
    Widget* w = (Widget*)pSender;
    GAMELOG("tag = %d", w->getTag());
    int tag = w->getTag();
    switch(tag)
    {
        case 275:
        {
            UICtrl::getInstance()->poptoScene(UICtrl::getInstance()->lastSceneID);
            IMessageManager::getInstance()->postNotification(MES_ROOM_CKXQADDGOLDS);
            
        }break;
        default:
            break;
    }
}


bool GameLink::Notify(const string& name, Args& args)
{
    if(name.empty())
    {
        return true;
    }
    
    return true;
}