//
//  debris.h
//  PinLe
//
//  Created by HongFeng on 15-2-6.
//
//

#ifndef __PinLe__debris__
#define __PinLe__debris__

#pragma once
#include "cocos2d.h"
USING_NS_CC;
class DebrisSprite : public cocos2d::Sprite
{
public:
    DebrisSprite();
    ~DebrisSprite();
    
    static DebrisSprite* create(const char *pszFileName,int row,int col,int width,int height,float sX,float sY,int posx,int posy);
    void mMoveTo(Vec2 DesP,bool isFirst,FiniteTimeAction *funcn,float srcScale);
    void mSelectScale(bool isHL,FiniteTimeAction *funcn,float srcScale);
    void callbackN(Node* sender);
    int m_nRowIndexSrc;       //原始行序号
    int m_nColIndexSrc;       //原始列序号
    int m_nRowIndexNow;       //当前行序号
    int m_nColIndexNow;       //当前列序号
    
    Vec2 srcPosition;           //用于记录拖动移动的原始位置
    
    
    // Debris(int srcRow,int srcCol,char* PngAdd);
    
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    
    //void initSplitBoard();
    
    // implement the "static create()" method manually
    //CREATE_FUNC(Debris);
};
#endif /* defined(__PinLe__debris__) */
