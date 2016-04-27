//
//  ShareSDKHelper.cpp
//  PinLe
//
//  Created by Kennedy on 15/9/2.
//
//

#include "ShareSDKHelper.h"
#include "../../framework/config.h"
#include "../../gameini/GameIni.h"


using namespace cn::sharesdk;


static ShareSDKHelper* _shareSDKHelper = nullptr;

ShareSDKHelper::ShareSDKHelper()
{
    string path = StringUtils::format("%s/%s/%s", IMAGE_ROOT_FINDER, IMAGE_THUMBNAIL_FINDER, IMAGE_THUMBNAIL_FILENAME);
    if(!IFile::isFileExist_Writable(path))
    {
        ssize_t size = 0;
        unsigned char* titlech = FileUtils::getInstance()->getFileData("res/Default/Icon-114.png", "r", &size);
        if(size > 0)
        {
            std::string load_str = std::string((const char*)titlech, size);
            IFile::writeFile(path, load_str);
        }
    }
}

ShareSDKHelper::~ShareSDKHelper()
{
    
}

ShareSDKHelper* ShareSDKHelper::getInstance()
{
    if(_shareSDKHelper == nullptr)
    {
        _shareSDKHelper = new ShareSDKHelper();
    }
    return _shareSDKHelper;
}
void ShareSDKHelper::destroyInstance()
{
    SAFE_DELETE(_shareSDKHelper);
    C2DXShareSDK::close();
}


void ShareSDKHelper::initSDK(const string& AppID)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        __String* appid = new __String(AppID);
        C2DXShareSDK::open(appid, false);
        delete appid;
    }
    #elif  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        C2DXShareSDK::open(AppID.c_str(), false);
    }
    #endif
}

void ShareSDKHelper::initSDKAndPlatformConfig(const string& AppID)
{
    initSDK(AppID);
    initPlatformConfig();
}

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, __Dictionary *shareInfo, __Dictionary *error)
{
    GAMELOG("----shareResultHandler--");
    switch (state)
    {
        case C2DXResponseStateSuccess:
        {
            GAMELOG("分享成功");
            IMessageManager::getInstance()->postNotification(MES_SHARESDK_SUCCEED);
        }break;
            
        case C2DXResponseStateFail:
        {
            GAMELOG("分享失败");
            IMessageManager::getInstance()->postNotification(MES_SHARESDK_FAILED);
        }break;
            
        case C2DXResponseStateBegin:
        {
            GAMELOG("开始分享");
            IMessageManager::getInstance()->postNotification(MES_SHARESDK_START);
        }break;
            
        case C2DXResponseStateCancel:
        {
            GAMELOG("取消分享");
            IMessageManager::getInstance()->postNotification(MES_SHARESDK_CANCELED);
        }break;
        default:
            break;
    }
}



//Key	 描述
//content	 要分享的内容，注意在文档中content对应的是text字段
//image	 要分享的图片，可以是本地路径imagePath（如：/sdcard/a.jpg）或是一个网址imageUrl
//title	 要分享的标题
//description	 内容简要
//url	 网页分享，仅需要图文分享时，不要传这个字段
//type	 内容类型，具体参考enum C2DXContentType
//site	 内容来源的站点名称
//siteUrl	 内容来源的站点链接
//musicUrl	 音乐分享，音乐文件的网址
//extInfo	 微信分享应用时传给应用的附加信息，参考微信API文档

void ShareSDKHelper::sendImage(const string& title, const string& description, const string& imagePath)
{
    GAMELOG("ShareSDKHelper::sendImage");
    __Dictionary *content = __Dictionary::create();
    content -> setObject(String::create(title), "title");
    //QQ空间不可以单独分享图片，只能分享新闻模式，有图片，文字，链接, QQ空间这个字段必须带
//    content -> setObject(String::create(GameIni::getInstance()->shareSdkInfo.webpageUrl), "url");
//    content -> setObject(String::create(description), "content"); //要分享的内容，注意在文档中content对应的是text字段
//    content -> setObject(String::create(description), "description"); //要分享的内容，注意在文档中content对应的是text字段
    content -> setObject(String::create(imagePath), "image");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
    content -> setObject(String::create(APP_NAME), "site");
//    content -> setObject(String::create(GameIni::getInstance()->shareSdkInfo.webpageUrl), "siteUrl");
    C2DXShareSDK::showShareMenu(NULL, content, cocos2d::Point(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
}

void ShareSDKHelper::sendLink(const string& title, const string& description, const string& webpageUrl, const string& imagePaht)
{
    GAMELOG("ShareSDKHelper::sendLink");
    __Dictionary *content = __Dictionary::create();
    content -> setObject(String::create(title), "title");
    content -> setObject(String::create(webpageUrl), "url");
    content -> setObject(String::create(description), "content"); //要分享的内容，注意在文档中content对应的是text字段
    content -> setObject(String::create(description), "description"); //要分享的内容，注意在文档中content对应的是text字段
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(String::create(APP_NAME), "site");
    content -> setObject(String::create(webpageUrl), "siteUrl");
    string thumbpath = GameIni::getInstance()->getLocalSavedImagePath(IMAGE_THUMBNAIL_FILENAME,
                                                                      GameIni::ImagePathType::THUMBNAIL, YES);
    if(IFile::isFileExist(imagePaht))
    {
        content -> setObject(String::create(imagePaht), "image");
    }
    C2DXShareSDK::showShareMenu(NULL, content, cocos2d::Point(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
}


void ShareSDKHelper::initPlatformConfig()
{
    //新浪微博
//    __Dictionary *sinaConfigDict = __Dictionary::create();
//    sinaConfigDict -> setObject(CCString::create(OPEN_SINA_APPID), "app_key");
//    sinaConfigDict -> setObject(CCString::create(OPEN_SINA_APPSECRET), "app_secret");
//    sinaConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //短信
    //C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //QQ空间
//    __Dictionary *qzConfigDict = __Dictionary::create();
//    qzConfigDict -> setObject(CCString::create(OPEN_QQ_APPID), "app_id");
//    qzConfigDict -> setObject(CCString::create(OPEN_QQ_APPKEY), "app_key");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //QQ
    __Dictionary *qqConfigDict = __Dictionary::create();
    qqConfigDict -> setObject(CCString::create(OPEN_QQ_APPID), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    
    //微信
    __Dictionary *wcConfigDict = __Dictionary::create();
    wcConfigDict -> setObject(CCString::create(OPEN_WECHAT_APPID), "app_id");
    wcConfigDict -> setObject(CCString::create(OPEN_WECHAT_APPSECRET), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    //C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);

//    //腾讯微博
//    __Dictionary *tcConfigDict = __Dictionary::create();
//    tcConfigDict -> setObject(CCString::create("801307650"), "app_key");
//    tcConfigDict -> setObject(CCString::create("ae36f4ee3946e1cbb98d6965b0b2ff5c"), "app_secret");
//    tcConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
//    
//    //Facebook
//    __Dictionary *fbConfigDict = __Dictionary::create();
//    fbConfigDict -> setObject(CCString::create("107704292745179"), "api_key");
//    fbConfigDict -> setObject(CCString::create("38053202e1a5fe26c80c753071f0b573"), "app_secret");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFacebook, fbConfigDict);
//    
//    //Twitter
//    __Dictionary *twConfigDict = __Dictionary::create();
//    twConfigDict -> setObject(CCString::create("mnTGqtXk0TYMXYTN7qUxg"), "consumer_key");
//    twConfigDict -> setObject(CCString::create("ROkFqr8c3m1HXqS3rm3TJ0WkAJuwBOSaWhPbZ9Ojuc"), "consumer_secret");
//    twConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTwitter, twConfigDict);
//    
//    //Google+
//    __Dictionary *gpConfigDict = __Dictionary::create();
//    gpConfigDict -> setObject(CCString::create("232554794995.apps.googleusercontent.com"), "client_id");
//    gpConfigDict -> setObject(CCString::create("PEdFgtrMw97aCvf0joQj7EMk"), "client_secret");
//    gpConfigDict -> setObject(CCString::create("http://localhost"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeGooglePlus, gpConfigDict);
//    
//    //人人网
//    __Dictionary *rrConfigDict = __Dictionary::create();
//    rrConfigDict -> setObject(CCString::create("226427"), "app_id");
//    rrConfigDict -> setObject(CCString::create("fc5b8aed373c4c27a05b712acba0f8c3"), "app_key");
//    rrConfigDict -> setObject(CCString::create("f29df781abdd4f49beca5a2194676ca4"), "secret_key");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeRenren, rrConfigDict);
//    
//    //开心网
//    __Dictionary *kxConfigDict = __Dictionary::create();
//    kxConfigDict -> setObject(CCString::create("358443394194887cee81ff5890870c7c"), "api_key");
//    kxConfigDict -> setObject(CCString::create("da32179d859c016169f66d90b6db2a23"), "secret_key");
//    kxConfigDict -> setObject(CCString::create("http://www.sharesdk.cn/"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeKaixin, kxConfigDict);
//    
//    //邮件
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeMail, NULL);
//    
//    //打印
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeAirPrint, NULL);
//    
//    //拷贝
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeCopy, NULL);
//    
//    //搜狐微博
//    __Dictionary *shwbConfigDict = __Dictionary::create();
//    shwbConfigDict -> setObject(CCString::create("SAfmTG1blxZY3HztESWx"), "consumer_key");
//    shwbConfigDict -> setObject(CCString::create("yfTZf)!rVwh*3dqQuVJVsUL37!F)!yS9S!Orcsij"), "consumer_secret");
//    shwbConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "callback_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuWeibo, shwbConfigDict);
//    
//    //网易微博
//    __Dictionary *neConfigDict = __Dictionary::create();
//    neConfigDict -> setObject(CCString::create("T5EI7BXe13vfyDuy"), "consumer_key");
//    neConfigDict -> setObject(CCString::create("gZxwyNOvjFYpxwwlnuizHRRtBRZ2lV1j"), "consumer_secret");
//    neConfigDict -> setObject(CCString::create("http://www.shareSDK.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatType163Weibo, neConfigDict);
//    
//    //豆瓣
//    __Dictionary *dbConfigDict = __Dictionary::create();
//    dbConfigDict -> setObject(CCString::create("02e2cbe5ca06de5908a863b15e149b0b"), "api_key");
//    dbConfigDict -> setObject(CCString::create("9f1e7b4f71304f2f"), "secret");
//    dbConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDouBan, dbConfigDict);
//    
//    //印象笔记
//    __Dictionary *enConfigDict = __Dictionary::create();
//    enConfigDict -> setObject(CCString::create("sharesdk-7807"), "consumer_key");
//    enConfigDict -> setObject(CCString::create("d05bf86993836004"), "consumer_secret");
//    enConfigDict -> setObject(CCString::create("0"), "host_type");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeEvernote, enConfigDict);
//    
//    //LinkedIn
//    __Dictionary *liConfigDict = __Dictionary::create();
//    liConfigDict -> setObject(CCString::create("ejo5ibkye3vo"), "api_key");
//    liConfigDict -> setObject(CCString::create("cC7B2jpxITqPLZ5M"), "secret_key");
//    liConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_url");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeLinkedIn, liConfigDict);
//    
//    //Pinterest
//    __Dictionary *piConfigDict = __Dictionary::create();
//    piConfigDict -> setObject(CCString::create("1432928"), "client_id");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePinterest, piConfigDict);
//    
//    //Pocket
//    __Dictionary *poConfigDict = __Dictionary::create();
//    poConfigDict -> setObject(CCString::create("11496-de7c8c5eb25b2c9fcdc2b627"), "consumer_key");
//    poConfigDict -> setObject(CCString::create("pocketapp1234"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePocket, poConfigDict);
//    
//    //Instapaper
//    __Dictionary *ipConfigDict = __Dictionary::create();
//    ipConfigDict -> setObject(CCString::create("4rDJORmcOcSAZL1YpqGHRI605xUvrLbOhkJ07yO0wWrYrc61FA"), "consumer_key");
//    ipConfigDict -> setObject(CCString::create("GNr1GespOQbrm8nvd7rlUsyRQsIo3boIbMguAl9gfpdL0aKZWe"), "consumer_secret");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstapaper, ipConfigDict);
//    
//    //有道云笔记
//    __Dictionary *ydConfigDict = __Dictionary::create();
//    ydConfigDict -> setObject(CCString::create("dcde25dca105bcc36884ed4534dab940"), "consumer_key");
//    ydConfigDict -> setObject(CCString::create("d98217b4020e7f1874263795f44838fe"), "consumer_secret");
//    ydConfigDict -> setObject(CCString::create("http://www.sharesdk.cn/"), "oauth_callback");
//    ydConfigDict -> setObject(CCString::create("1"), "host_type");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeYouDaoNote, ydConfigDict);
//    
//    //搜狐随身看
//    __Dictionary *shkConfigDict = __Dictionary::create();
//    shkConfigDict -> setObject(CCString::create("e16680a815134504b746c86e08a19db0"), "app_key");
//    shkConfigDict -> setObject(CCString::create("b8eec53707c3976efc91614dd16ef81c"), "app_secret");
//    shkConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuKan, shkConfigDict);
//    
//    //Flickr
//    __Dictionary *flickrConfigDict = __Dictionary::create();
//    flickrConfigDict -> setObject(CCString::create("33d833ee6b6fca49943363282dd313dd"), "api_key");
//    flickrConfigDict -> setObject(CCString::create("3a2c5b42a8fbb8bb"), "api_secret");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFlickr, flickrConfigDict);
//    
//    //Tumblr
//    __Dictionary *tumblrConfigDict = __Dictionary::create();
//    tumblrConfigDict -> setObject(CCString::create("2QUXqO9fcgGdtGG1FcvML6ZunIQzAEL8xY6hIaxdJnDti2DYwM"), "consumer_key");
//    tumblrConfigDict -> setObject(CCString::create("3Rt0sPFj7u2g39mEVB3IBpOzKnM3JnTtxX2bao2JKk4VV1gtNo"), "consumer_secret");
//    tumblrConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "callback_url");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTumblr, tumblrConfigDict);
//    
//    //Dropbox
//    __Dictionary *dropboxConfigDict = __Dictionary::create();
//    dropboxConfigDict -> setObject(CCString::create("7janx53ilz11gbs"), "app_key");
//    dropboxConfigDict -> setObject(CCString::create("c1hpx5fz6tzkm32"), "app_secret");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDropbox, dropboxConfigDict);
//    
//    //Instagram
//    __Dictionary *instagramConfigDict = __Dictionary::create();
//    instagramConfigDict -> setObject(CCString::create("ff68e3216b4f4f989121aa1c2962d058"), "client_id");
//    instagramConfigDict -> setObject(CCString::create("1b2e82f110264869b3505c3fe34e31a1"), "client_secret");
//    instagramConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstagram, instagramConfigDict);
//    
//    //VK
//    __Dictionary *vkConfigDict = __Dictionary::create();
//    vkConfigDict -> setObject(CCString::create("3921561"), "application_id");
//    vkConfigDict -> setObject(CCString::create("6Qf883ukLDyz4OBepYF1"), "secret_key");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeVKontakte, vkConfigDict);
}







