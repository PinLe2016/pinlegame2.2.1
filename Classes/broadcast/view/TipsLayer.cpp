//
//  TipsLayer.cpp
//  PinLe
//
//  Created by 李辛 on 15/6/18.
//
//

#include "TipsLayer.h"


TipsButton::TipsButton()
{
    broadCast = new Broadcast();
}
 TipsButton::~TipsButton()
{
    
}
bool TipsButton::init()
{
    if(!Button::init())
    {
        return false;
    }
    return true;
}

void TipsButton::setData(Broadcast *_broadcast)
{
    broadCast = _broadcast->copy();
}



////*****************************************最新


static BroadcastUIData *_broadcastUIData = nullptr;

BroadcastUIData::BroadcastUIData()
{
    IMessageManager::getInstance()->attach(this, MES_BROADCAST_GETLIST);
    sum = 0;
    v_bcArr.clear();
    broad = M_GET_BROADCAST;
    loadBroadcast();
    
}
BroadcastUIData::~BroadcastUIData()
{
    IMessageManager::getInstance()->detach(this, MES_BROADCAST_GETLIST);
}

bool BroadcastUIData::Notify(const string& name, Args& args)
{
    if(name.compare(MES_BROADCAST_GETLIST) == 0)
    {
        loadBroadcast();
        return true;
    }
    return true;
}
BroadcastUIData *BroadcastUIData::getInstance()
{
    if(_broadcastUIData == nullptr)
    {
        _broadcastUIData = new BroadcastUIData();
    }
    return _broadcastUIData;
}
bool BroadcastUIData::isInArray(Broadcast *bb)
{
    for(vector<ItemInfo>::iterator tt = v_bcArr.begin(); tt != v_bcArr.end(); tt++)
    {
        if(tt->b->id.compare(bb->id) ==0)
        {
            return true;
        }
    }
    return false;
}
void BroadcastUIData::loadBroadcast()   // 加载广播条数
{
    GAMELOG("***size == %lu",broad->getList().size());
    for(vector<Broadcast*>::const_iterator tt = broad->getList().begin(); tt != broad->getList().end(); tt++)
    {
        Label* tipsLabel = Label::create();
        tipsLabel->setString((*tt)->content);
        tipsLabel->setSystemFontSize(28);
        
        ItemInfo item;
        item.b = (*tt)->copy();
        item.width = tipsLabel->getContentSize().width;
        item.y = clippingHeight / 2;
        if(v_bcArr.size() == 0)
        {
            item.x = clippingWidth;
        }
        else
        {
            vector<ItemInfo>::iterator itt = v_bcArr.end() - 1;
            if(clippingWidth - itt->x - itt->width > broadcastinterval)
            {
                item.x = clippingWidth;
            }
            else
            {
                item.x = itt->x + itt->width + broadcastinterval;
            }
        }
        
//        v_bcArr.push_back(item);
        if((*tt)->used)
            v_bcArr.push_back(item);
        else
            v_bcArr.insert(v_bcArr.end(), item);
    }
    GAMELOG("***v_bcArr == %lu",v_bcArr.size());
    IMessageManager::getInstance()->postNotification(MES_BroadcastUIData_data);
}
void BroadcastUIData::updateBCArr()     //  更新最新广播
{
    
}
void BroadcastUIData::sumBroadcastNum()     //广播个数减一
{
    sum--;
    if(sum == 0)
    {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(BroadcastUIData::update),
                                                            BroadcastUIData::getInstance());
        broad->stopMonitor();
    }
}
void BroadcastUIData::addBroadcastNum()     //广播个数加一
{
    if(sum == 0)
    {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(BroadcastUIData::update),
                                                          BroadcastUIData::getInstance(), 1/60, false);
        broad->startMonitor(5);
    }
    sum++;
}
void BroadcastUIData::update(float ft)      //定时器
{
    for(vector<ItemInfo>::iterator tt = v_bcArr.begin(); tt != v_bcArr.end();)
    {
        tt->x -= speedX;
        if(tt->x <= 0 - tt->width - 100)
        {
            delete tt->b;
            tt = v_bcArr.erase(tt);
        }
        else
            tt++;
    }
    if(v_bcArr.size() != 0)
    {
        vector<ItemInfo>::iterator itt = v_bcArr.end() - 1;
        if(clippingWidth - itt->x - itt->width > broadcastinterval)
        {
            loadBroadcast();
        }
    }
}



BroadcastUI::BroadcastUI()
{
    IMessageManager::getInstance()->attach(this, MES_BroadcastUIData_data);
    
    layout = Layout::create();
    addChild(layout);
    layout->setContentSize(Size(574,40));
    layout->setClippingEnabled(true);
}
BroadcastUI::~BroadcastUI()
{
    IMessageManager::getInstance()->detach(this, MES_BroadcastUIData_data);
    BroadcastUIData::getInstance()->sumBroadcastNum();
}

bool BroadcastUI::init()
{
    if(!LayerColor::initWithColor(Color4B(112,122,99,122), clippingWidth, clippingHeight))
    {
        return false;
    }
    BroadcastUIData::getInstance()->addBroadcastNum();
    loadBC();
    setLocalZOrder(BroadcastZOrder);
    return true;
}

bool BroadcastUI::Notify(const string& name, Args& args)
{
    if(name.compare(MES_BroadcastUIData_data) == 0)
    {
        loadBC();
    }
    return true;
}
void BroadcastUI::setVisible(bool b)
{
    
}


void BroadcastUI::loadBC()
{
    removeAllBroadcast();
    
    TTFConfig config;
    config.fontFilePath = "res/CSres/public/publicUI/font.ttf";
    config.fontSize = 28;
    
    GAMELOG("***BroadcastUI::loadBC");
    for(vector<ItemInfo>::iterator tt = BroadcastUIData::getInstance()->v_bcArr.begin(); tt != BroadcastUIData::getInstance()->v_bcArr.end(); tt++)
    {
        switch(tt->b->type)
        {
            case 0:
            {
                Label* tipsLabel = Label::create();
                tipsLabel->setTTFConfig(config);
                tipsLabel->setString(tt->b->content.c_str());
                tipsLabel->setAnchorPoint(Vec2(0, 0.5));
                tipsLabel->setColor(tt->b->color3B);
                layout->addChild(tipsLabel);
                tipsLabel->setPosition(Vec2(tt->x,tt->y));
                v_broadcastArr.push_back(tipsLabel);
                
            }break;
            case 1:
            case 2:
            {
                TipsButton *button = TipsButton::create();
                
                button->setData(tt->b);
                button->setAnchorPoint(Vec2(0,0.5));
                
                Label *lala = Label::create();
                lala->setTTFConfig(config);
                lala->setString(tt->b->content.c_str());
                lala->setColor(tt->b->color3B);
                lala->setAnchorPoint(Vec2(0,0));
                button->addChild(lala);
                
                layout->addChild(button);
                button->setScale9Enabled(true);
                button->setSize(lala->getContentSize());
                button->setTag(tt->b->tag);
                button->addTouchEventListener(CC_CALLBACK_2(BroadcastUI::eventCallBack, this));
                button->setPosition(Vec2(tt->x, tt->y));
                v_broadcastArr.push_back(button);
                
                
                GAMELOG("red = %d, green = %d, blue = %d",tt->b->color3B.r,tt->b->color3B.g,tt->b->color3B.b);
                DrawNode* drawLine = DrawNode::create();
                drawLine->drawLine(Vec2(0,0), Vec2(lala->getContentSize().width,0), Color4F((float)tt->b->color3B.r/255, (float)tt->b->color3B.g/255, (float)tt->b->color3B.b/255, 1));
                drawLine->setPosition(Vec2(0,2));
                button->addChild(drawLine);
            }break;
            default:
                break;
        }
        v_bcList.push_back(tt->b->copy());
        GAMELOG("***v_broadcastArr == size == %lu   x = %f  y = %f",v_broadcastArr.size(),tt->x,tt->y);
    }
    
    
    
    if(v_broadcastArr.size() == 0)
    {
        unscheduleUpdate();
    }
    else
    {
        scheduleUpdate();
    }
}

void BroadcastUI::removeAllBroadcast()
{
    for(auto sp : v_broadcastArr)
    {
        sp->removeFromParent();
    }
    v_broadcastArr.clear();
    
    for(vector<Broadcast*>::iterator tt = v_bcList.begin(); tt != v_bcList.end(); tt++)
    {
        delete (*tt);
    }
    v_bcList.clear();
}

bool BroadcastUI::isInArrary(Broadcast *bb)
{
    for(vector<Broadcast*>::iterator tt = v_bcList.begin(); tt != v_bcList.end(); tt++)
    {
        if((*tt)->id.compare(bb->id) == 0)
        {
            return true;
        }
    }
    return false;
}
void BroadcastUI::update(float)
{
    for(auto sp : v_broadcastArr)
    {
        sp->setPosition(sp->getPosition() - Vec2(speedX, 0));
        if(sp->getPositionX() < 0 - sp->getContentSize().width - 100)
        {
            sp->removeFromParent();
            v_broadcastArr.erase(v_broadcastArr.begin());
            
            vector<Broadcast*>::iterator tt = v_bcList.begin();
            delete (*tt);
            v_bcList.erase(tt);
        }
    }
    if(v_broadcastArr.size() ==0)
    {
        unscheduleUpdate();
    }
}

void BroadcastUI::eventCallBack(Ref *pSender, Widget::TouchEventType type)
{
    //    Widget  *btn = (Widget*)pSender;
    TipsButton *btn = (TipsButton*)pSender;
    
    if(type == Widget::TouchEventType::ENDED)
    {
        GAMELOG("type == %d   tag == %d    content == %s   linkurl == %s",btn->broadCast->type,btn->broadCast->tag,btn->broadCast->content.c_str(),btn->broadCast->linkstr.c_str());
        
        switch(btn->broadCast->type)
        {
//            case 0:
//            {
//                GAMELOG("***button button button");
//            }break;
            case 1:
            {
                GAMELOG("btn->broadCast->linkstr = %s", btn->broadCast->linkstr.c_str());
                Application::getInstance()->openURL(btn->broadCast->linkstr);
            }break;
            case 2:
            {
                GAMELOG("btn->broadCast->tag = %d", btn->broadCast->tag);
                if(UICtrl::getInstance()->sceneID != btn->broadCast->tag)
                    UICtrl::getInstance()->gotoScene(btn->broadCast->tag);
            }break;
            default:
                break;
        }
    }
}






