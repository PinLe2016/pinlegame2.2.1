//
//  dataPicker.hpp
//  PinLe
//
//  Created by Kennedy on 16/4/22.
//
//

#ifndef dataPicker_h
#define dataPicker_h

#include <stdio.h>
#include "ui/UIScrollView.h"
using namespace cocos2d;
using namespace std;//public cocos2d::ui::ScrollView,cocos2d::ui::ListView
using namespace ui;
#define FLAG        2

class dataPicker : public cocos2d::ui::ScrollView
{
public :
    float _height;
    void set_height(float height);
    float get_height();
    dataPicker();
    virtual ~dataPicker();
    static dataPicker* create();
    //Base Function
    //touch event callback
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    
    
    /**
     * Changes the size that is widget's size
     * @param contentSize A content size in `Size`.
     */
    virtual void setContentSize(const Size& contentSize) override;
    //
    virtual void update(float dt) override;
    //
    //public:
    //
    //   /*
    //    设定拾取器指针指向位置
    //    pos 指针指向位置
    //    */
    void setPickerPointPos(cocos2d::Vec2 pos);
    //
    //
    //
    //	//往末尾添加一个Item
    void pushBackItem(Widget* item);
    
    
    void clearItems(); //清除所有Item
    
    void remedyItemPos(float duration = 0.2); //纠正 item 位置
    
    int getCellPos();
    
//    void inline setType(ModeType type ){m_type=type;}
    
//    void dispatchItemPickMsg();
    
    void setOffsetLayout(int offset);
    
public:
    vector<std::string> m_pickLuaCallbackName;
    Vec2 m_pickerPointPos; // 拾取器取向的索引
    int m_pickIndex; //选中了第几个
    int m_indexOffset = 0; //索引偏移量 每次从末尾取一个元素放到最前时，值减1 从最前最后一个到最后时，值加1
    int m_iCurIdx = -1; //真是索引ß
    list<Widget*> m_items;
    //
    //	string m_pickerTagItemName;
    Vec2 m_preInnerPos; //上一次内容器的位置
    bool m_bNeedRemedy; // 是否需要纠正位置
    bool m_bIsTouching; // 是否处于触摸阶段
    float m_begin;      // 记录初始偏移
    int m_offset_cell;  //第一次进入跳转
    int flag_num;
//    ModeType m_type;//记录传入类型
    
    float m_last_point;//记录上次偏移
    
};

#endif /* dataPicker_hpp */
