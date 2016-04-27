//
//  Msg_Layer.cpp
//  framework
//
//  Created by Kennedy on 14-6-27.
//
//

#include "Msg_Layer.h"


static stack<Msg_Layer*> _LayerStack;


Msg_Layer::Msg_Layer()
{
    _tag = 0;
    _widgetRoot = NULL;
    _widgetLayer= NULL;
    _listener = NULL;
    _outsideClose = false;
    _autoCloseTime = -1.f;
    _enterType  = EnterType::Normal;
    _msgType    = MsgType::Normal;
    addEventListener();
}

Msg_Layer::~Msg_Layer()
{
    _widgetVector.clear();
    removeEventListener();
}


bool Msg_Layer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    
    _widgetLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    addChild(_widgetLayer);
    
//    Snappy.
    
    return true;
}



void Msg_Layer::addEventListener()
{
    if(_listener == NULL)
    {
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        _listener = EventListenerTouchOneByOne::create();//创建一个触摸监听
        if(_listener)
        {
            _listener->setSwallowTouches(true);
            _listener->onTouchBegan = CC_CALLBACK_2(Msg_Layer::onTouchBegan, this);
            _listener->onTouchMoved = CC_CALLBACK_2(Msg_Layer::onTouchMoved, this);
            _listener->onTouchEnded = CC_CALLBACK_2(Msg_Layer::onTouchEnded, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
        }
    }
}

void Msg_Layer::removeEventListener()
{
    if(_listener)
    {
        _eventDispatcher->removeEventListener(_listener);
        _listener = NULL;
    }
}


Action* Msg_Layer::getActionByEnterType(EnterType type)
{
    Action* action = NULL;
    switch (type)
    {
        case EnterType::ZoomPop:
        {
            action = Sequence::create(ScaleTo::create(0.08, 1.02),
                                      ScaleTo::create(0.08, 0.97),
                                      ScaleTo::create(0.08, 1.f),
                                      NULL);
        }break;
        case EnterType::ScalePop:
        {
            _widgetLayer->setScale(0);
            action = Sequence::create(EaseExponentialInOut::create(ScaleTo::create(0.15, 1.0)),
                                      NULL);
        }break;
        default:
            action = Show::create();
            break;
    }
    return action;
}


void Msg_Layer::setMsgType(int type)
{
    _msgType = type;
}

void Msg_Layer::setEnterType(EnterType type)
{
    _enterType = type;
    if(_widgetLayer)
    {
        _widgetLayer->runAction(getActionByEnterType(type));
    }
}





void Msg_Layer::setOutsideClose(bool outsideClose)
{
    _outsideClose = outsideClose;
}


void Msg_Layer::close()
{
    if(_LayerStack.size() > 0)
        _LayerStack.pop();
    if(getParent())
        removeFromParent();
    GAMELOG("close _LayerStack.size() = %ld", _LayerStack.size());

    if(_msgType & MsgType::Normal )
    {
        if(_msgType & MsgType::Hide)
        {
            Msg_Layer* last = getTopLayer();
            if(last)
            {
                last->setVisible(true);
                last->setEnterType(last->getEnterType());
            }
        }
    }
}


void Msg_Layer::autoclose(float time)
{
    _autoCloseTime = time;
    if(time >= 0)
    {
        auto action = Sequence::create(DelayTime::create(time),
                                       CallFuncN::create( CC_CALLBACK_0(Msg_Layer::close, this)),
                                       NULL);
        runAction(action);
    }
}

void Msg_Layer::btnEvent_pwd(Ref *pSender, ui::Widget::TouchEventType type)
{
}



bool Msg_Layer::file_find(const char* filename)
{
    if(!filename)
        return false;
    return FileUtils::getInstance()->isFileExist(string(filename));
}



//Node* Msg_Layer::addWidgetRoot(const char *fileName)
//{
//    if(file_find(fileName))
//    {
//        _widgetRoot = CSLoader::createNode(fileName);
//        
////        _widgetRoot = GUIReader::getInstance()->widgetFromJsonFile(fileName);
//        addChild(_widgetRoot);
//    }
//    return _widgetRoot;
//}
//


Widget* Msg_Layer::addWidget(const char *fileName, int tag)
{
    if(file_find(fileName))
    {
        Widget* widget = GUIReader::getInstance()->widgetFromJsonFile(fileName);
        widget->setTag(tag);
        _widgetVector.push_back(widget);
        _widgetLayer->addChild(widget);
        return widget;
    }
    return NULL;
}

//Widget* Msg_Layer::getWidgetByTag(int tag)
//{
//    for(vector<Widget*>::iterator it = _widgetVector.begin();
//       it != _widgetVector.end(); it ++)
//    {
//        Widget* widget = *it;
//        if(widget && widget->getTag() == tag)
//        {
//            return widget;
//        }
//    }
//    return NULL;
//}


Msg_Layer* Msg_Layer::create(const Color4B& color, int type)
{
    Msg_Layer * pLayer = Msg_Layer::create();
    
    GAMELOG("create _LayerStack.size() = %ld", _LayerStack.size());
    
    if(pLayer)
    {
        pLayer->setMsgType(type);
        Msg_Layer* last = getTopLayer();

        if(type & MsgType::Normal)
        {
            if((int)type & (int)MsgType::Hide)
            {
                if(last)
                    last->setVisible(false);
            }
        }
        else
        {
            if(_LayerStack.size() > 0)
                _LayerStack.pop();
            if(last)
                last->removeFromParent();
        }
        
        if(type & MsgType::Msgbox)
        {
            pLayer->setZOrder(10000);
        }
        else if(type & MsgType::Warnning)
        {
            pLayer->setZOrder(20000);
        }
        
        pLayer->setColor(Color3B(color.r, color.g, color.b));
        pLayer->setOpacity(color.a);
        
        _LayerStack.push(pLayer);
    }
    return pLayer;
}

Scene* Msg_Layer::getRunningScene()
{
    return Director::getInstance()->getRunningScene();
}

Msg_Layer* Msg_Layer::createEx(const Color4B& color, int type)
{
    Msg_Layer* pLayer = NULL;
    Scene* pScene = getRunningScene();
    if(pScene)
    {
        if((pLayer = Msg_Layer::create(color, type)))
        {
            pScene->addChild(pLayer);
        }
    }
    return pLayer;
}

Msg_Layer* Msg_Layer::createWithWidget(const Color4B& color, const char *fileName, int type)
{
    Msg_Layer * pLayer = Msg_Layer::create(color, type);
    if(pLayer)
    {
        pLayer->addWidget(fileName);
    }
    return pLayer;
}
Msg_Layer* Msg_Layer::createWithWidgetEx(const Color4B& color, const char *fileName, int type)
{
    Msg_Layer * pLayer = NULL;
    Scene * pScene = getRunningScene();
    if(pScene)
    {
        if((pLayer = Msg_Layer::createWithWidget(color, fileName, type)))
        {
            pScene->addChild(pLayer);
        }
    }
    return pLayer;
}





Msg_Layer* Msg_Layer::getTopLayer()
{
    Msg_Layer* last = NULL;
    if(_LayerStack.size() > 0)
        last = _LayerStack.top();
    return last;
}
Msg_Layer* Msg_Layer::getRunningLayer()
{
    return getTopLayer();
}

void Msg_Layer::addWidgetEventCallFunc(SEL_CallFuncND callFuncND)
{
    
}


void Msg_Layer::deleteAllLayersAndClean()
{
    Msg_Layer* top;
    while ((top = getTopLayer()))
    {
        _LayerStack.pop();
        top->removeFromParent();
    }
}

bool Msg_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
    GAMELOG("------------Msg_Layer::onTouchBegan-------------");
    return true;
}

void Msg_Layer::onTouchMoved(Touch *touch, Event *unused_event)
{
    GAMELOG("------------Msg_Layer::onTouchMoved-------------");
}


void Msg_Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
    GAMELOG("------------Msg_Layer::onTouchMoved-------------");
    if(_outsideClose)
        close();
}






