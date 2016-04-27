//
//  dataPicker.cpp
//  PinLe
//
//  Created by Kennedy on 16/4/22.
//
//

#include "dataPicker.h"
#include "dataPicker.h"
#include "ui/UIListView.h"
#include "ui/UIHelper.h"
#include "ui/GUIExport.h"
//#include "Tools/CppCallLua.h"

#define REMEDY_SCROLL_DIRATION 0.1f

using namespace cocos2d;
using namespace ui;
dataPicker::dataPicker()
{
    _height=69;
    m_bNeedRemedy = false;
    m_bIsTouching = false;
    m_offset_cell = -1;
    flag_num=0;
    m_last_point=0.0;
    m_items.clear();
}

dataPicker::~dataPicker()
{
    
}


dataPicker* dataPicker::create()
{
    dataPicker* widget = new (std::nothrow) dataPicker();
    if (widget && widget->init())
    {
        widget->autorelease();
        //widget->setDirection(Direction::HORIZONTAL); ////默认水平方向
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void dataPicker::setContentSize(const Size& contentSize)
{
    ScrollView::setContentSize(contentSize);
    //	m_pickerPointPos = contentSize / 2;
}

//touch event callback
bool dataPicker::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    //     CCLOG("0--begin--- %f  ",_innerContainer->getPositionY());
    bool retValue = ScrollView::onTouchBegan(touch, unusedEvent);
    m_bIsTouching = retValue;
    m_begin = _innerContainer->getPositionY();
    
    return retValue;
}

void dataPicker::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    ScrollView::onTouchMoved(touch, unusedEvent);
}

void dataPicker::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    ScrollView::onTouchEnded(touch, unusedEvent);
    m_bIsTouching = false;
    
}

void dataPicker::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    ScrollView::onTouchCancelled(touch, unusedEvent);
    m_bIsTouching = false;
}

void dataPicker::remedyItemPos(float duration)
{
    
    if (m_items.size() == 0)
    {
        return;
    }
    auto innerPos = _innerContainer->getPosition();
    
    int itemCount = 0;
    
    
    for (auto& item : m_items)
    {
        auto pos =item->getPosition();// converPosFromNodeToNode(item, this);
        //        CCLOG("---%f",item->getPositionY());
        Rect rectItem; //子项的矩形区域
        auto itemSize = item->getContentSize();
        rectItem.size = itemSize;
        rectItem.origin = pos - itemSize / 2; //求得子项的矩形区域
        if (rectItem.containsPoint(m_pickerPointPos)) //子项在指针内。 开始纠正位置ø™ ºæ¿’˝Œª÷√
        {
            m_pickIndex = itemCount;
            if (_direction == Direction::HORIZONTAL) //水平方向
            {
                auto x = innerPos.x ;
                innerPos.x -= pos.x - m_pickerPointPos.x;
            }
            else if (_direction == Direction::VERTICAL) //垂直
            {
                innerPos.y -= pos.y - m_pickerPointPos.y;
            }
            if (duration < 0.00001 ) {
                _innerContainer->setPosition(innerPos);
            }
            else {
                startAutoScrollChildrenWithDestination(innerPos, duration, true);
            }
            
            //dispatchItemPickMsg();
            return;
            
        }
        ++itemCount;
    }
    
}


void dataPicker::pushBackItem(Widget* item)
{
    CCASSERT(nullptr != item, "dataPicker Item can't be nullptr!");
    //CCASSERT(m_items.size() <= m_maxItemNums, "itemNum had over maxNum!"); //不能超过最大设定值
    item->setAnchorPoint(Vec2(0.5,0.5));
    
    
    Vec2 pos; //新加进来的子项坐标
    Size innerSize = getInnerContainerSize();
    if (_direction == Direction::HORIZONTAL) //水平方向
    {
        innerSize.width = 0;
        for (auto i : m_items)
        {
            pos.x += i->getContentSize().width; //水平叠加宽度
            innerSize.width += i->getContentSize().width;
        }
        innerSize.width += item->getContentSize().width;
        pos.x += item->getContentSize().width / 2; //锚点 0.5,0.5 。 位置只要宽度的一半 Œª÷√÷ª“™øÌ∂»µƒ“ª∞Î
        pos.y = getContentSize().height / 2; //保证子项在中间
    }
    else if (_direction == Direction::VERTICAL) //垂直方向
    {
        innerSize.height = 0;
        if (!m_items.empty()) {
            for (auto i : m_items)
            {
                pos.y += i->getContentSize().height; //垂直叠加高度
                innerSize.height += i->getContentSize().height;
            }
        }
        
        innerSize.height += item->getContentSize().height;
        pos.y += item->getContentSize().height / 2; //锚点 0.5,0.5 。 位置只要高度的一半
        pos.x = getContentSize().width / 2; //保证子项在中间
    }
    else //BOTH NONE
    {
        CCASSERT(false, "dataPicker Direction just can be HORIZONTAL or VERTICAL");
    }
    item->setPosition(pos);
    addChild(item);
    m_items.push_back(item);
    setInnerContainerSize(innerSize);
    //logicItemMove();
    //remedyItemPos();
}


void dataPicker::clearItems()
{//清除所有子项
    if (m_items.empty()) {
        return;
    }
    for (auto& item : m_items)
    {
        item->removeFromParent();
        item = nullptr;
    }
    m_items.clear();
    m_indexOffset = 0;
    m_iCurIdx = -1;
}
//
//
void dataPicker::setPickerPointPos(Vec2 pos)
{//搭取器指针的位置
    m_pickerPointPos = pos;
    
}


void dataPicker::setOffsetLayout(int offset)
{
    m_offset_cell=offset;
    
}

int dataPicker::getCellPos()
{
    int cell=-(_innerContainer->getPositionY())/_height;
    if(cell<0) cell=0;
    CCLOG("%d   -",cell);
    return cell;
}
void dataPicker::set_height(float height){
    _height=height;
    
}
float dataPicker::get_height(){
    return _height;
}
void dataPicker::update(float dt)
{
    ScrollView::update(dt);
    if (m_preInnerPos == _innerContainer->getPosition()) { //不再发生变化
        if (!m_bIsTouching&& m_bNeedRemedy)
        {
            log("update not touch ...");
            //			remedyItemPos(0.1);
            m_bNeedRemedy = false;
            //            CCLOG("_innerContainer->getPositionY() %f %f",_innerContainer->getPositionY()
            //                  ,_innerContainer->getContentSize().height);
            int bads=_innerContainer->getPositionY()/_height;
            CCLOG("%f ",_height);
            int off_dex=0;
            if (_innerContainer->getPositionY()<m_last_point  and m_offset_cell==-1) {
                off_dex=-_height;
                if(bads*_height+off_dex<-_innerContainer->getContentSize().height+8*_height)
                    off_dex=0;
            }
            
            if (m_offset_cell!=-1) {
                bads=-m_offset_cell;
                m_offset_cell=-1;
            }
            _innerContainer->setPosition(Vec2(0,bads*_height+off_dex));
            
            m_last_point=_innerContainer->getPositionY();
            getCellPos();
//            dispatchItemPickMsg();
        }
        return;
    }
    
    m_bNeedRemedy = true;
    m_preInnerPos = _innerContainer->getPosition();
}