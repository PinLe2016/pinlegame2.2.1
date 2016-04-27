//
//  UI_Pokedex.h
//  PinLe
//
//  Created by Kennedy on 15/7/6.
//
//

#ifndef __PinLe__UI_Pokedex__
#define __PinLe__UI_Pokedex__

#include "../../framework/config.h"
#include "../../gameini/GameIni.h"


class UI_Pokedex : public IUILayer
{
public:
    UI_Pokedex();
    ~UI_Pokedex();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Pokedex);
    
    //    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool Notify(const string& name, Args& args);
    
public:
    IPokedexData*       _pokedex;
    IRoomData*          _room;
    Node*               rootNode;
    Node*               _imageShow;
    ListView*           _list;
    vector<Advertise*> v_list;
    ImageView*      getAdvertiseImage(uint i);
    void            openImageShow(int index);
    void            closeImageShow();
    void            setImage(int page);
    
    int             pageMax;
    int             pageNow;
    
    
    void    eventCallback(Ref * pSender, Widget::TouchEventType type);
};



#endif /* defined(__PinLe__UI_Pokedex__) */
