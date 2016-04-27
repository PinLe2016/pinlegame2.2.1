//
//  UI_Activity.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#include "UI_Activity.h"
#include "../../gameini/GameIni.h"
#include "../../gameroom/view/GameScene.h"
#include "../../gameroom/IRoomData.h"
#include "../../UIcontroller/UIcontroller.h"
#include "../../option/config.h"
#include "../../option/view/PersonCenterLayerEx.h"
#define cellwide  691
#define cellhight 485
static  string  ActivityID=" ";
static  string  touxiang;
static  string  _mingzi;
static  string  fenshu;
static  string  juewei;
static  string  paiming;
static  string  db_title;
string string_number[31] = {"一","二","三","四","五","六","七","八","九","十","十一","十二","十三","十四","十五","十六","十七","十八","十九","二十","二十一","二十二","二十三","二十四","二十五","二十六","二十七","二十八","二十九","三十","三十一"};
Scene* UI_Activity::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Activity::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Activity::UI_Activity()
{
    _time=0;
    is_show=true;
    is_show_look=true;
    is_details=true;
    is_tinggao=false;
    _activity = M_GET_ACTIVITY;
    messageBox = NULL;
    needReloadAddressInfo = false;
    quanguoHD = false;
    shSelectTag = 0;
    sheng_selectTag = -1;
    
    IUILayer::attachNetworkMes();
    gamelog("----UI_Activity::UI_Activity---");
    IMessageManager::getInstance()->attach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->attach(this, MESSAGEBOXOK);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_PHB_INFOR);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_JF_INFOR);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_PHB_IMAGE_OK);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_JPXQ_IMAGE_OK);
    IMessageManager::getInstance()->attach(this, MES_OPTION_CLOSE);
    IMessageManager::getInstance()->attach(this, MES_ROOM_SUBMITREWARE);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_POSTADDRESSOK);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_GETLISTDONE);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->attach(this, "liaojian_xq");
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_Winning);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_AwardPreview);
     IMessageManager::getInstance()->attach(this, MES_Eventdetails);
    IMessageManager::getInstance()->attach(this, MES_ACTIVITY_RankingRecord);
}

UI_Activity::~UI_Activity()
{
    IUILayer::detachNetworkMes();
    gamelog("----UI_Activity::~UI_Activity---");
    IMessageManager::getInstance()->detach(this, MES_Eventdetails);
    IMessageManager::getInstance()->detach(this, MESSAGEBOXOK);
    IMessageManager::getInstance()->detach(this, MES_ROOM_GAMESTART);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_PHB_INFOR);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_JF_INFOR);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_PHB_IMAGE_OK);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_JPXQ_IMAGE_OK);
    IMessageManager::getInstance()->detach(this, MES_OPTION_CLOSE);
    IMessageManager::getInstance()->detach(this, MES_ROOM_SUBMITREWARE);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_POSTADDRESSOK);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_GETLISTDONE);
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_GETIMAGEDONE);
    IMessageManager::getInstance()->detach(this, "liaojian_xq");
     IMessageManager::getInstance()->detach(this, "MES_ACTIVITY_Winning");
    IMessageManager::getInstance()->detach(this, MES_ACTIVITY_AwardPreview);
     IMessageManager::getInstance()->detach(this, MES_ACTIVITY_RankingRecord);
}


bool UI_Activity::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    
    _tab = E_ActivityType::start;
    _nowstate = 0;
    
    rootNode = CSLoader::createNode("CSres/Activities/HuodongZhuanQuScene.csb");
    addChild(rootNode);
    
    auto no_start = (Text *)rootNode->getChildByTag(98)->getChildByTag(79);
    no_start->setString(NO_START);
    auto start_text = (Text *)rootNode->getChildByTag(99)->getChildByTag(80);
    start_text->setString(START_TEXT);
    auto complete = (Text *)rootNode->getChildByTag(100)->getChildByTag(81);
    complete->setString(COMPLETE);
    
    ssize_t size = 0;
    std::string load_str;
    
    unsigned char* titlech = FileUtils::getInstance()->getFileData("CSres/Setup/FreeRes/city.json", "r", &size);
    load_str = std::string((const char*)titlech,size);
    readdoc.Parse<0>(load_str.c_str());
    
    
    
    int a[] = {13, 98, 99, 100,126, 1012};
    for(int i = 0; i < sizeof(a)/sizeof(a[1]); i ++)
    {
        Widget * widget = (Widget*)rootNode->getChildByTag(a[i]);
        if(widget && widget->isTouchEnabled())
        {
            widget->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
        }
    }
    
    gameOverNode = CSLoader::createNode("CSres/Activities/Reward.csb");
    addChild(gameOverNode);
    //动画
    ImageView* lightr = (ImageView*)gameOverNode->getChildByTag(1211);
    lightr->runAction(RepeatForever::create(RotateBy::create(1,45)));
    lightr = (ImageView*)gameOverNode->getChildByTag(1212);
    lightr->runAction(RepeatForever::create(RotateBy::create(1, -45)));
    
    gameOverNode->setVisible(false);
    int a1[] = {submit_fq, submit_tj, submit_xg, submit_yj1, submit_yj2};
    for(int i = 0; i < sizeof(a1)/sizeof(a1[0]); i++)
    {
        Widget * widget = (Widget*)gameOverNode->getChildByTag(a1[i]);
        if(widget && widget->isTouchEnabled())
        {
            widget->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
        }
    }
    
//光动画
    ImageView* imageView = (ImageView*)rootNode->getChildByTag(1601);
    imageView->runAction(RepeatForever::create(RotateBy::create(1, 360)));
    Node* guangNode = (Node*)rootNode->getChildByTag(1602);
    for(int i = 0; i < 12; i++)
    {
        imageView = (ImageView*)guangNode->getChildByTag(2601 + i);
        imageView->runAction(RepeatForever::create(Sequence::create(
                                                                    DelayTime::create(0.5),
                                                                    CallFunc::create([=](void){
                            if(imageView->getTag() == 1)
                            {
                                imageView->loadTexture("CSres/Activities/ActivitiesUI/1activity_hbguang.png");
                                imageView->setTag(11);
                            }
                            else
                            {
                                imageView->loadTexture("CSres/Activities/ActivitiesUI/1activity_hguang.png");
                                imageView->setTag(1);
                            }
                        
                        }),
                                                                    NULL)));
    }
    
//邮寄地址信息页面
    _addressDetails = rootNode->getChildByTag(1013);
    int a3[] = {dz_back, dz_baocun};
    for(int i = 0; i < sizeof(a3) / sizeof(a3[0]); i++)
    {
        Widget* widget = (Widget*)_addressDetails->getChildByTag(a3[i]);
        widget->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    }
    
    Button* button;
    
    button = (Button*)_addressDetails->getChildByTag(dz_sheng);
    button->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    IPlayerData* iplayer = M_GET_PLAYER;
    shengid = iplayer->postprovinceid;
    shengname = iplayer->postprovincename;
    shiid = iplayer->postcityid;
    shiname = iplayer->postcityname;
    postaddress = iplayer->postaddress;
    postphone = iplayer->postphone;
    postname = iplayer->postname;
    
    button->setTitleText(shengname);
    list_sheng = (ListView*)_addressDetails->getChildByTag(dz_shenglist);
    list_sheng->addEventListener(CC_CALLBACK_2(UI_Activity::eventSH_select, this));
    list_sheng->setItemModel(list_sheng->getItem(0));
    
    rapidjson::Value& provinces = readdoc["provinces"];
    if(provinces.IsArray())
    {
        list_sheng->removeAllItems();
        for(int i = 0; i < provinces.Size(); i++)
        {
            list_sheng->pushBackDefaultItem();
            button = (Button*)list_sheng->getItem(i);
            button->setTitleText(provinces[i]["name"].GetString());
        }
        
        string tempstr;
        for(int i = 0; i < provinces.Size(); i++)
        {
            tempstr = provinces[i]["name"].GetString();
            if(tempstr.compare(shengname) == 0)
            {
                sheng_selectTag = i;
            }
        }
    }
    
    
    
    
    
    button = (Button*)_addressDetails->getChildByTag(dz_shi);
    button->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    button->setTitleText(shiname);
    
    list_shi = (ListView*)_addressDetails->getChildByTag(dz_shilist);
    list_shi->addEventListener(CC_CALLBACK_2(UI_Activity::eventSH_select, this));
    list_shi->setItemModel(list_shi->getItem(0));
    list_shi->removeAllItems();
    
    if(sheng_selectTag != -1)
    {
        rapidjson::Value& citys = provinces[sheng_selectTag]["citys"];
        for(int i = 0; i < citys.Size(); i++)
        {
            list_shi->pushBackDefaultItem();
            Button* b = (Button*)list_shi->getItem(i);
            b->setTitleText(citys[i]["name"].GetString());
        }
    }
    
//     = 1001,      //详情－－规则说明
//     = 217,      //详情－－活动详情
//     = 126,      //详情－－立即参与
    
    _activityDetails = rootNode->getChildByTag(124);
    int a2[] = {xq_back, xq_gzsm ,xq_hdxq, xq_ljcy};
    for(int i = 0; i < sizeof(a2)/sizeof(a2[0]); i++)
    {
        Widget* widget = (Widget*)_activityDetails->getChildByTag(a2[i]);
        widget->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    }
    _activityXiangqing = _activityDetails->getChildByTag(1004);
    _rewardDetails = rootNode->getChildByTag(186);
    _rankLayer = rootNode->getChildByTag(85);
    _myGradeLayer = rootNode->getChildByTag(143);
    _activityRules = _activityDetails->getChildByTag(1005);
    
    //添加自己姓名  公爵lv
    IPlayerData*    _player = M_GET_PLAYER;
    Text* w_name = (Text*)_activityXiangqing->getChildByTag(194); //name
    w_name->setString(_player->nickname);
    _mingzi=_player->nickname;
    Text* touxian = (Text*)_activityXiangqing->getChildByTag(195);  //公爵
    touxian->setString(_player->rankname);
    juewei=_player->rankname;
    PersonCenterLayerEx p;
    ImageView* championHeadIV = (ImageView*)_activityXiangqing->getChildByTag(174);
    string  str1=StringUtils::format("res/newUI/resUI/Information/Head/%s",
                                 ITools::Intercept_string(_player->uHeadIndex, "/").c_str());
    string  str2=StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str());
    touxiang=str2;
    championHeadIV->loadTexture(p.Gethead_img());
    championHeadIV->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->uHeadIndex.c_str()));
    //省份选择－选择要看的广告（全国，还是本省）
    _shengfenNode = rootNode->getChildByTag(1014);
    Text *shengfenText;
    shengfenText = (Text*)_shengfenNode->getChildByTag(2002);
    shengfenText->setString(iplayer->cityname);
    Button* shengfenBtn = (Button*)_shengfenNode->getChildByTag(2003);
    shengfenBtn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    shengfenBtn = (Button*)_shengfenNode->getChildByTag(2004);
    shengfenBtn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    _shengfenNode->setVisible(false);

    
//规则说明初始化
    _guizeList = (ListView*)_activityRules->getChildByTag(2001);
    _guizeList->setItemModel(_guizeList->getItem(0));
    _guizeList->removeAllItems();
//排行榜列表
    _phbList = (ListView*)_rankLayer->getChildByTag(709);
    _phbList->setItemModel(_phbList->getItem(0));
    _phbList->removeAllItems();
    //_list->addEventListener(CC_CALLBACK_2(UI_Activity::selectedItemEvent, this));
    
//个人对比积分
RankingRecord= CSLoader::createNode("res/CSres/Activities/RankingRecord.csb");
    RankingRecord->setVisible(false);
    _dbList = (ListView*)RankingRecord->getChildByTag(487);
    _dbList->setItemModel(_dbList->getItem(0));
    _dbList->removeAllItems();
    
    

    
//积分排行榜
     PersonalRecord = CSLoader::createNode("res/CSres/Activities/PersonalRecord.csb");//初始化Winning
    _pjfList = (ListView*)PersonalRecord->getChildByTag(488);
    _pjfList->setItemModel(_pjfList->getItem(0));
    _pjfList->removeAllItems();
//奖项预览
    AwardPreview = CSLoader::createNode("res/CSres/Activities/AwardPreview.csb");
    AwardPreview->setVisible(false);
    addChild(AwardPreview);
    _title=(Text *)AwardPreview->getChildByTag(321)->getChildByTag(323);
    _AwardPreviewList = (ListView*)AwardPreview->getChildByTag(325);
    _AwardPreviewList->setItemModel(_AwardPreviewList->getItem(0));
    _AwardPreviewList->removeAllItems();
    Button * _but= (Button *)AwardPreview->getChildByTag(321)->getChildByTag(322);
    _but->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
        AwardPreview->setVisible(false);
        is_show_look=true;
    });

//获奖名单
    _Winning = CSLoader::createNode("res/CSres/Activities/Winning.csb");
    _Winning->setVisible(false);
    addChild(_Winning);
    _WinningList = (ListView*)_Winning->getChildByTag(638);
    _WinningList->setItemModel(_WinningList->getItem(0));
    _WinningList->removeAllItems();
    Button * but= (Button *)_Winning->getChildByTag(283)->getChildByTag(285);
    but->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
        _Winning->setVisible(false);
        is_show=true;
    });

//活动列表
    _list = (ListView*)rootNode->getChildByTag(50);
    _list->setItemModel(_list->getItem(0));
    _list->removeAllItems();
    _list->addEventListener(CC_CALLBACK_2(UI_Activity::selectedItemEvent, this));
    //_list->setHandlerPriority();
    setTabShow(_tab);
    setActivityByType(_tab);
    
    schedule(schedule_selector(UI_Activity::updateTime),1.f);
    
    IAudio::getInstance()->playBackground(music_bg_activity, NO, YES);
    
    return true;
}

void UI_Activity::eventSH_select(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
    {
        return;
    }
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);
    GAMELOG("select child end index = %ld", listView->getCurSelectedIndex());
    
    
    
    
    IPlayerData* iplayer = M_GET_PLAYER;
    
    if(shSelectTag == shtag_sheng)
    {
        uint index = (uint)listView->getCurSelectedIndex();
        uint id = readdoc["provinces"][index]["id"].GetInt();
        string name = readdoc["provinces"][index]["name"].GetString();
        
        shengname = name;
        shengid = id;
        sheng_selectTag = index;
        
        list_sheng->setVisible(false);
        Button* button = (Button*)_addressDetails->getChildByTag(dz_sheng);
        button->setTitleText(name);
        
        rapidjson::Value& provinces = readdoc["provinces"];
        if(provinces.IsArray() && index < provinces.Size())
        {
            rapidjson::Value& citys = provinces[index]["citys"];
            if(citys.IsArray())
            {
                list_shi->removeAllItems();
                for(int i = 0; i < citys.Size(); i++)
                {
                    list_shi->pushBackDefaultItem();
                    Button* but = (Button*)list_shi->getItem(i);
                    but->setTitleText(citys[i]["name"].GetString());
                }
            }
        }
        
        list_shi->setVisible(true);
        list_shi->jumpToTop();
        shSelectTag = shtag_shi;
        Button* bb = (Button*)_addressDetails->getChildByTag(dz_shi);
        bb->setTitleText(readdoc["provinces"][index]["citys"][(uint)0]["name"].GetString());
        shiid = readdoc["provinces"][index]["citys"][(uint)0]["id"].GetInt();
        shiname = readdoc["provinces"][index]["citys"][(uint)0]["name"].GetString();
        
    }
    else if(shSelectTag == shtag_shi)
    {
        uint index = (uint)listView->getCurSelectedIndex();
        GAMELOG("index = %d, sheng_selecttag = %d", index, sheng_selectTag);
        uint id = readdoc["provinces"][sheng_selectTag]["citys"][index]["id"].GetInt();
        string name = readdoc["provinces"][sheng_selectTag]["citys"][index]["name"].GetString();
        shiid = id;
        shiname = name;
        
        list_shi->setVisible(false);
        Button* bb = (Button*)_addressDetails->getChildByTag(dz_shi);
        bb->setTitleText(shiname);
    }
}


void UI_Activity::setDetailsTime()
{
    IActivityData* a = _activity;
    
    Node *secondNode = rootNode->getChildByTag(124)->getChildByTag(1004);
    Text *text = (Text*)secondNode->getChildByTag(173);
    //text->setString(ITools::getTimeString(a->v_activityList[_tab][_cellIndex]->remainingtime, hanzi_tsfm));
}

void UI_Activity::showJPXQ()
{
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    
    _rewardDetails->setVisible(true);
    Button* back = (Button*)_rewardDetails->getChildByTag(jpxq_back);
    back->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    
    GAMELOG("---jpxq == %lu", activity->v_activityPrize.size());
    for(int i = 0; i < 3; i++)
    {
        Node* node = _rewardDetails->getChildByTag(1001 + i);
        if(i < activity->v_activityPrize.size())
        {
            ImageView* iv = (ImageView*)node->getChildByTag(1001);
            iv->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->v_activityPrize[i]->url, "/"), GameIni::ImagePathType::ADREWARD, true));
            //奖品等级
            //        Text* t2 = (Text*)node->getChildByTag(1002);
            //奖品数量
            Text* t3 = (Text*)node->getChildByTag(1003);
            t3->setString(StringUtils::format("%d",activity->v_activityPrize[i]->amount));
            //奖品名称
            Text* t4 = (Text*)node->getChildByTag(1004);
            t4->setString(activity->v_activityPrize[i]->name);
            //光
            Sprite* s1 = (Sprite*)node->getChildByTag(1005);
            s1->runAction(getLightAction());
        }
        else
        {
            node->setVisible(false);
        }
        
    }
}

void UI_Activity::submitRewarkInfo()         //提交奖品订单信息
{
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    
    gameOverNode->setAnchorPoint(Vec2(0.5,0.5));
    gameOverNode->setPosition(Vec2(375,660));
    
    
    Sprite* over_light = (Sprite*)gameOverNode->getChildByTag(237);
    over_light->runAction(RepeatForever::create(RotateBy::create(6.0f,360)));
    
//    GAMELOG("###==%d  size==%ld", activity->matchInfo.prizewinning, activity->v_activityPrize.size());
//    GAMELOG("###==%s", GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->v_activityPrize[activity->matchInfo.prizewinning - 1]->url, "/"), GameIni::ImagePathType::ADREWARD, true).c_str());
    ImageView* goodSprite = (ImageView*)gameOverNode->getChildByTag(about_details);
    goodSprite->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->v_activityPrize[activity->matchInfo.prizewinning - 1]->url, "/"), GameIni::ImagePathType::ADREWARD, true));
    
    activity->sumitRewardUrl = GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->v_activityPrize[activity->matchInfo.prizewinning - 1]->url, "/"), GameIni::ImagePathType::ADREWARD, true);
    

    
    //恭喜获得。。。69
    Text *goodInfo = (Text*)gameOverNode->getChildByTag(69);
    goodInfo->setString(activity->v_activityPrize[activity->matchInfo.prizewinning - 1]->describe);
    
    //    //地址75
    addressInfo = (Text*)gameOverNode->getChildByTag(75);
    //    //手机号79
    phoneInfo = (Text*)gameOverNode->getChildByTag(79);
    //    //姓名81
    nameInfo = (Text*)gameOverNode->getChildByTag(81);
    reloadActivityInfo();
    
    //修改个人信息80
    Button *changeInfoBtn = (Button*)gameOverNode->getChildByTag(submit_xg);
    changeInfoBtn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback,this));
    //提交
    Button *backBtn = (Button*)gameOverNode->getChildByTag(submit_tj);
    backBtn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback,this));
    //放弃按钮66
    Button *closeBtn = (Button*)gameOverNode->getChildByTag(submit_fq);
    closeBtn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback,this));
    
    
    IPlayerData* iplayer = M_GET_PLAYER;
    if(iplayer->golds < 1000)
        setPostWay(false);          //货到付款
    
    
}

void UI_Activity::reloadActivityInfo()       //更新玩家信息
{
    IPlayerData *iplayerData = M_GET_PLAYER;
    
    string addressStr = "";
    addressStr = addressStr + iplayerData->postprovincename + "-" + iplayerData->postcityname + "-" + iplayerData->postaddress;
    GAMELOG("***address == %s",addressStr.c_str());
    addressInfo->setString(addressStr);
    phoneInfo->setString(iplayerData->postphone);
    nameInfo->setString(iplayerData->postname);
}

void UI_Activity::   showAwardPreview(){
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    CCLOG("%lu  ",activity->matchInfo.v_LookWinning.size());
    _AwardPreviewList = (ListView*)AwardPreview->getChildByTag(325);
    _AwardPreviewList->setItemModel(_AwardPreviewList->getItem(0));
    _AwardPreviewList->removeAllItems();
    string str[3] = {"一","二","三"};
    vector<__Array*> imageInfo;
    for(int i = 0; i<activity->matchInfo.v_LookWinning.size(); i++)
    {
        _AwardPreviewList->pushBackDefaultItem();
        Widget* cell = _AwardPreviewList->getItem(i);
        Text* Ranking = (Text*)cell->getChildByTag(541);
        if (activity->matchInfo.v_LookWinning[i].lookawardcount==1) {
            Ranking->setString(StringUtils::format("第%d名 ",activity->matchInfo.v_LookWinning[i].lookawardcount));
        }
        else{
            if (activity->matchInfo.v_LookWinning[i-1].lookawardcount+1==activity->matchInfo.v_LookWinning[i].lookawardcount) {
                Ranking->setString(StringUtils::format("第%d名 ",activity->matchInfo.v_LookWinning[i].lookawardcount));
            }
            else{
                if(activity->matchInfo.v_LookWinning[i-1].lookawardcount+1>200  || activity->matchInfo.v_LookWinning[i-1].lookawardcount+1<0){
                     Ranking->setString(StringUtils::format("第%d名-第%d名 ",1,activity->matchInfo.v_LookWinning[i].lookawardcount));
                }
                else{
            Ranking->setString(StringUtils::format("第%d名-第%d名 ",activity->matchInfo.v_LookWinning[i-1].lookawardcount+1,activity->matchInfo.v_LookWinning[i].lookawardcount));
                }
            }
        }
        Text* awardorder = (Text*)cell->getChildByTag(540);
        awardorder->setString(StringUtils::format("%s等奖 ",str[activity->matchInfo.v_LookWinning[i].lookawardorder-1].c_str()));
        Text* _goodsname = (Text*)cell->getChildByTag(539);
        _goodsname->setString(activity->matchInfo.v_LookWinning[i].lookgoodsname);
        ImageView* goodsimageurl = (ImageView*)cell->getChildByTag(537);
        
        
        
        GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_LookWinning[i].lookgoodsimageurl, "/"), GameIni::ImagePathType::ADLIST, true);
        
        string filename = ITools::Intercept_string(activity->matchInfo.v_LookWinning[i].lookgoodsimageurl,"/");//截取图片名
        __String* path = __String::create(StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER,
                                                              IMAGE_ADLIST_FINDER, filename.c_str()));
        __String* url  = __String::create(activity->matchInfo.v_LookWinning[i].lookgoodsimageurl);
        __Array* arr = __Array::create(url, path, NULL);
        imageInfo.push_back(arr);
      




        goodsimageurl->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_LookWinning[i].lookgoodsimageurl, "/"), GameIni::ImagePathType::ADLIST, true));
        CCLOG("%s ",GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_LookWinning[i].lookgoodsimageurl, "/"), GameIni::ImagePathType::ADLIST, true).c_str());
        ImageView* medal = (ImageView*)cell->getChildByTag(538);
        string _string=StringUtils::format("CSres/Activities/ActivitiesUI/jiangxingyulan_%d.png",activity->matchInfo.v_LookWinning[i].lookawardorder);
        medal->loadTexture(_string.c_str());
        //medal->loadTexture("CSres/Activities/ActivitiesUI/jiangxingyulan_1.png");
    }
    myHttpWork* http = myHttpWork::createHttp();
    http->http_request_downLoadImage(imageInfo, MES_ACTIVITY_GETIMAGEDONE);
    imageInfo.clear();

}
void UI_Activity::   showWinning(){
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    CCLOG("%lu  ",activity->matchInfo.v_Winning.size());
    _WinningList = (ListView*)_Winning->getChildByTag(638);
    _WinningList->setItemModel(_WinningList->getItem(0));
    _WinningList->removeAllItems();
     vector<__Array*> imageInfo;
    for(int i = 0; i<activity->matchInfo.v_Winning.size(); i++)
    {
        _WinningList->pushBackDefaultItem();
        Widget* cell = _WinningList->getItem(i);
        Text* rank = (Text*)cell->getChildByTag(301);
        rank->setString(StringUtils::format("第%d名 ",activity->matchInfo.v_Winning[i].Winningrank));
        ImageView* headimageurl = (ImageView*)cell->getChildByTag(307);
        headimageurl->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Winning[i].Winningheadimageurl, "/"), GameIni::ImagePathType::HEAD));
        Text* nickname = (Text*)cell->getChildByTag(313);
        nickname->setString(activity->matchInfo.v_Winning[i].Winningnickname);
        Text* points = (Text*)cell->getChildByTag(325);
        points->setString(StringUtils::format("%d ",activity->matchInfo.v_Winning[i].Winningpoints));
        Text* goodsname = (Text*)cell->getChildByTag(332);
        goodsname->setString(activity->matchInfo.v_Winning[i].Winninggoodsname);
        ImageView* goodsimageurl = (ImageView*)cell->getChildByTag(331);
        CCLOG("%s",activity->matchInfo.v_Winning[i].Winninggoodsimageurl.c_str());
        GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Winning[i].Winninggoodsimageurl, "/"), GameIni::ImagePathType::ADREWARD, true);
        
        string filename = ITools::Intercept_string(activity->matchInfo.v_Winning[i].Winninggoodsimageurl,"/");//截取图片名
        __String* path = __String::create(StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER,
                                                              IMAGE_ADREWARD_FINDER, filename.c_str()));
        __String* url  = __String::create(activity->matchInfo.v_Winning[i].Winninggoodsimageurl);
        __Array* arr = __Array::create(url, path, NULL);
        imageInfo.push_back(arr);
        goodsimageurl->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Winning[i].Winninggoodsimageurl, "/"), GameIni::ImagePathType::ADREWARD, true));

           CCLOG("%s ",GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Winning[i].Winninggoodsimageurl, "/"), GameIni::ImagePathType::ADREWARD, true).c_str());
        
        
           }
    myHttpWork* http = myHttpWork::createHttp();
    http->http_request_downLoadImage(imageInfo, MES_ACTIVITY_GETIMAGEDONE);
    imageInfo.clear();
    _Winning->setVisible(true);

}
void UI_Activity::show_oneself(){
            IActivityData* a = _activity;
            Activity* activity = a->v_activityList[_tab][_cellIndex];
            a->setSelectedActivity(_tab, _cellIndex);
            PersonalRecord = CSLoader::createNode("res/CSres/Activities/PersonalRecord.csb");
            PersonalRecord->setVisible(false);
            addChild(PersonalRecord);
            ImageView* bg= (ImageView*)PersonalRecord->getChildByTag(515);
            Text* title= (Text *)bg->getChildByTag(518);
            title->setString(StringUtils::format("%s个人纪录", activity->v_activityPrize[0]->name.c_str()));
            Button* _back= (Button*)bg->getChildByTag(517);
            _back->addTouchEventListener(CC_CALLBACK_2(UI_Activity::Call_back,this));
            //IActivityData* a = _activity;
            a->setSelectedActivity(_tab, _cellIndex);
            GAMELOG("a->getSelectedActivity()->id = %s", a->getSelectedActivity()->id.c_str());
            //CCLOG("  %s",changeUserId(activity->matchInfo.v_Champion[0].userid).c_str());
            a->http_request_integralRankingList(a->getSelectedActivity()->id, " ");
            
            _pjfList = (ListView*)PersonalRecord->getChildByTag(488);
            //            Layout *  layout_list = (Layout*)_pjfList->getChildByTag(741);
            //            Text * text_lishi=(Text * )layout_list->getChildByTag(524);
            //text_lishi->setString(StringUtils::format("%d",rand()%1243-500));
            
            //            _pjfList->setItemModel(_pjfList->getItem(0));
            //            for(int i = 0; i<6; i++)  //i < activity->matchInfo.v_Integral.size()
            //            {
            //                _pjfList->pushBackDefaultItem();
            //                Widget* cell = _pjfList->getItem(i);
            //            }
    
}
//对比积分榜

void  UI_Activity::  showDB(){
    
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    if (activity->matchInfo.v_Champion[_tag].nickname==_mingzi) {
        CCLOG("是自己");
        show_oneself();
        return;
    }
    //RankingRecord= CSLoader::createNode("res/CSres/Activities/RankingRecord.csb");
    _dbList = (ListView*)RankingRecord->getChildByTag(487);
    _dbList->setItemModel(_dbList->getItem(0));
    _dbList->removeAllItems();
    ImageView* _call = (ImageView*)RankingRecord->getChildByTag(393);
    ImageView* _touxiang = (ImageView*)_call->getChildByTag(394);
     _touxiang->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Champion[_tag].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));
    Text * mingzi=(Text*)_call->getChildByTag(395);
    mingzi->setString(activity->matchInfo.v_Champion[_tag].nickname);
    Text * w_juewei=(Text*)_call->getChildByTag(396);
    w_juewei->setString(StringUtils::format("%s", activity->matchInfo.v_Champion[_tag].rankname.c_str()));
    Text * w_paiming=(Text*)_call->getChildByTag(397);
    w_paiming->setString(StringUtils::format("第%d名", _tag + 1));
    Text * _title=(Text*)RankingRecord->getChildByTag(183)->getChildByTag(186);
    _title->setString(db_title);
    //string str[12] = {"一","二","三","四","五","六","七","八","九","十","十一","十二"};
    CCLOG("个人积分对比  %lu  %lu ",activity->matchInfo.v_Ranking.size(),activity->matchInfo.v_RankingRecord.size());
    int num=0;
    int m;//=activity->matchInfo.v_Ranking[activity->matchInfo.v_Ranking.size()-1].cycle;
    if (activity->matchInfo.v_Ranking.size()==0) {
        m=1;
    }
    else{
        m=activity->matchInfo.v_Ranking[activity->matchInfo.v_Ranking.size()-1].cycle;
    }

    int n=0;//activity->matchInfo.v_RankingRecord[activity->matchInfo.v_RankingRecord.size()-1].cycle;
    
    if (activity->matchInfo.v_RankingRecord.size()==0) {
        n=1;
    }
    else{
        n=activity->matchInfo.v_RankingRecord[activity->matchInfo.v_RankingRecord.size()-1].cycle;
    }
    
    bool count=false;
    if (m>n) {
        num=m;//activity->matchInfo.v_Ranking.size();
        count=true;
    }
    else{
        num=n;//activity->matchInfo.v_RankingRecord.size();//di
        count=false;
    }
    if (num==0) {
        return;
    }
    for(int i = 0; i<num; i++)
    {
        _dbList->pushBackDefaultItem();
        Widget* cell = _dbList->getItem(i);
        Text* hero = (Text*)cell->getChildByTag(407);
        hero->setColor(Color3B::YELLOW);
//        if(!count && i>=activity->matchInfo.v_Ranking.size()){
//           
//            hero->setString(StringUtils::format("%d",0));
//
//        }
//        else{
//        hero->setString(StringUtils::format("%d",activity->matchInfo.v_Ranking[i]._points));
//        }
        
        
        for (int j=0; j<activity->matchInfo.v_Ranking.size(); j++) {
            if (i==activity->matchInfo.v_Ranking[j].cycle-1) {
                CCLOG("%d",activity->matchInfo.v_Ranking[j]._points);
                hero->setString(StringUtils::format("%d",activity->matchInfo.v_Ranking[j]._points));
                break;
            }
            else{
                hero->setString("0");
            }
            
        }

        Text* enemy = (Text*)cell->getChildByTag(406);
        enemy->setColor(Color3B::YELLOW);
//        if(count && i>=activity->matchInfo.v_RankingRecord.size()){
//             enemy->setString(StringUtils::format("%d",0));
//            
//        }
//       
//        else{
//            enemy->setString(StringUtils::format("%d",activity->matchInfo.v_RankingRecord[i]._points));
//        }
        CCLOG("%s,%s",activity->matchInfo.v_Champion[_tag].nickname.c_str(),_mingzi.c_str());
        if (activity->matchInfo.v_Champion[_tag].nickname==_mingzi) {
            for (int j=0; j<activity->matchInfo.v_Ranking.size(); j++) {
                if (i==activity->matchInfo.v_Ranking[j].cycle-1) {
                    CCLOG("%d",activity->matchInfo.v_Ranking[j]._points);
                    enemy->setString(StringUtils::format("%d",activity->matchInfo.v_Ranking[j]._points));
                    break;
                }
                else{
                    enemy->setString("0");
                }
                
            }

        }
        else{
        
        
        for (int j=0; j<activity->matchInfo.v_RankingRecord.size(); j++) {
            if (i==activity->matchInfo.v_RankingRecord[j].cycle-1) {
                CCLOG("%d",activity->matchInfo.v_RankingRecord[j]._points);
                enemy->setString(StringUtils::format("%d",activity->matchInfo.v_RankingRecord[j]._points));
                break;
            }
            else{
                enemy->setString("0");
            }
            
        }
        }

        Text* zhouqi_activity = (Text*)cell->getChildByTag(405);
        zhouqi_activity->setColor(Color3B::YELLOW);
        CCLOG("%d",lx_huodong);
        if (lx_huodong==0) {
            zhouqi_activity->setString(StringUtils::format("第%s月",string_number[i].c_str()));
        }
        else if (lx_huodong==1){
            zhouqi_activity->setString(StringUtils::format("第%s周",string_number[i].c_str()));
        }
        else if (lx_huodong==2){
            zhouqi_activity->setString(StringUtils::format("第%s日",string_number[i].c_str()));
        }
        else if(lx_huodong==5){
            zhouqi_activity->setString(StringUtils::format("第%s天",string_number[i].c_str()));
        }
        

        
    }
//    for(int i = 0; i<activity->matchInfo.v_RankingRecord.size(); i++)
//    {
//        _dbList->pushBackDefaultItem();
//        Widget* cell = _dbList->getItem(i);
//        Text* enemy = (Text*)cell->getChildByTag(406);
//        enemy->setColor(Color3B::YELLOW);
//        enemy->setString(StringUtils::format("%d",activity->matchInfo.v_RankingRecord[i]._points));
//        
//        Text* zhouqi_activity = (Text*)cell->getChildByTag(405);
//        zhouqi_activity->setColor(Color3B::YELLOW);
//        CCLOG("%d",lx_huodong);
//        if (lx_huodong==0) {
//            zhouqi_activity->setString(StringUtils::format("第%s月",str[i].c_str()));
//        }
//        else if (lx_huodong==1){
//            zhouqi_activity->setString(StringUtils::format("第%s周",str[i].c_str()));
//        }
//        else if (lx_huodong==2){
//            zhouqi_activity->setString(StringUtils::format("第%s日",str[i].c_str()));
//        }
//        else if(lx_huodong==5){
//            zhouqi_activity->setString(StringUtils::format("第%s天",str[i].c_str()));
//        }
//
//        
//    }
    RankingRecord->setVisible(true);

}
//积分排行榜
void UI_Activity::showJFB(){
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    //string str[12] = {"一","二","三","四","五","六","七","八","九","十","十一","十二"};
    CCLOG("%lu  ",activity->matchInfo.v_Integral.size());
    _pjfList = (ListView*)PersonalRecord->getChildByTag(488);
    _pjfList->setItemModel(_pjfList->getItem(0));
     _pjfList->removeAllItems();
    _pjfList->setVisible(true);
    int num =0;
    if (activity->matchInfo.v_Integral.size()==0) {
        num=1;
        _pjfList->setVisible(false);
    }
    else{
        num=activity->matchInfo.v_Integral[activity->matchInfo.v_Integral.size()-1].cycle;
    }
    for(int i = 0; i<num; i++)
    {
        _pjfList->pushBackDefaultItem();
        Widget* cell = _pjfList->getItem(i);
        Text* zhouqi = (Text*)cell->getChildByTag(522);
        zhouqi->setColor(Color3B::YELLOW);
        Text* _jinbi = (Text*)cell->getChildByTag(524);
        for (int j=0; j<activity->matchInfo.v_Integral.size(); j++) {
            if (i==activity->matchInfo.v_Integral[j].cycle-1) {
                CCLOG("%d",activity->matchInfo.v_Integral[j]._points);
                _jinbi->setString(StringUtils::format("%d",activity->matchInfo.v_Integral[j]._points));
                break;
            }
            else{
                _jinbi->setString("0");
            }

        }
        
        if (lx_huodong==0) {
            zhouqi->setString(StringUtils::format("第%s月",string_number[i].c_str()));
        }
        else if (lx_huodong==1){
            zhouqi->setString(StringUtils::format("第%s周",string_number[i].c_str()));
        }
        else if (lx_huodong==2){
            zhouqi->setString(StringUtils::format("第%s日",string_number[i].c_str()));
        }
        else if (lx_huodong==5){
            zhouqi->setString(StringUtils::format("第%s天",string_number[i].c_str()));
        }
        
    }
    
}
void  UI_Activity::  PHTouch(Ref *psender,Widget::TouchEventType type){
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    Widget *widget = (Widget*)psender;
    _tag=widget->getTag();
    widget->setBright(true);
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    //activity->matchInfo.v_Champion[b].userid.c_str()
    a->http_request_RankingRecordList(a->getSelectedActivity()->id, activity->matchInfo.v_Champion[_tag].userid);
    RankingRecord= CSLoader::createNode("res/CSres/Activities/RankingRecord.csb");
    RankingRecord->setVisible(false);
    addChild(RankingRecord);
    _list->addEventListener(CC_CALLBACK_2(UI_Activity::selectedItemEvent, this));
    Button * _butback= (Button *)RankingRecord->getChildByTag(2015);
    //_butback->setPosition(Vec2(200,500));
    _butback->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
        RankingRecord->setVisible(false);
    });
    //个人对比积分
}
void UI_Activity::showPHB()
{
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    CCLOG("wodemingzi %d mingzi  %d",_tab,_cellIndex);
    
    _rankLayer->setVisible(true);
    _phbList->removeAllItems();
    Button* back = (Button*)_rankLayer->getChildByTag(phb_back);
    back->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    
    
    for(int i = 0; i < activity->matchInfo.v_Champion.size(); i++)
    {
       
        _phbList->pushBackDefaultItem();
        Widget* cell = _phbList->getItem(i);
        cell->setTouchEnabled(true);
        cell->setTag(i);
        if (lx_huodong==3  || lx_huodong==4   ) {
            cell->setTouchEnabled(false);
        }
       cell->addTouchEventListener(CC_CALLBACK_2(UI_Activity::PHTouch, this));
        //玩家头像
        ImageView* touxiang = (ImageView*)cell->getChildByTag(1002);
//        touxiang->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Champion[i].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));

        touxiang->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Champion[i].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));
        
        //玩家昵称
        Text* nicheng = (Text*)cell->getChildByTag(1003);
        nicheng->setString(activity->matchInfo.v_Champion[i].nickname);
        nicheng->setColor(Color3B(22, 216, 224));
        
        //玩家等级
        Text* dengji = (Text*)cell->getChildByTag(1004);
        dengji->setString(StringUtils::format("lv.%s", activity->matchInfo.v_Champion[i].rankname.c_str()));
        dengji->setColor(Color3B(35,237,50));
        
        //玩家积分
        Text* jifen = (Text*)cell->getChildByTag(1005);
        jifen->setColor(Color3B::YELLOW);
        jifen->setString(StringUtils::format("%d", activity->matchInfo.v_Champion[i].playerIntegral));
        jifen->setVisible(true);
    
         ImageView* gaunming = (ImageView*)cell->getChildByTag(1253);
        string str_pai=StringUtils::format("CSres/Activities/ActivitiesUI/pai_%d.png",i);
        if (i<3) {
            gaunming->loadTexture(str_pai.c_str());
        }
        //玩家名次
//        char *   a[3] = {"金冠","银冠","铜冠"};
//        
//        if (i<3) {
//                Text* mingci = (Text*)cell->getChildByTag(1001);
//                mingci->setString(StringUtils::format("%s", a[i]));
//                mingci->setColor(Color3B(35,237,225));
//
//                }
//        else {
           Text* mingci = (Text*)cell->getChildByTag(1001);
           mingci->setString(StringUtils::format("第%d名", i + 1));
           mingci->setColor(Color3B(35,237,225));
       // }
    }
    
    
    IPlayerData* iplayer = M_GET_PLAYER;
    Node* myNode = _rankLayer->getChildByTag(1004);
    
    //我的头像
    ImageView* v2 = (ImageView*)myNode->getChildByTag(1001);
    v2->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(iplayer->uHeadIndex, GameIni::ImagePathType::HEAD));
    //我的积分
    ActivityData * act;
     myJifen = (Text*)myNode->getChildByTag(1002);
    myJifen->setColor(Color3B::YELLOW);
    myJifen->setString(StringUtils::format("%d", act->getw_jifen()));
    //我的名次
     myMingci = (Text*)myNode->getChildByTag(1003);
    myMingci->setString(StringUtils::format("%dTH",act->getw_paiming() )); //(int)activity->matchInfo.ranking
    return;
    
    
//    GAMELOG("----phbNumber == %lu",activity->matchInfo.v_Champion.size());
//    for(int i = 0; i < 3; i++)
//    {
//        Node* node = _rankLayer->getChildByTag(1001 + i);
//        
//        if(i < activity->matchInfo.v_Champion.size())
//        {
//            //玩家头像
//            ImageView* iv1 = (ImageView*)node->getChildByTag(1001);
//            iv1->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Champion[i].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));
//            //玩家拼图
//            ImageView* iv2 = (ImageView*)node->getChildByTag(1002);
//            iv2->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_Champion[i].adUrl, "/"), GameIni::ImagePathType::AD, true));
//            //玩家昵称
//            Text* t3 = (Text*)node->getChildByTag(1003);
//            t3->setString(activity->matchInfo.v_Champion[i].nickname);
//            //玩家电话
//            Text* t4 = (Text*)node->getChildByTag(1004);
//            t4->setString(changeUserId(activity->matchInfo.v_Champion[i].userid));
//            //玩家等级
//            Text* t5 = (Text*)node->getChildByTag(1005);
//            t5->setString(activity->matchInfo.v_Champion[i].rankname);
//            //玩家成绩
//            Text* t6 = (Text*)node->getChildByTag(1006);
//            int result = (int)activity->matchInfo.v_Champion[i].result;
////            t6->setString(StringUtils::format("%02d:%02d:%03d",result/1000/60, (result/1000)%60, result%1000));
//            t6->setString(ITools::getTimeString((long)result, fenhao_fm));
//            //光
//            Sprite* s1 = (Sprite*)node->getChildByTag(1007);
//            s1->runAction(getLightAction());
//        }
//        else
//        {
//            node->setVisible(false);
//        }
//    }
    
//    IPlayerData* iplayer = M_GET_PLAYER;
//    Node* myNode = _rankLayer->getChildByTag(1004);
//    myNode->setVisible(false);
    //我的头像
//    ImageView* v2 = (ImageView*)myNode->getChildByTag(1002);
//    v2->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(iplayer->uHeadIndex, GameIni::ImagePathType::HEAD));
    //我的成绩信息
//    if(!activity->matchInfo.v_MyResult.empty())
//    {
//        //我的拼图信息
//        ImageView* v1 = (ImageView*)myNode->getChildByTag(1001);
//        v1->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_MyResult[0].adUrl, "/"), GameIni::ImagePathType::AD, true));
//        
//        //我的最佳成绩
//        Text* mt = (Text*)myNode->getChildByTag(1003);
//        int myreset = (int)activity->matchInfo.v_MyResult[0].result;
//        mt->setString(StringUtils::format("%02d:%02d:%03d",myreset/1000/60, (myreset/1000)%60, myreset%1000));
//        mt->setString(ITools::getTimeString((long)myreset, fenhao_fm));
//        //光
//        Sprite* s2 = (Sprite*)myNode->getChildByTag(1004);
//        s2->runAction(getLightAction());
//    }
}

void UI_Activity::showWDCJ()
{
    _myGradeLayer->setVisible(true);
    Button* back = (Button*)_myGradeLayer->getChildByTag(wdcj_back);
    back->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
    
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    
    GAMELOG("----wdpmNumber == %lu",activity->matchInfo.v_MyResult.size());
    
    for(int i = 0; i < 3 && i < activity->matchInfo.v_MyResult.size(); i++)
    {
        Node* node = _myGradeLayer->getChildByTag(1001 + i);
        
        //拼图
        ImageView* iv = (ImageView*)node->getChildByTag(1001);
        iv->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->matchInfo.v_MyResult[i].adUrl, "/"), GameIni::ImagePathType::AD, true));
        //我的成绩
        Text* t1 = (Text*)node->getChildByTag(1002);
        int result = (int)activity->matchInfo.v_MyResult[i].result;
//        t1->setString(StringUtils::format("%02d:%02d:%03d",result/1000/60, (result/1000)%60, result%1000));
        t1->setString(ITools::getTimeString((long)result, fenhao_fm));
        //光
        Sprite* s1 = (Sprite*)node->getChildByTag(1003);
        s1->runAction(getLightAction());
    }
}
void UI_Activity::showGZ(bool b)
{
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    _guizeList->removeAllItems();    
    
    for(int i=0; i< activity->hdGuiZeVec.size(); i++)
    {
        _guizeList->pushBackDefaultItem();
        Node* node = _guizeList->getItem(i);
        Text* text = (Text*)node->getChildByTag(3001);
        text->setString(activity->hdGuiZeVec.at(i));
    }
    ActivityData * act;
    for(int i=0; i< act->get_guize().size(); i++)
    {
        _guizeList->pushBackDefaultItem();
        Node* node = _guizeList->getItem(i);
        Text* text = (Text*)node->getChildByTag(3001);
        text->setString(act->get_guize().at(i));
    }

    
    _activityRules->setVisible(b);
    _activityXiangqing->setVisible(!b);
}
void UI_Activity::openActivityDetails(bool bOpen)
{
    
    if(bOpen == YES)
    {
        _nowstate = 1;
        setDetailsTime();
        _list->setVisible(NO);
        _activityDetails->setVisible(YES);
        activitySation();
        setTabShow(xq_hdxq);
        _shengfenNode->setVisible(false);
    }
    else
    {
        _nowstate = 0;
        _list->setVisible(YES);
        _activityDetails->setVisible(NO);
        _shengfenNode->setVisible(false);
    }
}
void  UI_Activity::qw(){
    openActivityDetails(YES);
}
void UI_Activity::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if(type != ListView::EventType::ON_SELECTED_ITEM_END)
        return;
    is_details=true;
    IActivityData* b = _activity;
    if (b->Activity_status=="2" || b->Activity_status=="0" ) {
        is_details=false;
    }
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    auto jifenjilu=(Button*)_activityXiangqing->getChildByTag(jf_ju);
    jifenjilu->setVisible(true);
     buto=(Button*)_activityXiangqing->getChildByTag(xq_pmb);
    if (is_tinggao) {
        buto->setPositionY(buto->getPositionY()-50);
        is_tinggao=false;
    }
    ListView* listView = static_cast<ListView*>(pSender);
    CC_UNUSED_PARAM(listView);
    GAMELOG("select child end index = %ld", listView->getCurSelectedIndex());
    
    uint index = (uint)listView->getCurSelectedIndex();
    
    int isplayed = _activity->v_activityList[_tab][index]->matchInfo.isplayed;
    GAMELOG("curtime == %d", _activity->v_activityList[_tab][index]->matchInfo.curtime);
   
    
    
    switch (_tab)
    {
        case E_ActivityType::unstart:
        {
            IActivityData* a = _activity;
             _cellIndex = index;
            if (a->Activity_status=="0" && is_show_look ) {
//                UIMessage::createMesBox("活动未开始");
                return;
            }
           
            //openActivityDetails(YES);
        }break;
            
        case E_ActivityType::start:
        {
            _cellIndex = index;
            //openActivityDetails(YES);
            break;
            
            if(isplayed)
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_actjoined));
            }
            else
            {
                _cellIndex = index;
               // openActivityDetails(YES);
            }
        }break;
            
        case E_ActivityType::done:
        {
            IActivityData* a = _activity;
            _cellIndex = index;
            Activity* activity = a->v_activityList[_tab][_cellIndex];
            //a->http_request_activityInfo(activity->v_activityPrize[0]->activityid); //ActivityId
            ActivityID=activity->v_activityPrize[0]->activityid;
            if (a->Activity_status=="2"  && is_show ) {
                
                if(activity->v_activityPrize[0]->_isswardprize){
                    a->setSelectedActivity(_tab, _cellIndex);
                     a->http_request_WinnersList(ActivityID, " ");
                }
                else{
                    UIMessage::createMesBox("获奖名单两个工作日内公布，耐心等待一下吧~");
                }
                return;
            }
         
            //openActivityDetails(YES);
            break;
            
            if(isplayed)
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_actjoined));
            else
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_actover));
        }break;
            
        default:
            break;
    }
    // IActivityData* a = M_GET_ACTIVITY;//活动详情发送请求
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    a->http_request_activityInfo(activity->v_activityPrize[0]->activityid); //ActivityId
    ActivityID=activity->v_activityPrize[0]->activityid;
    showGZ();
}
string UI_Activity::getActivityID(){
    return ActivityID;
}

void UI_Activity::setActivityByType(int type)
{
    
    is_show=true;
    is_show_look=true;
    //ActivityData  * act;
    _tab = type;
    IActivityData* a = _activity;
    _list->removeAllItems();
    
    GAMELOG("a->v_activityList[%d].size() == %lu",type,a->v_activityList[type].size());
    
    for(int i = 0;i < a->v_activityList[type].size();i++){
        for(int j = 0;j < a->v_activityList[type].size()-i-1;j++){
            if(a->v_activityList[type][j]->v_activityPrize[0]->activity_Type > a->v_activityList[type][j+1]->v_activityPrize[0]->activity_Type){
                auto  t = a->v_activityList[type][j];
                a->v_activityList[type][j] = a->v_activityList[type][j+1];
                a->v_activityList[type][j+1] = t;
            }
        }
    }
    
    for(uint i = 0; i < a->v_activityList[type].size(); i ++)
    {
        
        _list->pushBackDefaultItem();
        Widget* cell = _list->getItem(i);
        if(cell)
        {
            ImageView* image;
            //图片
            image = (ImageView*)cell->getChildByTag(282);
            
            GAMELOG("iamgerul = %s", a->v_activityList[type][i]->ownerurl.c_str());
            GAMELOG("path = %s",GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(a->v_activityList[type][i]->ownerurl, "/"), GameIni::ImagePathType::ADLIST, true).c_str());
            image->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(a->v_activityList[type][i]->ownerurl, "/"), GameIni::ImagePathType::ADLIST, true));
            //活动类型（年 月 日  周  热门 。。。）
            ActivityType = (ImageView*)cell->getChildByTag(343);
            //灯光
            int timer1 = ITools::getRand(2, 5);
            Sprite* s1 = (Sprite*)cell->getChildByTag(161);
            s1->runAction(RepeatForever::create(Sequence::create(
                                                                 RotateTo::create(timer1, 90),
                                                                 RotateTo::create(timer1, -10),
                                                NULL)));
            
            Sprite* s2 = (Sprite*)cell->getChildByTag(163);
            s2->runAction(RepeatForever::create(Sequence::create(
                                                                 RotateTo::create(timer1, -90),
                                                                 RotateTo::create(timer1, 10),
                                                                 NULL)));
            
            
            Button * hd_qk = (Button *)cell->getChildByTag(1082);
            Button * hd_hjxi = (Button *)cell->getChildByTag(1081);
            hd_hjxi->setVisible(false);
            Text * hd_introduce = (Text *)cell->getChildByTag(541);
            hd_introduce->setColor(Color3B::YELLOW);
            Text * hd_daojishi = (Text *)cell->getChildByTag(668);
            hd_qk->setSwallowTouches(true);
            hd_qk->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
                is_show_look=false;
                is_details=false;
                if(type==Widget::TouchEventType::ENDED){
                CCLOG("奖项预览");
               
                AwardPreview->setVisible(true);
                a->setSelectedActivity(_tab, _cellIndex);
                a->http_request_FirstlookList(ActivityID, " ");
                _title->setString("奖品抢先看");
                }
                
            });

            
            
            
            hd_hjxi->setSwallowTouches(true);
            hd_hjxi->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
                CCLOG("获奖信息");
                is_details=false;
                is_show=false;
            //_Winning = CSLoader::createNode("res/CSres/Activities/Winning.csb");
                if(type==Widget::TouchEventType::ENDED){
            //_Winning->setVisible(true);
            //addChild(_Winning);
                
                //IActivityData* a = _activity;
                a->setSelectedActivity(_tab, _cellIndex);
                a->http_request_WinnersList(ActivityID, " ");
                }
                
            });
            
            
            
            if (a->Activity_status.empty() ||  a->Activity_status=="1"  ) {
                hd_qk->setVisible(false);
                hd_introduce->setVisible(false);
                hd_daojishi->setVisible(true);
            }
            else if(a->Activity_status=="0"){
                hd_qk->setVisible(false);
                hd_introduce->setVisible(false);
                hd_daojishi->setVisible(true);
            }
            else{
                hd_qk->setVisible(false);
                hd_introduce->setVisible(true);
                hd_daojishi->setVisible(false);
            }

            
            
            
            //最佳奖品
            
            Text* championRewardT = (Text*)cell->getChildByTag(185);
//            championRewardT->setString(a->v_activityList[type][i]->title);
            if(!a->v_activityList[type][i]->v_activityPrize.empty())
            {
                championRewardT->setString(a->v_activityList[type][i]->v_activityPrize[0]->name);
                string Act_type = StringUtils::format("CSres/Activities/ActivitiesUI/%d.png",a->v_activityList[type][i]->v_activityPrize[0]->activity_Type );
                
                ActivityType->loadTexture(Act_type);
            }
            
            //当前擂主
            Text* championT = (Text*)cell->getChildByTag(156);
            ImageView* championAndtimeIV = (ImageView*)cell->getChildByTag(91);
            switch (type)
            {
                case unstart:
                {
                    championT->setString("活动倒计时:");
                    Text* startTimeT = (Text*)cell->getChildByTag(160);
                    startTimeT->setVisible(true);
                    //剩余时间图片
                    championAndtimeIV->loadTexture("CSres/public/publicUI/timekeeper.png");
                }break;
                case start:
                {
                    championT->setString("当前擂主");
                    if(a->v_activityList[type][i]->matchInfo.v_Champion.empty())
                    {
                        Text* text1 = (Text*)cell->getChildByTag(156);
                        text1->setString("暂无擂主");
                        break;
                    }
                    //擂主昵称
                    Text* championNickNameT = (Text*)cell->getChildByTag(157);
                    championNickNameT->setString(a->v_activityList[type][i]->matchInfo.v_Champion[0].nickname);
                    championNickNameT->setVisible(false);//测试
                    //擂主时间
                    Text* championTimeT = (Text*)cell->getChildByTag(159);
////                    championTimeT->setString(StringUtils::format("%d%s",(int)a->v_activityList[type][i]->matchInfo.v_Champion[0].result/1000,"秒"));
//                    championTimeT->setString(ITools::getTimeString((long)a->v_activityList[type][i]->matchInfo.v_Champion[0].result, hanzi_tsfm));
                    championTimeT->setString(StringUtils::format("%d",a->v_activityList[type][i]->matchInfo.v_Champion[0].playerIntegral));
                    championTimeT->setVisible(false); //测试
                    //成绩
                    Text* chengji = (Text*)cell->getChildByTag(158);
                    chengji->setString("积分:");
                    chengji->setVisible(false); //测试
                    //擂主头像
                    championAndtimeIV->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(a->v_activityList[type][i]->matchInfo.v_Champion[0].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));
                    GAMELOG("##=%s",GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(a->v_activityList[type][i]->matchInfo.v_Champion[0].uHeadIndex, "/"), GameIni::ImagePathType::HEAD).c_str());
                }break;
                case done:
                {

                    championT->setString("活动擂主");
                    if(a->v_activityList[type][i]->matchInfo.v_Champion.empty())
                    {
                        break;
                    }
                    //擂主昵称
                    Text* championNickNameT = (Text*)cell->getChildByTag(157);
                    championNickNameT->setString(a->v_activityList[type][i]->matchInfo.v_Champion[0].nickname);
                    championNickNameT->setVisible(true);
                    //擂主时间
                    Text* championTimeT = (Text*)cell->getChildByTag(159);
//                    championTimeT->setString(StringUtils::format("%d%s",(int)a->v_activityList[type][i]->matchInfo.v_Champion[0].result,"秒"));
//                    GAMELOG("##%lld",a->v_activityList[type][i]->matchInfo.v_Champion[0].result);
//                    championTimeT->setString(ITools::getTimeString((long)a->v_activityList[type][i]->matchInfo.v_Champion[0].result, hanzi_tsfm));
                    championTimeT->setString(StringUtils::format("%d",a->v_activityList[type][i]->matchInfo.v_Champion[0].playerIntegral));
                    championTimeT->setVisible(true);
                    //成绩
                    Text* chengji = (Text*)cell->getChildByTag(158);
                    chengji->setString("积分:");
                    chengji->setVisible(true);
                    //擂主头像
                    championAndtimeIV->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(a->v_activityList[type][i]->matchInfo.v_Champion[0].uHeadIndex, "/"), GameIni::ImagePathType::HEAD));
                    
                }break;
                default:
                    break;
            }
            //判断是否中奖  2016.4.5
//            championT->setString("活动倒计时:");
            if(a->v_activityList[type][i]->v_activityPrize[0]->_prizewinning  &&
               a->v_activityList[type][i]->status == E_ActivityType::done)
            {
                image = (ImageView*)cell->getChildByTag(1160);//中奖
                CCLOG("恭喜中奖");
                image->setVisible(true);  //YES
            }

            
//            if(a->v_activityList[type][i]->matchInfo.curtime >= a->v_activityList[type][i]->matchInfo.totaltime)
           // CCLOG("%d",a->v_activityList[type][i]->matchInfo.prizewinning);
            if(true)
            {
//                image = (ImageView*)cell->getChildByTag(37);//蒙黑底板
//                image->setVisible(YES);
                if(a->v_activityList[type][i]->matchInfo.prizewinning)
                {
                    image = (ImageView*)cell->getChildByTag(1160);//中奖
                    CCLOG("sdfdsfdsfdsafdsf");
                    //image->setVisible(false);  //YES
                }
                else
                {
                    image = (ImageView*)cell->getChildByTag(65);//已经完成图标
                    image->setPositionY(image->getPositionY()+80);
                    //image->setVisible(false);
                }
                
                
            }
            else if(a->v_activityList[type][i]->status == E_ActivityType::done)
            {
                
                if(a->v_activityList[type][i]->matchInfo.prizewinning > 0)
                {
                    image = (ImageView*)cell->getChildByTag(1160);//中奖
                    //image->setVisible(false); //YES
                }
                else
                {
                    image = (ImageView*)cell->getChildByTag(66);//已经完成
                    //image->setVisible(false);//YES
                }
            }
        }
    }
    _list->jumpToPercentVertical(0);
    updateTime(1.0);
}


void UI_Activity::setActivityTime()
{
    IActivityData* a = _activity;
    long pasttime = UICtrl::getInstance()->pasttime / 1000;
    for(int i = 0; i < E_ActivityType::end; i ++)
    {
        for(int j = 0; j < a->v_activityList[i].size(); j ++)
        {
            a->v_activityList[i][j]->remainingtime -= pasttime;
            if(a->v_activityList[i][j]->remainingtime < 0)
                a->v_activityList[i][j]->remainingtime = 0;
        }
    }
}


void UI_Activity::updateTime(float dt)
{
    _time=dt+_time;
    IActivityData* a = _activity;
    for(int i = 0; i < E_ActivityType::end; i ++)
    {
        for(int j = 0; j < a->v_activityList[i].size(); j ++)
        {
            a->v_activityList[i][j]->remainingtime --;
            if(a->v_activityList[i][j]->remainingtime < 0)
                a->v_activityList[i][j]->remainingtime = 0;
            
            if(_tab == i)
            {
                Widget* cell = _list->getItem(j);
                if(cell)
                {
                    Text* text;
                    //剩余时间
                    
                    text = (Text*)cell->getChildByTag(160);
                    text->setVisible(true);

                    if (a->Activity_status=="2" ) {
                        text->setVisible(false);
                    }
                    else if (a->Activity_status=="0" ) {//未开始
//                        text->setString(ITools::getTimeString(a->v_activityList[i][j]->begintime-a->v_activityList[i][j]->currenttime, hanzi_tsfm));
                        CCLOG("%lld  %lld ",a->v_activityList[i][j]->currenttime, a->v_activityList[i][j]->begintime);
                        text->setString(ITools::getTimeString(a->v_activityList[i][j]->begintime- a->v_activityList[i][j]->currenttime-_time, hanzi_tsfm));
                        //参与人数
                        text = (Text*)cell->getChildByTag(291);
                        string number = StringUtils::format("%d", a->v_activityList[i][j]->matchInfo.number);
                        text->setString(number);
                    }
                    else if (a->Activity_status=="1" ) {//开始活动
//                        text->setString(ITools::getTimeString(a->v_activityList[i][j]->finishtime-a->v_activityList[i][j]->currenttime-_time, hanzi_tsfm));
                        text->setString(ITools::getTimeString(a->v_activityList[i][j]->finishtime-a->v_activityList[i][j]->currenttime-_time, hanzi_tsfm));
                        //参与人数
                        text = (Text*)cell->getChildByTag(291);
                        string number = StringUtils::format("%d", a->v_activityList[i][j]->matchInfo.number);
                        text->setString(number);
                    }
                   
                }
            }
        }
    }
    if(_nowstate == 1)
    {
        setDetailsTime();
    }
}


void UI_Activity::activitySation()
{
    ActivityData * act;
    IActivityData* a = _activity;
    Activity* activity = a->v_activityList[_tab][_cellIndex];
    
    Node *secondNode = _activityXiangqing;
    secondNode->setVisible(true);
   
    
    GAMELOG("activitySation::requirementsummary == %s", activity->requirementsummary.c_str());
    //活动图片
    ImageView* image = (ImageView*)secondNode->getChildByTag(56);

    image->loadTexture(GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->ownerurl, "/"), GameIni::ImagePathType::ADLIST, true));
    CCLOG("%s",GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->ownerurl, "/"), GameIni::ImagePathType::ADLIST, true).c_str());
    CCLOG("000 %f, %f",image->getContentSize().width,image->getContentSize().height);
    image->setAnchorPoint(Vec2(0.25, 0.6));
    image->setScaleX(0.85);
    image->setScaleY(0.6);
    image->setLocalZOrder(100);
//    image->setPosition(Vec2(image->getPositionX()+140,image->getPositionY()-15));
//    image->addTouchEventListener([=](Ref *psender,Widget::TouchEventType type){
//        ImageView * image=(ImageView *)psender;
//        
//        if (type == Widget::TouchEventType::BEGAN) {
////            _title->setString("奖项预览");
////            AwardPreview->setVisible(true);
////            a->setSelectedActivity(_tab, _cellIndex);
////            a->http_request_FirstlookList(ActivityID, " ");
////            image->setScale(0.5175,0.54);
//            
//        }
//        if (type == Widget::TouchEventType::ENDED) {
////            image->setScale(0.345,0.36);
//        }
//
//       
//    });

    
    
    
    
    
    
    
    if(!activity->v_activityPrize.empty())
    {
        //奖品数量
        
        Text* tnum = (Text*)secondNode->getChildByTag(167);
        //tnum->setString(StringUtils::format("%d个",activity->v_activityPrize[0]->amount));
        tnum->setString(StringUtils::format("%d个",act->getjx_nun()));
        //奖品名称
        Text* rewardName = (Text*)secondNode->getChildByTag(168);
        CCLOG("%d ",activity->v_activityPrize[0]->activity_Type);
        lx_huodong=activity->v_activityPrize[0]->activity_Type;
        rewardName->setString(activity->v_activityPrize[0]->name);
        db_title=StringUtils::format("%s纪录", activity->v_activityPrize[0]->name.c_str());
    }
    //按钮添加
    //Button * buto=(Button*)secondNode->getChildByTag(xq_pmb);
    int ab[] = {xq_ckjx,xq_pmb,xq_ckxq,jf_ju};
    for(int i = 0; i < sizeof(ab)/sizeof(ab[0]); i++)
    {
        if (i==1) {
            buto->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
        }
        
        else{
        Button* btn = (Button*)secondNode->getChildByTag(ab[i]);
        btn->addTouchEventListener(CC_CALLBACK_2(UI_Activity::eventCallback, this));
            if (i==3  && (lx_huodong==3 || lx_huodong==4 )) {
                //btn->setTouchEnabled(false);
                btn->setVisible(false);
                buto->setPositionY(buto->getPositionY()+50);
                is_tinggao=true;
            }
        }
        
    }
   
         //当前擂主
    if(!activity->matchInfo.v_Champion.empty())
    {
        //擂主头像
        
        //擂主昵称
        Text* championNickName = (Text*)secondNode->getChildByTag(169);
        championNickName->setString(activity->matchInfo.v_Champion[0].nickname);
        championNickName->setVisible(false);
        //擂主成绩
        Text* championGrade = (Text*)secondNode->getChildByTag(171);
//        int championtime = (int)activity->matchInfo.v_Champion[0].result;
//        championGrade->setString(ITools::getTimeString((long)championtime, fenhao_fm));
        championGrade->setString(StringUtils::format("积分:%d", activity->matchInfo.v_Champion[0].playerIntegral));
        championGrade->setVisible(false);
        //擂主电话
        Text* championPhone = (Text*)secondNode->getChildByTag(170);
        championPhone->setString(changeUserId(activity->matchInfo.v_Champion[0].userid));
        championPhone->setVisible(false);
    }
    else
    {
        //擂主头像
        ImageView* championHeadIV = (ImageView*)secondNode->getChildByTag(174);
        championHeadIV->loadTexture("");
        //擂主昵称
        Text* championNickName = (Text*)secondNode->getChildByTag(169);
        championNickName->setString("暂无擂主");
        championNickName->setVisible(false);
        //擂主成绩
        Text* championGrade = (Text*)secondNode->getChildByTag(171);
        //        int championtime = (int)activity->matchInfo.v_Champion[0].result;
        //        championGrade->setString(ITools::getTimeString((long)championtime, fenhao_fm));
        championGrade->setString(StringUtils::format("积分:%d", 0));
        championGrade->setVisible(false);
        //擂主电话
        Text* championPhone = (Text*)secondNode->getChildByTag(170);
        championPhone->setString(changeUserId("00000000000"));
        championPhone->setVisible(false);
    }
    
    //我的积分
    Text* myGrade = (Text*)secondNode->getChildByTag(191);
    //myGrade->setString(StringUtils::format("%d", activity->matchInfo.myintegral));
    myGrade->setString(StringUtils::format("%d", act->getw_paiming()));
    paiming= StringUtils::format("第%d名", act->getw_paiming());
    myGrade->setColor(Color3B(255,255,0));//我的积分颜色改变
    GAMELOG("####%d", activity->matchInfo.myintegral);
    //我的排名
    Text* myPaiming = (Text*)secondNode->getChildByTag(172);
    //myGrade->setString(StringUtils::format("%d", activity->matchInfo.myintegral));
    myPaiming->setString(StringUtils::format("%d", act->getw_jifen()));
    fenshu=StringUtils::format("%d", act->getw_jifen());
    myPaiming->setColor(Color3B(255,255,0));//我的积分颜色改变
    GAMELOG("####%d", activity->matchInfo.myintegral);
    //剩余次数
    Text* cishu = (Text*)secondNode->getChildByTag(254);
    if(activity->matchInfo.totaltime - activity->matchInfo.curtime >= 0)
        cishu->setString(StringUtils::format("%d", activity->matchInfo.totaltime - activity->matchInfo.curtime));
    
    //活动倒计时
    Text* activityTime = (Text*)secondNode->getChildByTag(173);
    activityTime->setAnchorPoint(Vec2(0.5,0.5));
    //string str=activity->begintime;
    CCLOG("%s   ====   %lu   %s",ITools::getTimeString(61820, hanzi_sfm).c_str(),ITools::getNum("2016-03-30T00:00:00"),ITools::Intercept_string("2016-03-30T00:00:00","T").c_str());
    activityTime->setString(ITools::getTimeString(61820, hanzi_sfm));
    activityTime->setScale(0.7);
    activityTime->setVisible(false);
    
    
    //活动剩下的次数
    string str_cishu = StringUtils::format("剩下%d次", activity->matchInfo.totaltime - activity->matchInfo.curtime).c_str();
    
    //设置详情中，按钮的状态 未开始，已开始，结束后等状态
    Button* btn = (Button*)_activityDetails->getChildByTag(xq_ljcy);
    Text* t1 = (Text*)btn->getChildByTag(15);
    if(_tab == done)
    {
//        t1->setString("提交订单");
//        btn->setEnabled(true);        
        if(activity->matchInfo.prizewinning > 0)
        {
            if(!activity->matchInfo.isswardprize)
            {
                t1->setString("已经结束");//提交订单  立即兑换
                btn->setEnabled(true);
            }
            else
            {
                t1->setString("已经结束");//奖品已发放   已经兑换
                btn->setEnabled(false);
            }
            return;
            
        }
        else
        {
            if(activity->matchInfo.curtime > 0)
            {
                t1->setString("已经结束");  //已经参与
            }
            else
            {
                t1->setString("已经结束"); //已经结束
            }
            
            btn->setEnabled(false);
            return;
        }
    }
    else if(_tab == start)
    {
        if(activity->matchInfo.prizewinning>0)
        {
            if (!activity->matchInfo.isswardprize)
            {
                t1->setString("开 始");  //立即兑换
                btn->setEnabled(true);
                return;
            }
            else
            {
                t1->setString("开 始");  //已经兑换
                btn->setEnabled(true);//拼乐测试
                return;
            }
        }
        else
        {
            if(activity->matchInfo.curtime > 0){
                t1->setString("开 始");//已经参与  2016 2.29  1 str_cishu
                int special_activity=activity->activitytype; //activitytype 特殊活动 1
                if (special_activity==1) {
                   t1->setString("开 始");
                    ActivityData  * act;
                    string str=act->getstr_url();
                   Application::getInstance()->openURL(str.c_str());
                    return;
                }
            }
            else
                t1->setString("开 始");   //1 立即参与
        }
        
        btn->setEnabled(true);
    }
    else if(_tab == unstart)
    {
        t1->setString("未开始");
        btn->setEnabled(false);
    }
    
    return;
}
void UI_Activity::setTabShow(int tab)
{
    Button *b;
    
    if((tab >= 0) && (tab<=3))
    {
        //    98, 99, 100
        for(int i = 98; i <= 100; i ++)
        {
            b = (Button*)rootNode->getChildByTag(i);
            if(b)
            {
                if(i - 98 == _tab)
                {
                    b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_button_down.png");
                }
                else
                {
                    b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_button_up.png");
                }
            }
        }
    }
    
    
    //详情按钮变换
    if(tab == xq_hdxq || tab == xq_gzsm)
    {
        if(tab == xq_hdxq)
        {
            b = (Button*)_activityDetails->getChildByTag(xq_hdxq);
            b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_aDetail_off.png");//1activity_aDetail_off.png
            b = (Button*)_activityDetails->getChildByTag(xq_gzsm);
            b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_rule_on.png");//1activity_rule_on.png
        }
        else
        {
            b = (Button*)_activityDetails->getChildByTag(xq_hdxq);
            b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_aDetail_on.png");//1activity_aDetail_on.png
            b = (Button*)_activityDetails->getChildByTag(xq_gzsm);
            b->loadTextureNormal("CSres/Activities/ActivitiesUI/1activity_rule_off.png");//1activity_rule_off.png
        }
    }
    
}

void UI_Activity::setPostWay(bool b)   //设置邮寄方式
{
    Button* btn1 = (Button*)gameOverNode->getChildByTag(submit_yj1);
    ImageView* iv1 = (ImageView*)btn1->getChildByTag(745);
    Button* btn2 = (Button*)gameOverNode->getChildByTag(submit_yj2);
    ImageView* iv2 = (ImageView*)btn2->getChildByTag(746);
    IActivityData* iad = M_GET_ACTIVITY;
    if(b)
    {
        //金币抵扣
        iv1->setVisible(false);
        iv2->setVisible(true);
        iad->postWay = 1;
        
    }
    else
    {
        //货到付款
        iv1->setVisible(true);
        iv2->setVisible(false);
        iad->postWay = 0;
    }
    
}

void UI_Activity::setAddressInfo()       //初始化，邮寄地址信息
{
    TextField* tf;
    IPlayerData* playerdate = M_GET_PLAYER;
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_name);
    tf->setString(playerdate->postname);
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_phone);
    tf->setString(playerdate->postphone);
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_address);
    tf->setString(playerdate->postaddress);
}

bool UI_Activity::addressIsPerfect()     //判断地址填写完善
{
    TextField* tf;
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_name);
    if(tf->getString().empty())
    {
        UIMessage::createMesBox("收件人姓名不能为空！");
        return false;
    }
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_phone);
    if(!IsPhoneNumber(tf->getString()))
    {
        UIMessage::createMesBox("收件人手机号不正确！");
        return false;
    }
    
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_address);
    if(tf->getString().empty())
    {
        UIMessage::createMesBox("收件人地址不能为空！");
        return false;
    }
    postaddress = tf->getString();
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_name);
    postname = tf->getString();
    
    tf = (TextField*)_addressDetails->getChildByTag(textfield_dz_phone);
    postphone = tf->getString();
    
    IPlayerData* iplayer = M_GET_PLAYER;
    if(iplayer->postphone.compare(postphone) == 0 &&
       iplayer->postname.compare(postname) == 0 &&
       iplayer->postaddress.compare(postaddress) == 0 &&
       iplayer->postcityname.compare(shiname) == 0 &&
       iplayer->postprovincename.compare(shengname) == 0
       )
    {
        return false;
    }
    
    return true;
}

void UI_Activity::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case 13:// 返回
        {
            if(_nowstate == 0)
            {
                Args args;
                args.content = ui_jinxiba;
                UICtrl::getInstance()->gotoScene(UI_MAIN, args);
            }
            else if(_nowstate == 1)
            {
                openActivityDetails(NO);
            }
        }break;
        case xq_back:       //详情－－返回按钮
        {
            openActivityDetails(NO);
        }break;
        case xq_ckjx:       //详情－－查看奖项
        {
            IActivityData* a = _activity;
            Activity* activity = a->v_activityList[_tab][_cellIndex];
            vector<__Array*> downloadImageInfo;//奖项图片缓冲
            GAMELOG("vector_size = %lu",activity->v_activityPrize.size());
            for(int i = 0; i< activity->v_activityPrize.size(); i++)
            {
                string strUrl;
                strUrl = activity->v_activityPrize[i]->url;
                GAMELOG("strUrl = %s", strUrl.c_str());
                string path = "";
                
                string  str= GameIni::getInstance()->getLocalSavedImagePath(ITools::Intercept_string(activity->v_activityPrize[i]->url, "/"), GameIni::ImagePathType::HEAD);
                CCLOG("%s",str.c_str());
                path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_ADREWARD_FINDER + "/" + ITools::Intercept_string(activity->v_activityPrize[i]->url, "/");
                GAMELOG("path = %s",path.c_str());
                __Array* arr1 = __Array::create(__String::create(strUrl), __String::create(str), NULL);
                downloadImageInfo.push_back(arr1);
            }
            myHttpWork* myhttp = myHttpWork::createHttp();
            myhttp->http_request_downLoadImage(downloadImageInfo, MES_ACTIVITY_JPXQ_IMAGE_OK);
            
            popNetConnectingView();
            
            
        }break;
        case xq_ckxq:       //详情－－查看详情我的成绩
        {
            showWDCJ();
        }break;
        case xq_hdxq:       //详情－－活动详情
        {
            showGZ();
            setTabShow(xq_hdxq);
        }break;
        case xq_gzsm:       //详情－－规则说明
        {
            showGZ(true);
            setTabShow(xq_gzsm);
        }break;
        case xq_ljcy: //126      //立即参与，进入游戏
        {
//            gameOverNode->setVisible(true);
//            submitRewarkInfo();
//            break;
            IActivityData* a = _activity;
            a->setSelectedActivity(_tab, _cellIndex);
            Activity* activity1 = a->v_activityList[_tab][_cellIndex];
 
            if(_tab == E_ActivityType::done)
            {
                setPostWay(true);
                GAMELOG("tijiaodingdan....");
                gameOverNode->setVisible(true);
                submitRewarkInfo();
                break;
            }
            
            if(_tab == E_ActivityType::unstart)
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_actunstart));
                break;
            }
            
            
            
            //玩家中奖提交订单
//            if(!activity1->matchInfo.isswardprize && activity1->matchInfo.prizewinning > 0)
//            {
//                if(activity1->activitytype == ACTIVITY_TYPE::onece_type){
//                    Application::getInstance()->openURL(activity1->adweburl);
//                    CCLOG("平安活动提交订单  %s",activity1->adweburl.c_str());
//                }
//                else if(activity1->activitytype == ACTIVITY_TYPE::nomal_type)
//                {
//                    setPostWay(true);
//                    GAMELOG("tijiaodingdan....");
//                    gameOverNode->setVisible(true);
//                    submitRewarkInfo();
//                }
//                break;
//            }
            
            
            IPlayerData *iplayer = M_GET_PLAYER;
            if(!iplayer->isCompletedInfo())
            {
                messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_wsgrxx),  1);
                messageBox->_args.code = tips_info_wsgrxx;
                break;
            }
            
            
            //判断玩家参加活动次数
            GAMELOG("%d--%d",activity1->matchInfo.curtime,activity1->matchInfo.totaltime);
            CCLOG("测试 activity1->matchInfo.prizewinning＝ %d",activity1->matchInfo.prizewinning);
            if(activity1->matchInfo.prizewinning && !activity1->matchInfo.isswardprize)
            {
                 CCLOG("立即兑换url链接  %s",activity1->adweburl.c_str());
                Application::getInstance()->openURL(activity1->adweburl);
                //break;
            }
//            if(activity1->matchInfo.prizewinning && activity1->matchInfo.isswardprize)
//            {//拼乐测试  已经兑换
//                
//                messageBox = UIMessage::createMesBox(StringUtils::format("您已经参加了%d次了，请参加其他活动吧！",activity1->matchInfo.totaltime));                break;
//            }
//
//            if(activity1->matchInfo.curtime >= activity1->matchInfo.totaltime && !activity1->matchInfo.prizewinning)
//            {//拼乐测试还未兑换就出现框
//                messageBox = UIMessage::createMesBox(StringUtils::format("您已经参加了%d次了，请参加其他活动吧！",activity1->matchInfo.totaltime));
//                 
//                break;
//            }
            //判断当前玩家金币是否够扣除数
            if(activity1->matchInfo.needGold.size() > 0)
            {
//                GAMELOG("%d--%d--%d",iplayer->golds,activity1->matchInfo.needGold[activity1->matchInfo.curtime],activity1->matchInfo.curtime);
//                if(iplayer->golds < activity1->matchInfo.needGold[activity1->matchInfo.curtime])
                if(iplayer->golds < activity1->matchInfo.needGold[0])
                {
                    messageBox = UIMessage::createMesBox("您的金币不足！挣够了金币再来赢大奖吧");
                    break;
                }
            }
            
            
//            if(activity1->remainingtime <= 0)
//            {
//                messageBox = UIMessage::createMesBox("活动已结束！！！");
//                break;
//            }
            
            IRoomData* room = M_GET_ROOM;
            room->enterGamePlay(GameMode::activity, a->getSelectedActivity());
        }break;
        case xq_pmb:        //详情－－排行榜
        {
             if(type==Widget::TouchEventType::ENDED){
            IActivityData* a = _activity;
            a->setSelectedActivity(_tab, _cellIndex);
            GAMELOG("a->getSelectedActivity()->id = %s", a->getSelectedActivity()->id.c_str());
            a->http_request_activityRankingList(a->getSelectedActivity()->id, 1);
             }
        }break;
        case jf_ju:        //详情－－积分排行榜
        {
            if(type==Widget::TouchEventType::ENDED){
            IActivityData* a = _activity;
            Activity* activity = a->v_activityList[_tab][_cellIndex];
            a->setSelectedActivity(_tab, _cellIndex);
            PersonalRecord = CSLoader::createNode("res/CSres/Activities/PersonalRecord.csb");
            PersonalRecord->setVisible(false);
            addChild(PersonalRecord);
            ImageView* bg= (ImageView*)PersonalRecord->getChildByTag(515);
            Text* title= (Text *)bg->getChildByTag(518);
            title->setString(StringUtils::format("%s个人纪录", activity->v_activityPrize[0]->name.c_str()));
            Button* _back= (Button*)bg->getChildByTag(517);
            _back->addTouchEventListener(CC_CALLBACK_2(UI_Activity::Call_back,this));
            //IActivityData* a = _activity;
            a->setSelectedActivity(_tab, _cellIndex);
            GAMELOG("a->getSelectedActivity()->id = %s", a->getSelectedActivity()->id.c_str());
            //CCLOG("  %s",changeUserId(activity->matchInfo.v_Champion[0].userid).c_str());
            a->http_request_integralRankingList(a->getSelectedActivity()->id, " ");
            
            _pjfList = (ListView*)PersonalRecord->getChildByTag(488);
//            Layout *  layout_list = (Layout*)_pjfList->getChildByTag(741);
//            Text * text_lishi=(Text * )layout_list->getChildByTag(524);
            //text_lishi->setString(StringUtils::format("%d",rand()%1243-500));
            
//            _pjfList->setItemModel(_pjfList->getItem(0));
//            for(int i = 0; i<6; i++)  //i < activity->matchInfo.v_Integral.size()
//            {
//                _pjfList->pushBackDefaultItem();
//                Widget* cell = _pjfList->getItem(i);
//            }
            }
            
        }break;
        case wdcj_back:     //我的成绩－－返回按钮
        {
            _myGradeLayer->setVisible(false);
        }break;
        case jpxq_back:     //奖品详情－－返回按钮
        {
            _rewardDetails->setVisible(false);
        }break;
        case phb_back:      //排行榜返回
        {
            _rankLayer->setVisible(false);
        }break;
        case hdgz_close:    //活动规则关闭
        {
            _activityRules->setVisible(false);
        }break;
        case 98:// 未开始
        case 99:
        case 100:
        {
            IActivityData* a = M_GET_ACTIVITY;
            a->Activity_status=StringUtils::format("%d",tag-98);
            a->http_request_activityList();
            int type = tag - 98;
            
            if(_nowstate != 0)
            {
                openActivityDetails(NO);
                _nowstate = 0;
            }
            if(_tab == type)
                break;
            
            setActivityByType(type);
            setTabShow(type);
        }break;
            
//        case 126: //进入游戏
//        {
//            
//        }break;
        
        case submit_tj:
        {
            IRoomData* iroomData = M_GET_ROOM;
            IPlayerData* iplayerData = M_GET_PLAYER;
            IActivityData* iad = M_GET_ACTIVITY;
            iad->setSelectedActivity(_tab, _cellIndex);
            
            if(iplayerData->postprovinceid > 0 && iplayerData->postcityid > 0 && iplayerData->postname.length() > 1 && iplayerData->postphone.length() > 1)
            {
                Activity* aasd = iad->getSelectedActivity();
                GAMELOG("size==%ld", aasd->v_activityPrize.size());
                iad->rewardUrl = iad->getSelectedActivity()->v_activityPrize[iad->getSelectedActivity()->matchInfo.prizewinning - 1]->url;
                iroomData->sendSubmitRewardInfo();
            }
            else
            {
                messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_changeinfo), 1);
                messageBox->_args.content = change_player_info;
                messageBox->setButtonString("前往", "取消");
            }
        }break;
        case submit_xg:
        {
            _addressDetails->setVisible(true);
            setAddressInfo();
            needReloadAddressInfo = true;
            gameOverNode->setVisible(false);
            gameOverNode->setTag(1);
        }break;
        case submit_fq:
        {
            gameOverNode->setVisible(false);
        }break;
        case submit_yj1:    //货到付款
        {
            setPostWay(false);
        }break;
        case submit_yj2:    //金币抵消
        {
            IPlayerData* iplayer = M_GET_PLAYER;
            if(iplayer->golds > 1000)
            {
                setPostWay(true);
            }
            else
            {
                UIMessage::createMesBox("金币不足！！！");
            }
            
            
        }break;
        case 1012:          //修改邮寄地址
        {
            _addressDetails->setVisible(true);
            setAddressInfo();
            needReloadAddressInfo = false;
            _shengfenNode->setVisible(false);
        }break;
        case dz_back:
        {
            _addressDetails->setVisible(false);
            _shengfenNode->setVisible(false);
            
            if(gameOverNode->getTag() == 1)
            {
                gameOverNode->setVisible(true);
                gameOverNode->setTag(11);
            }
            
        }break;
        case dz_baocun:
        {
            IPlayerData* playerdate = M_GET_PLAYER;
            if(addressIsPerfect())
            {
                playerdate->http_request_sendPostAddress(postname, postphone, shengid, shiid, postaddress);
            }
            
        }break;
        case dz_sheng:
        {
            shSelectTag = shtag_sheng;
            
            if(list_sheng->isVisible())
            {
                list_sheng->setVisible(false);
            }
            else
            {
                list_sheng->setVisible(true);
            }
            
        }break;
        case dz_shi:
        {
            shSelectTag = shtag_shi;
            
            if(list_shi->isVisible())
            {
                list_shi->setVisible(false);
            }
            else
            {
                if(list_shi->getItems().size() > 0)
                    list_shi->setVisible(true);
            }
        }break;
        case 2003:
        {
            Button* btn = (Button*)_shengfenNode->getChildByTag(2004);
            if(btn->isVisible())
            {
                btn->setVisible(false);
                break;
            }
            
            btn->setVisible(true);
            Text* tx = (Text*)btn->getChildByTag(3001);
            if(!quanguoHD)
                tx->setString("全国");
            else
            {
                IPlayerData* playerData = M_GET_PLAYER;
                tx->setString(playerData->cityname);
            }
            
        }break;
        case 2004:
        {
            Text* tx = (Text*)_shengfenNode->getChildByTag(2002);
            if(!quanguoHD)
            {
                tx->setString("全国");
            }
            else
            {
                IPlayerData* playerData = M_GET_PLAYER;
                tx->setString(playerData->cityname);
            }
            _shengfenNode->getChildByTag(2004)->setVisible(false);
            
            
            IActivityData* a = M_GET_ACTIVITY;
            if(!quanguoHD)
                a->http_request_activityList(1);
            else
                a->http_request_activityList(0);
                        
            quanguoHD = !quanguoHD;
            
        }break;
        default:
            break;
    }
}
void  UI_Activity::Call_back(Ref * pSender,Widget::TouchEventType type){
    PersonalRecord->setVisible(false);
}


bool UI_Activity::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return YES;
    
    Notify_Compare(MES_ROOM_GAMESTART)
    {
        UICtrl::getInstance()->gotoScene(UI_ROOM);
        return YES;
    }
    if (name.compare(MESSAGEBOXOK) == 0) {
        _activityDetails->setVisible(true);
    }
    if (name.compare(MES_Eventdetails) == 0) {
        _activityDetails->setVisible(true);
    }
    Notify_Compare(SYSTEM_ENTERFOREGROUND)
    {
        setActivityTime();
        return YES;
    }
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        Ref *pSender = rootNode->getChildByTag(13);
        
        if(_rankLayer->isVisible())
        {
            pSender = _rankLayer->getChildByTag(phb_back);
        }
        else if(_rewardDetails->isVisible())
        {
            pSender = _rewardDetails->getChildByTag(jpxq_back);
        }
        else if(_myGradeLayer->isVisible())
        {
            pSender = _myGradeLayer->getChildByTag(wdcj_back);
        }
        else if(gameOverNode->isVisible())
        {
            pSender = gameOverNode->getChildByTag(submit_fq);
        }
        else if(_addressDetails->isVisible())
        {
            pSender = _addressDetails->getChildByTag(dz_back);
        }
        
        if(pSender)
            eventCallback(pSender, Widget::TouchEventType::ENDED);
        return YES;
    }
    
    if(name.compare(MES_ACTIVITY_PHB_INFOR) == 0)
    {
//        IActivityData* a = _activity;
//        Activity* activity = a->v_activityList[_tab][_cellIndex];
//        
//        vector<__Array*> downloadImageInfo;
//        GAMELOG("vector_size = %lu",activity->matchInfo.v_Champion.size());
//        for(int i = 0; i< activity->matchInfo.v_Champion.size(); i++)
//        {
//            string path = "";
//            path = path + IMAGE_ROOT_FINDER + "/" + IMAGE_AD_FINDER + "/" + ITools::Intercept_string(activity->matchInfo.v_Champion[i].adUrl, "/");
//            
//            __Array* arr1 = __Array::create(__String::create(activity->matchInfo.v_Champion[i].adUrl), __String::create(path), NULL);
//            downloadImageInfo.push_back(arr1);
//        }
//        myHttpWork* myhttp = myHttpWork::createHttp();
//        myhttp->http_request_downLoadImage(downloadImageInfo, MES_ACTIVITY_PHB_IMAGE_OK);
//        
//        
//        //等待下载图片－－转菊花
//        popNetConnectingView();
        /////////////
        showPHB();
        
        return YES;
    }
    if(name.compare(MES_ACTIVITY_PHB_IMAGE_OK) == 0)
    {
        closeNetConnectingView();
        showPHB();
        return YES;
    }
    if(name.compare(MES_ACTIVITY_PHB_IMAGE_OK) == 0)
    {
        closeNetConnectingView();
        showPHB();
        return YES;
    }
    if(name.compare(MES_ACTIVITY_JF_INFOR) == 0)
    {
        //closeNetConnectingView();
        
        showJFB();
        PersonalRecord->setVisible(true);
        return YES;
    }
    if(name.compare(MES_ACTIVITY_RankingRecord) == 0)
    {
        showDB();
        //RankingRecord->setVisible(true);
        return YES;
    }
    
    if(name.compare(MESSAGEBOX_OK) == 0)
    {
        if(args.code == tips_info_wsgrxx)
        {
            messageBox = NULL;
            UICtrl::getInstance()->gotoScene(UI_OPTION, false);
            return YES;
        }
        
        if(args.content.compare(change_player_info) == 0)
        {
            messageBox = NULL;
//            UICtrl::getInstance()->gotoScene(UI_OPTION, false);
            Widget* widget = (Widget*)gameOverNode->getChildByTag(submit_xg);
            eventCallback(widget, Widget::TouchEventType::ENDED);
            return YES;
            
        }
        
        
        if(args.code == mesbox_submit_over)
        {
            messageBox = NULL;
            gameOverNode->setVisible(false);
            
            Button* btn = (Button*)_activityDetails->getChildByTag(xq_ljcy);
            Text* t1 = (Text*)btn->getChildByTag(15);
            t1->setString("奖品已发放");
            btn->setEnabled(false);
            return true;
        }
        
//        if(_addressDetails->isVisible())
//        {
//            _addressDetails->setVisible(false);
//            _shengfenNode->setVisible(false);
//            if(gameOverNode->getTag() == 1)
//            {
//                gameOverNode->setVisible(true);
//                gameOverNode->setTag(11);
//            }
//            return YES;
//        }
        
        return YES;
    }
    
    if(name.compare(MESSAGEBOX_CANCEL) == 0)
    {
        if(args.code == tips_info_wsgrxx)
        {
            
        }
        messageBox = NULL;
        return YES;
    }
    
    if(name.compare(MES_OPTION_CLOSE) == 0)
    {
        if(_tab == E_ActivityType::done)
            reloadActivityInfo();
        return YES;
    }
    
    if(name.compare(MES_ROOM_SUBMITREWARE) == 0)
    {
        messageBox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_getreward));
        messageBox->_args.code = mesbox_submit_over;
        return YES;
    }
    if(name.compare(MES_ACTIVITY_Winning) == 0)
    {
        showWinning();
    }
    if(name.compare(MES_ACTIVITY_AwardPreview) == 0)
    {
        showAwardPreview();
    }
    if(name.compare(MES_PLAYER_POSTADDRESSOK) == 0)
    {
        IPlayerData* iplayer = M_GET_PLAYER;
        iplayer->postname = postname;
        iplayer->postphone = postphone;
        iplayer->postaddress = postaddress;
        iplayer->postcityid = shiid;
        iplayer->postcityname = shiname;
        iplayer->postprovinceid = shengid;
        iplayer->postprovincename = shengname;
        
        UIMessage::createMesBox("邮寄地址修改成功！");
        if(needReloadAddressInfo)
            reloadActivityInfo();
        
        needReloadAddressInfo = false;
    }
    
    if(name.compare("liaojian_xq") == 0 && is_details==true )
    {
        qw();
        return YES;
    }
    if(name.compare(MES_ACTIVITY_GETLISTDONE) == 0)
    {
        popNetConnectingView();
        return YES;
    }
    if(name.compare(MES_ACTIVITY_GETIMAGEDONE) == 0)
    {
        closeNetConnectingView();
        setActivityByType(_tab);
        return YES;
    }
    return YES;
}

string UI_Activity::changeUserId(string str)
{
    string s = str;
    s[3] = '*';
    s[4] = '*';
    s[5] = '*';
    s[6] = '*';
    return s;
}

Action* UI_Activity::getLightAction()
{
    Action* action = RepeatForever::create(Sequence::create(
                                           RotateBy::create(3, 360),
                          NULL));
    return action;
}
