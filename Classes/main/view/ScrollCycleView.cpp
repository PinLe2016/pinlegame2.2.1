//
//  ScrollCycleView.cpp
//  PinLe
//
//  Created by 李辛 on 15/7/2.
//
//

#include "ScrollCycleView.h"
#include "UI_Main.h"
bool one_touch=true;
bool ItemsCell::init()
{
    if(!Node::init())
    {
        return false;
    }
    return true;
}
void ItemsCell::setGrey()
{
    
}
ItemsCell* ItemsCell::create(string str)
{
    ItemsCell *itemCell;
    return itemCell;
}


ScrollCycleView::ScrollCycleView()
{
    nowPoType = 0;
    movePoint = false;
    beginTouchX = 0;
    secondOut1 = false;
    secondOut2 = false;
    fristOut = false;
    threeOut = false;
    moveType = 0;
    touchTag = -1;
    touchValue = false;
    beginStart = false;
}
ScrollCycleView::~ScrollCycleView()
{
    
}
bool ScrollCycleView::init()
{
    if(!LayerColor::initWithColor(Color4B(123,123,123,0), 750, 700))
    {
        return false;
    }
    
    setContentSize(Size(750,700));
    setSwallowsTouches(false);
    setTouchEnabled(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    item1 = ImageView::create("CSres/Main/MainUI/main_jxb.png"); //惊喜吧
    item1->setColor(Color3B(125,125,125));
    item1->setTag(2);
    addChild(item1);
    item1->setLocalZOrder(15);
    item1->setPosition(fristVec);
    item1->setScale(ITEM_SCALE);
    item1dir=false;
    fristBtn = item1;
    
    item2 = ImageView::create("CSres/Main/MainUI/main_go.png");//go
    item2->setColor(Color3B(255,255,255));
    item2->setTag(0);
    addChild(item2);
    item2->setLocalZOrder(13);
    item2->setPosition(secondVec);
    item2->setScale(ITEM_SCALE);
    item2dir=true;
    secondBtn = item2;
    
    item3 = ImageView::create("CSres/Main/MainUI/main_plb.png"); //拼了吧
    item3->setColor(Color3B(125,125,125));
    item3->setTag(4);
    addChild(item3);
    item3->setLocalZOrder(14);
    item3->setPosition(threeVec);
    item3->setScale(ITEM_SCALE);
    item3dir=true;
    threeBtn = item3;
    
    rotateLight = Sprite::create("CSres/Main/MainUI/main_light.png");
    rotateLight->setPosition(Vec2(166, 395));
    rotateLight->retain();
    rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
    
    
    
    if(UICtrl::getInstance()->_args.content.compare(ui_jinxiba) == 0)
    {
        swopItem(jinxiba, true);
    }
    else if(UICtrl::getInstance()->_args.content.compare(ui_pinleba) == 0)
    {
        swopItem(pinleba, true);
    }
    else
    {
        swopItem(gogo, true);
    }
    
    setPosition(Vec2(0,positionY));
    if (one_touch) {
        //this->scheduleUpdate();
 //       schedule(schedule_selector(ScrollCycleView::update_move), 1.0f);
        //schedule(schedule_selector(ScrollCycleView::update_move));
       
    }
    
    if(!USERDEFAUL_GET_BOOL("guide_move"))
    {
         move_guide();
    }
    
    return true;
}

void ScrollCycleView::move_guide()
{
    Layout* layer=Layout::create();
    layer->setTouchEnabled(true);
    Size size =Director::getInstance()->getWinSize();
    layer->setContentSize(Size(size.width,size.height));
    layer->setTag(10086);
    addChild(layer,-1);
    layer->addTouchEventListener(CC_CALLBACK_2(ScrollCycleView::guide_TouchEnd, this));

    
    spr_head=Sprite::create("res/shou.png");
    spr_head->setPosition(Vec2(375.5 - 180,100));
    spr_head->setOpacity(0);
    addChild(spr_head,20);
    
    _prompt=Sprite::create("res/prompt.png");
    _prompt->setPosition(Vec2(375.5,30));
    addChild(_prompt,40);
    auto call_fist = CallFunc::create([this](){
        spr_head->setPosition(Vec2(375.5 - 180,100));
       });

    auto fadein=FadeIn::create(0.5);
    auto fadeout=FadeOut::create(0.5);
    auto firstSeq=Sequence::create(fadein,MoveTo::create(1.0,Vec2(375.5 + 180,100)),fadeout,call_fist,DelayTime::create(1.0),NULL);
    spr_head->runAction(RepeatForever::create(firstSeq));
    
}

void ScrollCycleView::guide_TouchEnd(cocos2d::Ref *psender, Widget::TouchEventType type)
{
    return;
}
void ScrollCycleView::update_move(float dt)
{
//    it-=50;
//    if (it<=-200) {
//        it=0;
//        touch_end();//返回
//    }
//    moveItem(it * move_scale);
    
//    it-=10;
//    if (it<-100) {
//        it=0;
//        touch_end();
//    }
    
    int it=-200;//主界面旋转
    moveItem(it * move_scale);
    touch_end();
    
    
}



bool ScrollCycleView::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    if(beginStart)
    {
        return false;
    }
    
    UI_Main* layer = (UI_Main*)getParent();
    if(layer && layer->isGuide)
    {
        return false;
    }
    
    beginTouchX = touch->getLocation().x;
    touchBeginPoint = touch->getLocation();

    if(touchBeginPoint.x > secondVec.x - secondBtn->getContentSize().width/2
       && touchBeginPoint.x < secondVec.x + secondBtn->getContentSize().width/2
       && touchBeginPoint.y > secondVec.y + positionY - secondBtn->getContentSize().height/2
       && touchBeginPoint.y < secondVec.y + positionY + secondBtn->getContentSize().height/2)
    {
        touchValue = true;
    }
    
    if(touch->getLocation().y < 400 || touch->getLocation().y > 1200)
        return false;
    
    beginStart = true;
    return true;
}
void ScrollCycleView::onTouchMoved(Touch *touch, Event *unused_event)
{
    
    float movex = touch->getLocation().x - beginTouchX;
    beginTouchX = touch->getLocation().x;
//    log("*** touchx = %f   touchy = %f  movex = %f  beginTouchX = %f",touch->getLocation().x,touch->getLocation().y ,movex,beginTouchX);
    if(abs(touchBeginPoint.x - touch->getLocation().x) > 10 || abs(touchBeginPoint.y - touch->getLocation().y) > 10)
    {
        if (!USERDEFAUL_GET_BOOL("guide_move")) {
            stop_button();
        }
        touchValue = false;
    }
    moveItem(movex * move_scale);
    
}
void ScrollCycleView::onTouchEnded(Touch *touch, Event *unused_event)
{
    touch_end();
}
void ScrollCycleView::touch_end()
{
    secondOut2 = false;
    secondOut1 = false;
    fristOut = false;
    threeOut = false;
    
    if(touchValue)
    {
        if (USERDEFAUL_GET_BOOL("guide_move")) {
            eventCallBack(touchTag);
        }
    }
    else
    {
        setNowPotype(secondBtn->getPositionX() - secondVec.x);
        swopItem(nowPoType);
        setItemScale(secondVec.x);
    }
    beginStart = false;

    
}

void ScrollCycleView::setNowPotype(float ft)
{
    if(ft > 50)
    {
        if(nowPoType >= 2)
        {
            nowPoType = 0;
        }
        else
        {
            nowPoType++;
        }
        moveType = move_right;
    }
    
    if(ft < -50)
    {
        if(nowPoType <= 0)
        {
            nowPoType = 2;
        }
        else
        {
            nowPoType--;
        }
        moveType = move_left;
    }
}


void ScrollCycleView::button_light_control(ImageView* par,int zorder)
{
//    par->setLocalZOrder(1);
//    if (par->getPositionX()==secondVec.x)
//    {
    par->setLocalZOrder(zorder);
    rotateLight->removeFromParent();
    par->addChild(rotateLight);
    rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));

    //        par->setScale(0.8);
    par->setColor(Color3B(125,125,125));
    float scal=ITEM_SCALE;
    if (zorder==3) {

        scal=ITEM_SCALE_MORE;
        par->setColor(Color3B(255,255,255));
    }

    auto scale=ScaleTo::create(1.5, scal);
    par->runAction(scale);

}

void ScrollCycleView::stop_button()
{
//    fristBtn->stopAllActions();
//    secondBtn->stopAllActions();
    spr_head->stopAllActions();
    spr_head->removeFromParent();
    _prompt->setVisible(false);
    this->removeChildByTag(10086);
    USERDEFAUL_SET_BOOL("guide_move", true);
//    fristVec = Vec2(375.5 - 180,300);
//    secondVec = Vec2(375.5,300);
//    threeVec = Vec2(375.5 + 180,300);
}


void ScrollCycleView::moveItem(float ft)
{
    //fristBtn //惊喜吧 secondBtn //go threeBtn //拼了吧
//    if (ft==-1)
//    {
//        Vec2 firstBuf[]= {threeVec,secondVec,fristVec};
//        Vec2 secondBuf[]= {fristVec,threeVec,secondVec};
//        Vec2 threeBuf[]= {secondVec,fristVec,threeVec};
//        float time=1.5;
//        auto call_fist = CallFunc::create([this](){
//                button_light_control(fristBtn,3);
//            });
//        auto call_fist_1 = CallFunc::create([this](){
//            button_light_control(fristBtn,1);
//        });
//
//        auto call_second = CallFunc::create([this](){
//            button_light_control(secondBtn,3);
//        });
//        auto call_second_1 = CallFunc::create([this](){
//            button_light_control(secondBtn,1);
//        });
//
//        auto call_three = CallFunc::create([this](){
//            button_light_control(threeBtn,3);
//        });
//        auto call_three_1 = CallFunc::create([this](){
//            button_light_control(threeBtn,1);
//        });
    
//        AffineTransform tran = AffineTransformMake(2,1,1,0,0,0);
//        secondBtn->setAdditionalTransform(AffineTransformRotate(tran, 3));
        
//        auto firstSeq=Sequence::create(Spawn::create(MoveTo::create(time,firstBuf[0]),call_fist_1,NULL),
//                                       Spawn::create(MoveTo::create(time,firstBuf[1]),call_fist,NULL),
//                                       Spawn::create(MoveTo::create(time,firstBuf[2]),call_fist_1,NULL),NULL);
//
//        auto secondSeq=Sequence::create(Spawn::create(MoveTo::create(time,secondBuf[0]),call_second_1,NULL),
//                                        Spawn::create(MoveTo::create(time,secondBuf[1]),call_second_1,NULL),
//                                        Spawn::create(MoveTo::create(time,secondBuf[2]),call_second,NULL),NULL);
//
//        auto threeSeq=Sequence::create(Spawn::create(MoveTo::create(time,threeBuf[0]),call_three,NULL),
//                                       Spawn::create(MoveTo::create(time,threeBuf[1]),call_three_1,NULL),
//                                       Spawn::create(MoveTo::create(time,threeBuf[2]),call_three_1,NULL), NULL);
//
//        fristBtn->runAction(RepeatForever::create(firstSeq));
//        secondBtn->runAction(RepeatForever::create(secondSeq));
//        threeBtn->runAction(RepeatForever::create(threeSeq));
//        return;
//        
//    }
    
    
    float secPointX = secondBtn->getPositionX();
//    log("***secPointx = %f  vec2 = %f  ft == %f",secPointX,secondVec.x,ft);
    ///seconBtn 移动
    if(secondOut1 || secondOut2)
    {
        if(secondBtn->getPositionX() - ft < left_range && secondBtn->getPositionX() > left_range && secondOut2)
        {
            secondOut2 = false;
            secondBtn->setPositionX( left_range + (left_range - (secondBtn->getPositionX() - ft)));
        }
        else if(secondBtn->getPositionX() - ft > right_range && secondBtn->getPositionX() < right_range && secondOut1)
        {
            secondOut1 = false;
            secondBtn->setPositionX( right_range - (secondBtn->getPositionX() - ft - right_range));
        }
        else
        {
            secondBtn->setPositionX(secondBtn->getPositionX() - ft);
        }
    }
    else
    {
        if(secondBtn->getPositionX() + ft < left_range && secondBtn->getPositionX() > left_range && !secondOut2)
        {
            secondOut2 = true;
            secondBtn->setPositionX( left_range + (left_range - (secondBtn->getPositionX() + ft)));
        }
        else if(secondBtn->getPositionX() + ft > right_range && secondBtn->getPositionX() < right_range && !secondOut1)
        {
            secondOut1 = true;
            secondBtn->setPositionX( right_range - (secondBtn->getPositionX() + ft - right_range));
            
        }
        else
        {
            secondBtn->setPositionX(secondBtn->getPositionX() + ft);
        }
    }
    
        //fristBtn 移动
    if(secondBtn->getPositionX() >= secondVec.x)
    {
        if(secPointX >= secondVec.x)
            fristBtn->setPositionX(fristBtn->getPositionX() + ft);
        else
        {
            fristBtn->setPositionX(fristVec.x + abs(secondBtn->getPositionX() - secondVec.x));
        }
    }
    else
    {
        if(secPointX < secondVec.x)
        {
            if(fristOut)
            {
                if(fristBtn->getPositionX() + ft > threeVec.x)
                {
                    fristOut = false;
                    fristBtn->setPositionX(threeVec.x - (fristBtn->getPositionX() + ft - threeVec.x) * 1.2);
                }
                else
                    fristBtn->setPositionX(fristBtn->getPositionX() + ft);
            }
            else
            {
                if(fristBtn->getPositionX() > threeVec.x - 66)
                {
                    if(fristBtn->getPositionX() - ft > threeVec.x)
                    {
                        fristOut = true;
                        fristBtn->setPositionX(threeVec.x - (fristBtn->getPositionX() - ft - threeVec.x));
                    }
                    else
                        fristBtn->setPositionX(fristBtn->getPositionX() - ft);
                }
                else
                {
                    if(fristBtn->getPositionX() < fristVec.x + 50)
                    {
                        fristBtn->setPositionX(fristBtn->getPositionX() - ft * 1.2);
                    }
                    else
                        fristBtn->setPositionX(fristBtn->getPositionX() - ft * move_double);
                    
                    if(fristBtn->getPositionX() >threeVec.x)
                    {
                        fristBtn->setPositionX(threeVec.x - (fristBtn->getPositionX() - threeVec.x));
                        fristOut = true;
                    }
                }
            }
        }
        else
        {
            float move1x = secondVec.x - secondBtn->getPositionX();
            if(move1x < 50)
            {
                fristBtn->setPositionX(fristVec.x + move1x * 1.2);
            }
            else
            {
                fristBtn->setPositionX(fristVec.x + 50 * 1.2 + (move1x - 50) * move_double);
            }
            
            
            
        }
    }
    
    ///threeBtn 移动
    if(secondBtn->getPositionX() >= secondVec.x)
    {
        if(secPointX >= secondVec.x)
        {
            if(threeOut)
            {
//                log("***1`1111");
                if(threeBtn->getPositionX() + ft < fristVec.x)
                {
//                    log("***2222");
                    threeOut = false;
                    threeBtn->setPositionX(fristVec.x + (fristVec.x - (threeBtn->getPositionX() + ft)) * 1.2);
                }
                else
                    threeBtn->setPositionX(threeBtn->getPositionX() + ft);
            }
            else
            {
//                log("***3333");
                if(threeBtn->getPositionX() < fristVec.x + 66)
                {
//                    log("***4444");
                    if(threeBtn->getPositionX() > fristVec.x && threeBtn->getPositionX() - ft < fristVec.x)
                    {
//                        log("***4...00000  %f    %f",threeBtn->getPositionX(),fristVec.x);
                        threeOut = true;
                        threeBtn->setPositionX(fristVec.x + (fristVec.x - (threeBtn->getPositionX() - ft)));
                    }
                    else
                    {
//                        log("***4...11111  %f    %f",threeBtn->getPositionX(),fristVec.x);
                        threeBtn->setPositionX(threeBtn->getPositionX() - ft);
                    }
                    
                }
                else
                {
//                    log("***5555");
                    if(threeBtn->getPositionX() > threeVec.x - 50)
                    {
                        threeBtn->setPositionX(threeBtn->getPositionX() - ft * 1.2);
                    }
                    else
                        threeBtn->setPositionX(threeBtn->getPositionX() - ft * move_double);
                    
                    if(threeBtn->getPositionX() < fristVec.x)
                    {
                        threeBtn->setPositionX(fristVec.x + (fristVec.x - threeBtn->getPositionX()));
                        threeOut = true;
                    }
                }
            }
        }
        else
        {
            float move3x = secondBtn->getPositionX() - secondVec.x;
            if(move3x > 50)
            {
                threeBtn->setPositionX(threeVec.x - 50 * 1.2 - move3x * move_double);
            }
            else
            {
                threeBtn->setPositionX(threeVec.x - move3x * 1.2);
            }
        }
    }
    else
    {
        if(secPointX > secondVec.x)
        {
            threeBtn->setPositionX(threeVec.x - (secondVec.x - secondBtn->getPositionX()));
        }
        else
        {
            threeBtn->setPositionX(threeBtn->getPositionX() + ft);
        }
    }

    
    
    checkZorder();
}

void ScrollCycleView::swopItem(int type , bool fristInit)
{
    secondBtn->setScale(ITEM_SCALE);
//    log("*** nowPotype == %d", nowPoType);
    nowPoType = type;
    
    IAudio::getInstance()->playEffect(music_ef_pagechange);
    
    Vec2 vec1 = fristBtn->getPosition();
    Vec2 vec2 = secondBtn->getPosition();
    Vec2 vec3 = threeBtn->getPosition();
    
    switch(type)
    {
        case 0:
        {
            fristBtn = item1;
            secondBtn = item2;
            threeBtn = item3;
        }break;
        case 1:
        {
            fristBtn = item3;
            secondBtn = item1;
            threeBtn = item2;
        }break;
        case 2:
        {
            fristBtn = item2;
            secondBtn = item3;
            threeBtn = item1;
        }break;
        default:
            break;
    }
    
    fristBtn->setColor(Color3B(125,125,125));
    secondBtn->setColor(Color3B(255,255,255));
    threeBtn->setColor(Color3B(125,125,125));
    
    
    secondBtn->setScale(ITEM_SCALE_MORE);
    
    if(moveType == lrtype::move_right)
    {
        fristBtn->setLocalZOrder(1);
        threeBtn->setLocalZOrder(2);
    }
    else
    {
        fristBtn->setLocalZOrder(2);
        threeBtn->setLocalZOrder(1);
    }
    
    touchTag = secondBtn->getTag();
    
    
    secondBtn->setLocalZOrder(3);
    if(rotateLight->getParent())
    {
        if(rotateLight->getParent()->getTag() != secondBtn->getTag())
        {
            rotateLight->removeFromParent();
            secondBtn->addChild(rotateLight);
            rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
        }
    }
    else
    {
        secondBtn->addChild(rotateLight);
        rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
    }
    
    
    if(fristInit)
    {
        secondBtn->setPosition(secondVec);
        fristBtn->setPosition(fristVec);
        threeBtn->setPosition(threeVec);
    }
    else
    {
        ActionInterval *action2 = MoveTo::create(0.2,secondVec);
        secondBtn->runAction(action2);
        
        ActionInterval *action1 = MoveTo::create(0.2,fristVec);
        fristBtn->runAction(action1);
        
        ActionInterval *action3 = MoveTo::create(0.2,threeVec);
        threeBtn->runAction(action3);
    }
    
    
//    log("*****frist x = %f    y = %f",fristBtn->getPosition().x,fristBtn->getPosition().y);
//    log("*****three x = %f    y = %f",threeBtn->getPosition().x,threeBtn->getPosition().y);
}

void ScrollCycleView::checkZorder()
{
    if(secondBtn->getPositionX() > secondVec.x)
    {
        fristBtn->setLocalZOrder(2);
        threeBtn->setLocalZOrder(1);
    }
    else
    {
        fristBtn->setLocalZOrder(1);
        threeBtn->setLocalZOrder(2);
    }    
    
    if(secondBtn->getPositionX() > secondVec.x + swop_item)
    {
        fristBtn->setLocalZOrder(3);
        if(rotateLight->getParent())
        {
            if(rotateLight->getParent()->getTag() != fristBtn->getTag())
            {
                rotateLight->removeFromParent();
                fristBtn->addChild(rotateLight);
                rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
                
            }
        }
        else
        {
            fristBtn->addChild(rotateLight);
            rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
        }
        secondBtn->setLocalZOrder(2);
        threeBtn->setLocalZOrder(1);
//        log("***11111");
    }
    else if(secondBtn->getPositionX() < secondVec.x - swop_item)
    {
        fristBtn->setLocalZOrder(1);
        secondBtn->setLocalZOrder(2);
        threeBtn->setLocalZOrder(3);
        if(rotateLight->getParent())
        {
            if(rotateLight->getParent()->getTag() != threeBtn->getTag())
            {
                rotateLight->removeFromParent();
                threeBtn->addChild(rotateLight);
                rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
            }
        }
        else
        {
            threeBtn->addChild(rotateLight);
            rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
        }
//        log("***3333");
    }
    else
    {
        secondBtn->setLocalZOrder(3);
        if(rotateLight->getParent())
        {
            if(rotateLight->getParent()->getTag() != secondBtn->getTag())
            {
                rotateLight->removeFromParent();
                secondBtn->addChild(rotateLight);
                rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
            }
        }
        else
        {
            secondBtn->addChild(rotateLight);
            rotateLight->runAction(RepeatForever::create(RotateBy::create(2.0f,360)));
        }
//        log("***2222");
    }
    
    setItemScale(secondBtn->getPositionX());
}

void ScrollCycleView::setItemScale(float pintX)
{
    float addScale = abs(pintX - secondVec.x) / 900;
//    log("***addScle == %f",addScale);
    float colorNum;
    if(pintX > secondVec.x)
    {
        fristBtn->setScale(0.8 + addScale);
        secondBtn->setScale(1 - addScale);
        threeBtn->setScale(0.8);
        
        colorNum = 255 - 150*(abs(fristBtn->getPositionX()-secondVec.x) / 180);
        fristBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        colorNum = 255 - 150*(abs(secondBtn->getPositionX()-secondVec.x) / 180);
        secondBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        
        
    }
    
    if(pintX < secondVec.x)
    {
        fristBtn->setScale(0.8);
        secondBtn->setScale(1.0 - addScale);
        threeBtn->setScale(0.8 + addScale);
        
        colorNum = 255 - 150*(abs(threeBtn->getPositionX()-secondVec.x) / 180);
        threeBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        colorNum = 255 - 150*(abs(secondBtn->getPositionX()-secondVec.x) / 180);
        secondBtn->setColor(Color3B(colorNum,colorNum,colorNum));
    }
    if(pintX == secondVec.x)
    {
        fristBtn->setColor(Color3B(125,125,125));
        secondBtn->setColor(Color3B(255,255,255));
        threeBtn->setColor(Color3B(125,125,125));
    }
    
}


void ScrollCycleView::loadImageText(int tag)
{
    
//    fristBtn->setColor(Color3B(255,255,255));
//    secondBtn->setColor(Color3B(255,255,255));
//    threeBtn->setColor(Color3B(255,255,255));
    
//    string str;
//    string mainStr = "CSres/Main/MainUI/main_item%d.png";
    float colorNum;
    switch(tag)
    {
        case left_up:
        {
//            log("left_up  fris = %d   secon = %d  three = %d",fristBtn->getTag(),secondBtn->getTag(),threeBtn->getTag());
//            fristBtn->loadTexture(StringUtils::format(mainStr.c_str(),fristBtn->getTag()));
//            secondBtn->loadTexture(StringUtils::format(mainStr.c_str(),secondBtn->getTag() + 1));
            colorNum = 255 - 150*(abs(fristBtn->getPositionX()-secondVec.x) / 180);
            fristBtn->setColor(Color3B(colorNum,colorNum,colorNum));
            colorNum = 255 - 150*(abs(secondBtn->getPositionX()-secondVec.x) / 180);
            secondBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        }break;
        case module_up:
        {
//            log("module_up  fris = %d   secon = %d  three = %d",fristBtn->getTag(),secondBtn->getTag(),threeBtn->getTag());
//            fristBtn->loadTexture(StringUtils::format(mainStr.c_str(),fristBtn->getTag() + 1));
//            secondBtn->loadTexture(StringUtils::format(mainStr.c_str(),secondBtn->getTag()));
//            threeBtn->loadTexture(StringUtils::format(mainStr.c_str(),threeBtn->getTag() + 1));
            colorNum = 255 - 150*(abs(fristBtn->getPositionX()-secondVec.x) / 180);
//            GAMELOG("***colorNum == %f",colorNum);
            fristBtn->setColor(Color3B(colorNum,colorNum,colorNum));
            colorNum = 255 - 150*(abs(secondBtn->getPositionX()-secondVec.x) / 180);
//            GAMELOG("***colorNum == %f",colorNum);
            secondBtn->setColor(Color3B(colorNum,colorNum,colorNum));
            colorNum = 255 - 150*(abs(threeBtn->getPositionX()-secondVec.x) / 180);
//            GAMELOG("***colorNum == %f",colorNum);
//            GAMELOG("threeBtn position = %f seocnd.x = %f    sum = %f   ddd = %f",threeBtn->getPositionX(),secondVec.x,abs(threeBtn->getPositionX()-secondVec.x),130*abs(threeBtn->getPositionX()-secondVec.x) / 180);
            threeBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        }break;
        case right_up:
        {
//            log("right_up  fris = %d   secon = %d  three = %d",fristBtn->getTag(),secondBtn->getTag(),threeBtn->getTag());
//            secondBtn->loadTexture(StringUtils::format(mainStr.c_str(),secondBtn->getTag() + 1));
//            threeBtn->loadTexture(StringUtils::format(mainStr.c_str(),threeBtn->getTag()));
            colorNum = 255 - 150*(abs(secondBtn->getPositionX()-secondVec.x) / 180);
            secondBtn->setColor(Color3B(colorNum,colorNum,colorNum));
            colorNum = 255 - 150*(abs(threeBtn->getPositionX()-secondVec.x) / 180);
            threeBtn->setColor(Color3B(colorNum,colorNum,colorNum));
        }break;
        default:
            break;
    }
}


void ScrollCycleView::eventCallBack(int tag)
{
//    return;
    IAudio::getInstance()->playEffect(music_ef_pageplay);
    switch(tag)
    {
        case 0: //点go
        {
            IRoomData* room = M_GET_ROOM;
            room->enterGamePlay(GameMode::daily_simple);
            room->Redbaojilu();
        }break;
        case 2: //惊喜吧
        {
            IActivityData* a = M_GET_ACTIVITY;
            a->Activity_status="1";
            a->http_request_activityList();
        }break;
        case 4: //拼乐吧
        {
            UICtrl::getInstance()->gotoScene(UI_PK);
        }break;
        default:
            break;
    }

}



