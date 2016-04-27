//
//  UI_Market.h
//  PinLe
//
//  Created by Kennedy on 15/6/11.
//
//

#ifndef __PinLe__UI_Market__
#define __PinLe__UI_Market__


#include "../../framework/config.h"
#include "../../gameini/GameIni.h"



using namespace cocos2d::experimental::ui;

class UI_Market : public IUILayer
{
public:
    UI_Market();
    ~UI_Market();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Market);
    
    
    
    virtual bool Notify(const string& name, Args& args);
    
    
    bool onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url);
    
public:
    void        eventCallback(Ref * pSender, Widget::TouchEventType type);
    void        openWebView(const string& url);
public:
    string      currentURL; //当前URL
    Node*       rootNode;
    WebView*    _webview;
    
};























#endif /* defined(__PinLe__UI_Market__) */
