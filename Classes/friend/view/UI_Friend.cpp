//
//  UI_Friend.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#include "UI_Friend.h"

#include "../../ThirdParty/C2DXShareSDK/ShareSDKHelper.h"


Scene* UI_Friend::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Friend::create();
    
    //    auto layer = PersonCenterLayerEx::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Friend::UI_Friend()
{
    _player = M_GET_PLAYER;//
    IUILayer::attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_PLAYER_SETINVITECODE);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_GETFRIENDREWARD);
    IMessageManager::getInstance()->attach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
}

UI_Friend::~UI_Friend()
{
    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_PLAYER_SETINVITECODE);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_GETFRIENDREWARD);
    IMessageManager::getInstance()->detach(this, MES_GAMEINI_SCREENSHOOT_SUCCEED);
}




enum
{
    P_INVITE = 62,      //邀请码容器
    B_BACK = 21,
    B_INVITE = 32,
    B_INVITE_BACK = 306,
    B_INVITE_SAVE = 67,
    B_GETGOLDS = 37,
    B_YQHY = 351,       //邀请好友
    T_MYPHONE = 60, //我的邀请码
    I_INVITE_AUTOCLOSE = 99,
    TF_INVITE_PHONE = 66,
    T_INVITEPHONE = 61,     //邀请我的人
    B_INVITE_YQSM = 884,    //邀请好友说明
    N_INVITE_YQSM = 885,    //邀请好友说明Node
    B_YQHYSM_BACK = 887,    //邀请好友说明界面返回按钮
    
    L_FRIENDS = 39,
    T_TOTALPOINTS = 36,
    T_TOTALGOLDS = 35,
    
    T_FRIENDS_TOTALGOLDS = 63,
    T_FRIENDS_TOTALPOINTS = 64,
    
    T_FRIENDS_COUNT = 300,
    
};


bool UI_Friend::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    Text* t;
    Button* b;
    ImageView* image;
    
    rootNode = CSLoader::createNode("CSres/Setup/Invitation.csb");
    addChild(rootNode);
    
    inviteNode = rootNode->getChildByTag(N_INVITE_YQSM);
    Widget* ww = (Widget*)inviteNode->getChildByTag(B_YQHYSM_BACK);
    ww->addTouchEventListener(CC_CALLBACK_2(UI_Friend::eventCallback, this));
    inviteList = (ListView*)inviteNode->getChildByTag(889);
    
    int arr[] = {B_BACK, B_YQHY, B_GETGOLDS, B_INVITE_YQSM};
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i ++)
    {
        Widget* widget = (Widget*)rootNode->getChildByTag(arr[i]);
        widget->addTouchEventListener(CC_CALLBACK_2(UI_Friend::eventCallback, this));
    }
    t = (Text*)rootNode->getChildByTag(T_MYPHONE);
    t->setString(_player->userid);
    
//    p_invite = (Node*)rootNode->getChildByTag(P_INVITE);
    
//    int arr1[] = {B_INVITE_SAVE, B_INVITE_BACK};
//    for(int i = 0; i < sizeof(arr1)/sizeof(arr1[0]); i ++)
//    {
//        Widget* widget = (Widget*)p_invite->getChildByTag(arr1[i]);
//        widget->addTouchEventListener(CC_CALLBACK_2(UI_Friend::eventCallback, this));
//    }
    
//    TextField* tf = (TextField*)p_invite->getChildByTag(TF_INVITE_PHONE);
//    tf->addEventListener(CC_CALLBACK_2(UI_Friend::textFieldCallBack, this));
//    tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    
    t = (Text*)rootNode->getChildByTag(T_FRIENDS_COUNT);
    t->setString(StringUtils::format("%lu", _player->v_friends.size()));
    
    setInviteCode(_player->invitecode);
    
    l_friends = (ListView*)rootNode->getChildByTag(L_FRIENDS);
    l_friends->setItemModel(l_friends->getItem(0));
    setFriendsList();
    
    return true;
}


void UI_Friend::setFriendsList()
{
    Text* t;
    
    t = (Text*)rootNode->getChildByTag(T_FRIENDS_TOTALGOLDS);
    t->setString(StringUtils::format("%d", _player->friendsTotalGolds));
    GAMELOG("friendsTotalGolds = %d", _player->friendsTotalGolds);
    t = (Text*)rootNode->getChildByTag(T_FRIENDS_TOTALPOINTS);
    t->setString(StringUtils::format("%d", _player->friendsTotalPoints));
    GAMELOG("friendsTotalPoints = %d", _player->friendsTotalPoints);
    IPlayerData *ipayerData = M_GET_PLAYER;
//    ipayerData->golds+=_player->friendsTotalGolds;
//    ipayerData->totalpoints+=_player->friendsTotalPoints;
    t = (Text*)rootNode->getChildByTag(T_TOTALGOLDS);
    t->setString(StringUtils::format("%d",_player->friendsgolds));// _player->friendsTotalGolds));
    
    t = (Text*)rootNode->getChildByTag(T_TOTALPOINTS);
    t->setString(StringUtils::format("%d", _player->friendspoints));//_player->friendsTotalPoints));
    
    l_friends->removeAllItems();
    
    for(uint i = 0; i < _player->v_friends.size(); i ++)
    {
        Text* t;
        l_friends->pushBackDefaultItem();
        Widget* cell = l_friends->getItem(i);
        ImageView* head = (ImageView*)cell->getChildByTag(41);//头像
        head->loadTexture(StringUtils::format("CSres/Setup/Head/%s", _player->v_friends[i].uHeadIndex.c_str()));
        t = (Text*)cell->getChildByTag(42);//昵称
        t->setString(_player->v_friends[i].nickname);
        t = (Text*)cell->getChildByTag(43);//被邀请人等级
        t->setString(_player->v_friends[i].rankname);
//        t = (Text*)cell->getChildByTag(45);//等级记录
//        t->setString(_player->v_friends[i].recordgrade);
        t = (Text*)cell->getChildByTag(47);//总金币
        t->setString(StringUtils::format("%d", _player->v_friends[i].totalgolds));
        t = (Text*)cell->getChildByTag(49);//今日贡献金币
        t->setString(StringUtils::format("%d", _player->v_friends[i].todaygolds));
    }
    l_friends->jumpToPercentVertical(0);
}

void UI_Friend::setInviteCode(const string& s)
{
    if(!s.empty())
    {
        Text* t = (Text*)rootNode->getChildByTag(T_INVITEPHONE);
        t->setString(s);
        Widget* widget = (Widget*)rootNode->getChildByTag(B_INVITE);
        widget->setVisible(NO);
    }
}




void UI_Friend::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case B_BACK:
        {
            UICtrl::getInstance()->gotoScene(UI_OPTION);
        }break;
//        case B_INVITE:
//        {
//            p_invite->setVisible(YES);
//        }break;
//        case B_INVITE_SAVE:
//        {
//            TextField* tf = (TextField*)p_invite->getChildByTag(TF_INVITE_PHONE);
//            if(!isVauled(tf) || !IsPhoneNumber(tf->getString()))
//            {
//                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_phone));
//            }
//            else if(tf->getString().compare(_player->userid) == 0)
//            {
//                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_invitenotself));
//            }
//            else
//            {
//                _player->http_request_setInviteCode(tf->getString());
//            }
//        }break;
//        case B_INVITE_BACK:
//        {
//            p_invite->setVisible(NO);
//        }break;
//        case I_INVITE_AUTOCLOSE:
//        {
//            p_invite->setVisible(NO);
//        }break;
        case B_GETGOLDS:
        {
            //if(_player->friendsTotalGolds <= 0 || _player->friendsTotalPoints <= 0)
            if(_player->friendspoints <= 0 && _player->friendsgolds <= 0)
            {
                UIMessage::createMesBox("没有可领取的奖励");
            }
            else
            {
                _player->http_request_getFriendsReward();
            }
        }break;
        case B_YQHY:        //邀请好友  分享
        {
//            分享 链接＋内容＋本地图片
            ShareSDKHelper::getInstance()->sendLink(GameIni::getInstance()->shareSdkInfo.title,
                                                     GameIni::getInstance()->shareSdkInfo.description,
                                                    GameIni::getInstance()->shareSdkInfo.webpageUrl,
                                                    GameIni::getInstance()->getShareLocalPath("res/Share/share_phone.png"));
            CCLOG("分享 %s 12 %s 23 %s ",GameIni::getInstance()->shareSdkInfo.title.c_str(),GameIni::getInstance()->shareSdkInfo.description.c_str(),GameIni::getInstance()->shareSdkInfo.webpageUrl.c_str());
//            break;
            //分享截屏
//            GameIni::getInstance()->screenShoot();      //截屏发送分享流程    不可删除 IMAGE_SCREENSHOOT_FILENAME
            //分享本地图片
//            ShareSDKHelper::getInstance()->sendImage(GameIni::getInstance()->shareSdkInfo.title,
//                                                     GameIni::getInstance()->shareSdkInfo.description,
//                                                     GameIni::getInstance()->getShareLocalPath("res/Share/share_phone.png"));
        }break;
        case B_INVITE_YQSM:
        {
            inviteNode->setVisible(true);
            inviteList->jumpToTop();
        }break;
        case B_YQHYSM_BACK:
        {
            inviteNode->setVisible(false);
        }break;
        default:
            break;
    }
}

//void UI_Friend::textFieldCallBack(Ref* pSender, TextField::EventType type)
//{
//    TextField* tf = (TextField*)pSender;
//    GAMELOG("tf->getTag = %d, type = %d", tf->getTag(), type);
//    switch (type)
//    {
//        case TextField::EventType::ATTACH_WITH_IME:
//        {
//            IAudio::getInstance()->playEffect(music_ef_ok);
//            if(!tf->getString().compare(tf->getPlaceHolder()) || tf->getString().empty())
//            {
//                tf->setString("");
//            }
//        }break;
//        case TextField::EventType::DETACH_WITH_IME:
//        {
//            IAudio::getInstance()->playEffect(music_ef_cancel);
//            if(tf->getString().empty())
//            {
//                tf->setString(tf->getPlaceHolder());
//            }
//        }break;
//        default:
//            break;
//    }
//}


bool UI_Friend::Notify(const string& name, Args& args)
{
    if(IUILayer::Notify(name, args))
        return true;
    
//    Notify_Compare(MES_PLAYER_SETINVITECODE)
//    {
//        p_invite->setVisible(NO);
//        setInviteCode(_player->invitecode);
//        UIMessage::createMesBox("提交完成");
//        return YES;
//    }
    Notify_Compare(MES_PLAYER_GETFRIENDREWARD)
    {
        setFriendsList();
        UIMessage::createMesBox("领取成功");
        return YES;
    }
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
//        if(p_invite->isVisible())
//        {
//            p_invite->setVisible(NO);
//            return YES;
//        }
        Ref* sender = rootNode->getChildByTag(B_BACK);
        if(sender)
            eventCallback(sender, Widget::TouchEventType::ENDED);
        return YES;
    }
    
    Notify_Compare(MES_GAMEINI_SCREENSHOOT_SUCCEED)
    {
        GAMELOG("---%s",args.content.c_str());
        ShareSDKHelper::getInstance()->sendImage(GameIni::getInstance()->shareSdkInfo.title,
                                                 GameIni::getInstance()->shareSdkInfo.description,
                                                 args.content);
        return YES;
    }
    
    return true;
}






