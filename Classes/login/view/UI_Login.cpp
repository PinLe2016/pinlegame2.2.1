//
//  UI_Login.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/8.
//
//

#include "UI_Login.h"
#include "../../UIcontroller/UIcontroller.h"
#include "../../option/view/PersonCenterLayerEx.h"
#define VERITY_TIME     60              //验证码倒计时时间



Scene* UI_Login::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UI_Login::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


UI_Login::UI_Login()
{
    is_show=true;
    IUILayer::attachNetworkMes();
    IMessageManager::getInstance()->attach(this, MES_PLAYER_LOGIN);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_VERITYCODE);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_SETPASSWORD);
    IMessageManager::getInstance()->attach(this, MES_PLAYER_TONGYIXIEYI);
    
    RandVerifycode = "-1";
    isForgetPsw = NO;
    mesbox = NULL;
    m_quitgame = NULL;
    _player = M_GET_PLAYER;
    showPsw = YES;
    isFristSendPsw = false;
    _xieyiNode = NULL;
//    agreeXiyi = 0;
    
    UserDefault::getInstance()->setStringForKey(userid_key, "");
    UserDefault::getInstance()->setStringForKey(userpsw_key, "");
    UserDefault::getInstance()->flush();
}

UI_Login::~UI_Login()
{
    gamelog("----UI_Login::~UI_Login---");
    IUILayer::detachNetworkMes();
    IMessageManager::getInstance()->detach(this, MES_PLAYER_LOGIN);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_VERITYCODE);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_SETPASSWORD);
    IMessageManager::getInstance()->detach(this, MES_PLAYER_TONGYIXIEYI);
    
    //    IMessageManager::getInstance()->detach(this, SYSTEM_ENTERFOREGROUND);
}



//1.	登录界面：40
//a)	手机号输入框：11
//b)	密码输入框：12
//c)	登录按钮：13
//d)	忘记密码按钮：19
//e)	现在注册按钮：15
//f)	版本号：17
//2.	修改密码界面：168
//a)	手机号输入框：173
//b)	验证码输入框：174
//c)	获取验证码按钮：175
//d)	密码输入框：185
//e)	密码再次输入框：182
//f)	完成按钮：176
//g)	返回按钮178
//h)	显示密码按钮：123
//i)	注册界面（好友邀请码）容器：125
//i.	“邀请码”输入框：64
#define APP_PHONE_NULL     "手机号为空"
#define APP_PSW_NULL     "密码为空"
#define APP_PHONE_ORGINAL  "请输入手机号"
#define APP_PAW_ORGINAL   "请输入密码"
#define APP_PAW_towORGINAL   "请输入新密码"
enum
{
    P_LOGIN = 40,                               //登陆主界面
    P_PSW = 168,                                //修改密码界面
    P_INVITE = 125,                             //邀请好友
    TF_LOGIN_PHONE = 11,
    TF_LOGIN_PSW = 12,
    B_LOGIN_LOGIN = 13,
    B_LOGIN_FORGET = 2016,
    B_LOGIN_REGIST = 15,
    T_LOGIN_VERSION = 17,
    
    TF_PSW_PHONE = 173,//手机号
    TF_PSW_CODE = 174,//验证码
    B_PSW_CODE = 175,
    TF_PSW_PSW = 185,//输入密码
    TF_PSW_PSW2 = 182,//再次输入密码
    B_PSW_FINISH = 176,//完成
    B_PSW_SHOW = 543,
    B_PSW_CLOSE = 544,
    
    TF_INVITE_CODE = 64,
    
    B_PSW_BACK = 178,
};

bool UI_Login::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !IUILayer::init() )
    {
        return false;
    }
    
    
    
    
    IAudio::getInstance()->stopBackgroundMusic();
    
    rootNode = CSLoader::createNode("CSres/login/login.csb");//登陆主界面
    addChild(rootNode);
    
    P_Login = rootNode->getChildByTag(P_LOGIN);
    //版本号
    Text* text = (Text*)P_Login->getChildByTag(T_LOGIN_VERSION);
    text->setString(StringUtils::format("v%s", APP_VERSION));
    
    int tf_arr[] = {TF_LOGIN_PHONE, TF_LOGIN_PSW};
    for(int i = 0; i < sizeof(tf_arr) / sizeof(tf_arr[0]); i ++)
    {
        TextField* tf = (TextField*)P_Login->getChildByTag(tf_arr[i]);
        tf->addEventListener(CC_CALLBACK_2(UI_Login::textFieldCallBack, this));
        tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
        if(tf_arr[i] == TF_LOGIN_PHONE)
        {
            string userID = _player->getLocalUserID();
            if(IsPhoneNumber(userID))
                tf->setString(userID);
        }
    }

    int arr[] = {B_LOGIN_LOGIN, B_LOGIN_FORGET, B_LOGIN_REGIST};
    for(int i = 0; i < sizeof(arr) / sizeof(arr[0]); i ++)
    {
        Widget* b = (Button*)P_Login->getChildByTag(arr[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Login::eventCallback, this));
//        if(arr[i] == B_LOGIN_FORGET)
//        {
//            ITools::addUnderline(b);
//        }
    }

    P_Psw = rootNode->getChildByTag(P_PSW);
    
    int tf_arr2[] = {TF_PSW_PHONE, TF_PSW_CODE, TF_PSW_PSW, TF_PSW_PSW2};
    //新增
    for (int i=180; i<183; i++) {
        TextField* tf12 = (TextField*)P_Psw->getChildByTag(i);
        tf12->setVisible(false);

    }
    for (int i=543; i<546; i++) {
        TextField* tf12 = (TextField*)P_Psw->getChildByTag(i);
        tf12->setVisible(false);
        
    }
    for(int i=183;i<186;i++){
        TextField* tf12 = (TextField*)P_Psw->getChildByTag(i);
        tf12->setPositionY(tf12->getPositionY());
    }
    
    for(int i = 0; i < sizeof(tf_arr2) / sizeof(tf_arr2[0]); i ++)
    {
        TextField* tf = (TextField*)P_Psw->getChildByTag(tf_arr2[i]);
        tf->addEventListener(CC_CALLBACK_2(UI_Login::textFieldCallBack, this));
        tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    }
    //
    int arr2[] = {B_PSW_BACK, B_PSW_CODE, B_PSW_FINISH, B_PSW_SHOW, B_PSW_CLOSE};
    for(int i = 0; i < sizeof(arr2) / sizeof(arr2[0]); i ++)
    {
        Button* b = (Button*)P_Psw->getChildByTag(arr2[i]);
        b->addTouchEventListener(CC_CALLBACK_2(UI_Login::eventCallback, this));
    }
    
    P_Invite = P_Psw->getChildByTag(P_INVITE);
//    P_Invite->setVisible(false);
    int tf_arr3[] = {TF_INVITE_CODE};
    for(int i = 0; i < sizeof(tf_arr3) / sizeof(tf_arr3[0]); i ++)
    {
        TextField* tf = (TextField*)P_Invite->getChildByTag(tf_arr3[i]);
        tf->addEventListener(CC_CALLBACK_2(UI_Login::textFieldCallBack, this));
        tf->setPlaceHolderColor(Color4B(0, 0, 0, 0));
    }

//    string filepath = GameIni::getInstance()->getLocalSavedImagePath("logceshi.txt", GameIni::ImagePathType::SCREENSHOOT, true);
//    Text* tts = Text::create();
//    tts->setFontSize(15);
//    tts->setColor(Color3B(244,0,0));
//    tts->setPosition(Vec2(300,600));
//    tts->setString(filepath);
//    tts->setRotation(90);
//    addChild(tts,10000);
    
       return true;
}




void UI_Login::textFieldCallBack(Ref* pSender, TextField::EventType type)
{
    TextField* tf = (TextField*)pSender;
    GAMELOG("tf->getTag = %d, type = %d", tf->getTag(), type);
    switch (type)
    {
        case TextField::EventType::ATTACH_WITH_IME:
        {
            IAudio::getInstance()->playEffect(music_ef_ok);
            if(!tf->getString().compare(tf->getPlaceHolder()) || tf->getString().empty())
            {
                if(tf->getTag() == 185 || tf->getTag() == 182 || tf->getTag() == 12)
                {
//                    tf->setPasswordEnabled(true);
                    tf->setPasswordEnabled(showPsw);//启用输入密码模式
                }
                tf->setString("");
            }
        }break;
        case TextField::EventType::DETACH_WITH_IME:
        {
            IAudio::getInstance()->playEffect(music_ef_cancel);
            if(tf->getString().empty())
            {
                if(tf->getTag() == 185 || tf->getTag() == 182 || tf->getTag() == 12)
                {
                    tf->setPasswordEnabled(false);
                }
                tf->setString(tf->getPlaceHolder());
            }
        }break;
        default:
            break;
    }
}


void UI_Login::updateVerify(float dt)
{
    Button* b = (Button*)P_Psw->getChildByTag(B_PSW_CODE);
    if(verifyTime < 0)
        setVerifyButton(0);
    else
        b->setTitleText(StringUtils::format("获取验证码(%ld)", verifyTime));
    verifyTime --;
}


void UI_Login::setVerifyButton(int type)
{
    Button* b = (Button*)P_Psw->getChildByTag(B_PSW_CODE);
    if(type == 1)
    {
        b->setEnabled(NO);
        b->setBright(NO);
//        b->set
        verifyTime = VERITY_TIME;
        updateVerify(1);
        isFristSendPsw = true;
        schedule(schedule_selector(UI_Login::updateVerify), 1.f);
    }
    else
    {
        b->setEnabled(YES);
        b->setBright(YES);
        if(!isFristSendPsw)
        {
            b->setTitleText("获取验证码");
        }
        else
        {
            b->setTitleText("重新获取");
        }
        
        unschedule(schedule_selector(UI_Login::updateVerify));
    }
}



void UI_Login::openRegist(bool bOpen)
{
    if(bOpen == YES)
    {
        showPsw = YES;
        P_Login->setVisible(NO);
        P_Psw->setVisible(YES);
//        P_Invite->setVisible(!isForgetPsw);
        Button* b = (Button*)P_Psw->getChildByTag(B_PSW_CODE);
        b->setEnabled(YES);
        b->setBright(YES);
        b->setTitleText("获取验证码");
        
        
        int arr[] = {TF_PSW_PHONE, TF_PSW_CODE, TF_PSW_PSW, TF_PSW_PSW2};
        TextField* tf_;
        for(int i = 0; i < sizeof(arr) / sizeof(arr[0]); i ++)
        {
            TextField* tf = (TextField*)P_Psw->getChildByTag(arr[i]);
            
            if(arr[i] == TF_PSW_PSW || arr[i] == TF_PSW_PSW2 )
            {
                tf->setPasswordEnabled(false);
            }
            if (i==2  && !is_show) {
                tf_=tf;
            }
            if (i==2  && is_show) {
                tf_=tf;
            }
            
            
            tf->setString(tf->getPlaceHolder());
            
        }
        if (!is_show) {
            tf_->setString("请输入新密码");
        }else{
            tf_->setString("请输入密码");
        }
        
        showPassword(true);
        TextField* tf = (TextField*)P_Invite->getChildByTag(TF_INVITE_CODE);
        tf->setString(tf->getPlaceHolder());
    }
    else
    {
        P_Login->setVisible(YES);
        P_Psw->setVisible(NO);
        isForgetPsw = NO;
        if(mesbox)
        {
            mesbox->close();
            mesbox = NULL;
        }
        unschedule(schedule_selector(UI_Login::updateVerify));
    }
}

void  UI_Login::showPassword(bool b)
{
    Button* btnShow = (Button*)P_Psw->getChildByTag(B_PSW_SHOW);
    btnShow->setVisible(false);  //新改  b
    btnShow->setEnabled(b);
    Button* btnClose = (Button*)P_Psw->getChildByTag(B_PSW_CLOSE);
    btnClose->setVisible(false);   //新改  !b
    Text* text = (Text*)P_Psw->getChildByTag(545);
    if(b)
    {
        text->setString("显示密码");
    }
    else
    {
        text->setString("显示密文");
    }
}

void UI_Login::initXieyiLayer()
{
    _xieyiNode = CSLoader::createNode("CSres/login/xieyilayer.csb");
    addChild(_xieyiNode);
    
    ListView* listview = (ListView*)_xieyiNode->getChildByTag(1003);
    listview->setItemModel(listview->getItem(0));
    listview->removeAllItems();
    ImageView* imageX;
    for(int i = 0; i < 4; i++)
    {
        listview->pushBackDefaultItem();
        imageX = (ImageView*)listview->getItem(i)->getChildByTag(302);
        imageX->loadTexture(StringUtils::format("CSres/login/loginUI/xieyi_%d.png", i+1));
    }
    
//

    Button* button;
    button = (Button*)_xieyiNode->getChildByTag(1001);
    button->addTouchEventListener(CC_CALLBACK_2(UI_Login::eventCallback, this));
    button = (Button*)_xieyiNode->getChildByTag(1002);
    button->addTouchEventListener(CC_CALLBACK_2(UI_Login::eventCallback, this));
    
}


void UI_Login::eventCallback(Ref * pSender,Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    IAudio::getInstance()->playEffect(music_ef_ok);
    
    Widget* widget = (Widget*)pSender;
    int tag = widget->getTag();
    GAMELOG("widget->getTag = %d", tag);
    switch (tag)
    {
        case B_LOGIN_REGIST://注册
        {
            is_show=true;
            if(!GameIni::getInstance()->agreeXiyi)
            {
                initXieyiLayer();
                break;
            }
            
            isFristSendPsw = false;
            P_Login->setVisible(NO);
            isForgetPsw = NO;
            openRegist(YES);
        }break;
            
        case B_LOGIN_FORGET://忘记密码
        {
            is_show=false;
            isFristSendPsw = false;
            isForgetPsw = YES;
            openRegist(YES);
        }break;
            
        case B_LOGIN_LOGIN://登陆
        {
            Director::getInstance()->getOpenGLView()->setIMEKeyboardState(0);
            TextField* tf;
            tf = (TextField*)P_Login->getChildByTag(TF_LOGIN_PHONE);
            _player->userid = tf->getString();
            if(!isVauled(tf) || !IsPhoneNumber(_player->userid))
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_phone));
                break;
            }
            tf = (TextField*)P_Login->getChildByTag(TF_LOGIN_PSW);
            _player->userpsw = tf->getString();
            if(!isVauled(tf) || !PassWordRule(_player->userpsw))
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_pswtype));
                break;
            }

            _player->http_request_login();
        }break;
            
        case B_PSW_BACK://注册界面返回
        {
            openRegist(NO);
        }break;
            
        case B_PSW_CODE://发送验证码
        {
            TextField*  tf;
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PHONE);
            if(!isVauled(tf) || !IsPhoneNumber(tf->getString()))//发送验证码检测手机号是否正确
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_phone));
                break;
            }
            RandVerifycode = StringUtils::format("%d", ITools::getRand(1000, 9999));
            GAMELOG("验证码RandVerifycode = %s", RandVerifycode.c_str());
            setVerifyButton(1);
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PHONE);
            if(isForgetPsw)
                _player->http_request_getVerityCode(tf->getString(), RandVerifycode, 2);
            else
                _player->http_request_getVerityCode(tf->getString(), RandVerifycode, 1);
        }break;
        case B_PSW_CLOSE:
        case B_PSW_SHOW://显示密码
        {
            showPsw = !showPsw;
            
            showPassword(showPsw);
           
            TextField* tf;
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PSW);
            if(isVauled(tf))
            {
                tf->setPasswordEnabled(showPsw);
                tf->setString(tf->getString());
            }
            if(tf)
            {
                tf->setPasswordEnabled(showPsw);
            }
            
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PSW2);
            if(isVauled(tf))
            {
                tf->setPasswordEnabled(showPsw);
                tf->setString(tf->getString());
            }
            if(tf)
            {
                tf->setPasswordEnabled(showPsw);
            }
        }break;
            
        case B_PSW_FINISH://完成
        {
            TextField* tf, *tf2;
            UserDefault::getInstance()->setBoolForKey("guide_move", false);
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PHONE);//手机号为空
            CCLOG("手机号为空 %s  ",tf->getString().c_str());
            if (tf->getString()==(APP_PHONE_ORGINAL) ) {
                UIMessage::createMesBox(APP_PHONE_NULL);
                break;
            }
            tf  = (TextField*)P_Psw->getChildByTag(TF_PSW_PSW);
            if (tf->getString()==(APP_PAW_ORGINAL)  || tf->getString()==(APP_PAW_towORGINAL)) {
                UIMessage::createMesBox(APP_PSW_NULL);
                break;
            }

            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_CODE);
            if(tf->getString().compare(RandVerifycode) != 0)//验证码不相同
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_veritycode));
                break;
            }
            
            tf = (TextField*)P_Psw->getChildByTag(TF_PSW_PHONE);//手机号不正确
            if(!isVauled(tf) || !IsPhoneNumber(tf->getString()))
            {
                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_phone));
                break;
            }
            _player->userid = tf->getString();
            tf  = (TextField*)P_Psw->getChildByTag(TF_PSW_PSW);
            tf2 = (TextField*)P_Psw->getChildByTag(TF_PSW_PSW2);
//            if(tf->getString().compare(tf2->getString()) != 0) //两次密码不相同
//            {
//                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_psw2));
//                break;
//            }
//            if(!PassWordRule(tf->getString()) || !PassWordRule(tf2->getString()))//密码格式不正确
//            {
//                UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_pswtype));
//                break;
//            }
            _player->userpsw = tf->getString();
            
            if(isForgetPsw)  //忘记密码
            {
                _player->http_request_changePassword();
            }
            else
            {
                tf  = (TextField*)P_Invite->getChildByTag(TF_INVITE_CODE);//邀请码
                GAMELOG("tf = %s", tf);
                GAMELOG("tf!!! = %s", tf->getString().c_str());
                if(isVauled(tf))
                {
                    if(!IsPhoneNumber(tf->getString()))
                        UIMessage::createMesBox("邀请码手机号格式不对");
                    else if(tf->getString().compare(_player->userid) == 0)
                    {
                        UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_invitenotself));
                    }
                    else
                        _player->http_request_regist(tf->getString());
                }
                else
                {
                    _player->http_request_regist("");
                }
                
//                if(!isVauled(tf))
//                {
//                    if(!IsPhoneNumber(tf->getString()))
//                        UIMessage::createMesBox("邀请码手机号格式不对");
//                    else if(tf->getString().compare(_player->userid) == 0)
//                    {
//                        UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_error_invitenotself));
//                    }
//                    else
//                        _player->http_request_regist(tf->getString());
//                }
//                else
//                    _player->http_request_regist("");
            }
        }break;
          
        case 1001:
        {
            _player->http_request_sendXieyi();
        }break;
        case 1002:
        {
            _xieyiNode->removeFromParent();
            _xieyiNode = NULL;
        }break;
        default:break;
    }
}



void UI_Login::mesBoxCallback(Ref * pSender, Widget::TouchEventType type)
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
            openRegist(NO);
            TextField* tf = (TextField*)P_Login->getChildByTag(TF_LOGIN_PHONE);
            TextField* tf2 = (TextField*)P_Psw->getChildByTag(TF_PSW_PHONE);
            tf->setString(tf2->getString());
        }break;
        default:break;
    }
}

#define QUITAPP     10000

bool UI_Login::Notify(const string& name, Args& args)
{
    Notify_Compare(NETWORK_ERROR_RESPONSE)
    {
        if(verifyTime > 0)
            setVerifyButton(0);
        return YES;
    }
    Notify_Compare(NETWORK_ERROR_SERVICE)
    {
        if(verifyTime > 0)
            setVerifyButton(0);
        return YES;
    }
    
    
    if(IUILayer::Notify(name, args) == YES)
        return true;
    
    Notify_Compare(MES_PLAYER_LOGIN)
    {
        GAMELOG("denglu-------------------21");
        //addChild(PersonCenterLayerEx::create());
        UICtrl::getInstance()->gotoScene(UI_MAIN);
        
        
        return YES;
    }
    
    Notify_Compare(MES_PLAYER_REGIST)//注册后登入界面
    {
        
        UICtrl::getInstance()->gotoScene(UI_MAIN);
        return YES;
    }

    Notify_Compare(MES_PLAYER_SETPASSWORD)
    {
        mesbox = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_changepswsuc));
        mesbox->bOK->addTouchEventListener(CC_CALLBACK_2(UI_Login::mesBoxCallback, this));
        return YES;
    }
    
    Notify_Compare(SYSTEM_ENTERFOREGROUND)
    {
        verifyTime -= UICtrl::getInstance()->pasttime/1000;
        return YES;
    }
    
    Notify_Compare(SYSTEM_KEYBACK)
    {
        if(P_Login->isVisible())
        {
            if(!m_quitgame)
            {
                m_quitgame = UIMessage::createMesBox(GameIni::getInstance()->getTips(tips_info_quitapp), 1);
                m_quitgame->_args.code = QUITAPP;
            }
            else
            {
                m_quitgame->close();
                m_quitgame = nullptr;
            }
        }
        else if(P_Psw->isVisible())
        {
            eventCallback(P_Psw->getChildByTag(B_PSW_BACK), Widget::TouchEventType::ENDED);
        }
        
        if(!_xieyiNode)
        {
            _xieyiNode->removeFromParent();
            _xieyiNode = NULL;
        }
        return YES;
    }
    Notify_Compare(MESSAGEBOX_OK)
    {
        if(args.code == QUITAPP)
        {
            GameIni::getInstance()->exitApp();
            return YES;
        }
        return YES;
    }
    Notify_Compare(MESSAGEBOX_CANCEL)
    {
        if(args.code == QUITAPP)
        {
            m_quitgame = nullptr;
            return YES;
        }
    }
    Notify_Compare(MES_PLAYER_TONGYIXIEYI)
    {
        _xieyiNode->removeFromParent();
        _xieyiNode = nullptr;
        isFristSendPsw = false;
        P_Login->setVisible(NO);
        isForgetPsw = NO;
        openRegist(YES);
    }
    
    return true;
    
    
}
















