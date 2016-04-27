//
//  UIcontroller.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#include "UIcontroller.h"
#include "../framework/config.h"
#include "../activity/view/UI_Activity.h"
#include "../gameroom/view/GameScene.h"
#include "../option/view/UI_Option.h"
#include "../main/view/UI_Main.h"
#include "../market/view/UI_Market.h"
#include "../login/view/UI_Login.h"
#include "../logo/view/UI_Logo.h"
#include "../competitive/view/UI_Choice.h"
#include "../friend/view/UI_Friend.h"
#include "../pokedex/view/UI_Pokedex.h"
#include "../account/view/UI_Account.h"
#include "../gamelink/GameLink.h"

static UICtrl* uictrl = NULL;


UICtrl::UICtrl()
{
    scene = NULL;
    lastScene = NULL;
    sceneID = 0;
    lastSceneID = 0;
    INIT_ARGS(_args);
}

UICtrl::~UICtrl()
{
    
}


UICtrl* UICtrl::getInstance()
{
    if(uictrl == NULL)
    {
        uictrl = new UICtrl();
    }
    return uictrl;
}


void UICtrl::destroyInstance()
{
    SAFE_DELETE(uictrl);
}



Scene* UICtrl::getScene(int sceneid)
{
    Scene* s = NULL;
    switch (sceneid)
    {
        case UI_ACTIVITY:
            s = UI_Activity::createScene();
            break;
        case UI_ROOM:
            s = GameScene::createScene();
            break;
        case UI_OPTION:
            s = UI_Option::createScene();
            break;
        case UI_MAIN:
            s = UI_Main::createScene();
            break;
        case UI_MARKET:
            s = UI_Market::createScene();
            break;
        case UI_LOGIN:
            s = UI_Login::createScene();
            break;
        case UI_LOGO:
            s = UI_Logo::createScene();
            break;
        case UI_PK:
            s = UI_Choice::createScene();
            break;
        case UI_FRIEND:
            s = UI_Friend::createScene();
            break;
        case UI_POKEDEX:
            s = UI_Pokedex::createScene();
            break;
        case UI_ACCOUNT:
            s = UI_Account::createScene();
            break;
        case UI_LINK:
            s = GameLink::createScene(_args.content);
            break;
        default:
            break;
    }
    
    return s;
}


void UICtrl::gotoScene(int sceneid, Args args, bool replace)
{
    gotoScene(sceneid, TransType::Normal, args, replace);
}


void UICtrl::gotoScene(int sceneid, int  transType, Args args, bool replace)
{
    _args = args;
    gotoScene(sceneid, transType, replace);
}

void UICtrl::gotoScene(int sceneid, int  transType, bool replace)
{
    lastSceneID = sceneID;
    lastScene   = scene;
    sceneID = sceneid;
    Scene* s = getScene(sceneid);
    scene = s;
    
    if(sceneid == UI_LOGIN)
        transType = CrossFade;
    
    switch (transType)
    {
        case TransType::Fade:
            s = TransitionFadeTR::create(1.0f, s);
            break;
        case TransType::MoveIn:
            s = TransitionMoveInR::create(0.1f, s);
            break;
        case TransType::CrossFade:
            s = TransitionCrossFade::create(0.7f, s);
            break;
        default:
            break;
    }
    if(s)
    {
        if(replace)
            Director::getInstance()->replaceScene(s);
//        if(replace)
//            Director::getInstance()->pushScene(s);
        else
            Director::getInstance()->runWithScene(s);
    }
    INIT_ARGS(_args);
}

void UICtrl::gotoScene(int sceneid, bool replace)
{
    gotoScene(sceneid, TransType::Normal, replace);
}


void UICtrl::poptoScene(int sceneid)
{
    lastSceneID = sceneID;
    lastScene   = scene;
    sceneID = sceneid;

//    Scene* s = getScene(sceneid);
    Director::getInstance()->popScene();
//    if(s)
//    {
//        Director::getInstance()->popScene();
//        scene = s;
//    }
    INIT_ARGS(_args);
}



Msg_Layer*  UICtrl::createMesBox(const string& s, int type)
{
    Msg_Layer* layer = Msg_Layer::createEx(Color4B(0, 0, 0, 0));
    if(layer)
    {
        
    }
    return layer;
}

bool UICtrl::needModifyInfo()
{
    if(lastSceneID == UI_ROOM)          return true;
    if(lastSceneID == UI_ACTIVITY)      return true;
    if(lastSceneID == UI_PK)            return true;
    return false;
}


UIMessage::UIMessage()
{
    INIT_ARGS(_args);
}


UIMessage::~UIMessage()
{
    
}


void UIMessage::setButtonString(const string& left, const string& right)
{
    if(bOK)
    {
        Text *t = (Text*)bOK->getChildByTag(0);
        t->setString(left);
    }
    if(bCancel)
    {
        Text *t = (Text*)bCancel->getChildByTag(0);
        t->setString(right);
    }
}

UIMessage*  UIMessage::createMesBox(const string& s, const Args* args, int type)
{
    UIMessage* m = createMesBox(s, type);
    if(m && args)
        m->_args = *args;
    return m;
}

UIMessage*  UIMessage::createMesBox(const string& s, const Args& args, int type)
{
    UIMessage* m = createMesBox(s, type);
    if(m)
        m->_args = args;
    return m;
}
 UIMessage*  UIMessage::create_box(const string& s, int type ){
     UIMessage* mes = new UIMessage();
     mes->msgLayer = Msg_Layer::createEx(Color4B(0, 0, 0, 0));
     mes->msgLayer->setEnterType(Msg_Layer::EnterType::ScalePop);
     mes->root = CSLoader::createNode("CSres/public/Dialog_1.csb");
     mes->bOK = (Button*)mes->root->getChildByTag(MESBOX_OK_TAG);
     mes->bOK->addTouchEventListener(CC_CALLBACK_2(UIMessage::event_Callback, mes));
     mes->bCancel = (Button*)mes->root->getChildByTag(MESBOX_CANCEL_TAG);
     mes->bCancel->addTouchEventListener(CC_CALLBACK_2(UIMessage::event_Callback, mes));
     mes->content = (Text*)mes->root->getChildByTag(66);
     mes->content->setString(s);
     mes->msgLayer->_widgetLayer->addChild(mes->root);
     
}
UIMessage*  UIMessage::createMesBox(const string& s, int type)
{
    UIMessage* mes = new UIMessage();
    mes->msgLayer = Msg_Layer::createEx(Color4B(0, 0, 0, 0));
    mes->msgLayer->setEnterType(Msg_Layer::EnterType::ScalePop);

//76确认  //70取消  //文本66
    
    if(type == 0)
    {
        mes->root = CSLoader::createNode("CSres/public/Dialog.csb");
        mes->bOK = (Button*)mes->root->getChildByTag(MESBOX_OK_TAG);
        mes->bOK->addTouchEventListener(CC_CALLBACK_2(UIMessage::eventCallback, mes));
    }
    else
    {
        mes->root = CSLoader::createNode("CSres/public/Dialog_1.csb");
        mes->bOK = (Button*)mes->root->getChildByTag(MESBOX_OK_TAG);
        mes->bOK->addTouchEventListener(CC_CALLBACK_2(UIMessage::eventCallback, mes));
        mes->bCancel = (Button*)mes->root->getChildByTag(MESBOX_CANCEL_TAG);
        mes->bCancel->addTouchEventListener(CC_CALLBACK_2(UIMessage::eventCallback, mes));
    }
    mes->content = (Text*)mes->root->getChildByTag(66);
    mes->content->setString(s);
    mes->msgLayer->_widgetLayer->addChild(mes->root);
    
    return mes;
}


//err_code	描述
//0	成功
//1	调用失败
//2	身份验证失败
//3	数据不合法
//4	Post方法名不存在（functionname）
//5	当前玩家不存在
//6	密码验证失败
//7	该账号已存在
//8	好友关系已存在
//9	好友关系不存在
//10	该收藏已存在
//11	该收藏不存在
//12	该游戏模式不存在
//13	短信发送失败
//14	缺省玩家资料
//15	该奖品不存在
//16	LoginKey过期，需要重新登录
//17	get请求参数不全
//18	未找到版本信息
//19	活动ID为空
//20	省份编号不存在
//21	城市编号不存在
//22	未找到当前活动
//23	该奖品图片不存在
void    UIMessage::event_Callback(Ref * pSender, Widget::TouchEventType type){
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
            
        case MESBOX_OK_TAG:
        {
            IAudio::getInstance()->playEffect(music_ef_ok);
            
            if(_args.err_code == 16)//账号重复登陆
            {
                //                GameIni::getInstance()->exitApp();
                UICtrl::getInstance()->gotoScene(UI_LOGIN);
            }
            
            else if(_args.retry)//网络连接错误
            {
                if(_args.ptr)
                {
                    myHttpWork* http = (myHttpWork*)_args.ptr;
                    http->sendRequest();
                }
            }
            
            IMessageManager::getInstance()->postNotification(MESSAGEBOXOK, &_args);
            close();
        }break;
        case MESBOX_CANCEL_TAG:
        {
            IAudio::getInstance()->playEffect(music_ef_cancel);
            
            if(_args.retry)//网络连接错误
            {
                if(_args.ptr)
                {
                    myHttpWork* http = (myHttpWork*)_args.ptr;
                    http->release();
                }
            }
            IMessageManager::getInstance()->postNotification(MESSAGEBOX_CANCEL, &_args);
            close();
        }break;
    }

}
void UIMessage::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
            
        case MESBOX_OK_TAG:
        {
            IAudio::getInstance()->playEffect(music_ef_ok);
            
            if(_args.err_code == 16)//账号重复登陆
            {
//                GameIni::getInstance()->exitApp();
                UICtrl::getInstance()->gotoScene(UI_LOGIN);
            }
            
            else if(_args.retry)//网络连接错误
            {
                if(_args.ptr)
                {
                    myHttpWork* http = (myHttpWork*)_args.ptr;
                    http->sendRequest();
                }
            }
            
            IMessageManager::getInstance()->postNotification(MESSAGEBOX_OK, &_args);
            close();
        }break;
        case MESBOX_CANCEL_TAG:
        {
            IAudio::getInstance()->playEffect(music_ef_cancel);
            
            if(_args.retry)//网络连接错误
            {
                if(_args.ptr)
                {
                    myHttpWork* http = (myHttpWork*)_args.ptr;
                    http->release();
                }
            }
            IMessageManager::getInstance()->postNotification(MESSAGEBOX_CANCEL, &_args);
            close();
        }break;
    }
}

void UIMessage::close()
{
    if(msgLayer)
        msgLayer->close();
    delete this;
}


void UIMessage::createIconAnition(int type, int num)
{
    Scene *scene = Director::getInstance()->getRunningScene();
    Layer* layer = Layer::create();
    
    Node* node = CSLoader::createNode("CSres/public/iconAnimation.csb");
    auto at = CSLoader::createTimeline("CSres/public/iconAnimation.csb");
    node->runAction(at);
    at->gotoFrameAndPlay(0);
    
    Text* t = (Text*)node->getChildByTag(19);
    switch (type) {
        case addicon:
        {
            t->setString(StringUtils::format("+%d", num));
        }break;
        case sumbicon:
        {
            t->setString(StringUtils::format("-%d", num));
        }break;
        default:
            break;
    }
    
    layer->addChild(node);
    layer->setZOrder(20000);
    layer->setOpacity(0);
    
    node->runAction(Spawn::create(
                                  MoveBy::create(2, Vec2(0,150)),
                                  Sequence::create(
                                                   DelayTime::create(1),
                                                   FadeOut::create(1),
                                                   CallFuncN::create([=](Node *node){
                                                      node->stopAllActions();
                                                      layer->removeFromParent();
                                                  }),
                                                   NULL),
                                  NULL));
    
    scene->addChild(layer);
    
}














