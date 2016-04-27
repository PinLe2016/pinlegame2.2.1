//
//  debris.cpp
//  PinLe
//
//  Created by HongFeng on 15-2-6.
//
//

#include "DebrisSprite.h"
#include "../../framework/config.h"
//#include "PlayGameLayer.h"
USING_NS_CC;
//Debris::Debris(int srcRow,int srcCol,char* PngAdd){
//
//}
DebrisSprite::DebrisSprite()
{
    
}

DebrisSprite::~DebrisSprite()
{
    
}

DebrisSprite* DebrisSprite::create(const char *pszFileName,int row,int col,int width,int height,float sX,float sY,int posx,int posy)
{
    DebrisSprite *pobSprite = new DebrisSprite();
    pobSprite->srcPosition=Vec2(posx,posy);
    cocos2d::Rect rect = cocos2d::Rect(col*width, row*height,width, height);
    pobSprite->m_nRowIndexSrc=row;
    pobSprite->m_nColIndexSrc=col;
    
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->setScale(sX,sY);
        pobSprite->autorelease();
        //pobSprite->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
        pobSprite->setPosition(pobSprite->srcPosition);
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void DebrisSprite::callbackN(Node* sender)
{
    if(sender != 0) {
        sender->setLocalZOrder(10);
        //((DebrisSprite*)sender)->srcPosition=sender->getPosition();
    }
}

void DebrisSprite::mMoveTo(Vec2 DesP,bool isFirst,FiniteTimeAction *funcn,float srcScale)
{
    ActionInterval * DSmoveTo= CCMoveTo::create(0.3,DesP);
    ActionInterval * DSTint;
    ActionInterval * DSScale;
    ActionInterval * DSScale2;
    //    FiniteTimeAction *funcn = CallFuncN::create(this, callfuncN_selector(PlayGameLayer::callbackN));
    if (isFirst) {
        
        DSScale=CCScaleBy::create(0.1, 1.1);
        DSScale2=CCScaleTo::create(0.1f,srcScale);
        this->setLocalZOrder(11);
//        this->runAction(CCSequence::create(DSScale,DSmoveTo,DSScale2,DSScale->reverse(),funcn,NULL));
//        this->runAction(CCSequence::create(DSScale,DSmoveTo,DSScale2,funcn,NULL));
        
        this->runAction(CCSequence::create(DSScale,DSmoveTo,DSScale2,NULL));
        
    }else
    {
        GAMELOG("%f ~~ %f",this->getPosition().x,this->getPosition().y);
        this->setLocalZOrder(9);
        DSTint=CCTintTo::create(0.1, 100, 100, 100);
        DSScale=CCScaleBy::create(0.1,0.8);
        DSScale2=CCScaleTo::create(0.1,srcScale);
        this->runAction(CCSequence::create(DSScale,DSmoveTo,DSScale2,funcn,NULL));
        
    }
    
    srcPosition=DesP;
    
    
}

//设置碎片高亮
void DebrisSprite::mSelectScale(bool isHL,FiniteTimeAction *funcn,float srcScale)
{
    ActionInterval *DSScale1;
    ActionInterval *DSScale2;
    if (isHL) {
        DSScale1=CCScaleBy::create(0.1, 0.9);
        DSScale2=CCScaleTo::create(0.1, 0.98);
        this->runAction(CCSequence::create(DSScale1,DSScale2,funcn,NULL));
    }else{
        DSScale1=CCScaleBy::create(0.1, 0.9);
        DSScale2=CCScaleTo::create(0.1, srcScale);
        this->runAction(CCSequence::create(DSScale1,DSScale2,funcn,NULL));
    }
    
    
}
