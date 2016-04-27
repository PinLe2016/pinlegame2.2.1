//
//  GameLink.h
//  PinLe
//
//  Created by 李辛 on 15/9/16.
//
//

#ifndef __PinLe__GameLink__
#define __PinLe__GameLink__

#include <stdio.h>
#include "../framework/config.h"
#include "../gameini/GameIni.h"


using namespace cocos2d::experimental::ui;
enum LinkType
{
    _GamePlayLink = 0,          //玩家拼图完成后的图片链接
    _GameActivityReward,        //活动结束奖品链接
};


class GameLink : public IUILayer
{
public:
    GameLink();
    ~GameLink();
    CREATE_FUNC(GameLink);
    virtual bool init();
    virtual bool Notify(const string& name, Args& args);
    
    static GameLink* createLayer(string linkUrl);
    static GameLink* getInstance();
    static Scene* createScene(string linkUrl);
    bool initGameLink(string linkUrl);
    
    bool onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url);
    
    void eventCall(Ref* pSender, Widget::TouchEventType type);
    
    WebView*    _webview;
    Node*       rootNode;
    string      _linkUrl;
    
};



#endif /* defined(__PinLe__GameLink__) */
