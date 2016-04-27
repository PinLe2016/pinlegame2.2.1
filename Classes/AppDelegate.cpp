#include "AppDelegate.h"

#include "framework/config.h"

#include "player/IPlayerData.h"
#include "moduleInit.h"
#include "gameini/GameIni.h"
#include "activity/view/UI_Activity.h"
#include "UIcontroller/UIcontroller.h"
#include "login/view/UI_Login.h"
#include "option/view/UI_Option.h"
#include "main/view/UI_Main.h"
#include "PluginHelper.h"
#include "gamelink/GameLink.h"

#include "ThirdParty/C2DXShareSDK/ShareSDKHelper.h"
USING_NS_CC;


using namespace cn::sharesdk;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}


//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = GLViewImpl::createWithRect("framework", Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, ResolutionPolicy::EXACT_FIT);
    CCLOG("---10gjei ---");
#ifdef GAME_TEST
    // turn on display FPS
    director->setDisplayStats(false);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    FileUtils::getInstance()->addSearchPath("res");
    // create a scene. it's an autorelease object
    
    Module::init();
    GameIni::getInstance()->preloadMusic();
    GameIni::getInstance()->getDeviceInfo();
    //初始化ShareSDK
    ShareSDKHelper::getInstance()->initSDKAndPlatformConfig(SHARESDK_APPID);
    
    //测试

UICtrl::getInstance()->gotoScene(UI_LOGO);
//    UICtrl::getInstance()->gotoScene(UI_MAIN);
//    UICtrl::getInstance()->gotoScene(UI_LINK);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    UICtrl::getInstance()->stoptime = ITools::GetNowSec();
    IMessageManager::getInstance()->postNotification(SYSTEM_ENTERBACKGROUND);
    
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    UICtrl::getInstance()->pasttime = ITools::GetNowSec() - UICtrl::getInstance()->stoptime;
    IMessageManager::getInstance()->postNotification(SYSTEM_ENTERFOREGROUND);
    
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    //每次进入刷新地图数据
    ((IPlayerData*)M_GET_PLAYER)->getCoordinate();
}


