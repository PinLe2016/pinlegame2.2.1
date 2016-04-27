//
//  UI_Activity.h
//  PinLe
//
//  Created by Kennedy on 15/6/5.
//
//

#ifndef __PinLe__UI_Activity__
#define __PinLe__UI_Activity__

#include "../../framework/config.h"
#include "../IActivityData.h"
#include "../../gameini/GameIni.h"
#include "../../gameroom/view/OverLayer.h"
#include "../module/ActivityData.h"
#define NO_START "即将上线"
#define START_TEXT "本期活动"
#define COMPLETE "往期回顾"
enum Button_tag
{
    xq_ckjx = 175,          //详情－－查看奖项
    xq_pmb = 177,           //详情－－排名榜
    jf_ju  =  192,          //积分纪录
    xq_ckxq = 179,          //详情－－查看详情、我的成绩
    xq_back = 185,          //详情－－返回按钮
    xq_gzsm = 1001,         //详情－－规则说明
    xq_hdxq = 1002,         //详情－－活动详情
    xq_ljcy = 126,          //详情－－立即参与
    jpxq_back = 204,        //奖品详情－－返回按钮
    phb_back = 87,          //排行榜－－返回按钮
    wdcj_back =146,         //我的成绩－－返回按钮
    hdgz_close = 346,       //活动规则－－关闭按钮
    submit_xg = 80,         //活动奖品－－修改
    submit_tj = 72,         //活动奖品－－提交
    submit_fq = 66,         //活动奖品－－放弃
    submit_yj1 = 739,       //活动奖品－－邮寄地址，货到付款
    submit_yj2 = 740,       //活动奖品－－邮寄地址，抵扣金币
    dz_back = 1014,         //地址－－返回按钮
    dz_baocun = 1018,       //地址－－保存按钮
    dz_sheng = 1168,
    dz_shi = 1169,
    dz_shenglist = 1170,
    dz_shilist = 1171,
};

enum ShengShiTag
{
    shtag_sheng = 1,
    shtag_shi,
};

enum GET_IMAGE_TAG
{
    writable_image = 0,     //下载图片
    head_image,         //头像
};

enum textfield_tag
{
    textfield_dz_name = 1015,       //邮寄地址－－姓名
    textfield_dz_phone = 1016,      //邮寄地址－－电话
    textfield_dz_address = 1017,    //邮寄地址－－地址
};


class   UI_Activity : public IUILayer //,public OverLayer //, public TableViewDataSource, public TableViewDelegate
{
public:
    void show_oneself();
    int _time;
    Text * _title;
    bool  is_details;
    bool  is_show;
    bool  is_show_look;
    Button * buto;
    bool is_tinggao;
    int lx_huodong;
    UI_Activity();
    ~UI_Activity();
    //Activity* act;
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(UI_Activity);
    
    virtual bool Notify(const string& name, Args& args);
public:
    int _tag;
    void    setActivityByType(int type);
    void    showPHB();
    void    PHTouch(Ref *psender,Widget::TouchEventType type);
    void    showJFB();  //积分排行榜
    void    showDB();
    void    showWinning();
    void    showAwardPreview();
    string    getActivityID();
private:
    void    setTabShow(int tab);
    void    activitySation();
    void    selectedItemEvent(Ref *pSender, ListView::EventType type);
    void    qw();
    void    openActivityDetails(bool bOpen);
    void    setDetailsTime();
    void    eventCallback(Ref * pSender,Widget::TouchEventType type);
    void    Call_back(Ref * pSender,Widget::TouchEventType type);
    void    setActivityTime();
    void    showJPXQ();
    //void    showPHB();
    void    showWDCJ();
    void    showGZ(bool b = false);                   //显示活动规则
    void    submitRewarkInfo();         //提交奖品订单信息
    void    reloadActivityInfo();       //更新玩家信息
    void    updateTime(float dt);
    string  changeUserId(string str);
    Action* getLightAction();
    ImageView* ActivityType ;  //活动类型 年月周日
    IActivityData*  _activity;
    Node*           rootNode;
    Node*           PersonalRecord;         //积分排行榜
    Node*           RankingRecord;
    Node*           AwardPreview;           //奖项预览
    Node*           _Winning;
    Node*           _activityDetails;
    Node*           _rewardDetails;         //奖品详情页面
    Node*           _rankLayer;             //排行榜页面
    Node*           _myGradeLayer;          //我的成绩页面
    Node*           _activityRules;         //活动规则页面
    Node*           gameOverNode;           //提交订单信息页面
    Node*           _activityXiangqing;     //活动详情页面
    Node*           _addressDetails;        //邮寄地址修改信息
    Node*           _shengfenNode;          //省份选择不同的活动（全国，本省）
    LayerColor*     layerColor;
    ssize_t         _cellcount;
    Size            _cellsize;
    uint            _tab;
    uint            _cellIndex;
    ListView*       _list;          //活动列表
    ListView*       _phbList;       //排行榜列表
    ListView*       _pjfList;       //积分排行榜
    ListView*       _dbList;
    ListView*       _WinningList;    //奖励
    ListView*       _AwardPreviewList;//奖项预览列表
    ListView*       _guizeList;     //规则说明列表
    int             _nowstate;      //是否显示详情
    UIMessage*      messageBox;     //提示信息
    Text*           addressInfo;
    Text*           phoneInfo;
    Text*           nameInfo;
                
    
private:
    bool    quanguoHD;                //活动是否是全国活动
    void    setPostWay(bool b);     //设置邮寄方式
    bool    activityRulesShow;      //规则说明是否显示
    bool    addressIsPerfect();     //判断地址填写完善
    void    setAddressInfo();       //初始化，邮寄地址信息
    bool    needReloadAddressInfo;  //是否需要重新加载玩家邮寄地址    
    
private:
    void    eventSH_select(Ref *pSender, ListView::EventType type);
    rapidjson::Document readdoc;
    int             shSelectTag;
    ListView*       list_sheng;
    ListView*       list_shi;
    int             shengid;
    int             shiid;
    int             sheng_selectTag;
    string          shengname;
    string          shiname;
    string          postname;
    string          postphone;
    string          postaddress;
public:
    Text * myJifen;
    Text * myMingci;
};







#endif /* defined(__PinLe__UI_Activity__) */
