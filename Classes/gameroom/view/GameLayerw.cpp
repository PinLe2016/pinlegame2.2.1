//
//  GameLayerw.cpp
//  Pinle
//
//  Created by 李辛 on 15/6/1.
//
//

#include "GameLayerw.h"
#include "../../gameini/GameIni.h"
#include "GameScene.h"
#include "../../option/config.h"
#include "../config.h"
#include "../../ThirdParty/C2DXShareSDK/ShareSDKHelper.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

GameLayerw::GameLayerw()
{
    _time=0;
    IUILayer::attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_ROOM_GAMEOVER);
    IMessageManager::getInstance()->attach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->attach(this, MESSAGEBOX_ERROR);
    IMessageManager::getInstance()->attach(this, MES_ROOM_IMAGEDOWNLOAD);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_UPDATEINFO);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->attach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_REWARDGOLDS);
    IMessageManager::getInstance()->attach(this, MES_ROOM_CKXQADDGOLDS);
    IMessageManager::getInstance()->attach(this, MES_ROOM_LAOHUJISHUJU);
    IMessageManager::getInstance()->attach(this, MES_ROOM_HONGBAOPAIHANG);
    
    shiwei_JiLu = 0;
    rootNode = NULL;
    xianshi_WanZhengTu = false;
    iroomData = M_GET_ROOM;
    iplayerData = M_GET_PLAYER;
    chanKanCiShu_jilu = iroomData->ChaKan_cishu;
    iroomData->setGameSuccess(false);
    iroomData->setTimeCount(0);
    iroomData->setRunCount(0);
    iGameState = 0;
    messageBox = nullptr;
    iroomData->lastGols = iplayerData->golds;
    detail_reward = false;
    person_change_fail = false;
    _args.content = "";
    ar_push = false;
    overLayer = NULL;
    
    GAMELOG("***GameLayerw::GameLayerw()");
}
GameLayerw::~GameLayerw()
{
    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_ROOM_GAMEOVER);
    IMessageManager::getInstance()->detach(this, MES_ROOM_HONGBAOPAIHANG);
    IMessageManager::getInstance()->detach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->detach(this, MESSAGEBOX_ERROR);
    IMessageManager::getInstance()->detach(this, MES_ROOM_IMAGEDOWNLOAD);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_UPDATEINFO);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->detach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_REWARDGOLDS);
    IMessageManager::getInstance()->detach(this, MES_ROOM_CKXQADDGOLDS);
    IMessageManager::getInstance()->detach(this, MES_ROOM_LAOHUJISHUJU);
    
    iroomData->enemyVerClear();
    GAMELOG("***GameLayerw::~~~GameLayerw()");
    SimpleAudioEngine::getInstance()->stopAllEffects();//playEffect(music_ef_321);
}

USING_NS_CC;
 using namespace cocostudio::timeline;


// on "init" you need to initialize your instance
bool GameLayerw::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
//    animationInit();//增加步数的动画
    setGameUI(iroomData->getGameType());    //初始化界面UI
    
    timeMap.clear();
    iSwapMode = 0;
    iMoveCount = 0;
    iSelectClickIndex = -1;
    if(iroomData->getGameType() == GameMode::daily_hard || iroomData->getGameType() == GameMode::challenge_hard || iroomData->getGameType() == GameMode::pvp_4 || iroomData->getGameType() == GameMode::pvp_2)
    {
        iRowCount = 5;
        iColCount = 4;
    }
    else
    {
        iRowCount = 4;
        iColCount = 3;
    }
    
    bSelectedNow = false;
    gameLevelSprite = iroomData->getGameInfo();
    
    //我的拼图
    Advertise* a = new Advertise();
    a->adid = gameLevelSprite->ownerid;
    CCLOG("liuyali234  %s",a->adid.c_str());
    a->imgurl = gameLevelSprite->imgurl;
    IPokedexData* _pokedex = M_GET_POKEDEX;
    _pokedex->insert(a);
    
    initSploitBord();       // 初始化拼盘
    gameTimeDowncount();            //游戏倒计时
    
    //showGuide();    //显示引导
    
    IAudio::getInstance()->playBackground(music_bg_game);
    return true;
}



bool GameLayerw::Notify(const string& name, Args& args)
{
    GAMELOG("@@@---==%d", UICtrl::getInstance()->sceneID);
    if(UICtrl::getInstance()->sceneID != UI_ROOM && UICtrl::getInstance()->sceneID != UI_LINK)
        return YES;
    
    if(IUILayer::Notify(name, args) == true)
        return true;
    
    if(name.compare(MES_ROOM_LAOHUJISHUJU) == 0)
    {
        overLayer->jinxiba_over();
    }
    if(name.compare(MES_ROOM_HONGBAOPAIHANG) == 0)
    {
        //overLayer->inithongbao();
    }
    if(name.compare(MES_ROOM_GAMEOVER) == 0)
    {
        this->unschedule(schedule_selector(GameLayerw::pvpGameUpdate));
        unschedule(schedule_selector(GameLayerw::updateGameTime));
        
        if(xianshi_WanZhengTu)
        {
            pWanCheng_Sprite->removeFromParentAndCleanup(true);
            xianshi_WanZhengTu = false;
        }
        
        iroomData->destroyRoomId();
        if(iroomData->getGameType() == GameMode::activity)
        {
            
            Split_scale_callbackN0(0);
            return YES;
            
            if(iroomData->getRewardInfo()->over)
            {
                string path = IMAGEDOWLOAD_PATH_ADREWARD +ITools::Intercept_string(iroomData->getRewardInfo()->goodurl, "/");
                if(IFile::isFileExist(path))
//                    initActivityOver();
                    Split_scale_callbackN0(0);
                else
                {
                    iroomData->downLoadImage(iroomData->getRewardInfo()->goodurl,IMAGE_ADREWARD_FINDER);
                }
            }
            else
            {
                IActivityData* a = M_GET_ACTIVITY;
//                iroomData->enterGamePlay(iroomData->getGameType(),iroomData->getActivityId());
                iroomData->enterGamePlay(iroomData->getGameType(), a->getSelectedActivity());

            }
        }
        else
        {
            Split_scale_callbackN0(0);
        }
        
        return true;
    }
    
    if(name.compare(MES_ROOM_GAMESTART) == 0)
    {
        GAMELOG("***MES_ROOM_GAMESTART");
        UICtrl::getInstance()->gotoScene(UI_ROOM);
        return true;
    }
    
    if(name.compare(MESSAGEBOX_OK) == 0)
    {
        
        if(args.content.compare(change_reward_info) == 0)
        {
            
            btnTouchEnd((Button*)gameOverNode->getChildByTag(80),Widget::TouchEventType::ENDED);
            messageBox = nullptr;
            return true;
            
        }
        
        if(args.code == mesbox_back_room)
        {
            if(iroomData->getGameType() >= 4 && iroomData->getGameType() <= 8)
            {
                UICtrl::getInstance()->gotoScene(UI_PK);
            }
            else
            {
                UICtrl::getInstance()->gotoScene(UI_MAIN, _args);
            }
            messageBox = nullptr;
            return true;
        }
        
        if(args.code == mesbox_details)
        {
            
            Args args;
            args.content = iroomData->getGameInfo()->rewardurl;
            UICtrl::getInstance()->gotoScene(UI_LINK, args,false);
            GAMELOG("*** 查看已经拼好的原图url == %s", iroomData->getGameInfo()->rewardurl.c_str());
            if(!detail_reward)
            {
                iplayerData->http_request_setReward((int)GameReward::advertiseDetails,iroomData->getGameInfo()->adid);
            }
//            detail_reward = true;
            messageBox = nullptr;
            return true;
//            if(iroomData->getGameType() == GameMode::activity)
//            {
//                Application::getInstance()->openURL(iroomData->getRewardInfo()->adweburl);
//                
//                if(!detail_reward)
//                {
//                    iplayerData->http_request_setReward((int)GameReward::activityDetails, iroomData->getActivityId());
//                }
//            }
//            else
//            {
//                Application::getInstance()->openURL(iroomData->getGameInfo()->rewardurl);
//                GAMELOG("*** url == %s", iroomData->getGameInfo()->rewardurl.c_str());
//                if(!detail_reward)
//                {
//                    iplayerData->http_request_setReward((int)GameReward::advertiseDetails,iroomData->getGameInfo()->adid);
//                }
//            }
//            detail_reward = true;
//            messageBox = nullptr;
//            return true;
        }
        
        if(args.err_code != 0)
        {
            UICtrl::getInstance()->gotoScene(UI_MAIN,_args);
            messageBox = nullptr;
            return true;
        }
        
        if(args.code == mesbox_reward_room)
        {
            UICtrl::getInstance()->gotoScene(UI_MAIN, _args);
            messageBox = nullptr;
            return true;
        }
        messageBox = nullptr;
        return true;
    }
    if(name.compare(MESSAGEBOX_CANCEL) == 0)
    {
        messageBox = nullptr;
        return true;
    }
    
//    if(name.compare(MES_OPTION_CLOSE) == 0)
//    {
//        reloadActivityInfo();
//        return YES;
//    }
    
    if(name.compare(MES_ROOM_SUBMITREWARE) == 0)
    {
        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_getreward));
        messageBox->_args.code = mesbox_reward_room;
        return YES;
    }
    
    
    if(name.compare(MES_ROOM_IMAGEDOWNLOAD) == 0)
    {
//        initActivityOver();
        Split_scale_callbackN0(0);
        return YES;
    }
    
    
    if(name.compare(SYSTEM_KEYBACK) == 0)
    {
        
        if(overLayer)
        {
            GAMELOG("-----#######if(overLayer)");
            if(overLayer->rootNode->getChildByTag(240)->getChildByTag(246)->isVisible() && (overLayer->getTag() == 1))
            {
                overLayer->buttonTouchEnd(overLayer->rootNode->getChildByTag(240)->getChildByTag(246),Widget::TouchEventType::ENDED);
                overLayer->setTag(11);
            }
            return true;
        }
        
        
        if(messageBox == nullptr)
        {
            btnTouchEnd(gameBackBtn, Widget::TouchEventType::ENDED);
        }
        else
        {
            if(ar_push)
            {
                return true;
            }
            messageBox->close();
            messageBox = nullptr;
        }
        
        return true;
    }
    
    if(name.compare(MES_PLAYER_UPDATEINFO) == 0)
    {
        updatePlayerInfo();
        return YES;
    }
    
    if(name.compare(MES_ACTIVITY_GETIMAGEDONE) == 0)
    {
        UICtrl::getInstance()->gotoScene(UI_ACTIVITY);
        return YES;
    }
    
    if(name.compare(MES_GAMEINI_SCREENSHOOT_SUCCEED) == 0)
    {
        ShareSDKHelper::getInstance()->sendImage(GameIni::getInstance()->shareSdkInfo.title,
                                                 GameIni::getInstance()->shareSdkInfo.description,
                                                 args.content);
        GAMELOG("imagefile == %s", args.content.c_str());
        return true;
    }
    
    if(name.compare(MES_PLAYER_REWARDGOLDS) == 0)
    {
        
    }
    
    if(name.compare(MES_ROOM_CKXQADDGOLDS) == 0)
    {
        
        this->runAction(Sequence::create(
                                         DelayTime::create(0.5),
                                         CallFunc::create([=](){
            if(iplayerData->tempAddGolds > 0 && !detail_reward)
            {
                detail_reward = true;
                UIMessage::createIconAnition(addicon, (abs)(iplayerData->tempAddGolds));
            }
            
            
            
            if(overLayer)
            {
                if(iroomData->getGameType() != GameMode::activity && iroomData->getGameType() != GameMode::pvp_2 && iroomData->getGameType() != GameMode::pvp_4)
                    if(overLayer->playerGold != nullptr && overLayer->playerGold->isVisible())
                        overLayer->playerGold->setString(StringUtils::format("%d", iplayerData->golds));
            }
        }),
                                         NULL));
        
        
        return true;
    }
    
    return true;
}

bool GameLayerw::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(isGuide)
    {
        return false;
    }
    
    if(xianshi_WanZhengTu)
    {
        return true;
    }
    GAMELOG("****GameLayerw::onTouchBegan");
    checkTouchPoint(touch->getLocation(), true);
    return true;
}

void GameLayerw::onTouchMoved(Touch *touch, Event *unused_event)
{
    if(xianshi_WanZhengTu)
    {
        return;
    }
    if(iSelectClickIndex >= 0)
    {
        DebrisSprite *srcDS = (DebrisSprite*)pSplitBoard->getChildByTag(100 + iSelectClickIndex);
        touchMoveDebris(touch->getLocation(),srcDS);
    }
}
void GameLayerw::onTouchEnded(Touch *touch, Event *unused_event)
{
    if(iSelectClickIndex >= 0)
    {
        checkTouchPoint(touch->getLocation(), false);
    }
    
    if(xianshi_WanZhengTu)
    {
        pWanCheng_Sprite->removeFromParentAndCleanup(true);
        xianshi_WanZhengTu = false;
        return;
    }
}

int GameLayerw::checkTouchPoint(Vec2 _touchP, bool isTouchDown)     //检测点击的焦点
{
    if(iGameState != 2)
    {
        return 0;
    }
    int touchIndex;
    if(isTouchDown)     //触摸安下
    {
        if(iSwapMode == swap_touch)  //点击交换模式
        {
            iSwapMode = swap_none;  //不做操作
        }
        
        if(iSwapMode == swap_none)     //纪录触摸按下时所在碎片序号
        {
            IAudio::getInstance()->playEffect(music_ef_move);
            touchIndex = getTouchDebris(_touchP);
            if(touchIndex == -1)
            {
                return 0;
            }
            iSelectClickIndex = touchIndex;
            touchdownP = _touchP;
            DebrisSprite *srcDs = (DebrisSprite*)pSplitBoard->getChildByTag(100 + touchIndex);
            TPoffsetWithDS=Vec2(touchdownP.x-srcDs->srcPosition.x,touchdownP.y-srcDs->srcPosition.y);//触摸按下时相对于被选中碎片的位置
            iSwapMode = swap_touch;
        }
        
    }
    else
    {
        IAudio::getInstance()->playEffect(music_ef_droppic);
        
        if(iSwapMode == swap_touch)
        {
            touchIndex = getTouchDebris(_touchP);
            if(bSelectedNow)//已选择
            {
                if(touchIndex == iSelectDebrisIndex)    //还是自己，取消点亮
                {
                    swapDebris(iSelectDebrisIndex, -1, swap_cancel);
                }
                else
                {
                    //点击交换
                    swapDebris(iSelectDebrisIndex, touchIndex, swap_touch);
                    
                }
                bSelectedNow = false;
                iSwapMode = swap_none;
                iSelectClickIndex = -1;
            }
            else
            {
                iSelectDebrisIndex = touchIndex;
                swapDebris(iSelectDebrisIndex, -1, swap_select);
                bSelectedNow = true;
            }
        }
        
        if(iSwapMode == swap_move) //触摸移动
        {
            touchIndex = getTouchDebris(_touchP);
            if(touchIndex == iSelectClickIndex)     //还是在自己的位置
            {
                swapDebris(iSelectClickIndex, -1, swap_none);
            }
            else
            {
                if(touchIndex == -1)//点击在外部
                {
                    swapDebris(iSelectClickIndex, -1, swap_none);
                }
                else
                {
                    swapDebris(iSelectClickIndex, touchIndex, swap_move);
                }
                
            }
            bSelectedNow = false;
            iSwapMode = swap_none;
            iSelectClickIndex = -1;
        }
    }
    return 0;
}

void GameLayerw::swapDebris(int srcIndex, int desIndex, int swapType)  //交换图片
{
    if(srcIndex == -1)
    {
        iSwapMode = swap_none;
        iSelectClickIndex = -1;
        return;
    }

    DebrisSprite *srcDS = (DebrisSprite*)pSplitBoard->getChildByTag(100 + srcIndex);
    DebrisSprite *desDS;
    Vec2 srcP = srcDS->srcPosition;
    Vec2 desP;

    if(desIndex != -1)
    {
        desDS = (DebrisSprite*)pSplitBoard->getChildByTag(100 + desIndex);
        desP = desDS->srcPosition;
        if(swapType == swap_move)
        {
            srcDS->setPosition(desP);
            srcDS->setLocalZOrder(10);
            srcDS->srcPosition = desP;
            iGameState = 4;
            desDS->mMoveTo(srcP,false,CallFuncN::create(this, callfuncN_selector(GameLayerw::callbackN)),fScaleX);
            (iMoveCount + 1 > 99) ? (iMoveCount = 99) : (iMoveCount++);
            
            long nowgametime = ITools::GetNowSec();
            timeMap.insert(pair<int,long>(iMoveCount,nowgametime-loneStepTime));
            loneStepTime = nowgametime;
            
            int srcDSrowIndex = srcDS->m_nRowIndexNow;
            int srcDScolIndex = srcDS->m_nColIndexNow;
            srcDS->m_nRowIndexNow = desDS->m_nRowIndexNow;
            srcDS->m_nColIndexNow = desDS->m_nColIndexNow;
            desDS->m_nRowIndexNow = srcDSrowIndex;
            desDS->m_nColIndexNow = srcDScolIndex;
        }
        if(swapType == swap_touch)
        {
            iGameState=4;
            srcDS->setScale(fScaleX,fScaleY);
            srcDS->mMoveTo(desP,true,CallFuncN::create(this, callfuncN_selector(GameLayerw::callbackN)),fScaleX);
            desDS->mMoveTo(srcP,false,CallFuncN::create(this, callfuncN_selector(GameLayerw::callbackN)),fScaleX);
            iSwapMode = swap_none;
            iSelectClickIndex = -1;
            (iMoveCount + 1 > 99) ? (iMoveCount = 99) : (iMoveCount ++);

            long nowgametime = ITools::GetNowSec();
            timeMap.insert(pair<int,long>(iMoveCount,nowgametime-loneStepTime));
            loneStepTime = nowgametime;

            //改变碎片当前行列序号
            int srcDSrowIndex=srcDS->m_nRowIndexNow;
            int srcDScolIndex=srcDS->m_nColIndexNow;
            srcDS->m_nRowIndexNow=desDS->m_nRowIndexNow;
            srcDS->m_nColIndexNow=desDS->m_nColIndexNow;
            desDS->m_nRowIndexNow=srcDSrowIndex;
            desDS->m_nColIndexNow=srcDScolIndex;
        }
        
        updateStepNum();
    }
    
    switch(swapType)
    {
        case swap_none:
        {
            srcDS->setPosition(srcDS->srcPosition);
            srcDS->setLocalZOrder(10);
        }break;
        case swap_touch:
        {
            srcDS->setLocalZOrder(10);
            srcDS->setScale(fScaleX, fScaleY);
            
            desDS->setLocalZOrder(10);
            desDS->setScale(fScaleX, fScaleY);
        }break;
        case swap_move:
        {
            srcDS->setLocalZOrder(10);
            srcDS->setScale(fScaleX, fScaleY);
            
            desDS->setLocalZOrder(10);
            desDS->setScale(fScaleX, fScaleY);
        }break;
        case swap_select:
        {
            iGameState = 4;
            srcDS->setLocalZOrder(11);
            srcDS->mSelectScale(true,CallFuncN::create(this, callfuncN_selector(GameLayerw::callbackN)),fScaleX);
        }break;
        case swap_cancel:
        {
            iGameState = 4;
            srcDS->setLocalZOrder(10);
            srcDS->mSelectScale(false,CallFuncN::create(this, callfuncN_selector(GameLayerw::callbackN)),fScaleX);
        }break;
        case swap_onlycancel:
        {
            srcDS->setLocalZOrder(10);
            srcDS->setScale(fScaleX, fScaleY);
        }break;
        default:
            break;
    }
    
}

void GameLayerw::callbackN(Node *sender)   //游戏结算回调
{
    if(person_change_fail)
        return;
    iGameState = 2;
    if(sender != 0)
    {
        sender->setLocalZOrder(10);
    }
    if(checkWin())
    {
        GAMELOG("***callbackN");
        this->unschedule(schedule_selector(GameLayerw::updateGameTime));
        this->unschedule(schedule_selector(GameLayerw::pvpGameUpdate));
        string sss = ITools::GetYMDHMS();
        iroomData->setEndTime(sss);
        iroomData->setGameSuccess(true);
        setTouchEnabled(false);
        if(iroomData->getGameType() == GameMode::pvp_2 || iroomData->getGameType() == GameMode::pvp_4)
        {
            pvpGameUpdate(0);
//            this->unschedule(schedule_selector(GameLayerw::updateGameTime));
        }
        else
        {
            iroomData->requestOverGame();//Redbaojilu
            //iroomData->Redbaojilu();
        }
    }
}

bool GameLayerw::checkWin()            //检测游戏是否成功
{
    for(int i = 0; i < iRowCount * iColCount; i++)
    {
        DebrisSprite *srcDS = (DebrisSprite*)pSplitBoard->getChildByTag(100 + i);
        if(srcDS->m_nRowIndexSrc != srcDS->m_nRowIndexNow || srcDS->m_nColIndexSrc != srcDS->m_nColIndexNow)
        {
            return false;
        }
    }
    return true;
}

void GameLayerw::updateStepNum()       //更新拼图步数
{
    iroomData->setRunCount(iMoveCount);
    stepNumLabel->setString(StringUtils::format("%d",iMoveCount));
//
//    int gewei = iMoveCount%10;
//    if (gewei != 0) {
//        gewei -= 1;
//    }else{
//        gewei = 9;
//    }
//    
//    int shiwei = iMoveCount/10;
//    if (shiwei != 0) {
//        shiwei -= 1;
//    }else{
//        shiwei = 9;
//    }
//    
//    if (shiwei_JiLu != shiwei) {
//        shiwei_JiLu = shiwei;
//        if (shiwei != 9) {
//            armature_shiwei->getAnimation()->playWithIndex(shiwei);
//        }
//    }
//    armature_gewei->getAnimation()->playWithIndex(gewei);
}

int GameLayerw::getTouchDebris(Vec2 _touchP)       //获取点击碎片
{
    for (int i=0;i<iRowCount*iColCount;i++) {
        DebrisSprite *SrcDS=(DebrisSprite*)pSplitBoard->getChildByTag(100+i);
        if(_touchP.x>SrcDS->srcPosition.x-iDebrisWidth/2 && _touchP.x<SrcDS->srcPosition.x+iDebrisWidth/2
           &&_touchP.y>SrcDS->srcPosition.y-iDebrisHeight/2&&_touchP.y<SrcDS->srcPosition.y+iDebrisHeight/2)
        {
            return i;
        }
    }
    return -1;
}


void GameLayerw::touchMoveDebris(Vec2 touchP, DebrisSprite *DS)    //移动碎片
{
    //限制四周超出拼盘
    if (touchP.x<BASE_POINT_X+fHalfDebrisWidth+TPoffsetWithDS.x+10)
    {
        touchP.x=BASE_POINT_X+fHalfDebrisWidth+TPoffsetWithDS.x+10;
    }
    if (touchP.x>BASE_POINT_X+PLAY_AREA_WIDTH-fHalfDebrisWidth+TPoffsetWithDS.x-9)
    {
        touchP.x=BASE_POINT_X+PLAY_AREA_WIDTH-fHalfDebrisWidth+TPoffsetWithDS.x-9;
    }
    if (touchP.y>BASE_POINT_Y-fHalfDebrisHeight+TPoffsetWithDS.y-9)
    {
        touchP.y=BASE_POINT_Y-fHalfDebrisHeight+TPoffsetWithDS.y-9;
    }
    if (touchP.y<BASE_POINT_Y-PLAY_AREA_HEIGHT+fHalfDebrisHeight+TPoffsetWithDS.y+8)
    {
        touchP.y=BASE_POINT_Y-PLAY_AREA_HEIGHT+fHalfDebrisHeight+TPoffsetWithDS.y+8;
    }
    
    
    
    float moveDVx=touchP.x-touchdownP.x;
    float moveDVy=touchP.y-touchdownP.y;
    
    if (abs(moveDVx)>1||abs(moveDVy)>1) {
        iSwapMode=2;
        DS->setLocalZOrder(11);
        //边界限制
        float newPx=DS->srcPosition.x+moveDVx;
        float newPy=DS->srcPosition.y+moveDVy;
        DS->setPosition(newPx,newPy);
        //取消高亮
        if (bSelectedNow) {
            swapDebris(iSelectDebrisIndex,-1,swap_onlycancel);//取消点击高亮
            bSelectedNow=false;
        }
    }
}

void GameLayerw::animationInit()
{
    ArmatureDataManager::getInstance()->addArmatureFileInfo("res/CSres/Battle/resAnimation/bushu.ExportJson");
    //创建动画
    armature_gewei = Armature::create("bushu");
    armature_gewei->setPosition(Point(647,1235));
    armature_gewei->setScale(0.65);
    this->addChild(armature_gewei,2);
    
    armature_shiwei = Armature::create("bushu");
    armature_shiwei->setPosition(Point(607,1235));
    armature_shiwei->setScale(0.65);
    this->addChild(armature_shiwei,2);
    
}

void GameLayerw::setGameUI(int _type)
{
    pSplitBoard = Layer::create();
    this->addChild(pSplitBoard,10,33);
    
    if(_type == pvp_2 || _type == pvp_4)
    {
        float f = 0;
        if(_type == pvp_2)
        {
            f = GameIni::getInstance()->space_of_war2;
        }
        else if (_type == pvp_4)
        {
            f = GameIni::getInstance()->space_of_war4;
        }
        schedule(schedule_selector(GameLayerw::pvpGameUpdate),f);
        GAMELOG("***fffff == %f",f);
        _args.content = ui_pinleba;
    }
    
    if(rootNode != NULL)
    {
        removeChild(rootNode);
        rootNode = NULL;
    }
    rootNode = CSLoader::createNode("res/CSres/Battle/XSHGameScene1.csb");
    addChild(rootNode);
    
    //玩家头像
    headSp = (Sprite*)rootNode->getChildByName("Sprite_2");
    string headpath = StringUtils::format("res/CSres/Setup/Head/%s",iplayerData->uHeadIndex.c_str());
    headSp->setTexture(headpath);
    
    //玩家昵称
    playerNick = (Text*)rootNode->getChildByName("Text_1");
    playerNick->setString(iplayerData->nickname.c_str());
    
    //玩家金币
    playerGold = (Text*)rootNode->getChildByName("Text_2");
    string str = StringUtils::format("%d",iplayerData->golds);
    playerGold->setString(str.c_str());
    
    //玩家等级
    playerRank = (Text*)rootNode->getChildByName("Text_1_1");
    playerRank->setString(iplayerData->rankname);
    
    _timeText = (Text*)rootNode->getChildByName("Text_7");      //时间
    if(iroomData->getGameType() == GameMode::challenge_hard || iroomData->getGameType() == GameMode::challenge_simple)
    {
        int tm = round(iroomData->getLevelType(iroomData->getGameType())->ilimitTime / 60);    // 分
        int ts = round(iroomData->getLevelType(iroomData->getGameType())->ilimitTime  % 60);    //秒
        string str = StringUtils::format("%02d:%02d",tm,ts);
        _timeText->setString(str);
        _args.content = ui_pinleba;
    }
    
    if(iroomData->getGameType() == GameMode::activity)
    {
        _args.content = ui_jinxiba;
    }
    
    
    stepNumLabel = (Text*)rootNode->getChildByTag(264);         //步数
    stepNumLabel->setString("0");
    
    gradeBar = (LoadingBar*)rootNode->getChildByTag(261);       //等级条
    gradeBar->setPercent(iplayerData->getPecentScore()*100);
    
    //返回
    gameBackBtn = (Button*)rootNode->getChildByName("Button_9");
    gameBackBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
    
    //查看原图
    checkPictureBtn = (Button*)rootNode->getChildByName("Button_9_0");
    checkPictureBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd,this));
    Sprite *checkPictureSp = Sprite::create("res/CSres/Battle/FreeRes/NEW_YJ_DIAN.png");
    checkPictureSp->setPosition(Vec2(checkPictureBtn->getContentSize().width,checkPictureBtn->getContentSize().height));
    checkPictureBtn->addChild(checkPictureSp);
    
    checkPictureLabel = Label::create();
    checkPictureLabel->setSystemFontSize(28);
    checkPictureLabel->setString(StringUtils::format("%d",chanKanCiShu_jilu));
    checkPictureLabel->setPosition(Vec2(checkPictureSp->getContentSize().width/2,checkPictureSp->getContentSize().height/2));
    checkPictureSp->addChild(checkPictureLabel);
    
    
    //声音按钮
    gameVoiceBtn = (Button*)rootNode->getChildByName("Button_9_1");
    gameVoiceBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
    voiceSprite = Sprite::create("res/CSres/Battle/FreeRes/cha.png");
    voiceSprite->setPosition(Vec2(gameVoiceBtn->getContentSize().width/2,gameVoiceBtn->getContentSize().height/2));
    gameVoiceBtn->addChild(voiceSprite);
    voiceSprite->setVisible(false);
    voiceOL = UserDefault::getInstance()->getBoolForKey(music_key, YES);
    if (!voiceOL)
    {
        voiceSprite->setVisible(true);
    }
    
    
    
    
}

void GameLayerw::updatePlayerInfo()    //更新玩家最新信息
{
    
    string headpath = StringUtils::format("res/CSres/Setup/Head/%s",iplayerData->uHeadIndex.c_str());
    headSp->setTexture(headpath);   //玩家头像
    
    playerNick->setString(iplayerData->nickname);   //玩家昵称
    
    string str = StringUtils::format("%d",iplayerData->golds);
    playerGold->setString(str);     //玩家金币
    
    playerRank->setString(iplayerData->rankname);   //玩家等级
}

void GameLayerw::gameBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void GameLayerw::checkPicture(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void GameLayerw::gameVoice(Ref *pSender,Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void GameLayerw::pvpGameUpdate(float ft)
{
//    GAMELOG("***pvpGameUpdate");
//    map<int,long>::iterator tempMap;
//    for(tempMap = timeMap.begin();tempMap != timeMap.end();tempMap++)
//    {
//        rapidjson::Value temp(rapidjson::kObjectType);
//        int sn = tempMap->first;
//        long timeNum = tempMap->second;
//
//        GAMELOG("sn == %d   ------  timeNum = %ld", sn, timeNum);
//    }
    
    iroomData->setMapData(timeMap);
    iroomData->commomGameStart();
    timeMap.clear();
}

void GameLayerw::initSploitBord()
{
    string path = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" +ITools::Intercept_string(gameLevelSprite->imgurl, "/");
    GAMELOG("GameImagePath == %s", path.c_str());
    
    iDebrisWidth = PLAY_AREA_WIDTH / iColCount;
    iDebrisHeight = PLAY_AREA_HEIGHT / iRowCount;
    
    int tempSrcDebrisWidth = PICTURE_WIDTH / iColCount;
    int tempSrcDebrisHeight = PICTURE_HEIGTH / iRowCount;
    
    int spIndex = 0;
    int posx;
    int posy;
    
    fScaleX = (float)iDebrisWidth / (float)tempSrcDebrisWidth;
    fScaleY = (float)iDebrisHeight / (float)tempSrcDebrisHeight;
    
    fHalfDebrisWidth = iDebrisWidth / 2 * fScaleX;
    fHalfDebrisHeight = iDebrisHeight / 2 * fScaleY;
    
    for(int i = 0; i < iRowCount; i++)
    {
        for(int j = 0; j < iColCount; j++)
        {
            posx = BASE_POINT_X + iDebrisWidth / 2 +(iDebrisWidth + PICTURE_CLEARANCE) * j;
            posy = BASE_POINT_Y - iDebrisHeight / 2 - (iDebrisHeight + PICTURE_CLEARANCE) * i;
            debrisIndex[spIndex] = spIndex;
            DebrisSprite *debrisSprite = DebrisSprite::create(path.c_str(), i, j,tempSrcDebrisWidth,tempSrcDebrisHeight,fScaleX,fScaleY,posx,posy);
            if (debrisSprite==NULL) {
                debrisSprite = DebrisSprite::create("0e9f4a9a-fcbe-47bc-bedb-3d5410be9a51.jpg", i, j,tempSrcDebrisWidth,tempSrcDebrisHeight,fScaleX,fScaleY,posx,posy);
            }
            pSplitBoard->addChild(debrisSprite, 10, 100+spIndex);
            debrisSprite->setVisible(true);
            debrisSprite->m_nRowIndexSrc=i;
            debrisSprite->m_nColIndexSrc=j;
            debrisSprite->m_nRowIndexNow=i;
            debrisSprite->m_nColIndexNow=j;
            spIndex++;
        }
    }
    
}

void GameLayerw::gameStart()   //游戏开始，倒计时
{
//    randomMoveDebris(11);
    iGameState = 2;
    loneStepTime = ITools::GetNowSec();
    lgameStartTime = ITools::GetNowSec();
    string sss = ITools::GetYMDHMS();
    iroomData->setStartTime(sss);
    this->removeChildByTag(1011);
    setTouchEnabled(true);
    schedule(schedule_selector(GameLayerw::updateGameTime),1.0f);
}

void GameLayerw::updateGameTime(float ft)  //更新游戏所用时间
{
    if(iroomData->getGameType() == GameMode::challenge_simple || iroomData->getGameType() == GameMode::challenge_hard)
    {
        showTimeForPersonal();
    }
    else
    {
        showTimeForOther();
    }
    
}

void GameLayerw::showTimeForPersonal()     //显示个人赛时间
{
    long nowsec = ITools::GetNowSec();  //获取设备时间
    long time_consuming = (nowsec - lgameStartTime)/1000;   //游戏消耗时间
    int remain_sec;
    int timeCount = (int)time_consuming;
    
    
    
    if(iroomData->getGameType() == GameMode::challenge_hard)
    {
        remain_sec = iroomData->getLevelType(iroomData->getGameType())->ilimitTime - timeCount;
    }
    else if(iroomData->getGameType() == GameMode::challenge_simple)
    {
        remain_sec = iroomData->getLevelType(iroomData->getGameType())->ilimitTime - timeCount;
    }
    
//    remain_sec = 1;
    if(remain_sec < 0)
    {
        remain_sec = 0;
    }
    
    iroomData->setTimeCount(iroomData->getLevelType(iroomData->getGameType())->ilimitTime - remain_sec);
    
    int tm = round(remain_sec / 60);    // 分
    int ts = round(remain_sec % 60);    //秒
    string str = StringUtils::format("%02d:%02d",tm,ts);
    _timeText->setString(str);
    
    if(remain_sec <= 0)
    {
        person_change_fail = true;
        setTouchEnabled(false);
        iroomData->setGameSuccess(false);
        this->unschedule(schedule_selector(GameLayerw::updateGameTime));
        
        for(int i = 0; i < iRowCount * iColCount; i++)
        {            
            DebrisSprite *srcDS = (DebrisSprite*)pSplitBoard->getChildByTag(100 + i);
            srcDS->m_nColIndexSrc = srcDS->m_nColIndexNow;
            srcDS->m_nRowIndexSrc = srcDS->m_nRowIndexNow;
        }
//        this->callbackN(0);
        iroomData->requestOverGame();
        //iroomData->Redbaojilu();

    }
}
void GameLayerw::showTimeForOther()        //显示其他比赛时间
{
    long nowsec = ITools::GetNowSec();
    long time_consuming = (nowsec - lgameStartTime) / 1000;
    
    int timeCount = (int)time_consuming;
    if(timeCount < 0)
    {
        timeCount = 0;
    }
    iroomData->setTimeCount((int)(timeCount));
    int tm = round(timeCount / 60);
    int ts = round(timeCount % 60);
    
    string str = StringUtils::format("%02d:%02d",tm,ts);
    _timeText->setString(str);
    
}



void GameLayerw::gameTimeDowncount()   //游戏倒计时动画
{
    iGameState = 1;
    Node *gameStartAction = CSLoader::createNode("res/CSres/Battle/Battle_Start.csb");
//    gameStartAction->setPosition(Vec2(33,60));
    gameStartAction->setTag(1102);
    gameStartAction->setPosition(gameStartAction->getPosition() + Vec2(2,68));
    this->addChild(gameStartAction,15,1011);
    auto startAction=CSLoader::createTimeline("res/CSres/Battle/Battle_Start.csb");
    gameStartAction->runAction(startAction);
    startAction->gotoFrameAndPlay(0,false);
    startAction->setLastFrameCallFunc(CC_CALLBACK_0( GameLayerw::gameStart, this));
    
    runAction(Sequence::create(
                               DelayTime::create(1.0f),
                               CallFuncN::create(CC_CALLBACK_0(GameLayerw::openMusic321, this)),
                               DelayTime::create(5.3f),
                               CallFuncN::create(CC_CALLBACK_1(GameLayerw::randomMoveDebris, this, _RandomMoveDebris_)),
                               NULL));
}


void GameLayerw::openMusic321()    //声音延迟播放openMusic321
{
   effectID= SimpleAudioEngine::getInstance()->playEffect(music_ef_321);
}


void GameLayerw::randomMoveDebris(Node *psender, int count) //打乱碎片顺序
{
//    timeval psv;
//    unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000;    // 初始化随机数
//    srand( tsrans );
//    return;
    int ranIndex1;
    int ranIndex2;
    
    if(iroomData->getGameType() == GameMode::pvp_2 || iroomData->getGameType() == GameMode::pvp_4)
    {
        setMorePersonDebris();
    }
    else
    {
        for (int i=0;i<count;i++)
        {
            ranIndex1 = ITools::getRand(0,iRowCount*iColCount-1);
            ranIndex2 = ITools::getRand(0,iColCount*iColCount-1);
            
            DebrisSprite *SrcDS = (DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[ranIndex1]); //获取被交换碎片指针
            DebrisSprite *DesDS = (DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[ranIndex2]); //获取被交换碎片指针;
            
            int ranTemp = ITools::getRand(0,2) + 2;
            if(i < 12)
            {
                ranIndex1 = i;
                ranIndex2 = (i + ranTemp) % 12;
            }
            else
            {
                if(SrcDS->m_nRowIndexSrc == DesDS->m_nRowIndexNow && SrcDS->m_nColIndexSrc == DesDS->m_nColIndexNow)
                {
                    ranIndex1 = (ranIndex1 + 2) % 12;
                }
                if(DesDS->m_nRowIndexSrc == SrcDS->m_nRowIndexNow && DesDS->m_nColIndexSrc == SrcDS->m_nColIndexNow)
                {
                    ranIndex2 = (ranIndex2 + 2) % 12;
                }
            }
            
            if (ranIndex1!=ranIndex2) {
                
                SrcDS=(DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[ranIndex1]); //获取被交换碎片指针
                DesDS=(DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[ranIndex2]); //获取被交换碎片指针;
                
                int srcTag=SrcDS->getTag();
                int desTag=DesDS->getTag();
                //交换双方的Tag值，
                SrcDS->setTag(desTag);
                DesDS->setTag(srcTag);
                
                //交换双方位置
                SrcDS->setPosition(DesDS->srcPosition);
                DesDS->setPosition(SrcDS->srcPosition);
                Vec2 tempv=SrcDS->srcPosition;
                SrcDS->srcPosition=DesDS->srcPosition;
                DesDS->srcPosition=tempv;
                //改变碎片当前行列序号
                
                int srcDSrowIndex=SrcDS->m_nRowIndexNow;
                int srcDScolIndex=SrcDS->m_nColIndexNow;
                SrcDS->m_nRowIndexNow=DesDS->m_nRowIndexNow;
                SrcDS->m_nColIndexNow=DesDS->m_nColIndexNow;
                DesDS->m_nRowIndexNow=srcDSrowIndex;
                DesDS->m_nColIndexNow=srcDScolIndex;
                
                //记录打乱序号
                int tempindex=debrisIndex[ranIndex1];
                debrisIndex[ranIndex1]=debrisIndex[ranIndex2];
                debrisIndex[ranIndex2]=tempindex;
            }
        }
    }
}


void GameLayerw::setMorePersonDebris()     //多人对战碎片打乱
{
    if(_RandomMoveDebris_ <= 1)
        return;
    
    int srcTagInt[debirsNum];
    Vec2 vecPosition[debirsNum];
    int srcDSrowIndexInt[debirsNum];
    int srcDScolIndexInt[debirsNum];
    
    for(int i=0;i<debirsNum;i++)
    {
        DebrisSprite *SrcDS=(DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[i]); //获取被交换碎片指针
        srcTagInt[i] = SrcDS->getTag();
        vecPosition[i] = SrcDS->getPosition();
        srcDSrowIndexInt[i]=SrcDS->m_nRowIndexNow;
        srcDScolIndexInt[i]=SrcDS->m_nColIndexNow;
    }
    
    DebrisSprite *SrcDS1[debirsNum];
    for(int i=0;i<debirsNum;i++)
    {
        SrcDS1[i]=(DebrisSprite*)pSplitBoard->getChildByTag(100+debrisIndex[i]);
    }
    
    for(int i=0;i<debirsNum;i++)
    {
        
        int tempInt = iroomData->getGameInfo()->spireorder[i] - 1;
        SrcDS1[tempInt]->setTag(srcTagInt[i]);
        SrcDS1[tempInt]->setPosition(vecPosition[i]);
        SrcDS1[tempInt]->srcPosition = vecPosition[i];
        SrcDS1[tempInt]->m_nRowIndexNow = srcDSrowIndexInt[i];
        SrcDS1[tempInt]->m_nColIndexNow = srcDScolIndexInt[i];
    }
}

void GameLayerw::Split_scale_callbackN0(float dt)
{
    LayerColor *layerColor = LayerColor::create(Color4B(0,0,0,200));
    addChild(layerColor,33);
    overLayer = OverLayer::create();
    overLayer->setTag(1);
    overLayer->setScale(0.01);
    addChild(overLayer,35);
    overLayer->runAction(
                         Sequence::create(
                                          ScaleTo::create(0.1,0.25),
                                          ScaleTo::create(0.1,0.5),
                                          ScaleTo::create(0.3,0.75),
                                          ScaleTo::create(0.3,1.0),
                                          CallFuncN::create([=](Node*){
                             if(!iroomData->getGoldLimit())
                             {
                                 //        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_quitgame));
                                 messageBox = UIMessage::createMesBox("金币达到上限");
                             }
                         }),
                         NULL)
    );
    
    //增加金币奖励动画
//    if(iroomData->getGameType() != GameMode::activity && iroomData->getGameType() != GameMode::pvp_2 && iroomData->getGameType()!= GameMode::pvp_4)
//    {
//        if(iroomData->isSuccess())
//            UIMessage::createIconAnition(addicon, iroomData->getLevelType(iroomData->getGameType())->igold);
//        else
//        {
//            UIMessage::createIconAnition(sumbicon, iroomData->getLevelType(iroomData->getGameType())->iyajin);
//        }
//    }
    
    if(iroomData->getGameType() == GameMode::pvp_2 || iroomData->getGameType() == GameMode::pvp_4)
    {
        if(iroomData->isSuccess())
        {
            UIMessage::createIconAnition(addicon, iroomData->getEnemyData(0)->u_pvp_addGold);
        }
        else
        {
            UIMessage::createIconAnition(sumbicon, abs(iroomData->getEnemyData(1)->u_pvp_addGold));
        }
    }
    
    GAMELOG("gameType == %d,  realawardurl = %s,  adweburl = %s",iroomData->getGameType(),iroomData->getGameInfo()->rewardurl.c_str(), iroomData->getRewardInfo()->adweburl.c_str());

    
    if(!iroomData->getGameInfo()->rewardurl.empty())
    {
        Button* detail_gold_back = (Button*)overLayer->rootNode->getChildByTag(1035);
        detail_gold_back->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
        detail_gold_back->setVisible(true);
        Text* detail_gold_text = (Text*)detail_gold_back->getChildByTag(1036);
        detail_gold_text->setString(GameIni::getInstance()->getTips(tips_info_adlinkinfo));
    }
    else
    {
        Button* detail_gold_back = (Button*)overLayer->rootNode->getChildByTag(1035);
        detail_gold_back->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
        detail_gold_back->setVisible(false);
//        Text* detail_gold_text = (Text*)detail_gold_back->getChildByTag(1036);
//        detail_gold_text->setString(GameIni::getInstance()->getTips(tips_info_adlinkinfo));
        
        detail_reward = true;
    }
    return;
}
void GameLayerw::initActivityOver()    //活动结束初始化
{

    iroomData->getRewardInfo()->over = false;
    
    LayerColor *layerColor = LayerColor::create(Color4B(0,0,0,200));
    addChild(layerColor,33);
    
    gameOverNode=CSLoader::createNode("CSres/Activities/Reward.csb");
    auto action = CSLoader::createTimeline("CSres/Activities/Reward.csb");
    gameOverNode->runAction(action);
    action->gotoFrameAndPlay(0);
    ImageView* lightr = (ImageView*)gameOverNode->getChildByTag(1211);
    lightr->runAction(RepeatForever::create(RotateBy::create(1,180)));
    
    lightr = (ImageView*)gameOverNode->getChildByTag(1212);
    lightr->runAction(RepeatForever::create(RotateBy::create(1, -180)));
    
    this->addChild(gameOverNode,100);
    gameOverNode->setAnchorPoint(Vec2(0.5,0.5));
    gameOverNode->setPosition(Vec2(375,660));
    gameOverNode->setScale(0.01);
    gameOverNode->runAction(Sequence::create(
                            ScaleTo::create(2,1),
                            NULL));
    
    runAction(Sequence::create(
                               DelayTime::create(1),
                               CallFuncN::create([=](Node*)
                                {
                                        IAudio::getInstance()->playEffect(music_ef_win);
                                }),
              
              NULL));
    
    
    Sprite* over_light = (Sprite*)gameOverNode->getChildByTag(237);
    over_light->runAction(RepeatForever::create(RotateBy::create(6.0f,360)));
    
    string path =IMAGEDOWLOAD_PATH_ADREWARD + ITools::Intercept_string(iroomData->getRewardInfo()->goodurl, "/");
    GAMELOG("***path == %s",path.c_str());
    ImageView* goodSprite = (ImageView*)gameOverNode->getChildByTag(about_details);
    goodSprite->loadTexture(path);
    goodSprite->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
    
    
    //恭喜获得。。。69
    Text *goodInfo = (Text*)gameOverNode->getChildByTag(69);
    goodInfo->setString(iroomData->getRewardInfo()->goodname.c_str());
    
    //    //地址75
    addressInfo = (Text*)gameOverNode->getChildByTag(75);
    //    //手机号79
    phoneInfo = (Text*)gameOverNode->getChildByTag(79);
    //    //姓名81
    nameInfo = (Text*)gameOverNode->getChildByTag(81);
    reloadActivityInfo();
    
    //修改个人信息80
    Button *changeInfoBtn = (Button*)gameOverNode->getChildByTag(80);
    changeInfoBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd,this));
    //返回72
    Button *backBtn = (Button*)gameOverNode->getChildByTag(72);
    backBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd,this));
    //关闭按钮66
    Button *closeBtn = (Button*)gameOverNode->getChildByTag(66);
    closeBtn->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd,this));
    
    if(!iroomData->getRewardInfo()->adweburl.empty())
    {
        Button* detail_gold_back = (Button*)gameOverNode->getChildByTag(1035);
        detail_gold_back->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
        detail_gold_back->setVisible(true);
        Text* detail_gold_text = (Text*)detail_gold_back->getChildByTag(1036);
        detail_gold_text->setString(GameIni::getInstance()->getTips(tips_info_adlinkinfo));
    }
    else
    {
        detail_reward = true;
    }
}

void GameLayerw::reloadActivityInfo()  // 重新加载邮寄信息
{
    
    string addressStr = "";
    addressStr = addressStr + iplayerData->postprovincename + "-" + iplayerData->postcityname + "-" + iplayerData->postaddress;
    GAMELOG("***address == %s",addressStr.c_str());
    addressInfo->setString(addressStr);
    phoneInfo->setString(iplayerData->postphone);
    nameInfo->setString(iplayerData->postname);
}


void GameLayerw::btnTouchEnd(Ref *psender,Widget::TouchEventType type) //按钮触发事件
{
    Widget *widget = (Widget*)psender;
    if(isGuide && widget->getTag() != guide_iknow)
    {
        return;
    }
    
    GAMELOG("***ddd = %s",iroomData->getGameInfo()->rewardurl.c_str());
    
    if(type == Widget::TouchEventType::ENDED)
    {
        if(widget->getTag() != btn_check_room)
            IAudio::getInstance()->playEffect(music_ef_ok);
        GAMELOG("btn_tag == %d",widget->getTag());
        switch(widget->getTag())
        {
            case btn_back_reward:       //活动奖励，提交信息
            {
                if(iplayerData->postprovinceid > 0 && iplayerData->postcityid > 0 && iplayerData->postname.length() > 1 && iplayerData->postphone.length() > 1)
                {
                    iroomData->sendSubmitRewardInfo();
                    ar_push = true;
                }
                else
                {
                    messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_changeinfo), 1);
                    messageBox->_args.content = change_reward_info;
                    messageBox->setButtonString("前往", "取消");
                }
                
            }break;
            case btn_changeinfo_reward:     //活动奖励，修改信息
            {
                UICtrl::getInstance()->gotoScene(UI_OPTION, false);
            }break;
            case btn_close_reward:      //活动奖励返回
            {
                UICtrl::getInstance()->gotoScene(UI_MAIN, _args);
            }break;                
            case btn_back_room:     // 返回，推出游戏
            {
                if(messageBox == nullptr)
                {
                    messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_quitgame), 1);
                    //messageBox = UIMessage::createMesBox("是否退出", 1);
                    messageBox->_args.code = mesbox_back_room;
                }
            }break;
            case btn_check_room:        //查看原图
            {
                IPlayerData *ipayerData = M_GET_PLAYER;
                if(ipayerData->golds<2){
                auto messageBox = UIMessage::createMesBox("你的金币不足");
                    return;
                }
                
                if (!xianshi_WanZhengTu)
                {
                    if (chanKanCiShu_jilu > 0) {//判断还有没有查看次数
                        IAudio::getInstance()->playEffect(music_ef_preview);
                        iplayerData->http_request_setReward((int)GameReward::viewImage);
                        xianshi_WanZhengTu = true;
                        chanKanCiShu_jilu--;
                        __String * m_pString = __String::createWithFormat("%d",chanKanCiShu_jilu);
                        checkPictureLabel->setString(m_pString->getCString());
                        string path = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" +ITools::Intercept_string(gameLevelSprite->imgurl, "/");
                        pWanCheng_Sprite = Sprite::create(path);//创建完成图
                        pWanCheng_Sprite->setPosition(Vec2(376,575));
                        pWanCheng_Sprite->setScale(0.91f);
                        this -> addChild(pWanCheng_Sprite,20);

                        //查看原图减少金币
                        UIMessage::createIconAnition(sumbicon, abs(GameIni::getInstance()->lostgold_acdsee));
                    }else{
                        //提示次数已经用完
                         IAudio::getInstance()->playEffect(music_ef_error);
                        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_nocount));
                    }
                }
                else
                {
                    pWanCheng_Sprite ->removeFromParentAndCleanup(true);
                    xianshi_WanZhengTu = false;
                }
            }break;
            case btn_voice_room:        //声音关闭，开启
            {
                voiceOL = !voiceOL;
                UserDefault::getInstance()->setBoolForKey(music_key, voiceOL);
                UserDefault::getInstance()->flush();
                voiceSprite->setVisible(!voiceOL);
                IAudio::getInstance()->setVolume(voiceOL);
                if(voiceOL)
                {
                    IAudio::getInstance()->playBackground(music_bg_game);
                }
                else
                {
                    IAudio::getInstance()->stopBackgroundMusic();
                }
                
            }break;
            case about_details:     //活动奖励，了解详情
            {
                if(!iroomData->getRewardInfo()->adweburl.empty())
                {
                    if(messageBox == nullptr)
                    {
                        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_details), 2);
                        messageBox->_args.code = mesbox_details;
                    }
                }
                else
                {
                    return;
                }
                
               
            }break;
            case 1035:      //活动奖励，了解详情
            {
                
                if(!iroomData->getRewardInfo()->adweburl.empty())
                {
                    if(messageBox == nullptr)
                    {
                        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_details), 2);
                        messageBox->_args.code = mesbox_details;
                    }
                }
                else if(!iroomData->getGameInfo()->rewardurl.empty())
                {
                    if(messageBox == nullptr)
                    {
                        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_details), 2);
                        messageBox->_args.code = mesbox_details;
                    }
                }
                else
                {
                    return;
                }
                
            }break;
            case guide_iknow:
            {
                UserDefault::getInstance()->setBoolForKey(guide_game_view, true);
                UserDefault::getInstance()->flush();
                closeGuideLayer();
            }break;
            default:
                break;
        }
    }
}


void GameLayerw::showGuide()
{
    if(!UserDefault::getInstance()->getBoolForKey(guide_game_view, NO))
    {
        openGuideLayer(30, Vec2(258,1090),8);
        
        Node *guideNode = CSLoader::createNode("CSres/public/GuideLayer.csb");
        guideLayer->addChild(guideNode);
        auto action = CSLoader::createTimeline("CSres/public/GuideLayer.csb");
        guideNode->runAction(action);
        action->gotoFrameAndPlay(0);
        
        Node *jiantou = guideNode->getChildByTag(40);
        jiantou->setPosition(Vec2(375,1000));
        
        Button* iKnow = (Button*)guideNode->getChildByTag(180);
        iKnow->setVisible(true);
        iKnow->addTouchEventListener(CC_CALLBACK_2(GameLayerw::btnTouchEnd, this));
        
        Text* guideText = (Text*)guideNode->getChildByTag(37);
        guideText->setString(StringUtils::format(GameIni::getInstance()->getTips(tips_info_checkImage).c_str(), abs(GameIni::getInstance()->lostgold_acdsee)));
        
    }
}




