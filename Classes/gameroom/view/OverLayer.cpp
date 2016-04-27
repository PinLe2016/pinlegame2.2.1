//
//  OverLayer.cpp
//  Pinle
//
//  Created by 李辛 on 15/6/9.
//
//

#include "OverLayer.h"
#include "../../gameini/GameIni.h"
#include "../../player/config.h"
#include "../config.h"
#include "../../gameroom/config.h"
#include "../../player/config.h"
#include "../../activity/view/UI_Activity.h"
static int zongjifen;
OverLayer::OverLayer()
{
    //is_buttonshow=false;
    rootNode = nullptr;
    playerGold = nullptr;
    root_Node= nullptr;
    
    IMessageManager::getInstance()->attach(this, "POS_LIGER");
    //is_touch=true;
}
OverLayer::~OverLayer()
{
    IMessageManager::getInstance()->detach(this, "POS_LIGER");
}

bool OverLayer::init()
{
    if(!LayerColor::init())
    {
        return false;
    }
//    if(!LayerColor::init())
//    {
//        return false;
//    }
    IAudio::getInstance()->stopBackgroundMusic();
    
    iroomData = M_GET_ROOM;
    iplayerData = M_GET_PLAYER;
    
    RenderTexture* renderTexture = RenderTexture::create(20, 20);
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    if(iroomData->isSuccess())
    {
        IAudio::getInstance()->playEffect(music_ef_win);
    }
    else
    {
        IAudio::getInstance()->playEffect(music_ef_lost);
    }
    
    switch(iroomData->getGameType())
    {
        case GameMode::activity:
        {
//            activityLayer();
            commonLayer();
        }break;
        case GameMode::challenge_hard:
        {
            commonLayer(); //测试
            //common_layer();
        }break;
        case GameMode::challenge_simple:
        {
            commonLayer(); //测试
            //common_layer();
        }break;
        case GameMode::daily_hard:
        {
            commonLayer();
        }break;
        case GameMode::daily_simple:
        {
            commonLayer();
        }break;
        case GameMode::pvp_2:
        {
            pvpLayer();
        }break;
        case GameMode::pvp_4:
        {
            pvpLayer();
        }break;
        case GameMode::pvp_friends:
        {
            
        }break;
        default:
            break;
    }
    
    return true;
}

bool OverLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}

void OverLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    log("***OverLayer::onTouchEnded");
    if(iroomData->getGameType() == GameMode::pvp_2)
    {
        
        for(int i = 0; i < 2; i++)
        {
            __String * m_pString_node = __String::createWithFormat("Node_%d",i+1);
            Node * m_pTouXiang_duishou = (Node*)rootNode->getChildByName(m_pString_node->getCString());
            m_pTouXiang_duishou->stopAllActions();
            m_pTouXiang_duishou->setVisible(true);
        }
        m_pJiXu_Button_shu->stopAllActions();
        m_pJiXu_Button_shu->setVisible(true);
        m_pJiXu_Button_shu->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
        m_pFanHui_Button->stopAllActions();
        m_pFanHui_Button->setVisible(true);
        m_pFanHui_Button->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
        
    }
    else if(iroomData->getGameType() == GameMode::pvp_4)
    {
        for(int i = 0; i < 4; i++)
        {
            __String * m_pString_node = __String::createWithFormat("Node_%d",i+1);
            Node * m_pTouXiang_duishou = (Node*)rootNode->getChildByName(m_pString_node->getCString());
            m_pTouXiang_duishou->stopAllActions();
            m_pTouXiang_duishou->setVisible(true);
        }
        m_pJiXu_Button_shu->stopAllActions();
        m_pJiXu_Button_shu->setVisible(true);
        m_pJiXu_Button_shu->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
        m_pFanHui_Button->stopAllActions();
        m_pFanHui_Button->setVisible(true);
        m_pFanHui_Button->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
    }
    else if(iroomData->getGameType() == GameMode::activity)
    {
        return;
    }
    else
    {
        gameRewardGold->setVisible(true);
        gameRewardGold->stopAllActions();
        goldSp0->setVisible(true);
        goldSp0->stopAllActions();
        playerGold->setVisible(true);
        playerGold->stopAllActions();
        goldSp1->setVisible(true);
        goldSp1->stopAllActions();
        playerGameTime->setVisible(true);
        playerGameTime->stopAllActions();
        timeSp->setVisible(true);
        timeSp->stopAllActions();
        playerGameRun->setVisible(true);
        playerGameRun->stopAllActions();
        runSp->setVisible(true);
        runSp->stopAllActions();
        runNumT->setVisible(true);
        runNumT->stopAllActions();
        
//        if(iroomData->getGameType() == GameMode::challenge_hard || iroomData->getGameType() == GameMode::challenge_simple)
//        {
//            temptButton1->setVisible(false);
//        }
//        else
//        {
//            temptButton1->setVisible(true);
//            temptButton1->stopAllActions();
//            temptButton1->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
//        }
        temptButton3->setVisible(true);
        temptButton3->stopAllActions();
        temptButton3->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
    }
    
}


void OverLayer::common_layer(){
    if(rootNode != nullptr)
    {
        rootNode->removeFromParent();
        rootNode = nullptr;
    }
    if(root_Node != nullptr)
    {
        root_Node = nullptr;
    }
    
    
    
    
    rootNode = CSLoader::createNode("CSres/Battle/JPHWanScene.csb");
    //    overLight = Sprite::create("CSres/Battle/BattleUI/over_light.png");
    //    overLight->setPosition(Vec2(375,980));
    //    addChild(overLight);
    //    overLight->runAction(RepeatForever::create(RotateBy::create(6.0f,360)));
    if(iroomData->getGameType() != GameMode::activity)
    {
        root_Node = CSLoader::createNode("CSres/Battle/caihongbao.csb");
        addChild(root_Node);
        buto = (Button *)root_Node->getChildByTag(548);
        buto->addTouchEventListener(CC_CALLBACK_2(OverLayer::activity_rota, this));
    }
    else {
        common_layer();
    }
}
void OverLayer::activity_rota(Ref *pSender, Widget::TouchEventType type){
    
    auto action1 = OrbitCamera::create(0.5,1,0,0,90,0,0);
    auto action2 = OrbitCamera::create(0.5,1,0,90,-90,0,0);
    //        auto action3 = RotateBy::create(5,720);
    //        auto action4 = ScaleBy::create(5,0,0);Spawn
    auto action = Sequence::create(action1,action2,nullptr);
    buto->runAction(Sequence::create(
                                     action,
                                     CallFunc::create( CC_CALLBACK_0(OverLayer::common_layer,this)),
                                     nullptr)
                    );

}
void  OverLayer::inithongbao(){
    // rootNode = CSLoader::createNode("CSres/Battle/JPHWanScene.csb");
    commonLayer();
}
void OverLayer:: fun_back(){
    Red_package->setVisible(false);
    //hb_bg->setVisible(true);
}
void OverLayer::commonLayer()
{
    if(rootNode != nullptr)
    {
        //rootNode->removeFromParent();
        rootNode = nullptr;
    }
//    overLight = Sprite::create("CSres/Battle/BattleUI/over_light.png");
//    overLight->setPosition(Vec2(375,980));
//    addChild(overLight);
//    overLight->runAction(RepeatForever::create(RotateBy::create(6.0f,360)));

    if(iroomData->getGameType() == GameMode::activity || iroomData->getGameType() == GameMode::daily_simple)
    {
        rootNode = CSLoader::createNode("CSres/Battle/JPHWanScene12.csb");
        addChild(rootNode);
       
    }
    else
    {
        rootNode = CSLoader::createNode("CSres/Battle/JPHWanScene.csb");
        addChild(rootNode);
        auto action = CSLoader::createTimeline("CSres/Battle/JPHWanScene.csb");
        rootNode->runAction(action);
        action->gotoFrameAndPlay(0);
    }
    
    
    Sprite *overLight = (Sprite*)rootNode->getChildByTag(236);
    overLight->runAction(RepeatForever::create(RotateBy::create(6.0f,360)));
    
    //
    m_pTu_ordinary = (ImageView*)rootNode->getChildByTag(about_details);
    string spPath = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(iroomData->getGameInfo()->imgurl, "/");
    m_pTu_ordinary->loadTexture(spPath);
    m_pTu_ordinary->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
    m_pTu_ordinary->setContentSize(Size(m_pTu_ordinary->getContentSize().width,m_pTu_ordinary->getContentSize().height));
    if(!iroomData->isSuccess())
    {
        Sprite *failSp = (Sprite*)rootNode->getChildByTag(497);
        failSp->setTexture("CSres/Battle/BattleUI/over_fail.png");
    }
    
    commonItemNode = (Node*)rootNode->getChildByTag(239);
    auto _size=Director::getInstance()->getWinSize();
    if(iroomData->getGameType() == GameMode::activity || iroomData->getGameType() == GameMode::daily_simple)
    {
        ImageView* hongbao_bg = (ImageView*)commonItemNode->getChildByTag(487);
        
            Button* _began = (Button*)hongbao_bg->getChildByTag(335);
        _began->setTouchEnabled(true);
            _began->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
                if(type!=Widget::TouchEventType::ENDED){
                    return;
                }
                bttt->setVisible(true);
                temptButton3->setVisible(true);
                temptButton1->setVisible(true);
                ImageView* Red_lower = (ImageView*)hongbao_bg->getChildByTag(334);
                Red_lower->setVisible(true);
                ImageView* Red_upper = (ImageView*)hongbao_bg->getChildByTag(687);
                Red_upper->setVisible(true);
                ListView* red_paper = (ListView*)hongbao_bg->getChildByTag(1026);
                red_paper->setVisible(true);
                Button* Red_but = (Button*)hongbao_bg->getChildByTag(486);
                Red_but->setVisible(true);
                _began->setTouchEnabled(false);
                auto action = CSLoader::createTimeline("CSres/Battle/JPHWanScene12.csb");
                rootNode->runAction(action);
                action->gotoFrameAndPlay(0,120,false);//红包动画一次
//                Red_but->runAction(Sequence::create( MoveTo::create(1.5, Vec2(Red_but->getPositionX(),_size.height*1/10)),
//                                                    
////                                                      CallFunc::create(this, callfunc_selector(OverLayer::fun_back)),
//                                                            nullptr)
//                                           );
            });

        
        
        
        _redbaoList = (ListView*)hongbao_bg->getChildByTag(1026);
        _redbaoList->setItemModel(_redbaoList->getItem(0));
        _redbaoList->removeAllItems();
        
        CCLOG("%lu",iroomData->roomdata_vec.size());
        for(int i = 0; i < iroomData->roomdata_vec.size(); i++)
        {
            _redbaoList->pushBackDefaultItem();
            Widget* cell = _redbaoList->getItem(i);
            ImageView* touxiang = (ImageView*)cell->getChildByTag(490);
            string spPath = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(iroomData->roomdata_vec[i].headimageurl, "/");
            CCLOG("%s ",spPath.c_str());
            
            
            touxiang->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(iroomData->roomdata_vec[i].headimageurl, "/"), GameIni::ImagePathType::HEAD));
            
            Text* nickname = (Text*)cell->getChildByTag(493);
            nickname->setString(iroomData->roomdata_vec[i].red_nickname.c_str());//
            
            
            Text* jinbishu = (Text*)cell->getChildByTag(494);
            jinbishu->setString(StringUtils::format("%d",iroomData->roomdata_vec[i].golds));
            jinbishu->setColor(Color3B::YELLOW);
            
        }
        
        
        ImageView* hb_bg = (ImageView*)commonItemNode->getChildByTag(487);
        Text* hb_num = (Text*)hb_bg->getChildByTag(486)->getChildByTag(338);
        hb_num->setColor(Color3B::YELLOW);
        hb_num->setString(StringUtils::format("%d",iroomData->goldscurrent));
        Text* z_golds = (Text*)hb_bg->getChildByTag(486)->getChildByTag(691);
        IPlayerData *ipayerData = M_GET_PLAYER;
        z_golds->setString(StringUtils::format("%d", ipayerData->golds));
        z_golds->setColor(Color3B::YELLOW);
    }
    
    
    
    
//活动加载活动的item
    if(iroomData->getGameType() == GameMode::activity)
    {
        commonItemNode->setVisible(false);
        initActivity();
        return;
    }
    
    //时间
    playerGameTime = (cocos2d::ui::Text*)commonItemNode->getChildByName("Text_2");
    int utm = round(iroomData->getTimeCount()/60);
    int uts = round((iroomData->getTimeCount()) % 60);

    
    __String* uns=__String::createWithFormat("%02d:%02d",utm,uts);
    playerGameTime->setString(uns->getCString());//自己的时间
    playerGameTime->setVisible(false);
    playerGameTime->runAction(getAction(1.0f));
    
    timeSp = (Sprite*)commonItemNode->getChildByTag(499);
    timeSp->setVisible(false);
    timeSp->runAction(getAction(1.0f));
    
    //奖励金币
    gameRewardGold = (cocos2d::ui::Text*)commonItemNode->getChildByName("Text_3_0_1");
//    gameRewardGold->setString(StringUtils::format("%d",iroomData->getLevelType(iroomData->getGameType())->igold));
    gameRewardGold->setString(StringUtils::format("%d",iroomData->goldscurrent));
    gameRewardGold->setVisible(false);
    gameRewardGold->runAction(getAction(1.8f));
    goldSp0 = (Sprite*)commonItemNode->getChildByTag(500);
    goldSp0->setVisible(false);
    goldSp0->runAction(getAction(1.8f));
    
    //步数
    playerGameRun = (cocos2d::ui::Text*)commonItemNode->getChildByName("Text_3");
    __String * m_pPV_Run_str = __String::createWithFormat("%d",iroomData->getRunCount());
    playerGameRun->setString(m_pPV_Run_str->getCString());//自己的步数
    playerGameRun->setVisible(false);
    playerGameRun->runAction(getAction(2.6f));
    
    runSp = (Sprite*)commonItemNode->getChildByTag(498);
    runSp->setVisible(false);
    runSp->runAction(getAction(2.6f));
    
    runNumT = (Text*)commonItemNode->getChildByTag(306);
    runNumT->setVisible(false);
    runNumT->runAction(getAction(2.6f));
    
    
    
    //金币
    playerGold = (cocos2d::ui::Text*)commonItemNode->getChildByName("Text_3_0");// 总金币
    playerGold->setString(StringUtils::format("%d",iplayerData->golds));
    playerGold->setVisible(false);
    playerGold->runAction(getAction(3.4f));
   
    goldSp1 = (Sprite*)commonItemNode->getChildByTag(501);
    goldSp1->setVisible(false);
    goldSp1->runAction(getAction(3.4f));
    
    
    if(iroomData->getGameType() == GameMode::challenge_hard || iroomData->getGameType() == GameMode::challenge_simple)
    {
        if(!iroomData->isSuccess()){
            __String * m_pPV_Gold_strs = __String::createWithFormat("-%d",iroomData->getLevelType(iroomData->getGameType())->iyajin);
            gameRewardGold->setString(m_pPV_Gold_strs->getCString());//奖励的金币
            __String * m_pPV_Gold_str = __String::createWithFormat("%d",iplayerData->golds);
            playerGold->setString(m_pPV_Gold_str->getCString());//自己的金币
        }
        else{
            __String * m_pPV_Gold_strs = __String::createWithFormat("+%d",iroomData->getLevelType(iroomData->getGameType())->igold);
            gameRewardGold->setString(m_pPV_Gold_strs->getCString());//奖励的金币
            __String * m_pPV_Gold_str = __String::createWithFormat("%d",iplayerData->golds);
            playerGold->setString(m_pPV_Gold_str->getCString());//自己的金币
        }
        bttt = (Button*)commonItemNode->getChildByTag(1820);
        //bttt->setVisible(false);//true
        bttt->setEnabled(true);
        bttt->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
        
        //随机按钮   出现胜利界面的按钮
        temptButton1 = (cocos2d::ui::Button*)commonItemNode->getChildByTag(193);
        //temptButton1->setVisible(false);//false
        //temptButton1->setPosition(Vec2(186,177));
        //temptButton1->setScale(0.8,0.8);
        temptButton1->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));

//        temptButton1 = (cocos2d::ui::Button*)commonItemNode->getChildByName("Button_1_3");
//        temptButton1->setVisible(false);
        temptButton3 = (cocos2d::ui::Button*)commonItemNode->getChildByTag(192);
        temptButton3->setVisible(false);
        temptButton3->runAction(Sequence::create(
                                                 getAction(4.2f),
                                                 CallFuncN::create(this,callfuncN_selector(OverLayer::buttonDelay)),
                                                 NULL)
                                );
    }
    else
    {
        bttt = (Button*)commonItemNode->getChildByTag(1820);
            bttt->setVisible(false);//true
            bttt->setEnabled(true);
            bttt->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));

        //随机按钮   出现胜利界面的按钮
        temptButton1 = (cocos2d::ui::Button*)commonItemNode->getChildByTag(193);
        temptButton1->setVisible(false);//false
        //temptButton1->setPosition(Vec2(186,177));
        //temptButton1->setScale(0.8,0.8);
        temptButton1->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));

         //返回按钮
        temptButton3 = (cocos2d::ui::Button*)commonItemNode->getChildByTag(192);
        //temptButton3->setPosition(Vec2(564,177));
        //temptButton3->setScale(0.8,0.8);
        temptButton3->setVisible(false);
        temptButton3->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
//        temptButton3->runAction(Sequence::create(
//                                                 getAction(4.2f),
//                                                 CallFuncN::create(this,callfuncN_selector(OverLayer::buttonDelay)),
//                                                 NULL)
//                                );
    }
    
    
    
}
void OverLayer::buttonDelay(Node *sender)
{
    Button *btn = (Button*)sender;
    btn->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
}

void OverLayer::shandengv(float time)
{
    for(vector<ImageView*>::iterator it = shandengVec.begin(); it != shandengVec.end(); it++)
    {
        (*it)->stopAllActions();
    }
    
    
    for(vector<ImageView*>::iterator it = shandengVec.begin(); it != shandengVec.end(); it++)
    {
        ImageView* imageview = (ImageView*)(*it);
        imageview->runAction(RepeatForever::create(Sequence::create(
                                                                DelayTime::create(time),
                                                                CallFunc::create([=](){
            if(imageview->getTag() == 1)
            {
                imageview->loadTexture("CSres/Battle/BattleUI/over_lhj_liang2.png");
                imageview->setTag(2);
            }
            else
            {
                imageview->loadTexture("CSres/Battle/BattleUI/over_lhj_liang1.png");
                imageview->setTag(1);
            }
        }),
                                                                
                                                                NULL)));
    }
    
}
//惊喜吧赢了后出现的老虎机画面
void OverLayer::initActivity()
{
    Node* activityItemNode = rootNode->getChildByTag(240);//老虎机出现
    activityItemNode->setVisible(true);
    Node* activityItemNode_win = rootNode->getChildByTag(497);//赢得字样消失
    //activityItemNode_win->setVisible(false);
    //m_pTu_ordinary->setScale(1.8,1.8);  //图片放大
    
    IActivityData* activity = M_GET_ACTIVITY;
    
    but_began= (Button*)activityItemNode->getChildByTag(242);
    but_began->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
    
    but_end= (Button*)activityItemNode->getChildByTag(243);
    but_end->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
    
    int a[] = {244,245,246};
    for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
    {
        if(i==1  || i==2){
            Button* button = (Button*)activityItemNode->getChildByTag(a[i]);
            button->setScale(0.6,0.6);//设置炫耀一下等按钮大小
            button->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
            button->setVisible(false);
            if (i==2) {
                button->setVisible(true);
                button->setScale(1.5,1.5);
            }
            

        }
         else if (i==0){
            Button* button = (Button*)activityItemNode->getChildByTag(a[i]);
            //button->setScale(0.5,0.5);//设置炫耀一下等按钮大小
            button->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
            button->setVisible(true);
            
        }
//        else {
//        Button* button = (Button*)activityItemNode->getChildByTag(a[i]);
//        button->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
//        }
//        if(i >= 2)
//        {
//            button->setVisible(false);
//        }
    
    }
    
//闪灯
//    shandengVec
    Node* dengNode = activityItemNode->getChildByTag(3001);
    for(int i = 0; i < 15; i++)
    {
        ImageView* imageview = (ImageView*)dengNode->getChildByTag(4001 + i);
        imageview->setTag(1);
        shandengVec.push_back(imageview);
    }
    
    shandengv(0.5);
    
//时间
    
    int utm = round(iroomData->getTimeCount()/60);
    int uts = round((iroomData->getTimeCount()) % 60);
    __String* uns=__String::createWithFormat("%02d:%02d",utm,uts);
    Text* jinbiText = (Text*)activityItemNode->getChildByTag(250);
    jinbiText->setString(uns->getCString());
//步数
    
    Text* bushuText = (Text*)activityItemNode->getChildByTag(251);
    bushuText->setString(StringUtils::format("%d", iroomData->getRunCount()));
    
    initdisplay();
    
    float positionxx = 542+30;
    float positionyy = 332;
    //    laoHuJi1->startGo();
    lhjVer.clear();
    laoHuJi3 = LaoHuJiDonghua::create();
    laoHuJi3->setDate("CSres/public/publicUI/number0-9", "item_", 10, Vec2(positionxx, positionyy));
    addChild(laoHuJi3);
    lhjVer.push_back(laoHuJi3);
    laoHuJi3->setStartSpeed(30);
    
    laoHuJi2 = LaoHuJiDonghua::create();
    laoHuJi2->setDate("CSres/public/publicUI/number0-9", "item_", 10, Vec2(positionxx - 128*1 - 1, positionyy));
    addChild(laoHuJi2);
    lhjVer.push_back(laoHuJi2);
    laoHuJi2->setStartSpeed(20);
    
    laoHuJi1 = LaoHuJiDonghua::create();
    laoHuJi1->setDate("CSres/public/publicUI/number0-9", "item_", 10, Vec2(positionxx - 128*2 - 1, positionyy));
    addChild(laoHuJi1);
    lhjVer.push_back(laoHuJi1);
    laoHuJi1->setStartSpeed(10);
    
    laoHuJi0 = LaoHuJiDonghua::create();
    laoHuJi0->setDate("CSres/public/publicUI/number0-9", "item_", 10, Vec2(positionxx - 128*3 - 2, positionyy));
    addChild(laoHuJi0);
    lhjVer.push_back(laoHuJi0);
    laoHuJi0->setStartSpeed(2);

    
}
void   OverLayer::init_display(){
    activityItemNode = rootNode->getChildByTag(240);//老虎机出现
    IActivityData* activity = M_GET_ACTIVITY;
    
    //累计积分
    jifenText = (Text*)activityItemNode->getChildByTag(248);
    jifenText->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.myintegral));//act->getw_paiming()
    CCLOG("  %d   %d",activity->getSelectedActivity()->matchInfo.myintegral ,activity->getSelectedActivity()->matchInfo.myintegral);
    
    if(iroomData->getGameType() == GameMode::activity || iroomData->getGameType() == GameMode::daily_simple)
    {
        //最佳积分
        zuijiafenText = (Text*)activityItemNode->getChildByTag(392);
                zuijiafenText->setString(StringUtils::format("%d",activity->getSelectedActivity()->matchInfo.bestpoints));
        //最佳排名
        zuijiapaiming = (Text*)activityItemNode->getChildByTag(391);
        zuijiapaiming->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.currentrank));
        //本次排名
        benchipaiming = (Text*)activityItemNode->getChildByTag(390);
        benchipaiming->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.Myrank));
        
    }
    //
    

}
void    OverLayer::initdisplay(){
    activityItemNode = rootNode->getChildByTag(240);//初始化
    IActivityData* activity = M_GET_ACTIVITY;

    //累计积分
    jifenText = (Text*)activityItemNode->getChildByTag(248);
//    jifenText->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.myintegral));//act->getw_paiming()
    ActivityData *  act;
    jifenText->setString(StringUtils::format("%d", act->getw_jifen()));
    CCLOG("  %d   %d",activity->getSelectedActivity()->matchInfo.myintegral ,activity->getSelectedActivity()->matchInfo.myintegral);
    
    if(iroomData->getGameType() == GameMode::activity || iroomData->getGameType() == GameMode::daily_simple)
    {
        //最佳积分
        zuijiafenText = (Text*)activityItemNode->getChildByTag(392);
//        zuijiafenText->setString(StringUtils::format("%d",activity->getSelectedActivity()->matchInfo.bestpoints));
        if (act->getw_jifen()==0) {
            zuijiafenText->setString(StringUtils::format("%d", act->getw_jifen()));
        }
        else{
            zuijiafenText->setString(StringUtils::format("%d",activity->getSelectedActivity()->matchInfo.bestpoints));
        }
        
        //最佳排名
        zuijiapaiming = (Text*)activityItemNode->getChildByTag(391);
//        zuijiapaiming->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.currentrank));
         zuijiapaiming->setString(StringUtils::format("%d", act->getw_paiming()));
        //本次排名
        benchipaiming = (Text*)activityItemNode->getChildByTag(390);
//        benchipaiming->setString(StringUtils::format("%d", activity->getSelectedActivity()->matchInfo.Myrank));
        benchipaiming->setString(StringUtils::format("%d", act->getw_paiming()));

    }
    //
    
    
    
    }

void OverLayer::pvpLayer()
{
    if (rootNode != NULL)
    {
        removeChild(rootNode);
        rootNode = NULL;
    }
    rootNode = CSLoader::createNode("CSres/Battle/PVP_MorePersonal_Over.csb");
    addChild(rootNode);
    auto action = CSLoader::createTimeline("CSres/Battle/PVP_MorePersonal_Over.csb");
    rootNode->runAction(action);
    action->gotoFrameAndPlay(0);
    
    
    string strName ;
    unsigned int score;
    int enemyNum;
    float timeDelay;
    if(iroomData->getGameType() == GameMode::pvp_2)
    {
        enemyNum = 2;
        timeDelay = 2.6f;
    }
    else
    {
        timeDelay = 4.2f;
        enemyNum = 4;
    }
    for (int i= 0;i < 4;i++)
    {
        __String * m_pString_node = __String::createWithFormat("Node_%d",i+1);
        Node * m_pTouXiang_duishou = (Node*)rootNode->getChildByName(m_pString_node->getCString());
        m_pTouXiang_duishou->setVisible(false);
        
        
        //昵称nickname
        Text* m_pPVP_Name = (Text*)m_pTouXiang_duishou->getChildByName("Text_5");
        m_pPVP_Name->setString(iroomData->getEnemyData(i)->u_pvp_Name);//敌人的名字u_pvp_Id
       
        //头像
        Sprite *headSp = (Sprite*)m_pTouXiang_duishou->getChildByName("zhutouxiang_1");
        string headpath = StringUtils::format("CSres/Setup/Head/%s",ITools::Intercept_string(iroomData->getEnemyData(i)->u_pvp_headUrl, "/").c_str());
        headSp->setTexture(headpath);
        //等级
        Text* m_pPVP_NickName = (Text*)m_pTouXiang_duishou->getChildByName("Text_7");
        score = iroomData->getEnemyData(i)->u_pvp_Score;
        m_pPVP_NickName->setString(iroomData->getEnemyData(i)->u_pvp_randName);
        
        //时间、步数、金币
        cocos2d::ui::Text* m_pPVP_Time = (cocos2d::ui::Text*)m_pTouXiang_duishou->getChildByName("Text_8");
        cocos2d::ui::Text* m_pPVP_Run = (cocos2d::ui::Text*)m_pTouXiang_duishou->getChildByName("Text_9");
        cocos2d::ui::Text* m_pPVP_Gold = (cocos2d::ui::Text*)m_pTouXiang_duishou->getChildByName("Text_10");
        
        if(iroomData->getEnemyData(i)->ChengGong)
        {
            //时间
            int ptmm = round(iroomData->getEnemyData(i)->u_pvp_TimeCount % 1000);
            int pts=round(iroomData->getEnemyData(i)->u_pvp_TimeCount / 1000 % 60);
            int ptm=round(iroomData->getEnemyData(i)->u_pvp_TimeCount / 1000 / 60);
            __String* pns=__String::createWithFormat("%02d:%02d:%03d",ptm,pts,ptmm);
            m_pPVP_Time->setString(pns->getCString());
            
            //步数
            __String * m_pPVP_Run_str = __String::createWithFormat("%02d",iroomData->getEnemyData(i)->u_pvp_Run);
            m_pPVP_Run->setString(m_pPVP_Run_str->getCString());
            
            //增加的金币
            __String * m_pPVP_Gold_str = __String::createWithFormat("+%d",iroomData->getEnemyData(i)->u_pvp_addGold);
            m_pPVP_Gold->setString(m_pPVP_Gold_str->getCString());//敌人的金币
            

        }
        else
        {
            m_pPVP_Time->setString("--:--:---");
            m_pPVP_Run->setString("--");
            m_pPVP_Gold->setString(StringUtils::format("%d",iroomData->getEnemyData(i)->u_pvp_addGold));

            
        }
        
        if(i < enemyNum)
        {
            m_pTouXiang_duishou->runAction(getAction(0.8*i + 1.0f));
        }
    }
    
    ImageView *m_pTu = (ImageView*)rootNode->getChildByTag(about_details);
    string spPath = FileUtils::getInstance()->getWritablePath() + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(iroomData->getGameInfo()->imgurl, "/");
    m_pTu->loadTexture(spPath);
    m_pTu->addTouchEventListener(CC_CALLBACK_2(OverLayer::buttonTouchEnd, this));
//    m_pTu->setContentSize(Size(m_pTu_ordinary->getContentSize().width,m_pTu_ordinary->getContentSize().height));
    //随机按钮
    m_pJiXu_Button_shu = (cocos2d::ui::Button*)rootNode->getChildByTag(3);   //继续游戏
    m_pJiXu_Button_shu->setVisible(false);
    m_pJiXu_Button_shu->runAction(Sequence::create(
                                             getAction(timeDelay),
                                             CallFuncN::create(this,callfuncN_selector(OverLayer::buttonDelay)),
                                             NULL)
                            );
    
    m_pFanHui_Button = (cocos2d::ui::Button*)rootNode->getChildByTag(4);   //返回
    m_pFanHui_Button->setVisible(false);
    m_pFanHui_Button->runAction(Sequence::create(
                                                   getAction(timeDelay),
                                                   CallFuncN::create(this,callfuncN_selector(OverLayer::buttonDelay)),
                                                   NULL)
                                  );
}
void OverLayer::activityLayer()
{
    
}

void OverLayer::buttonTouchEnd(Ref *pSender,Widget::TouchEventType type)
{
    if(type!=Widget::TouchEventType::ENDED){
        return;
    }
    Widget *widget = (Widget*)pSender;
    if(type == Widget::TouchEventType::ENDED)
    {
        int tag = widget->getTag();
        gamelog("OverLayer::buttonTouchEnd tag = %d", tag);
        switch(tag)
        {
            case common_continue:
            {
                GAMELOG("***common_continue");
                if(iroomData->getGameType() == GameMode::activity)
                {
                    IActivityData* activity = M_GET_ACTIVITY;
                    
//                    if(activity->getSelectedActivity()->matchInfo.curtime >= activity->getSelectedActivity()->matchInfo.totaltime)
//                    {
//                        messageBox = UIMessage::createMesBox(StringUtils::format("您已经参加了%d次了，请参加其他活动吧！",activity->getSelectedActivity()->matchInfo.totaltime));
//                        break;
//                    }
                    
                    if(activity->getSelectedActivity()->matchInfo.needGold.size() > 0)
                    {
                        if(iplayerData->golds < activity->getSelectedActivity()->matchInfo.needGold[activity->getSelectedActivity()->matchInfo.curtime])
                        {
                            messageBox = UIMessage::createMesBox("您的金币不足！挣够了金币再来赢大奖吧");
                            break;
                        }
                    }
                    
                    activity->getSelectedActivity()->matchInfo.curtime++;
                    iroomData->enterGamePlay(iroomData->getGameType());
                    UICtrl::getInstance()->lastSceneID = UI_ACTIVITY;
                    break;
                    
                }
                
                iroomData->enterGamePlay(iroomData->getGameType());
                
            }break;
            case common_back:
            {
                GAMELOG("***common_back");
                if(iroomData->getGameType() == GameMode::activity)
                {
                    IActivityData* a = M_GET_ACTIVITY;
                    a->http_request_activityList();
                    break;
                }
                
                if(UICtrl::getInstance()->lastSceneID == UI_PK)
                {
                    UICtrl::getInstance()->gotoScene(UI_PK);
                    break;
                }
                else
                {
                    Args arg;
                    arg.content = ui_gogo;
                    UICtrl::getInstance()->gotoScene(UI_MAIN, arg);
                }
                
            }break;
            case pvp_continue:      //继续竞技
            {
//                iroomData->enterGamePlay(iroomData->getGameType());
                UICtrl::getInstance()->gotoScene(UI_PK);
                IMessageManager::getInstance()->postNotification(MES_ROOM_PVPCONTINUE);
            }break;
            case pvp_back:
            {
                GAMELOG("***pvp_back");
                UICtrl::getInstance()->gotoScene(UI_PK);
                IAudio::getInstance()->playBackground(music_bg_main);
            }break;
            case about_details:     //了解详情   点击广告弹出框  ：是否查看详情
            {
                if(!iroomData->getGameInfo()->rewardurl.empty())
                {
//                    messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_details), 2);
//                    messageBox->_args.code = mesbox_details;
                    Args args;
                    args.content = iroomData->getGameInfo()->rewardurl;
                    UICtrl::getInstance()->gotoScene(UI_LINK, args,false);
                    GAMELOG("*** 查看已经拼好的原图url == %s", iroomData->getGameInfo()->rewardurl.c_str());
                }
            }break;
            case 242://老虎机开始
            {
                IPlayerData *ipayerData = M_GET_PLAYER;
                IActivityData* activity = M_GET_ACTIVITY;
                if (ipayerData->golds<activity->getSelectedActivity()->matchInfo.betgolds) {
                    UIMessage::createMesBox("金币不足");
                    return;
                    
                }
                for(vector<LaoHuJiDonghua*>::iterator it = lhjVer.begin(); it != lhjVer.end(); it++)
                {
                    (*it)->startGo();
                }
                
                shandengv(0.2);
                but_began->setTouchEnabled(false);
                but_began->setVisible(false);
                but_end->setVisible(false);
                but_end->setTouchEnabled(false);
                t_text=(ImageView *)rootNode->getChildByTag(240)->getChildByTag(1428);
                t_text->setVisible(false);
                runAction(Sequence::create(
                                                   DelayTime::create(1),
                                                   CallFunc::create([=](void){
                    CCLOG("－－－－－－－－－－－－－结束");
                    but_end->setTouchEnabled(true);
                    but_end->setVisible(true);
                }),
                                                   NULL));
                //老虎机结束链接
                
//                UI_Activity * act;
//                iroomData->http_surprise(act->getActivityID());

                
            }break;
            case 243://老虎机结束
            {
                bt_text=(Text *)but_began->getChildByTag(695);
                CCLOG("%s",bt_text->getString().c_str());
                if (bt_text->getString()!="开  始") {
                    UI_Activity * act;
                    iroomData->http_surprise(act->getActivityID());
                }
                else{
                     IMessageManager::getInstance()->postNotification("MES_ROOM_LAOHUJISHUJU");
                }
                
                
                 bt_text->setString("再摇一次");
                 //t_text->setVisible(true);
                shandengv(0.5);
//                IActivityData* activity = M_GET_ACTIVITY;
//                int tempn = activity->getSelectedActivity()->matchInfo.myintegralOnc;
//                GAMELOG("###integral = %d", tempn);
//                CCLOG("%lu",lhjVer.size());
//                for(vector<LaoHuJiDonghua*>::iterator it = lhjVer.begin(); it != lhjVer.end(); it++)
//                {
//                    int stopNum = 0;
//                    if(tempn > 0)
//                    {
//                        stopNum = tempn % 10;
//                        tempn = tempn / 10;
//                    }
//                    (*it)->stopGo(stopNum);
//                }
                but_began->setTouchEnabled(false);
                //but_began->setVisible(true);
                but_end->setVisible(false);
                but_end->setTouchEnabled(false);
                
               
//                runAction(Sequence::create(
//                                                 DelayTime::create(10),
//                                                 CallFunc::create([=](void){
//                    CCLOG("－－－－－－－－－－－－－开始");
//                    init_display();
//                    but_began->setVisible(true);
//                    but_began->setTouchEnabled(true);
//                     t_text->setVisible(true);
//               }),
//                                NULL));
             
                
            }break;
                
            case 244:
            {
                GameIni::getInstance()->screenShoot();
            }break;
            case 245:
            {
                if(iroomData->getGameType() == GameMode::activity)
                {
                    IActivityData* activity = M_GET_ACTIVITY;
                    
                    if(activity->getSelectedActivity()->activitytype == ACTIVITY_TYPE::onece_type)
                    {
                        CCLOG("特殊活动链接   %s",activity->getSelectedActivity()->adweburl.c_str());
                        //aurl=activity->getSelectedActivity()->adweburl;
                        Application::getInstance()->openURL(activity->getSelectedActivity()->adweburl.c_str());
                        break;
                    }
                    
                    
//                    if(activity->getSelectedActivity()->matchInfo.curtime >= activity->getSelectedActivity()->matchInfo.totaltime)
//                    {
//                        messageBox = UIMessage::createMesBox(StringUtils::format("您已经参加了%d次了，请参加其他活动吧！",activity->getSelectedActivity()->matchInfo.totaltime));
//                        break;
//                    }
                    
                    if(activity->getSelectedActivity()->matchInfo.needGold.size() > 0)
                    {
                        if(iplayerData->golds < activity->getSelectedActivity()->matchInfo.needGold[activity->getSelectedActivity()->matchInfo.curtime])
                        {
                            messageBox = UIMessage::createMesBox("您的金币不足！挣够了金币再来赢大奖吧");
                            break;
                        }
                    }
                    
                    activity->getSelectedActivity()->matchInfo.curtime++;
                    iroomData->enterGamePlay(iroomData->getGameType());
                    UICtrl::getInstance()->lastSceneID = UI_ACTIVITY;
                    break;
                }
            }break;
            case 246://返回
            {
                
                IActivityData* a = M_GET_ACTIVITY;
                a->http_request_activityList();
                GAMELOG("-----#######----####OverLayer::buttonTouchEnd");
                if(iroomData->getGameType() == GameMode::activity)
                {
                    IActivityData* a = M_GET_ACTIVITY;
                    a->http_request_activityList();
                    //a->Eventdetails();
                    break;
                }
            }break;
            case 1820:
            {
                GameIni::getInstance()->screenShoot();
            }break;
            default:
                break;
        }
    }
}
void OverLayer:: jinxiba_over(){
    IActivityData* activity = M_GET_ACTIVITY;
    int tempn = activity->getSelectedActivity()->matchInfo.myintegralOnc;
    GAMELOG("###integral = %d", tempn);
    CCLOG("%lu",lhjVer.size());
    for(vector<LaoHuJiDonghua*>::iterator it = lhjVer.begin(); it != lhjVer.end(); it++)
    {
        int stopNum = 0;
        if(tempn > 0)
        {
            stopNum = tempn % 10;
            tempn = tempn / 10;
        }
        (*it)->stopGo(stopNum);
    }

}

ActionInterval* OverLayer::getAction(float ft)
{
    ActionInterval *action = Sequence::create(
                                              DelayTime::create(ft),
                                              Show::create(),
    NULL
    );
    return action;
}


#pragma 接收老虎机停止消息

bool OverLayer::Notify(const string& name, Args& args)
{
    Notify_Compare("POS_LIGER")
    {
        init_display();
        but_began->setVisible(true);
        but_began->setTouchEnabled(true);
        t_text->setVisible(true);

        return  true;
    }
    return true;
}
