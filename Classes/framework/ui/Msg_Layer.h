//
//  Msg_Layer.h
//  framework
//
//  Created by Kennedy on 14-6-27.
//
//

#ifndef __framework__Msg_Layer__
#define __framework__Msg_Layer__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../tools/ITools.h"


using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocostudio;


class Msg_Layer : public LayerColor
{
public:
    Msg_Layer();
    ~Msg_Layer();
    virtual bool init();
    CREATE_FUNC(Msg_Layer);
    
    enum class EnterType
    {
        Normal = 0,
        ZoomPop,
        ScalePop,
    };
    
    enum MsgType
    {
        Normal        = 1,
        RemoveBefore  = 1<<2,
        Hide          = 1<<3,
        Msgbox        = 1<<4,
        Warnning      = 1<<5,
    };

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    
    Node* _widgetRoot;
    LayerColor*      _widgetLayer;
    
    static Msg_Layer* create(const Color4B& color, int type = MsgType::Normal);
    static Msg_Layer* createEx(const Color4B& color, int type = MsgType::Normal);
    static Msg_Layer* createWithWidget(const Color4B& color, const char *fileName, int type = MsgType::Normal);
    static Msg_Layer* createWithWidgetEx(const Color4B& color, const char *fileName, int type = MsgType::Normal);
    
    
//    Node* addWidgetRoot(const char *fileName);
    Widget* addWidget(const char *fileName, int tag = 0);
//    Widget* getWidgetByTag(int tag);
    inline  Layer*  getWidgetLayer() {return _widgetLayer; }
    inline  EnterType getEnterType() {return _enterType; }
    
    
    static Msg_Layer* getTopLayer();
    static Msg_Layer* getRunningLayer();
    static void deleteAllLayersAndClean();
    static Scene* getRunningScene();
    

    void btnEvent_pwd(Ref *pSender, ui::Widget::TouchEventType type);
    
    void setMsgType(int type);
    void setEnterType(EnterType type);
    
    void close();
    void autoclose(float time);
    void setOutsideClose(bool outsideClose);
    void addWidgetEventCallFunc(SEL_CallFuncND callFuncND);
    
    bool file_find(const char* filename);
    
    void addEventListener();
    void removeEventListener();
    
private:
    
//    LayerColor*      _backGround;
    int         _tag;
    int         _msgType;
    float       _autoCloseTime;
    bool        _outsideClose;
    EnterType   _enterType;
    
    EventListenerTouchOneByOne* _listener;
    vector<Node*> _widgetVector;
    
    Action* getActionByEnterType(EnterType type);
};



#endif /* defined(__framework__Msg_Layer__) */
