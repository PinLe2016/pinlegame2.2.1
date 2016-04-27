//
//  UI_Pokedex.cpp
//  PinLe
//
//  Created by Kennedy on 15/7/6.
//
//

#include "UI_Pokedex.h"



Scene* UI_Pokedex::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Pokedex::create();
    
    //    auto layer = PersonCenterLayerEx::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

#define cell_itemcount  3
#define imageMax    9


UI_Pokedex::UI_Pokedex()
{
    _pokedex = M_GET_POKEDEX;
    v_list = _pokedex->getList();
    pageMax = (int)v_list.size() / imageMax;
    if(v_list.size() % imageMax > 0)
        pageMax ++;
    if(pageMax == 0)
        pageMax = 1;
    GAMELOG("pageMax = %d, v_list.size() = %lu", pageMax, v_list.size());
    pageNow = 0;
    IMessageManager::getInstance()->attach(this, SYSTEM_KEYBACK);
}

UI_Pokedex::~UI_Pokedex()
{
    IMessageManager::getInstance()->detach(this, SYSTEM_KEYBACK);
}


enum
{
    B_BACK = 243,
    P_IMAGESHOW = 332,
    B_IMAGESHOW_BACK = 333,
    T_PAGEINFO = 358,
    B_LEFT = 356,
    B_RIGHT = 357,
};




void UI_Pokedex::setImage(int page)
{
    GAMELOG("setImage page = %d", page);
    for(int i = 0; i < imageMax; i ++)
    {
        ImageView* image = (ImageView*)rootNode->getChildByTag(500 + i);
        int n = page*imageMax + i;
        if(n < v_list.size())
        {
            image->setVisible(YES);
            ImageView* pic = (ImageView* )image->getChildByTag(0);
            
            string filename = ITools::Intercept_string(v_list[n]->imgurl, "/");
            string path = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + filename;
            pic->loadTexture(path); 
        }
        else
        {
            image->setVisible(NO);
        }
    }
    Text* pageinfo = (Text*)rootNode->getChildByTag(T_PAGEINFO);
    pageinfo->setString(StringUtils::format("%d / %d", page + 1, pageMax));
}

bool UI_Pokedex::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    Button* b;
    
    rootNode = CSLoader::createNode("CSres/Setup/pokedex2.csb");
    addChild(rootNode);
    
    int arr[] = {B_BACK, B_LEFT, B_RIGHT};
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i ++)
    {
        b = (Button*)rootNode->getChildByTag(arr[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Pokedex::eventCallback, this));
    }
    for(int i = 500; i < 500 + imageMax; i ++)
    {
        Widget* b = (Widget*)rootNode->getChildByTag(i);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Pokedex::eventCallback, this));
    }
    
    _imageShow = rootNode->getChildByTag(P_IMAGESHOW);
    b = (Button*)_imageShow->getChildByTag(B_IMAGESHOW_BACK);
    b->addTouchEventListener(CC_CALLBACK_2(UI_Pokedex::eventCallback, this));
    
    setImage(pageNow);
    
    
    return true;
}


void UI_Pokedex::openImageShow(int index)
{
    index += pageNow*imageMax;
    Node* bg = rootNode->getChildByTag(38);
    bg->setVisible(YES);
    _imageShow->setScale(0.2);
    _imageShow->setVisible(YES);
    ImageView* image = (ImageView*)_imageShow->getChildByTag(37);
    Advertise* a = _pokedex->getAdvertise(index);
    string filename = ITools::Intercept_string(a->imgurl, "/");
    string path = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + filename;
    image->loadTexture(path);
    _imageShow->runAction(ScaleTo::create(0.16, 1.f));
}

void UI_Pokedex::closeImageShow()
{
    Node* bg = rootNode->getChildByTag(38);
    bg->setVisible(NO);
    _imageShow->setVisible(NO);
}


ImageView* UI_Pokedex::getAdvertiseImage(uint i)
{
    Widget* cell = _list->getItem(i / cell_itemcount);
    Button* b = (Button*)cell->getChildByTag(i);
    ImageView* image = (ImageView*)b->getChildByTag(10);
    return image;
}


bool UI_Pokedex::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        Ref* sender = nullptr;
        if(_imageShow->isVisible())
            sender = _imageShow->getChildByTag(B_IMAGESHOW_BACK);
        else
            sender = rootNode->getChildByTag(B_BACK);
        if(sender)
            eventCallback(sender, Widget::TouchEventType::ENDED);
        return YES;
    }

    
    return true;
}


void UI_Pokedex::eventCallback(Ref * pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    
//    if(tag >= 1000)
//    {
//        openImageShow(tag - 1000);
//        return;
//    }
    
    if(tag >= 500 && tag < 500 + imageMax)
    {
        openImageShow(tag - 500);
        return;
    }
    
    switch (tag)
    {
        case B_BACK:
        {
            UICtrl::getInstance()->gotoScene(UI_OPTION);
        }break;
        case B_IMAGESHOW_BACK:
        {
            closeImageShow();
        }break;
        case B_LEFT:
        {
            if(pageNow > 0)
            {
                pageNow --;
                setImage(pageNow);
            }
        }break;
        case B_RIGHT:
        {
            if(pageNow < pageMax - 1)
            {
                pageNow ++;
                setImage(pageNow);
            }
        }break;
        default:break;
    }
}

















